/* Copyright (C) 2013 Jiva Bagale */

/* This program is free software: you can redistribute it and/or modify */
/* it under the terms of the GNU General Public License as published by */
/* the Free Software Foundation, either version 3 of the License, or */
/* (at your option) any later version. */

/* This program is distributed in the hope that it will be useful, */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the */
/* GNU General Public License for more details. */

#include "video_searcher.h"
#include "video_responder.h"
#include "applicationui.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/LocaleHandler>
#include<bb/cascades/ArrayDataModel>

#include <QString>

using namespace bb::cascades;

/* global variables */
#define XML_SCHEMA "app/native/video.xsd"

ApplicationUI::ApplicationUI(bb::cascades::Application *app) :
        QObject(app)
{
	packedobjectsdObject *pod_object2 = NULL;

    // prepare the localization
    m_pTranslator = new QTranslator(this);
    m_pLocaleHandler = new LocaleHandler(this);

    if(!QObject::connect(m_pLocaleHandler, SIGNAL(systemLanguageChanged()), this, SLOT(onSystemLanguageChanged()))) {
        // This is an abnormal situation! Something went wrong!
        // Add own code to recover here
        qWarning() << "Recovering from a failed connect()";
    }
    // initial load
    onSystemLanguageChanged();

    // Create scene document from main.qml asset, the parent is set
    // to ensure the document gets destroyed properly at shut down.
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);

    // Make the ApplicationUI object available to the UI as context property
    qml->setContextProperty("appObject", this);

    // Create root object for the UI
    AbstractPane *root = qml->createRootObject<AbstractPane>();

    // Set created root object as the application scene
    app->setScene(root);

    //set initial status message
    root->setProperty("tempText", "Hello POD");

    // Initialise video searcher
    pod_object2 = this->initialiseSearcher();

    cout << "Unique ID" << pod_object2->unique_id <<endl;

    if(pod_object2 !=NULL) {

    // creating a QThread to run video receiver to get search results

    QThread* thread = new QThread;
    ReceiveThread* rt1= new ReceiveThread(this, pod_object2);

    rt1->moveToThread(thread);
    connect(thread, SIGNAL(started()), rt1, SLOT(process()));
    thread->start();

    // creating QThread to run video responder to process search requests

    QThread* thread_responder = new QThread;
    ResponderThread* rt_responder= new ResponderThread(this, pod_object2);

    rt_responder->moveToThread(thread_responder);
    connect(thread_responder, SIGNAL(started()), rt_responder, SLOT(run_responder()));
    thread_responder->start();

    }
    else {
    	cout << "failed to initialise properly! Please check network status"<<endl;
    	this->setStatus(QString ("failed to initialise properly!"));
    }
}

void ApplicationUI::onSystemLanguageChanged()
{
    QCoreApplication::instance()->removeTranslator(m_pTranslator);
    // Initiate, load and install the application translation files.
    QString locale_string = QLocale().name();
    QString file_name = QString("MyCascadesProject_%1").arg(locale_string);
    if (m_pTranslator->load(file_name, "app/native/qm")) {
        QCoreApplication::instance()->installTranslator(m_pTranslator);
    }
}

packedobjectsdObject * ApplicationUI::initialiseSearcher()
{
	this->setStatus(QString ("Initialising..."));
	//pod_object1 = _initialiseSearcher();

	// Initialise packedobjectsd
	if((pod_object1 = init_packedobjectsd(XML_SCHEMA, SEARCHER)) == NULL) {
		printf("failed to initialise libpackedobjectsd\n");
	}

	if(pod_object1 !=NULL) {
		this->setStatus(QString ("Initialisation complete."));
	}

    return pod_object1;
}

int ApplicationUI::sendSearch(QString videoTitle, double maxPrice)
{

	xmlDocPtr doc_search;

	// Converting Qstring to char*
	QByteArray ba = videoTitle.toLocal8Bit();
    char *title_str = ba.data();

	doc_search = create_search(pod_object1, title_str, maxPrice);

	if(doc_search != NULL) {
		_sendSearch(pod_object1, doc_search);

		this->setStatus(QString ("Search request sent.."));
		return 1;
	}

    return -1;
}

ReceiveThread::ReceiveThread(ApplicationUI *app_object, packedobjectsdObject *pod_object2)
 {
	app_object1 = app_object;
	pod_object3 = pod_object2;
 }

void ReceiveThread::process()
 {
	 //qDebug("Hello QThread!");
	app_object1->setStatus(QString ("Ready to receive .."));
	_receiveResponse(app_object1, pod_object3);
	 emit finished();
 }

ResponderThread::ResponderThread(ApplicationUI *app_object, packedobjectsdObject *pod_object2)
 {
	app_object2 = app_object;
	pod_object4 = pod_object2;
	this->pod_resp_obj = _initialiseResponder();
 }

void ResponderThread::run_responder()
 {
	 //qDebug("Hello QThread!");
	app_object2->setStatus(QString ("Starting responder .."));
	start_responder(app_object2, this->pod_resp_obj);
	 emit finished();
 }
// Various functions to pass signals to QML

QString ApplicationUI::status()
{
    return str_status;
}

void ApplicationUI::setStatus(QString str)
{
// name is same as HPP WRITE Q_PROPERTY statement
	str_status = str;
    emit statusChanged();
}

QString ApplicationUI::title()
{
    return str_title;
}

void ApplicationUI::setTitle(QString str)
{
// name is same as HPP WRITE Q_PROPERTY statement
	str_title = str;
    emit titleChanged();
}

double ApplicationUI::price()
{
    return dbl_price;
}

void ApplicationUI::setPrice(double val)
{
// name is same as HPP WRITE Q_PROPERTY statement
	dbl_price = val;
    emit priceChanged();
}

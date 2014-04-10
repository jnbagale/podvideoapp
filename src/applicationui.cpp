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
#include <bb/cascades/ArrayDataModel>

#include <QString>

using namespace bb::cascades;

// XML namespace and url for xpath
#define NS_PREFIX "xs"
#define NS_URL "http://www.w3.org/2001/XMLSchema"

/* global variables */

#define XML_EXPORT "file:///accounts/1000/shared/misc/video.xml"
#define XML_SCHEMA "app/native/video.xsd"
#define XML_DATA "app/native/assets/video.xml"

ApplicationUI::ApplicationUI(bb::cascades::Application *app) :
        		QObject(app)
{
	packedobjectsdObject *pod_object2 = NULL;
	packedobjectsdObject *pod_obj_responder = NULL;


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

	// Initialise video searcher
	pod_object2 = this->initialiseSearcher();

	//set the value of searcher's unique id
	QString s_id_string = QString::number(pod_object2->unique_id, 10);

	qDebug() << "Searcher ID:- " << s_id_string << endl;
	root->setProperty("searcheridText", QVariant ("ID" + s_id_string));

	// Initialise video responder

	pod_obj_responder = _initialiseResponder();

	//set the value of responder's unique id
	QString r_id_string = QString::number(pod_obj_responder->unique_id, 10);

	qDebug() << "Responder ID:- " << r_id_string << endl;
	root->setProperty("responderidText", QVariant ("ID" + r_id_string));

	if(pod_object2 !=NULL) {

		// creating a QThread to run video receiver to get search results

		QThread* thread = new QThread;
		ReceiveThread* rt1= new ReceiveThread(this, pod_object2);

		rt1->moveToThread(thread);
		connect(thread, SIGNAL(started()), rt1, SLOT(process()));
		thread->start();

		// creating QThread to run video responder to process search requests

		QThread* thread_responder = new QThread;
		ResponderThread* rt_responder= new ResponderThread(this, pod_object2, pod_obj_responder);

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

			_sendSearch(this, pod_object1, doc_search);

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

ResponderThread::ResponderThread(ApplicationUI *app_object, packedobjectsdObject *pod_object2, packedobjectsdObject *pod_obj_responder)
{
	app_object2 = app_object;
	pod_object4 = pod_object2;
	pod_resp_obj = pod_obj_responder;

}

void ResponderThread::run_responder()
{
	//qDebug("Hello QThread!");
	app_object2->setStatus(QString ("Responder ready .."));
	start_responder(pod_object4, app_object2, pod_resp_obj);
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

QString ApplicationUI::size()
{
	return str_size;
}

void ApplicationUI::setSize(QString str)
{
	// name is same as HPP WRITE Q_PROPERTY statement
	str_size = str;
	emit sizeChanged();
}

QString ApplicationUI::query()
{
	return str_query;
}

void ApplicationUI::setQuery(QString str)
{
	// name is same as HPP WRITE Q_PROPERTY statement
	str_query = str;
	emit queryChanged();
}

QString ApplicationUI::size1()
{
	return str_size1;
}

void ApplicationUI::setSize1(QString str)
{
	// name is same as HPP WRITE Q_PROPERTY statement
	str_size1 = str;
	emit size1Changed();
}

static inline const char *QStringToCharPtr(QString qString1)
{
	// Converting QString to char pointer
	QByteArray byteArray = qString1.toUtf8();
	const char *cString1 = byteArray.constData();

	return cString1;
}


int ApplicationUI::deleteNode(QString originalTitle)
{
	int size;
	char xpath_exp[1000];
	xmlDocPtr doc;
	xmlXPathContextPtr xpathp = NULL;
	xmlXPathObjectPtr result = NULL;

	///////////////////// Initialising XML document ///////////////////
	qDebug() << "Deleting node from XML database: " << originalTitle;

	xmlKeepBlanksDefault(0);
	doc = xmlReadFile(XML_DATA, NULL, 0);

	if (doc == NULL)
	{
		printf("could not parse file %s", XML_DATA);
	}

	///////////////////// Initialising XPATH ///////////////////

	xpathp = xmlXPathNewContext(doc);
	if (xpathp == NULL) {
		printf("Error in xmlXPathNewContext.");
		xmlXPathFreeContext(xpathp);
		return -1;
	}

	if(xmlXPathRegisterNs(xpathp, (const xmlChar *)NS_PREFIX, (const xmlChar *)NS_URL) != 0)
	{
		printf("Error: unable to register NS.");
		xmlXPathFreeContext(xpathp);
		return -1;
	}

	///////////////////// Evaluating XPATH expression ///////////////////

	sprintf(xpath_exp, "/video/message/database/movie[atitle='%s']/*", QStringToCharPtr(originalTitle));

	/* Evaluate xpath expression */
	result = xmlXPathEvalExpression((const xmlChar *)xpath_exp, xpathp);
	if (result == NULL)
	{
		printf("Error in xmlXPathEvalExpression.");
		xmlXPathFreeObject(result);
		xmlXPathFreeContext(xpathp);
		return -1;
	}

	///////////////////// Processing result ///////////////////

	if(xmlXPathNodeSetIsEmpty(result->nodesetval))
	{
		xmlXPathFreeObject(result);
		xmlXPathFreeContext(xpathp);
		printf("the id does not exist on the database\n");
		return -1;
	}
	else
	{
		size = result->nodesetval->nodeNr;
		xmlNodePtr cur = result->nodesetval->nodeTab[0];

		qDebug() << "Deleting movie node with :" << size << "child nodes";
		// Unlink the node
		xmlUnlinkNode(cur->parent);

		// Free the unlinked node
		xmlFree(cur->parent);


	}

	// Save changes to file
	return xmlSaveFormatFileEnc(XML_DATA, doc, "UTF-8", 1);
}

static inline int updateChildNode(xmlDocPtr doc, xmlNodePtr cur, QString node, QString data)
{

	if(!(xmlStrcmp(cur->name, (const xmlChar *)QStringToCharPtr(node))))
	{
		// Update the value
		xmlNodeSetContent(cur, (const xmlChar *)QStringToCharPtr(data));

		xmlChar *key;
		key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);

		xmlFree(key);
	}

	return 0;
}

int ApplicationUI::updateNode(QString originalTitle, QString videoTitle, QString videoGenre,
		QString videoReleaseDate, QString videoDirector, QString videoPrice)
{
	int i;
	int ret;
	int size;
	char xpath_exp[1000];
	xmlDocPtr doc;
	xmlXPathContextPtr xpathp = NULL;
	xmlXPathObjectPtr result = NULL;

	///////////////////// Initialising XML document ///////////////////
	qDebug() << originalTitle << videoTitle << videoGenre << videoReleaseDate << videoDirector << videoPrice;

	xmlKeepBlanksDefault(0);
	doc = xmlReadFile(XML_DATA, NULL, 0);
	if (doc == NULL)
	{
		printf("could not parse file %s", XML_DATA);
	}

	///////////////////// Initialising XPATH ///////////////////

	xpathp = xmlXPathNewContext(doc);
	if (xpathp == NULL) {
		printf("Error in xmlXPathNewContext.");
		xmlXPathFreeContext(xpathp);
		return -1;
	}

	if(xmlXPathRegisterNs(xpathp, (const xmlChar *)NS_PREFIX, (const xmlChar *)NS_URL) != 0)
	{
		printf("Error: unable to register NS.");
		xmlXPathFreeContext(xpathp);
		return -1;
	}

	///////////////////// Evaluating XPATH expression ///////////////////

	sprintf(xpath_exp, "/video/message/database/movie[atitle='%s']/*", QStringToCharPtr(originalTitle));
	qDebug() << "Xpath expression " << xpath_exp;

	/* Evaluate xpath expression */
	result = xmlXPathEvalExpression((const xmlChar *)xpath_exp, xpathp);
	if (result == NULL)
	{
		printf("Error in xmlXPathEvalExpression.");
		xmlXPathFreeObject(result);
		xmlXPathFreeContext(xpathp);
		return -1;
	}


	///////////////////// Processing result ///////////////////

	if(xmlXPathNodeSetIsEmpty(result->nodesetval))
	{
		xmlXPathFreeObject(result);
		xmlXPathFreeContext(xpathp);
		printf("the id does not exist on the database\n");
		return -1;
	}
	else
	{
		size = result->nodesetval->nodeNr;
		xmlNodePtr cur = result->nodesetval->nodeTab[0];

		for(i = 0; i < size; i++)
		{
			// atitle, genre ... are node names from the XML
			updateChildNode(doc, cur, "atitle", videoTitle);
			updateChildNode(doc, cur, "genre", videoGenre);
			updateChildNode(doc, cur, "dateOfRelease", videoReleaseDate);
			updateChildNode(doc, cur, "director", videoDirector);
			updateChildNode(doc, cur, "price", videoPrice);

			cur = cur->next;
		}
	}

	// Dump to console
	// xmlSaveFormatFileEnc("-", doc, "UTF-8", 1);

	// Save changes to file
	ret = xmlSaveFormatFileEnc(XML_DATA, doc, "UTF-8", 1);
	xmlFreeDoc(doc);

	return ret;
}

static inline int addChildNode(xmlNodePtr movie_node, const char *node, QString qData)
{
	// QString to char pointer conversion
	QByteArray byteArray = qData.toUtf8();
	const char *data_string = byteArray.constData();

	if (data_string == NULL) {
		qDebug () << "Could not process the data value for: " << node;
		return -1;
	}

	// Add the new node
	xmlNewChild(movie_node , NULL, BAD_CAST node, (const xmlChar *) data_string);

	return 0;
}

int ApplicationUI::addNode(QString videoTitle, QString videoGenre,
		QString videoReleaseDate, QString videoDirector, QString videoPrice)
{
	int ret;
	xmlDocPtr doc;
	xmlNodePtr video_node, database_node, movie_node;

	qDebug() << videoTitle << videoGenre << videoReleaseDate << videoDirector << videoPrice;

	xmlKeepBlanksDefault(0);

	doc = xmlReadFile(XML_DATA, NULL, 0);
	if (doc == NULL) {
		printf("Failed to parse %s\n", XML_DATA);
		return -1;
	}

	video_node = xmlDocGetRootElement(doc);

	// Traversing to the database node

	database_node = video_node->children->children; //video/message/database

	// add item at database node
	movie_node = xmlNewChild(database_node , NULL, BAD_CAST "movie", BAD_CAST NULL);

	addChildNode(movie_node , "atitle",        videoTitle);
	addChildNode(movie_node , "genre",         videoGenre);
	addChildNode(movie_node , "dateOfRelease", videoReleaseDate);
	addChildNode(movie_node , "director",      videoDirector);
	addChildNode(movie_node , "price",         videoPrice);

	// Save new node to File
	ret = xmlSaveFormatFileEnc(XML_DATA, doc, "UTF-8", 1);

	// Dump to Console
	//xmlSaveFormatFileEnc("-", doc, "UTF-8", 1);

	xmlFreeDoc(doc);

	return ret;
}

int ApplicationUI::exportXML()
{
	int ret;
	xmlDocPtr doc = NULL;

	xmlKeepBlanksDefault(0);

	doc = xmlReadFile(XML_DATA, NULL, 0);
	if (doc == NULL) {
		printf("Failed to parse %s\n", XML_DATA);
		return -1;
	}

	// Export to disk
	ret = xmlSaveFormatFileEnc(XML_EXPORT, doc, "UTF-8", 1);
	xmlFreeDoc(doc);

	return ret;
}



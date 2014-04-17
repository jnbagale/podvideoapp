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

using namespace bb::cascades;


ApplicationUI::ApplicationUI(bb::cascades::Application *app) :
        		QObject(app)
{
	podObjSearcher = NULL;
	podObjResponder = NULL;


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
	podObjSearcher = this->initialiseSearcher();

	//set the value of searcher's unique id
	this->searcher_ID = "ID: " + QString::number(podObjSearcher->unique_id, 10);
	qDebug() << "Searcher ID:- " << this->searcher_ID << endl;

	// Initialise video responder

	podObjResponder = initialiseResponder();

	//set the value of responder's unique id
	this->responder_ID = "ID: " + QString::number(podObjResponder->unique_id, 10);
	qDebug() << "Responder ID:- " << this->responder_ID << endl;

	if(podObjSearcher != NULL) {

		// creating a QThread to run video searcher to receive search responses
		QThread* thread = new QThread;
		ReceiveThread* rt1= new ReceiveThread(this, podObjSearcher);

		rt1->moveToThread(thread);
		connect(thread, SIGNAL(started()), rt1, SLOT(process()));
		thread->start();

		// creating QThread to run video responder to process search requests
		QThread* thread_responder = new QThread;
		ResponderThread* rt_responder= new ResponderThread(this, podObjSearcher, podObjResponder);

		rt_responder->moveToThread(thread_responder);
		connect(thread_responder, SIGNAL(started()), rt_responder, SLOT(run_responder()));
		thread_responder->start();

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

QString ApplicationUI::getSearcherID()
{
	return this->searcher_ID;
}

QString ApplicationUI::getResponderID()
{
	return this->responder_ID;
}

packedobjectsdObject* ApplicationUI::initialiseSearcher()
{
	// Initialise packedobjectsd searcher
	if((podObjSearcher = init_packedobjectsd(XML_SCHEMA, SEARCHER)) == NULL) {
		qWarning() <<"failed to initialise libpackedobjectsd searcher";
		return NULL;
	}

	if(podObjSearcher != NULL) {
		qDebug()<<" Searcher initialisation complete.";
	}

	return podObjSearcher;
}

packedobjectsdObject* ApplicationUI::initialiseResponder()
{
	////////////////////// Initialising    ///////////////////

	// Initialise packedobjectsd responder
	if((podObjResponder = init_packedobjectsd(XML_SCHEMA, RESPONDER)) == NULL) {
		qWarning() <<"failed to initialise libpackedobjectsd";
		return NULL;
	}

	return podObjResponder;
}

int ApplicationUI::sendSearch(QString videoTitle, double maxPrice)
{

	xmlDocPtr doc_search;

	// Converting Qstring to char*
	QByteArray ba = videoTitle.toLocal8Bit();
	char *title_str = ba.data();

	doc_search = create_search(title_str, maxPrice);

	if(doc_search != NULL) {

			_sendSearch(this, podObjSearcher, doc_search);

		qDebug()<<"Search request sent...";
		return 1;
	}

	return -1;
}

ReceiveThread::ReceiveThread(ApplicationUI *appObject, packedobjectsdObject *podObjSearcher)
{
	app_Object = appObject;
	podObj_Searcher = podObjSearcher;
}

void ReceiveThread::process()
{
	qDebug()<<"Ready to receive search responses ...";
	_receiveResponse(app_Object, podObj_Searcher);
	emit finished();
}

ResponderThread::ResponderThread(ApplicationUI *appObject, packedobjectsdObject *podObjSearcher, packedobjectsdObject *podObjResponder)
{
	app_Object = appObject;
	podObj_Searcher = podObjSearcher;
	podObj_Responder = podObjResponder;

}

void ResponderThread::run_responder()
{
	qDebug()<<"Ready to receive search queries ...";
	start_responder(app_Object, podObj_Searcher, podObj_Responder);
	emit finished();
}
// Various functions to pass signals to QML
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

void ApplicationUI::setQuery()
{
	emit queryChanged();
}

void ApplicationUI::setSearchResponse()
{
	emit searchResponseChanged();
}

void ApplicationUI::setRecord()
{
	emit recordChanged();
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

	sprintf(xpath_exp, "/video/message/database/movie[title='%s']/*", QStringToCharPtr(originalTitle));

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
	qDebug() << "New Data "<< originalTitle << videoTitle << videoGenre << videoReleaseDate << videoDirector << videoPrice;

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

	sprintf(xpath_exp, "/video/message/database/movie[title='%s']/*", QStringToCharPtr(originalTitle));
	//qDebug() << "Xpath expression " << xpath_exp;

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
			// title, genre ... are node names from the XML
			updateChildNode(doc, cur, "title", videoTitle);
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

	this->recordChanged(); // send signal to QML to refresh the data model
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

	addChildNode(movie_node , "title",        videoTitle);
	addChildNode(movie_node , "genre",         videoGenre);
	addChildNode(movie_node , "dateOfRelease", videoReleaseDate);
	addChildNode(movie_node , "director",      videoDirector);
	addChildNode(movie_node , "price",         videoPrice);

	// Save new node to File
	ret = xmlSaveFormatFileEnc(XML_DATA, doc, "UTF-8", 1);

	// Dump to Console
	//xmlSaveFormatFileEnc("-", doc, "UTF-8", 1);
	this->recordChanged(); // send signal to QML to refresh the data model

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



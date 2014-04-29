/*
 * video_searcher.cpp
 *
 *  Created on: 1 Dec 2013
 *      Author: jivs
 */

/* Copyright (C) 2013 Jiva Bagale */

/* This program is free software: you can redistribute it and/or modify */
/* it under the terms of the GNU General Public License as published by */
/* the Free Software Foundation, either version 3 of the License, or */
/* (at your option) any later version. */

/* This program is distributed in the hope that it will be useful, */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the */
/* GNU General Public License for more details. */

// A simple program to broadcast video search to connected clients.
// Clients interested in the video then respond to this searcher

#include "video_searcher.h"


/* function prototypes */
int read_response(ApplicationUI *appObject, xmlDocPtr doc_response);

/* function definitions */

//////////////////////// RECEIVING SEARCH RESPONSE BACK //////////////////////////
int resetResponse()
{
	int ret;
	xmlDocPtr doc;
	xmlNodePtr video_node, message_node;

	xmlKeepBlanksDefault(0);

	doc = xmlNewDoc(BAD_CAST "1.0");

	/* create pod node as root node */
	video_node = xmlNewNode(NULL, BAD_CAST "video");
	xmlDocSetRootElement(doc, video_node);

	message_node = xmlNewChild(video_node, NULL, BAD_CAST "message", BAD_CAST NULL);
	xmlNewChild(message_node, NULL, BAD_CAST "response", BAD_CAST NULL);

	// Save blank XML RESPONSE to File
	ret = xmlSaveFormatFileEnc(XML_RESPONSE, doc, "UTF-8", 1);

	// Dump to Console
	//xmlSaveFormatFileEnc("-", doc, "UTF-8", 1);

	xmlFreeDoc(doc);
	return ret;
}

int addSearchResult(string responderID, string videoTitle, string videoPrice, string videoGenre,
		 string videoReleaseDate, string videoDirector)
{
	int ret;
	xmlDocPtr doc;
	xmlNodePtr video_node, response_node, movie_node;

	xmlKeepBlanksDefault(0);

	doc = xmlReadFile(XML_RESPONSE, NULL, 0);
	if (doc == NULL) {
		qDebug() << "Failed to parse " << XML_RESPONSE;
		return -1;
	}

	video_node = xmlDocGetRootElement(doc);

	// Traversing to the database node

	response_node = video_node->children->children; //video/message/response

	// add item at database node
	movie_node = xmlNewChild(response_node , NULL, BAD_CAST "movie", BAD_CAST NULL);

	xmlNewChild(movie_node , NULL, BAD_CAST "responderID",  BAD_CAST responderID.c_str());
	xmlNewChild(movie_node , NULL, BAD_CAST "title",         BAD_CAST videoTitle.c_str());
	xmlNewChild(movie_node , NULL, BAD_CAST "price",        BAD_CAST videoPrice.c_str());
	xmlNewChild(movie_node , NULL, BAD_CAST "genre",         BAD_CAST videoGenre.c_str());
	xmlNewChild(movie_node , NULL, BAD_CAST "dateOfRelease",BAD_CAST videoReleaseDate.c_str());
	xmlNewChild(movie_node , NULL, BAD_CAST "director",     BAD_CAST videoDirector.c_str());

	// Save new node to File
	ret = xmlSaveFormatFileEnc(XML_RESPONSE, doc, "UTF-8", 1);

	// Dump to Console
	//xmlSaveFormatFileEnc("-", doc, "UTF-8", 1);
	xmlFreeDoc(doc);

	return ret;
}

int read_response(ApplicationUI *appObject, xmlDocPtr doc_response)
{
	/* Declare variables */
	int i;
	int ret;
	int size;
	char *responder_id = NULL;
	char *movie_title = NULL;
	char *movie_price = NULL;
	char *movie_genre = NULL;
	char *movie_release_date = NULL;
	char *movie_director = NULL;

	char xpath_exp[1000];
	xmlXPathContextPtr xpathp = NULL;
	xmlXPathObjectPtr result = NULL;

	///////////////////// Initialising XPATH ///////////////////

	/* setup xpath context */
	xpathp = xmlXPathNewContext(doc_response);
	if (xpathp == NULL) {
		qDebug() << "Error in xmlXPathNewContext.";
		xmlXPathFreeContext(xpathp);
		return -1;
	}

	if(xmlXPathRegisterNs(xpathp, (const xmlChar *)NS_PREFIX, (const xmlChar *)NS_URL) != 0) {
		qDebug() << "Error: unable to register NS.";
		xmlXPathFreeContext(xpathp);
		return -1;
	}

	///////////////////// Evaluating XPATH expression ///////////////////

	sprintf(xpath_exp, "/video/message/response/*");

	result = xmlXPathEvalExpression((const xmlChar*)xpath_exp, xpathp);
	if (result == NULL) {
		qDebug() << "Error in xmlXPathEvalExpression.";
		xmlXPathFreeObject(result);
		xmlXPathFreeContext(xpathp);
		return -1;
	}

	///////////////////// Processing result ///////////////////

	/* check if xml doc consists of response data" */
	if(xmlXPathNodeSetIsEmpty(result->nodesetval)) {
		xmlXPathFreeObject(result);
		xmlXPathFreeContext(xpathp);
		qDebug() << "the response data is not in valid format";
		return -1;
	}
	else {
		size = result->nodesetval->nodeNr;
		xmlNodePtr cur = result->nodesetval->nodeTab[0];

		for(i = 0; i < size; i++)
		{
			if(!(xmlStrcmp(cur->name, (const xmlChar *)"responder-id"))) {
				xmlChar *key;
				key = xmlNodeListGetString(doc_response, cur->xmlChildrenNode, 1);
				responder_id = strdup((char *)key);
				xmlFree(key);
			}

			if(!(xmlStrcmp(cur->name, (const xmlChar *)"movie-title"))) {
				xmlChar *key;
				key = xmlNodeListGetString(doc_response, cur->xmlChildrenNode, 1);
				movie_title = strdup((char *)key);
				xmlFree(key);
			}

			if(!(xmlStrcmp(cur->name, (const xmlChar *)"price")))
			{
				xmlChar *key;
				key = xmlNodeListGetString(doc_response, cur->xmlChildrenNode, 1);
				movie_price = strdup((char *)key);
				xmlFree(key);
			}
			if(!(xmlStrcmp(cur->name, (const xmlChar *)"genre")))
			{
				xmlChar *key;
				key = xmlNodeListGetString(doc_response, cur->xmlChildrenNode, 1);
				movie_genre = strdup((char *)key);
				xmlFree(key);
			}
			if(!(xmlStrcmp(cur->name, (const xmlChar *)"dateOfRelease")))
			{
				xmlChar *key;
				key = xmlNodeListGetString(doc_response, cur->xmlChildrenNode, 1);
				movie_release_date = strdup((char *)key);
				xmlFree(key);
			}
			if(!(xmlStrcmp(cur->name, (const xmlChar *)"director")))
			{
				xmlChar *key;
				key = xmlNodeListGetString(doc_response, cur->xmlChildrenNode, 1);
				movie_director = strdup((char *)key);
				xmlFree(key);
			}

			cur = cur->next;
		}
		qDebug() << "********** search result details ***********";
		qDebug() <<"Responder ID:" << responder_id << "Title" << movie_title << "Price" << movie_price;

		// Add new result to the search.xml
		ret = addSearchResult(responder_id, movie_title, movie_price, movie_genre, movie_release_date, movie_director);

		if(ret!= -1) {
			appObject->setSearchResponse(); // Triggers searchResponseChanged() signal
		}
	}
	///////////////////// Freeing ///////////////////

	xmlXPathFreeObject(result);
	xmlXPathFreeContext(xpathp);

	return 1;
}

packedobjectsdObject *_receiveResponse(ApplicationUI *app_Object, packedobjectsdObject *podObj_Searcher)
{
	int ret;
	xmlDocPtr doc_response = NULL;
	clock_t start_t, end_t;

	///////////////////// Receiving search response ///////////////////
	while(1)
	{
		start_t = clock();

		if((doc_response = packedobjectsd_receive_response(podObj_Searcher)) == NULL) {
			qDebug() << "message could not be received";
		}

		end_t = clock();
		double cpu_time_elapsed = (double)end_t / CLOCKS_PER_SEC - (double)start_t / CLOCKS_PER_SEC ;

		qDebug() << "new search response received...";
		qDebug() << "Size of PO data" << podObj_Searcher->bytes_received << "Sized of Processed XML" << xml_doc_size(doc_response);
		qDebug() << "CPU time for Decode" << cpu_time_elapsed * 1000000.0 / 3000 << " microseconds";

		/* process the received response XML */
		if((ret = read_response(app_Object, doc_response)) != 1) {
			//xml_dump_doc(doc_response);
			qDebug() << "search response could not be processed ";
		}

		xmlFreeDoc(doc_response);
		usleep(1000);
	}

	return podObj_Searcher;
}



//////////////////////// SENDING SEARCH //////////////////////////


xmlDocPtr create_search(char *movie_title, double max_price)
{
	/* Declare variables */
	char maxPrice[32];
	xmlDocPtr doc_search = NULL;
	xmlNodePtr video_node = NULL, message_node = NULL, search_node = NULL;

	// converting price from double to string
	snprintf(maxPrice, 32, "%g", max_price);

	///////////////////// Creating Root and other parent nodes ///////////////////

	doc_search = xmlNewDoc(BAD_CAST "1.0");

	/* create pod node as root node */
	video_node = xmlNewNode(NULL, BAD_CAST "video");
	xmlDocSetRootElement(doc_search, video_node);

	message_node = xmlNewChild(video_node, NULL, BAD_CAST "message", BAD_CAST NULL);
	search_node = xmlNewChild(message_node, NULL, BAD_CAST "search", BAD_CAST NULL);

	///////////////////// Creating child elements inside response node ///////////////////

	/* create child elements to hold data */
	xmlNewChild(search_node, NULL, BAD_CAST "movie-title", BAD_CAST movie_title);
	xmlNewChild(search_node, NULL, BAD_CAST "max-price", BAD_CAST maxPrice);

	//xml_dump_doc(doc_search);
	return doc_search;
}

int _sendSearch(ApplicationUI *app_Object, packedobjectsdObject *podObjSearcher, xmlDocPtr doc_search)
{
	///////////////////// Sending search request ///////////////////
	int xml_size;
	int po_xml_size;
	char size_str[200];
	clock_t start_t, end_t;

	start_t = clock();

	if(packedobjectsd_send_search(podObjSearcher, doc_search) == -1){
		qDebug() << "message could not be sent";
		return -1;
	}

	xml_size = xml_doc_size(doc_search);
	po_xml_size = podObjSearcher->bytes_sent - 1;

	end_t = clock();
	double cpu_time_elapsed = (double)end_t / CLOCKS_PER_SEC - (double)start_t / CLOCKS_PER_SEC ;

	qDebug() << "Size of search XML" << xml_size << "Size PO Data" << po_xml_size;
	qDebug() << "CPU time for Encode" << cpu_time_elapsed * 1000000.0 / 3000 << " microseconds";

	sprintf(size_str, "Size of Search XML %d. Size PO Data %d", xml_size, po_xml_size);
	app_Object->setquerySize(QString(size_str));


	// reset previous search history
	//resetResponse(); // causes crash when search sent in quick succession
	return 0;
}



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

extern "C" {
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <packedobjectsd/packedobjectsd.h>
}

#include "video_searcher.h"

// XML namespace and url for xpath
#define NS_PREFIX "xs"
#define NS_URL "http://www.w3.org/2001/XMLSchema"

using namespace bb::cascades;

/* global variables */
#define XML_SCHEMA "app/native/video.xsd"
#define XML_DATA "app/native/video.xml"

/* function prototypes */
int read_response(ApplicationUI *app_object, xmlDocPtr doc_response);

/* function definitions */

//////////////////////// RECEIVING SEARCH RESPONSE BACK //////////////////////////


int read_response(ApplicationUI *app_object, xmlDocPtr doc_response)
{
	/* Declare variables */
	int i;
	int size;
	double movie_price = 0.0;
	char *responder_id = NULL;
	char *movie_title = NULL;
	char xpath_exp[1000];
	xmlXPathContextPtr xpathp = NULL;
	xmlXPathObjectPtr result = NULL;

	///////////////////// Initialising XPATH ///////////////////

	/* setup xpath context */
	xpathp = xmlXPathNewContext(doc_response);
	if (xpathp == NULL) {
		printf("Error in xmlXPathNewContext.");
		xmlXPathFreeContext(xpathp);
		return -1;
	}

	if(xmlXPathRegisterNs(xpathp, (const xmlChar *)NS_PREFIX, (const xmlChar *)NS_URL) != 0) {
		printf("Error: unable to register NS.");
		xmlXPathFreeContext(xpathp);
		return -1;
	}

	///////////////////// Evaluating XPATH expression ///////////////////

	sprintf(xpath_exp, "/video/message/response/*");

	result = xmlXPathEvalExpression((const xmlChar*)xpath_exp, xpathp);
	if (result == NULL) {
		printf("Error in xmlXPathEvalExpression.");
		xmlXPathFreeObject(result);
		xmlXPathFreeContext(xpathp);
		return -1;
	}

	///////////////////// Processing result ///////////////////

	/* check if xml doc consists of response data" */
	if(xmlXPathNodeSetIsEmpty(result->nodesetval)) {
		xmlXPathFreeObject(result);
		xmlXPathFreeContext(xpathp);
		printf("the response data is not in valid format\n");
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
				movie_price = atof((char *)key);
				xmlFree(key);
			}

			cur = cur->next;
		}
		printf("\n********** search result details ***********\n");
		printf("Responder ID: %s \n", responder_id);
		printf("Movie title: %s \n", movie_title);
		printf("Movie price: %g\n", movie_price);

		// set label on qml to the search result
		app_object->setTitle((QString) movie_title);
		app_object->setPrice(movie_price);
	}
	///////////////////// Freeing ///////////////////

	xmlXPathFreeObject(result);
	xmlXPathFreeContext(xpathp);

	return 1;
}

packedobjectsdObject *_receiveResponse(ApplicationUI *app_object, packedobjectsdObject *pod_obj)
{
	int ret;
	xmlDocPtr doc_response = NULL;

	///////////////////// Receiving search response ///////////////////
	while(1)
	{
		if((doc_response = packedobjectsd_receive_response(pod_obj)) == NULL) {
			printf("message could not be received\n");
			//exit(EXIT_FAILURE);
		}

		printf("\nnew search response received... \n");
		//root->setProperty("tempText", "receive a response.");

		/* process the received response XML */
		if((ret = read_response(app_object, doc_response)) != 1) {
			//xml_dump_doc(doc_response);
			printf("search response could not be processed \n");
		}

		xmlFreeDoc(doc_response);
		usleep(1000);
	}

	return pod_obj;
}



//////////////////////// SENDING SEARCH //////////////////////////


xmlDocPtr create_search(packedobjectsdObject *pod_obj, char *movie_title, double max_price)
{
	/* Declare variables */
	char maxPrice[32];
	xmlDocPtr doc_search = NULL;
	xmlNodePtr video_node = NULL, message_node = NULL, search_node = NULL;

	LIBXML_TEST_VERSION;

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

	xml_dump_doc(doc_search);
	// xmlFreeDoc(doc_search);
	return doc_search;
}

int _sendSearch(packedobjectsdObject *pod_object1, xmlDocPtr doc_search)
{
	///////////////////// Sending search request ///////////////////


	if(packedobjectsd_send_search(pod_object1, doc_search) == -1){
		printf("message could not be sent\n");
		return -1;
	}

	printf("search request sent to responders...\n");

	return 1;
}

packedobjectsdObject *_initialiseSearcher()
{
	packedobjectsdObject *pod_obj = NULL;

	printf("///////////////////// VIDEO SEARCHER  /////////////////// \n");
	////////////////////// Initialising    ///////////////////

	// Initialise packedobjectsd
	if((pod_obj = init_packedobjectsd(XML_SCHEMA, SEARCHER)) == NULL) {
		printf("failed to initialise libpackedobjectsd\n");
	}

	return pod_obj;
}




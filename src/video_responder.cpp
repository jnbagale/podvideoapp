/*
 * video_responder.cpp
 *
 *  Created on: 27 Nov 2013
 *      Author: jivs
 */
/*
 * Copyright (c) 2011-2013 Jiva Nath Bagale {jnbagale@gmail.com}
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

extern "C" {
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <packedobjectsd/packedobjectsd.h>
}

#include "video_responder.h"

// XML namespace and url for xpath
#define NS_PREFIX "xs"
#define NS_URL "http://www.w3.org/2001/XMLSchema"

#define XML_DATA "app/native/video.xml"
#define XML_SCHEMA "app/native/video.xsd"

/**
 * Use the PID to set the window group id.
 */

/* function prototypes */
void send_response(ApplicationUI *app_object, packedobjectsdObject *pod_obj, char *movie_title, double price, char *genre, char *dateofrelease, char *director);
int create_response(ApplicationUI *app_object, packedobjectsdObject *pod_obj, char *movie_title, double max_price);
int process_search(ApplicationUI *app_object, packedobjectsdObject *pod_obj_searcher, packedobjectsdObject *pod_obj, xmlDocPtr search);

/* function definitions */
void send_response(ApplicationUI *app_object, packedobjectsdObject *pod_obj, char *movie_title, double price, char *genre, char *dateofrelease, char *director)
{
	/* Declare variables */
	int xml_size;
	int po_xml_size;
	char size_str[200];
	char price_string[50];
	char id_string[100];
	xmlDocPtr doc_response = NULL;
	xmlNodePtr video_node = NULL, message_node = NULL, response_node = NULL;

	///////////////////// Creating Root and other parent nodes ///////////////////

	doc_response = xmlNewDoc(BAD_CAST "1.0");

	/* create pod node as root node */
	video_node = xmlNewNode(NULL, BAD_CAST "video");
	xmlDocSetRootElement(doc_response, video_node);

	message_node = xmlNewChild(video_node, NULL, BAD_CAST "message", BAD_CAST NULL);
	response_node = xmlNewChild(message_node, NULL, BAD_CAST "response", BAD_CAST NULL);

	///////////////////// Creating child elements inside response node ///////////////////

	/* create child elements to hold data */
	sprintf(price_string,"%g", price);
	sprintf(id_string,"%lu", pod_obj->unique_id);
	xmlNewChild(response_node, NULL, BAD_CAST "responder-id", BAD_CAST id_string);
	xmlNewChild(response_node, NULL, BAD_CAST "movie-title", BAD_CAST movie_title);
	xmlNewChild(response_node, NULL, BAD_CAST "price", BAD_CAST price_string);
	xmlNewChild(response_node, NULL, BAD_CAST "genre", BAD_CAST genre);
	xmlNewChild(response_node, NULL, BAD_CAST "dateOfRelease", BAD_CAST dateofrelease);
	xmlNewChild(response_node, NULL, BAD_CAST "director", BAD_CAST director);

	///////////////////// Sending response to the searcher ///////////////////
	//xml_dump_doc(doc_response);

	/* send the response doc to the searcher */
	if(packedobjectsd_send_response(pod_obj, doc_response) == -1){
		printf("message could not be sent\n");
		exit(EXIT_FAILURE);
	}

	xml_size = xml_doc_size(doc_response);
	printf("size of search XML %d\n", xml_size);
	po_xml_size = pod_obj->bytes_sent - 1;

	sprintf(size_str, "Size of Response XML %d. Size PO Data %d", xml_size, po_xml_size);

	app_object->setSize1(QString(size_str));

	printf("response sent to the searcher...\n");
	//xml_dump_doc(doc_response);

	xmlFreeDoc(doc_response);
}

int create_response(ApplicationUI *app_object, packedobjectsdObject *pod_obj, char *movie_title, double max_price)
{
	/* Declare variables */
	int i;
	int size;
	double price = 0.0;
	char *title = NULL;
	char *genre = NULL;
	char *dateofrelease = NULL;
	char *director = NULL;
	char xpath_exp[1000];
	xmlDocPtr doc_database = NULL;

	printf("checking in video database...\n");
	///////////////////// Initialising XML document ///////////////////

	if((doc_database = xml_new_doc(XML_DATA)) == NULL) {
		printf("did not find database.xml file");
		exit(EXIT_FAILURE);
	}

	xmlXPathContextPtr xpathp = NULL;
	xmlXPathObjectPtr result = NULL;

	///////////////////// Initialising XPATH ///////////////////

	/* setup xpath context */
	xpathp = xmlXPathNewContext(doc_database);
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

	sprintf(xpath_exp, "/video/message/database/movie[title='%s']/*", movie_title);

	/* Evaluate xpath expression */
	result = xmlXPathEvalExpression((const xmlChar *)xpath_exp, xpathp);
	if (result == NULL) {
		printf("Error in xmlXPathEvalExpression.");
		xmlXPathFreeObject(result);
		xmlXPathFreeContext(xpathp);
		return -1;
	}

	///////////////////// Processing result ///////////////////

	/* check if xml doc consists of data with title matching value from movie_title variable" */
	if(xmlXPathNodeSetIsEmpty(result->nodesetval)) {
		xmlXPathFreeObject(result);
		xmlXPathFreeContext(xpathp);
		printf("the movie does not exist on the database\n");
		return -1;
	}
	else {
		size = result->nodesetval->nodeNr;
		xmlNodePtr cur = result->nodesetval->nodeTab[0];

		for(i = 0; i < size; i++)
		{
			if(!(xmlStrcmp(cur->name, (const xmlChar *)"genre")))
			{
				xmlChar *key;
				key = xmlNodeListGetString(doc_database, cur->xmlChildrenNode, 1);
				genre = strdup((char *)key);
				xmlFree(key);
			}
			if(!(xmlStrcmp(cur->name, (const xmlChar *)"price")))
			{
				xmlChar *key;
				key = xmlNodeListGetString(doc_database, cur->xmlChildrenNode, 1);
				price = atof((char *)key);
				xmlFree(key);
			}
			if(!(xmlStrcmp(cur->name, (const xmlChar *)"dateOfRelease")))
			{
				xmlChar *key;
				key = xmlNodeListGetString(doc_database, cur->xmlChildrenNode, 1);
				dateofrelease = strdup((char *)key);
				xmlFree(key);
			}
			if(!(xmlStrcmp(cur->name, (const xmlChar *)"director")))
			{
				xmlChar *key;
				key = xmlNodeListGetString(doc_database, cur->xmlChildrenNode, 1);
				director = strdup((char *)key);
				xmlFree(key);
			}
			cur = cur->next;
		}

		///////////////////// Comparing search data with video database ///////////////////

		/* compare max price from search with price from database */

		if(price <= max_price) {
			printf("the movie exists on the database and matches price limit\n");

			///////////////////// Sending  search response ///////////////////

			/* send response to searcher */
			send_response(app_object, pod_obj, movie_title, price, genre, dateofrelease, director);
		}
		else {
			printf("the movie exists on the database but does not match price limit\n");
		}
	}

	///////////////////// Freeing ///////////////////

	free(title);
	xmlFreeDoc(doc_database);
	return 1;
}

int process_search(ApplicationUI *app_object, packedobjectsdObject *pod_obj_searcher, packedobjectsdObject *pod_obj, xmlDocPtr doc_search)
{
	/* Declare variables */
	int i;
	int ret;
	int size;
	double max_price = 0.0;
	char *movie_title = NULL;
	char xpath_exp[1000];
	xmlXPathContextPtr xpathp = NULL;
	xmlXPathObjectPtr result = NULL;

	///////////////////// Initialising XPATH ///////////////////

	/* setup xpath context */
	xpathp = xmlXPathNewContext(doc_search);
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

	sprintf(xpath_exp, "/video/message/search/*"); // xpath expression which should return the movie-title and max-price

	result = xmlXPathEvalExpression((const xmlChar *)xpath_exp, xpathp);
	if (result == NULL) {
		printf("Error in xmlXPathEvalExpression.");
		xmlXPathFreeObject(result);
		xmlXPathFreeContext(xpathp);
		return -1;
	}

	///////////////////// Processing search broadcast ///////////////////

	/* the xml doc matches "/video/message/search/asterik(*)" */
	if(xmlXPathNodeSetIsEmpty(result->nodesetval)) {
		xmlXPathFreeObject(result);
		xmlXPathFreeContext(xpathp);
		printf("the search request is not in valid format\n");
		return -1;
	}
	else {
		size = result->nodesetval->nodeNr;
		xmlNodePtr cur = result->nodesetval->nodeTab[0];

		for(i = 0; i < size; i++)
		{
			if(!(xmlStrcmp(cur->name, (const xmlChar *)"movie-title")))
			{
				xmlChar *key;
				key = xmlNodeListGetString(doc_search, cur->xmlChildrenNode, 1);
				movie_title = strdup((char *)key);
				xmlFree(key);
			}

			if(!(xmlStrcmp(cur->name, (const xmlChar *)"max-price")))
			{
				xmlChar *key;
				key = xmlNodeListGetString(doc_search, cur->xmlChildrenNode, 1);
				max_price = atof((char *)key);
				xmlFree(key);
			}
			//printf("cur- name %s \n", cur->name);
			cur = cur->next;
		}
	}

	printf("\n************** search request details **************\n");
	printf("Movie title: %s \n", movie_title);
	printf("Maximum price: %g\n", max_price);
	printf("Searcher's id:- %lu\n\n", pod_obj->last_searcher);

	// Compare the id with local SEARCHER ID and ignore

	if(pod_obj_searcher->unique_id == pod_obj->last_searcher) {
		printf("This is a local searcher request so ignoring this request!\n");
		ret = 1;
	}
	else {
		///////////////////// Checking on database ///////////////////

		/* checking if search broadcast matches record on the database */
		ret = create_response(app_object, pod_obj, movie_title, max_price);

		// Update search queries on GUI
		char temp_string[200];
		sprintf(temp_string, "%lu    %s    %g", pod_obj->last_searcher, movie_title, max_price);

		// set label on qml to the search result
		app_object->setQuery((QString) temp_string);

	}
	///////////////////// Freeing ///////////////////

	free(movie_title);
	xmlXPathFreeObject(result);
	xmlXPathFreeContext(xpathp);

	return ret;
}

packedobjectsdObject *_initialiseResponder()
{
	packedobjectsdObject *pod_obj = NULL;

	printf("///////////////////// VIDEO RESPONDER  /////////////////// \n");
	////////////////////// Initialising    ///////////////////

	// Initialise packedobjectsd
	if((pod_obj = init_packedobjectsd(XML_SCHEMA, RESPONDER)) == NULL) {
		printf("failed to initialise libpackedobjectsd\n");
		return NULL;
	}

	return pod_obj;
}

int start_responder(packedobjectsdObject *pod_obj_searcher, ApplicationUI *app_object, packedobjectsdObject *pod_obj)
{
	/* Declare variables */
	xmlDocPtr doc_search = NULL;


	while (1) {
		/* waiting for search broadcast */
		printf("waiting for new search broadcast\n");
		if((doc_search = packedobjectsd_receive_search(pod_obj)) == NULL) {
			printf("message could not be received\n");
			//exit(EXIT_FAILURE);
		}

		printf("\nnew search broadcast received... \n");


		///////////////////// Processing search broadcast ///////////////////

		/* process search broadcast to retrieve search details */
		process_search(app_object, pod_obj_searcher, pod_obj, doc_search);
		xmlFreeDoc(doc_search);

	}

	///////// Freeing ///////////////////

	/* free memory created by packedobjectsd but we should never reach here! */
	free_packedobjectsd(pod_obj);

	return 0;
}






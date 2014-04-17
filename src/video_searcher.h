/*
 * video_searcher.h
 *
 *  Created on: 1 Dec 2013
 *      Author: jivs
 */

#ifndef VIDEO_SEARCHER_H_
#define VIDEO_SEARCHER_H_

#include "applicationui.hpp"


packedobjectsdObject *_initialiseSearcher();
int _sendSearch(ApplicationUI *appObject, packedobjectsdObject *podObjSearcher, xmlDocPtr doc_search);
xmlDocPtr create_search(char *movie_title, double max_price);
packedobjectsdObject *_receiveResponse(ApplicationUI *app_Object, packedobjectsdObject *podObj_Searcher);

#endif /* VIDEO_SEARCHER_H_ */

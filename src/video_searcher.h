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
int _sendSearch(ApplicationUI *app_object, packedobjectsdObject *pod_object1, xmlDocPtr doc_search);
xmlDocPtr create_search(packedobjectsdObject *pod_obj, char *movie_title, double max_price);
packedobjectsdObject *_receiveResponse(ApplicationUI *app_object, packedobjectsdObject *pod_obj);

#endif /* VIDEO_SEARCHER_H_ */

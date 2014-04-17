/*
 * Copyright (c) 2011-2013 BlackBerry Limited.
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

import bb.cascades 1.2

Page {
    property alias queryPageTitle: pageTitleBar.title
    property alias searcherIDText: responderID.value
    property alias videoTitleText: videoTitle.value
    property alias videoPriceText: videoPrice.value
    
    //! [1]
    titleBar: TitleBar {
        id: pageTitleBar
    }
    //![1]
    
    Container {
        ViewerField {
            id:responderID
            horizontalAlignment: HorizontalAlignment.Fill
            title: qsTr("Responder ID")
            // value: to be set from main.qml
        }
        ViewerField {
            id:videoTitle
            horizontalAlignment: HorizontalAlignment.Fill
            title: qsTr("Video Title")
            // value: to be set from main.qml
        }
        ViewerField {
            id:videoPrice
            horizontalAlignment: HorizontalAlignment.Fill
            title: qsTr("Maximum Price")
            // value: to be set from main.qml
        }

    }
}
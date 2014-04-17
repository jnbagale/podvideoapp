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
    property alias itemPageTitle: pageTitleBar.title
    property alias videoTitleText: videoTitle.value
    property alias videoGenreText: videoGenre.value
    property alias videoReleaseDateText: videoReleaseDate.value
    property alias videoDirectorText: videoDirector.value
    property alias videoPriceText: videoPrice.value
    
    //! [1]
    titleBar: TitleBar {
        id: pageTitleBar
        
        // The 'Create/Save' action
        acceptAction: ActionItem {
            title: ( qsTr ("Update" ))
            imageSource: "asset:///images/update.png"
            
            onTriggered: {
                // Call libxml2 to save updated data
                console.log("Updating changes to XML File");  
                var ret;           
                
                ret = appObject.updateNode(pageTitleBar.title, videoTitle.value.toLowerCase(),
                videoGenre.value.toLowerCase(), videoReleaseDate.value,
                videoDirector.value.toLowerCase(), videoPrice.value);
                
                if(ret == -1) {
                    myDialogbox2.dialogMessage = "Could not update XML node data";
                }
                else {
                    myDialogbox2.dialogMessage = "Updated xml node data successfully";
                }
                
                myDialogbox2.open();          
                //recordsNavPane.pop()
            }
        }
    }
    //![1]
    
    Container {
        ViewerField {
            id:videoTitle
            horizontalAlignment: HorizontalAlignment.Fill
            title: qsTr("Video Title")
            // value: to be set from main.qml
        }
        ViewerField {
            id:videoGenre
            horizontalAlignment: HorizontalAlignment.Fill
            title: qsTr("Genre")
            // value: to be set from main.qml
        }
        ViewerField {
            id:videoReleaseDate
            horizontalAlignment: HorizontalAlignment.Fill
            title: qsTr("Date of Release")
            // value: to be set from main.qml
        }
        ViewerField {
            id:videoDirector
            horizontalAlignment: HorizontalAlignment.Fill
            title: qsTr("Director")
            // value: to be set from main.qml
        }
        ViewerField {
            id:videoPrice
            horizontalAlignment: HorizontalAlignment.Fill
            title: qsTr("Price")
            // value: to be set from main.qml
        }

    }
    attachedObjects: [
        DialogBox {
            id:myDialogbox2
        }    
    ]
}

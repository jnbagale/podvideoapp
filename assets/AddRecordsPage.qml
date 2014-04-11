import bb.cascades 1.2

Page {
    //! [1]
    titleBar: TitleBar {
        id: pageTitleBar
        
        // The 'Create/Save' action
        acceptAction: ActionItem {
            title: ( qsTr ("Add" ))
            
            onTriggered: {
                var ret;
                // Call libxml2 to save updated data
                console.log("Adding new data to XML File");                  
                ret = appObject.addNode(videoTitle.value,
                videoGenre.value, videoReleaseDate.value,
                videoDirector.value, videoPrice.value); 
                // TO DO:
                // # MAP DATA INPUT TO RESTRICTION FROM SCHEMA!
                if(ret == -1) {
                    myDialogbox3.dialogMessage = "Could not add XML node data";
                }
                else {
                    myDialogbox3.dialogMessage = "Added xml node data successfully";
                    clearAction.triggered();
                }
                
                myDialogbox3.open();
            }
        }
        
        // The 'Clear' action
        dismissAction: ActionItem {
            id:clearAction
            title: qsTr ("Clear")
            
            onTriggered: {
                videoTitle.value = ""
                videoGenre.value = ""
                videoReleaseDate.value = ""
                videoDirector.value = ""
                videoPrice.value = ""
            }
        
        }
    }
    //![1]
    Container {
        ViewerField {
            id:videoTitle
            horizontalAlignment: HorizontalAlignment.Fill
            title: qsTr("Video Title")
        
        
        }
        ViewerField {
            id:videoGenre
            horizontalAlignment: HorizontalAlignment.Fill
            title: qsTr("Genre")
        
        }
        ViewerField {
            id:videoReleaseDate
            horizontalAlignment: HorizontalAlignment.Fill
            title: qsTr("Date of Release")
        
        }
        ViewerField {
            id:videoDirector
            horizontalAlignment: HorizontalAlignment.Fill
            title: qsTr("Director")
        
        }
        ViewerField {
            id:videoPrice
            horizontalAlignment: HorizontalAlignment.Fill
            title: qsTr("Price")
        
        }
        
        attachedObjects: [
            DialogBox {
                id:myDialogbox3
            } 
        ]
    }
}

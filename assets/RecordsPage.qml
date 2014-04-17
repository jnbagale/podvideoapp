import bb.data 1.0
import bb.cascades 1.2

NavigationPane {
    id: recordsNavPane
    Page {
        
        Container {
            Label {
                // Localized text with the dynamic translation and locale updates support
                text: qsTr("View and Update records") + Retranslate.onLocaleOrLanguageChanged
                textStyle.base: SystemDefaults.TextStyles.BodyText
            }
            
            ListView {
                id: listViewRecords
                dataModel: recordsDataModel
                // Set the multiSelectAction property so that an action to enable
                // multiple selection mode appears in the context menu of each
                // list item when pressed for a longer period
                multiSelectAction: MultiSelectActionItem {
                } // opens multi select menu on a long press
                
                function refreshDataModel(){
                    recordsDataModel.clear();
                    dataSource.load();
                }
                
                multiSelectHandler {
                    status: "None selected"
                    actions: [
                        // Add the actions that should appear on the context menu
                        // when multiple selection mode is enabled
                        
                        DeleteActionItem {
                            title: "Delete"
                            onTriggered: {
                                console.log("Delete trigerred");  
                                var ret;
                                var selectedItems = listViewRecords.selectionList();
                                
                                for (var count = 0; count < selectedItems.length; count++) {
                                    var currentItem = recordsDataModel.data(selectedItems[count]);
                                    console.log("Deleting record for" + currentItem.title);
                                    
                                    ret = appObject.deleteNode(currentItem.title, currentItem.title,
                                    currentItem.genre, currentItem.dateOfRelease,
                                    currentItem.director, currentItem.price);
                                    
                                    if(ret == -1) {
                                        console.log("Could not delete XML node data");
                                    }
                                    else {
                                        console.log( "Deleted xml node data successfully");
                                    }
                                } 
                                listViewRecords.refreshDataModel(); // reload data model
                            }
                        }
                    ]
                    
                    // When multiple selection mode is enabled or disabled, update
                    // the label accordingly
                    onActiveChanged: {
                        if (active == true) {
                            console.log("Multiple selection mode is enabled.");
                        } else {
                            console.log("Multiple selection mode is disabled");
                        }
                    }
                }  
                
                // When a list item is selected or deselected, update the status text
                // to reflect the number of items that are currently selected
                onSelectionChanged: {
                    if (selectionList().length > 1) {
                        multiSelectHandler.status = selectionList().length +
                        " items selected";
                    } else if (selectionList().length == 1) {
                        multiSelectHandler.status = "1 item selected";
                    } else {
                        multiSelectHandler.status = "None selected";
                    }
                }
                listItemComponents: [
                    ListItemComponent {
                        type: "item"
                        
                        // Use a standard list item to display the data in the data model                   
                        StandardListItem {
                            title: ListItemData.title
                            description: ListItemData.price
                            contextActions: [
                                ActionSet {
                                }
                            ]
                        } 
                    }
                
                ]
                
                
                onTriggered: {
                    
                    if (indexPath.length > 1) {
                        
                        var chosenItem = recordsDataModel.data(indexPath);
                        
                        var contentpage = itemPageDefinition.createObject();
                        
                        contentpage.itemPageTitle = chosenItem.title;
                        contentpage.videoTitleText = chosenItem.title;
                        contentpage.videoGenreText = chosenItem.genre;
                        contentpage.videoReleaseDateText = chosenItem.dateOfRelease;
                        contentpage.videoDirectorText = chosenItem.director;
                        contentpage.videoPriceText = chosenItem.price;
                        
                        recordsNavPane.push(contentpage);
                    }
                }
                onCreationCompleted: {
                    appObject.recordChanged.connect(onCPPrecordChanged)
                }
                
                function onCPPrecordChanged()
                {
                   refreshDataModel();
                }
                
            
            } // ListviewRecords
            
            attachedObjects: [
                GroupDataModel {
                    id: recordsDataModel
                    sortingKeys: ["title"]
                    sortedAscending: true
                    grouping: ItemGrouping.ByFirstChar
                },
                DataSource {
                    id: dataSource
                    source: "video.xml"
                    query: "/video/message/database/movie"
                    
                    
                    onDataLoaded: {
                        if (data[0] == undefined) {
                            // The data returned is not a list, just one QVariantMap.
                            // Use insert to add one element.
                            console.log("Inserting one element");
                            recordsDataModel.insert(data)
                        } else {
                            //The data returned is a list. Use insertList.
                            console.log("Inserting list element of " + data.length + " items");
                            //console.log (data[1]);
                            recordsDataModel.insertList(data);
                        }
                    }
                },
                DialogBox {
                    id:myDialogbox1
                }                       
            ]
            
            
            // for listviewRecords
            onCreationCompleted: {
                dataSource.load(); 
            }   
        } // Container
        
        actions: [
            
            ActionItem {
                title: "Export Records"
                imageSource: "asset:///images/share.png"
                ActionBar.placement: ActionBarPlacement.InOverflow
                onTriggered: {
                    // Call libxml2 to export saved XML data
                    var ret;
                    console.log("Exporting XML File to 'file:///accounts/1000/shared/misc/'");             
                    
                    ret = appObject.exportXML();
                    
                    if(ret == -1) {
                        myDialogbox1.dialogMessage = "Could not export XML data";
                    }
                    else {
                        myDialogbox1.dialogMessage = "Exported XML data successfully";
                    }
                    myDialogbox1.open();
                }
            },
            ActionItem {
                title: "Add Records"
                imageSource: "asset:///images/create.png"
                ActionBar.placement: ActionBarPlacement.InOverflow
                onTriggered: {
                    var contentpage = addRecordsPageDefinition.createObject();
                    recordsNavPane.push(contentpage);                        
                }
            
            },
            ActionItem {
                title: "Refresh"
                imageSource: "asset:///images/refresh.png"
                ActionBar.placement: ActionBarPlacement.InOverflow
                onTriggered: {
                    listViewRecords.refreshDataModel();// reload the data model 
                }
            
            },
            MultiSelectActionItem {
                multiSelectHandler: listViewRecords.multiSelectHandler
                onTriggered: {
                    listViewRecords.multiSelectHandler.active = true;
                }
            }
        ]  
    } // Page 
    
    attachedObjects: [
        ComponentDefinition {
            id: itemPageDefinition
            source: "ItemDetailsPage.qml"
        },
        ComponentDefinition {
            id: addRecordsPageDefinition
            source: "AddRecordsPage.qml"
        }  
    ]
    
    onPopTransitionEnded: {
        // Transition is done destroy the Page to free up memory.
        page.destroy();
    }           
} // NavigationPane

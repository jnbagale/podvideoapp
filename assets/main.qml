/* Copyright (C) 2013 Jiva Bagale */

/* This program is free software: you can redistribute it and/or modify */
/* it under the terms of the GNU General Public License as published by */
/* the Free Software Foundation, either version 3 of the License, or */
/* (at your option) any later version. */

/* This program is distributed in the hope that it will be useful, */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the */
/* GNU General Public License for more details. */

import bb.data 1.0
import bb.cascades 1.2

TabbedPane{
    property string exportResult; // to be used with dialog box
    id: tabPane
    //showTabsOnActionBar: true
    
    onCreationCompleted: {
        tabPane.activeTab = searcherTab;
    }

    
    Tab{
        id:searcherTab
        title: "Searcher"
        // The first page, which appears at startup 
        Page {
            id: searcherPage
            ControlDelegate {
                id: controlDelegate1
                source: "SearcherPage.qml"
                delegateActive: (tabPane.activeTab == searcherTab)
            }
            
            onCreationCompleted: {
            }
        }
    }
    Tab {
        id:responderTab
        title: "Responder"
        
        Page {
            id: responderPage
            ControlDelegate {
                id: controlDelegate2
                source: "ResponderPage.qml"
                delegateActive: (tabPane.activeTab == responderTab)
            }
            
            onCreationCompleted: {
            
            }        
        }
    }

    Tab {   
        id: viewTab
        title: "View/Update"
        
        NavigationPane {
            id: navPane
            Page {
                Container {
                    Label {
                        // Localized text with the dynamic translation and locale updates support
                        text: qsTr("View and Update records") + Retranslate.onLocaleOrLanguageChanged
                        textStyle.base: SystemDefaults.TextStyles.BodyText
                    }
                    
                    ListView {
                        id: listView1
                        dataModel: dataModel
                        
                        onTriggered: {
                            
                            if (indexPath.length > 1) {
                                
                                //console.log(dataModel.data(indexPath));
                                var chosenItem = dataModel.data(indexPath);
                                
                                var contentpage = itemPageDefinition.createObject();
                                
                                contentpage.itemPageTitle = chosenItem.atitle;
                                contentpage.videoTitleText = chosenItem.atitle;
                                contentpage.videoGenreText = chosenItem.genre;
                                contentpage.videoReleaseDateText = chosenItem.dateOfRelease;
                                contentpage.videoDirectorText = chosenItem.director;
                                contentpage.videoPriceText = chosenItem.price;
                                
                                navPane.push(contentpage);
                            }
                        }
                    
                    } // Listview 1
                    
                    attachedObjects: [
                        GroupDataModel {
                            id: dataModel
                            sortingKeys: ["title"]
                        },
                        DataSource {
                            id: dataSource
                            source: "video.xml"
                            query: "/video/message/database/movie"
                            
                            
                            onDataLoaded: {
                                if (data[0] == undefined) {
                                    // The data returned is not a list, just one QVariantMap.
                                    // Use insert to add one element.
                                    //console.log("Inserting one element");
                                    dataModel.insert(data)
                                } else {
                                    //The data returned is a list. Use insertList.
                                    console.log("Inserting list element of " + data.length + " items");
                                    //console.log (data[1]);
                                    dataModel.insertList(data);
                                }
                            }
                        },
                        DialogBox {
                           id:myDialogbox1
                        }                       
                    ]
                    
                    
                    // for listview1
                    onCreationCompleted: { dataSource.load(); }   
                } // Container
                
                actions: [
                    ActionItem {
                        title: "Export XML"
                        ActionBar.placement: ActionBarPlacement.InOverflow
                        onTriggered: {
                            // Call libxml2 to export saved data
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
                    }
                ]
            } // Page 
            
            attachedObjects: [
                ComponentDefinition {
                    id: itemPageDefinition
                    source: "ItemDetailsPage.qml"
                }
            ]
            
            onPopTransitionEnded: {
                // Transition is done destroy the Page to free up memory.
                page.destroy();
                
                // TODO Reload only when something changes 
                
                // Clear the Data Model
                dataModel.clear();
                // Reload the Data Source
                dataSource.load();
            } 
        } // NavigationPane
    } // View/ Update Tab
    
    Tab {
        id: addTab
        title: "Add New"
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
                        }
                        
                        myDialogbox3.open();
                        
                        //RELOAD THE DATA SOURCE
                        dataModel.clear();
                        dataSource.load();
                        
                        // Go to View Tab
                        tabPane.activeTab = viewTab;
                    }
                }
                
                // The 'Cancel' action
                dismissAction: ActionItem {
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
                
                TextArea {
                    text:"ToDo:
                    Check data restriction as defined by the XML schema.
                    e.g. Genre is a enumeration type so only accepts certain values
                    "
                }
                attachedObjects: [
                    DialogBox {
                        id:myDialogbox3
                    } 
                ]
            }
        }
    
    
    }

}
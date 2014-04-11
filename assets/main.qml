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
        title: "Records"
        
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
                        
                        listItemComponents: [
                            ListItemComponent {
                                type: "item"
                                
                                // Use a standard list item to display the data in the data
                                // model
                                StandardListItem {
                                    title: ListItemData.title
                                    description: ListItemData.price
                                    }
                            }
                        ]
                        
                        
                        onTriggered: {
                            
                            if (indexPath.length > 1) {
                                
                                var chosenItem = dataModel.data(indexPath);
                                
                                var contentpage = itemPageDefinition.createObject();
                                
                                contentpage.itemPageTitle = chosenItem.title;
                                contentpage.videoTitleText = chosenItem.title;
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
                        title: "Export Records"
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
                    ActionBar.placement: ActionBarPlacement.InOverflow
                    onTriggered: {
                        var contentpage = addRecordsPageDefinition.createObject();
                        navPane.push(contentpage);
                        
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
                
                // TODO Reload only when something changes 
                
                // Clear the Data Model
                dataModel.clear();
                // Reload the Data Source
                dataSource.load();
            } 
        } // NavigationPane
    } // View/ Update Tab


}
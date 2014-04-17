import  bb.data 1.0
import bb.cascades 1.2

NavigationPane {
    id:queryNav
    Page { 
        Container {
            layout: AbsoluteLayout {
            
            }
            Label {
                layoutProperties: AbsoluteLayoutProperties {
                    positionX: 30
                    positionY: 30
                }
                
                id: welcomeMsg1
                text: "Real-time video search system"
                textFormat: TextFormat.Plain
                textStyle.color: Color.DarkYellow
                textStyle.fontStyle: FontStyle.Default
                textStyle.fontWeight: FontWeight.Bold
            
            } 
            
            Label {
                layoutProperties: AbsoluteLayoutProperties {
                    positionX: 30
                    positionY: 1100
                }
                
                id: responderID
                textFormat: TextFormat.Plain
                textStyle.color: Color.DarkYellow
                textStyle.fontStyle: FontStyle.Default     
                
                
                onCreationCompleted: {
                    appObject.responderIDChanged.connect(onCPPresponderIDChanged)
                }
                
                function onCPPresponderIDChanged()
                {
                    responderID.text = appObject.getResponderID();  
                }
            } 
            
            // Displying size message
            Label {
                layoutProperties: AbsoluteLayoutProperties {
                    positionX: 30
                    positionY: 1050
                }
                
                id: responseSizeText
                text: appObject.responseSize
                preferredWidth: 900
                preferredHeight: 100
                textStyle.fontSize: FontSize.Small
                textStyle.fontStyle: FontStyle.Italic

            }            
            
            Button {
                layoutProperties: AbsoluteLayoutProperties {
                    positionX: 400
                    positionY: 140
                }
                preferredWidth: 250
                
                id: reset1
                text: "Clear Results"
                onClicked: {
                    queryDataModel.clear();
                    //TODO: Reset the XML file 
                }
            }
            // Displaying search queries      
            ListView {
                id: searchQueries
                dataModel: queryDataModel
                
                layoutProperties: AbsoluteLayoutProperties {
                    positionX: 20
                    positionY: 440
                }
                
                function refreshDataModel(){
                    queryDataModel.clear();
                    dataSource.load();
                }
                
                listItemComponents: [
                    ListItemComponent {
                        type: "item"
                        
                        // Use a standard list item to display the data in the data model                   
                        StandardListItem {
                            title: ListItemData.title
                            description: qsTr(ListItemData.price)
                            
                            contextActions: [
                                ActionSet {
                                }
                            ]
                        } 
                    }
                
                ]
                onTriggered: {
                    
                    if (indexPath.length > 1) {
                        
                        var chosenItem = queryDataModel.data(indexPath);
                        
                        var contentpage = responsePageDefinition.createObject();
                        
                        contentpage.queryPageTitle = chosenItem.title;
                        contentpage.searcherIDText = chosenItem.searcherID;
                        contentpage.videoTitleText = chosenItem.title;
                        contentpage.videoPriceText = chosenItem.price;
                        
                        queryNav.push(contentpage);
                    }
                }
                
                onCreationCompleted: {
                    appObject.queryChanged.connect(onCPPqueryChanged)
                }
                
                function onCPPqueryChanged()
                {
                    refreshDataModel();
                }
            
            
            } // ListView
            
            attachedObjects: [
                GroupDataModel {
                    id: queryDataModel
                    sortingKeys: ["title"]
                    sortedAscending: true
                    grouping: ItemGrouping.ByFirstChar
                },
                DataSource {
                    id: dataSource
                    source: "query.xml"
                    query: "/video/message/query/movie"
                    
                    
                    onDataLoaded: {
                        if (data[0] == undefined) {
                            // The data returned is not a list, just one QVariantMap.
                            // Use insert to add one element.
                            console.log("Inserting one element");
                            queryDataModel.insert(data)
                        } else {
                            //The data returned is a list. Use insertList.
                            console.log("Inserting list element of " + data.length + " items");
                            //console.log (data[1]);
                            queryDataModel.insertList(data);
                        }
                    }
                },
                ComponentDefinition {
                    id: queryPageDefinition
                    source: "QueryPage.qml"
                }       
            ]
        }
    }
}

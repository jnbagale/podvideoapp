import bb.data 1.0
import bb.platform 1.2
import bb.cascades 1.2

NavigationPane {
    id: searcherNav
    Page{
        Container {
            layout: AbsoluteLayout {
            
            }
            
            Label {
                id: welcomeMsg
                layoutProperties: AbsoluteLayoutProperties {
                    positionX: 30
                    positionY: 30
                }                
                text: "Real-time video search system"
                textFormat: TextFormat.Plain
                textStyle.color: Color.DarkYellow
                textStyle.fontStyle: FontStyle.Default
                textStyle.fontWeight: FontWeight.Bold
            
            } 
            
            // to input title to search
            TextField {
                id: videoTitleText
                layoutProperties: AbsoluteLayoutProperties {
                    positionX: 50
                    positionY: 100
                }
                
                hintText: "Enter title to search here"
                maximumLength: 30
                verticalAlignment: VerticalAlignment.Bottom
                preferredWidth: 500
                input {
                    flags: TextInputFlag.AutoCapitalizationOff
                }
                
                property string searchTitle;
                onTextChanging: searchTitle = text
            }    
            
            // to display price selected by the Slider
            Label {
                id: selectedPrice
                layoutProperties: AbsoluteLayoutProperties {
                    positionX: 50
                    positionY: 200
                }
            }   
            
            // select price to search for the above entered video title
            Slider {
                id: price
                layoutProperties: AbsoluteLayoutProperties {
                    positionX: 50
                    positionY: 270
                }                               
                value: 1.0
                fromValue: 0.5
                toValue: 10.0
                
                property double	maxPrice;
                
                onImmediateValueChanged: {
                    selectedPrice.text = "Maximum Price " + immediateValue.toFixed(2);
                    maxPrice = immediateValue.toFixed(2); // fix price to 2 decimal places only          
                }
            }  
            
            Button {
                layoutProperties: AbsoluteLayoutProperties {
                    positionX: 100
                    positionY: 340
                }
                preferredWidth: 250
                
                id: search
                text: "search"
                onClicked: {
                    console.log("Sending search request...");
                    appObject.sendSearch(videoTitleText.searchTitle.toLowerCase(), price.maxPrice); 
                }
            }
            
            Button {
                layoutProperties: AbsoluteLayoutProperties {
                    positionX: 400
                    positionY: 340
                }
                preferredWidth: 250
                
                id: reset
                text: "Clear Results"
                onClicked: {
                    appObject.resetResponseXML(); 
                    searchResults.refreshDataModel();
                }
            }
            
            // Displaying search results      
            ListView {
                id: searchResults
                dataModel: searcherDataModel
                
                layoutProperties: AbsoluteLayoutProperties {
                    positionX: 20
                    positionY: 440
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
                        
                        var chosenItem = searcherDataModel.data(indexPath);
                        
                        var contentpage = responsePageDefinition.createObject();
                        
                        contentpage.searchResponsePageTitle = chosenItem.title;
                        contentpage.responderIDText = chosenItem.responderID;
                        contentpage.videoTitleText = chosenItem.title;
                        contentpage.videoGenreText = chosenItem.genre;
                        contentpage.videoReleaseDateText = chosenItem.dateOfRelease;
                        contentpage.videoDirectorText = chosenItem.director;
                        contentpage.videoPriceText = chosenItem.price;
                        
                        searcherNav.push(contentpage);
                    }
                }
                
                onCreationCompleted: {
                    appObject.searchResponseChanged.connect(onCPPresponseChanged)
                }
                
                function onCPPresponseChanged()
                {
                    refreshDataModel();
                    notification.notify();
                }
                
                function refreshDataModel(){
                    searcherDataModel.clear();
                    dataSource.load();
                }
            
            } // ListView
            
            
            Label {
                layoutProperties: AbsoluteLayoutProperties {
                    positionX: 30
                    positionY: 1100
                }
                
                id: searcherID
                textFormat: TextFormat.Plain
                textStyle.color: Color.DarkYellow
                textStyle.fontStyle: FontStyle.Default   
                onCreationCompleted: {
                    appObject.responderIDChanged.connect(onCPPresponderIDChanged)
                }
                
                function onCPPresponderIDChanged()
                {
                    searcherID.text = appObject.getSearcherID();   
                }
            } 
            
            // Displying size message
            Label {
                layoutProperties: AbsoluteLayoutProperties {
                    positionX: 30
                    positionY: 1050
                }
                
                id: sizeText
                text: appObject.querySize
                preferredWidth: 900
                preferredHeight: 100
                textStyle.fontSize: FontSize.Small
                textStyle.fontStyle: FontStyle.Italic
            
            }
            
            attachedObjects: [
                GroupDataModel {
                    id: searcherDataModel
                    sortingKeys: ["title"]
                    sortedAscending: true
                    grouping: ItemGrouping.ByFirstChar
                },
                DataSource {
                    id: dataSource
                    source: "response.xml"
                    query: "/video/message/response/movie"
                    
                    
                    onDataLoaded: {
                        if (data[0] == undefined) {
                            // The data returned is not a list, just one QVariantMap.
                            // Use insert to add one element.
                            console.log("Inserting one element");
                            searcherDataModel.insert(data)
                        } else {
                            //The data returned is a list. Use insertList.
                            console.log("Inserting list element of " + data.length + " items");
                            //console.log (data[1]);
                            searcherDataModel.insertList(data);
                        }
                    }
                },
                ComponentDefinition {
                    id: responsePageDefinition
                    source: "SearchResponsePage.qml"
                },
                Notification {
                    id: notification
                    title: qsTr ("PackedobjectsD")
                    body: qsTr ("New Search response received")
                    //soundUrl: _publicDir + ".wav"
                }   
                    
            ]
        }
    }
}
import bb.data 1.0
import bb.cascades 1.2

NavigationPane {
    id: searcherNav
    Page{
        Container {
            property int count1: 0;
            layout: AbsoluteLayout {
            
            }
            
            Label {
                layoutProperties: AbsoluteLayoutProperties {
                    positionX: 30
                    positionY: 30
                }
                
                id: welcomeMsg
                text: "Real-time video search system"
                textFormat: TextFormat.Plain
                textStyle.color: Color.DarkYellow
                textStyle.fontStyle: FontStyle.Default
                textStyle.fontWeight: FontWeight.Bold
            
            } 
            
            // to input title to search
            TextField {
                layoutProperties: AbsoluteLayoutProperties {
                    positionX: 50
                    positionY: 100
                }
                
                id: videoTitleText
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
                layoutProperties: AbsoluteLayoutProperties {
                    positionX: 50
                    positionY: 200
                }
                
                id: selectedPrice
            }   
            
            // select price to search for the above entered video title
            Slider {
                layoutProperties: AbsoluteLayoutProperties {
                    positionX: 50
                    positionY: 270
                }
                
                id: price
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
                    console.log("Sending search reuqest...");
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
                    dataModel.clear();            
                }
            }
            
            // Displaying search results      
            ListView {
                id: searchResults
                dataModel: dataModel
                
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
                //TODO: Open new pane for displaying more details
                onTriggered: {
                    
                    if (indexPath.length > 1) {
                        
                        var chosenItem = dataModel.data(indexPath);
                        
                        var contentpage = responsePageDefinition.createObject();
                        
                        contentpage.searchResponsePageTitle = chosenItem.title;
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
                    dataModel.clear();
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
                text:appObject.getSearcherID();  
            } 
            
            // Displying size message
            Label {
                layoutProperties: AbsoluteLayoutProperties {
                    positionX: 30
                    positionY: 1050
                }
                
                id: sizeText
                text: appObject.size
                preferredWidth: 900
                preferredHeight: 100
                textStyle.fontSize: FontSize.Small
                textStyle.fontStyle: FontStyle.Italic
            
            }
            
            attachedObjects: [
                GroupDataModel {
                    id: dataModel
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
                            dataModel.insert(data)
                        } else {
                            //The data returned is a list. Use insertList.
                            console.log("Inserting list element of " + data.length + " items");
                            //console.log (data[1]);
                            dataModel.insertList(data);
                        }
                    }
                },
                ComponentDefinition {
                    id: responsePageDefinition
                    source: "SearchResponsePage.qml"
                }       
            ]
        }
    }
}
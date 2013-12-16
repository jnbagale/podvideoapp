/* Copyright (C) 2013 Jiva Bagale */

/* This program is free software: you can redistribute it and/or modify */
/* it under the terms of the GNU General Public License as published by */
/* the Free Software Foundation, either version 3 of the License, or */
/* (at your option) any later version. */

/* This program is distributed in the hope that it will be useful, */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the */
/* GNU General Public License for more details. */

import bb.cascades 1.2

Page {
    property alias tempText:statusText.text;

    Container {
        layout: AbsoluteLayout {
            
        }
        
        // to display price selected by the Slider
        Label {
            layoutProperties: AbsoluteLayoutProperties {
                positionX: 130
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
            property string searchTitle;
            
            onTextChanging: searchTitle = text
            input {
                flags: TextInputFlag.AutoCapitalizationOff
            }
        }    
        
        // to display price selected by the Slider
        Label {
            layoutProperties: AbsoluteLayoutProperties {
                positionX: 50
                positionY: 200
            }
            
            id: selectedPrice
        }   
        
        // select maximum price to search for the above entered video title
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
                appObject.status = qsTr("Sending search reuqest...");
                appObject.sendSearch(videoTitleText.searchTitle, price.maxPrice);                
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
                theDataModel.clear();
                theDataModel.insert(0, ("Sender ID       Video Title         Price"));

            }
        }
        
        // Displaying search results      
        ListView {
            id: searchResults

            layoutProperties: AbsoluteLayoutProperties {
                positionX: 20
                positionY: 440
            }
            
            dataModel: ArrayDataModel {
                id: theDataModel
            }

            onCreationCompleted: {
                // for ( var a = 0; a < 20; a++ ) {
                //     theDataModel.append("Item" + a);
                // }
                // theDataModel.append(["Appended 1", "Appended 2"]);
                theDataModel.insert(0, ("Sender ID       Video Title         Price"));
                //theDataModel.removeAt(0);
                //theDataModel.insert(0,["Prepended 1", "Prepended 2"]);
                
                //quick hack to remove mysterious 0 on the list
                theDataModel.removeAt(1);
            }
        }

        // Displying status message
        Label {
            layoutProperties: AbsoluteLayoutProperties {
                positionX: 130
                positionY: 1200
            }

            id: statusText
            text: "status:- " + appObject.status
            preferredWidth: 500
            preferredHeight: 100
            textStyle.fontSize: FontSize.Small
            textStyle.fontStyle: FontStyle.Italic

        }
        // Invisible Label to re-direct search result

        Label {
            layoutProperties: AbsoluteLayoutProperties {
                positionX: 130
                positionY: 1230
            }

            id: dummyTitle
            text: appObject.price

            onTextChanged: {
                theDataModel.append(appObject.title + "   " + appObject.price);
            }
            textFormat: TextFormat.Plain
            visible: false
        }
    }
}

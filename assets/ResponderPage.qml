import bb.cascades 1.2


Container {
    property alias responderidText: responderID.text
    property int count2: 0;
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
    } 
    
    // Displying size message
    Label {
        layoutProperties: AbsoluteLayoutProperties {
            positionX: 30
            positionY: 1050
        }
        
        id: size1Text
        text: appObject.size1
        preferredWidth: 900
        preferredHeight: 100
        textStyle.fontSize: FontSize.Small
        textStyle.fontStyle: FontStyle.Italic
    
    }
    // Invisible Label to update search results on list view
    Label {
        layoutProperties: AbsoluteLayoutProperties {
            positionX: 130
            positionY: 970
        }
        
        id: dummyTitle1
        text: count2 + ":" + appObject.query
        
        onTextChanged: {
            if (count2 == 5)
            {
                count2 = 0;
                theDataModel1.clear();
                theDataModel1.insert(0, ("Searcher ID     Video Title     Max Price"));
            
            }
            theDataModel1.append(appObject.query);
            count2++;
        
        }
        textFormat: TextFormat.Plain
        visible: false
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
            theDataModel1.clear();
            theDataModel1.insert(0, ("Searcher ID       Video Title       Max Price"));
        
        }
    }
    // Displaying search queries      
    ListView {
        id: searchQueries
        
        layoutProperties: AbsoluteLayoutProperties {
            positionX: 20
            positionY: 240
        }
        
        dataModel: ArrayDataModel {
            id: theDataModel1
        }
        
        onCreationCompleted: {
            
            theDataModel1.insert(0, ("Searcher ID       Video Title      Max Price"));
            
            //quick hack to remove mysterious 0 on the list
            theDataModel1.removeAt(1);
        }
    }
}



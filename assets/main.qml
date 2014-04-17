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

TabbedPane{
    property string exportResult; // to be used with dialog box
    id: tabPane
    
    onCreationCompleted: {
        //tabPane.activeTab = searcherTab;
    }
    
    
    Tab{
        id:searcherTab
        title: "Searcher"
        imageSource: "asset:///images/search.png"
        // The first page, which appears at startup 
        
        delegateActivationPolicy: TabDelegateActivationPolicy.ActivateImmediately
        
        delegate: Delegate {
            id: searcherDelegate
            source: "SearcherPage.qml"
        }
    
    }
    
    Tab {
        id:responderTab
        title: "Responder"
        imageSource: "asset:///images/responder.png"
        
        delegateActivationPolicy: TabDelegateActivationPolicy.ActivateImmediately
        
        delegate: Delegate {
            id: responderDelegate
            source: "ResponderPage.qml"
        }
    }
    
    Tab {   
        id: recordsTab
        title: "Records"
        imageSource: "asset:///images/database.png"
        
        delegateActivationPolicy: TabDelegateActivationPolicy.ActivateWhenSelected
        
        delegate: Delegate {
            id: recordsDelegate
            source: "RecordsPage.qml"
        }
        
        //if (favDelegate.object != undefined) {
        //favDelegate.object.refreshFavModel();
        // }
    
    } // Records Tab


}

import bb.cascades 1.2

Dialog {
    property string dialogMessage;
    id:myDialogBox
    Container {
        horizontalAlignment: HorizontalAlignment.Center
        verticalAlignment: VerticalAlignment.Center
        
        Button {
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
            
            text: dialogMessage
            
            onClicked: myDialogBox.close()
        }
    }
}
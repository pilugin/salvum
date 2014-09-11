import QtQuick 1.1

Rectangle {

    Rectangle {
        id: paths
        height: 20
        anchors { left: parent.left; right: parent.right; top: parent.top }
        border.color: "black"

        TextInput {
            id: mediaPath
            width: parent.width/2
            anchors { left: parent.left; top: parent.top; bottom: parent.bottom }
        
            Rectangle {
                color: "black"
                anchors { right: parent.right; top: parent.top; bottom: parent.bottom }
                width: 1
            }
        
            text: "media"
        }
    
        TextInput {
            id: bitmapPath
            anchors { left: mediaPath.right; top: parent.top; bottom: parent.bottom; right: setSourceBtn.left }
        
            text: "bitmap"
        }
    
        Rectangle {
            id: setSourceBtn
            anchors { right: parent.right; top: parent.top; bottom: parent.bottom }
            width: height
        
            color: "blue"
        
            MouseArea {
                anchors.fill: parent
//                onClick: bcast.setSource(mediaPath.text, bitmapPath.text)
            }
        }
    }
    
    Rectangle {
        id: info
        anchors { right: parent.right; left: parent.left; top: paths.bottom; bottom: parent.bottom } 
        color: "yellow"
    }        
}    
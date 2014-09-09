import QtQuick 1.1

Rectangle {
    property double total: 100
    property double current: 42
    
    property string totalColor: "black"
    property string currentColor: "magenta"
    
    color: totalColor
    
    Rectangle {
        id: currentRect
        anchors { left: parent.left; top: parent.top; bottom: parent.bottom; margins: 3 }
        
        width: parent.width * current / total
        
        color: parent.currentColor
        
    }
    Text {
        text: current + "/" + total        
        anchors { right: currentRect.right; top: currentRect.top }
        color: totalColor
        font.pixelSize: currentRect.height 
    }
}
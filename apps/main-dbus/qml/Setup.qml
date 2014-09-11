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
        
            text: "/dev/urandom"
        }
    
        TextInput {
            id: bitmapPath
            anchors { left: mediaPath.right; top: parent.top; bottom: parent.bottom; right: setSourceBtn.left }
        
            text: "./shared_resources/bitmap.data"
        }
    
        Rectangle {
            id: setSourceBtn
            anchors { right: parent.right; top: parent.top; bottom: parent.bottom }
            width: height
        
            color: "blue"
        
            MouseArea {
                anchors.fill: parent
                onClicked: bcast.setSource(mediaPath.text, bitmapPath.text)
            }
        }
        
        Component.onCompleted: bcast.setSource(mediaPath.text, bitmapPath.text)
    }
    
    Rectangle {
        id: info
        anchors { right: parent.right; left: parent.left; top: paths.bottom; bottom: parent.bottom } 
        
        Column {
            id: bitmapInfo
            width: 190
            anchors { top: parent.top; left: parent.left }
        
            Text { text: "JpegHeads: " + bcast.bitmapInfo().jpegHeads }
            Text { text: "GoodHeads: " + bcast.bitmapInfo().goodHeads }
            Text { text: "Zeros: " + bcast.bitmapInfo().zero }
            Text { text: "Goods: " + bcast.bitmapInfo().goods }
            Text { text: "Decodable: " + bcast.bitmapInfo().decodable }
            Text { text: "Starts: " + bcast.bitmapInfo().starts }            
        }
        
        ListView {
            id: jpegHeadsView
            anchors { left: bitmapInfo.right; top: parent.top; bottom: parent.bottom }
            width: 100
        
            header: Text { text: "JPEG HEADS" }
            model: bcast.jpegHeads
            delegate: Text { 
                text: ListView.view.model[index].toString(16)
            }            
        }
        
        ListView {
            id: goodHeadsView
            anchors { left: jpegHeadsView.right; top: parent.top; bottom: parent.bottom }
            width: 100
        
            header: Text { text: "GOOD HEADS" }
            model: bcast.goodHeads
            delegate: Text { 
                text: ListView.view.model[index].toString(16)
            }            
        }
        
        
    }        
}    
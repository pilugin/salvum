import QtQuick 1.1
import SalvUi 1.0

Rectangle {

    Flickable {
        id: imageView
        anchors.left: parent.left        
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: controls.left
        contentWidth:  1000000//image.width * image.scale 
        contentHeight: 1000000//image.height * image.scale 
        clip:true
        
        Image {
            id: image
            fillMode: Image.PreserveAspectFit
            source: ctrl.image
            onSourceChanged: console.log("source == " + source);
            scale: 0.25
            Image {
                id: shade
                anchors.fill: parent
                source: ctrl.shade
            }
          
        }        
    }        
        
    Rectangle {
        color: "blue"
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: 30
        height: 34
        
        MouseArea {
            acceptedButtons: Qt.LeftButton | Qt.RightButton 
            onClicked: {
                if (mouse.button == Qt.LeftButton)
                    image.scale *= 1.3
            
                if (mouse.button == Qt.RightButton)
                    image.scale /= 1.3
            } 
            anchors.fill: parent
        }
    }
    


    Column {
        id: controls
        spacing: 4

        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        width: 350

        DecodedClustersView {
            id: clusters
            width: parent.width
            height: parent.height * 2/3
            model: ctrl.decodedClusters
        }
        TextInput {
            id: filename
            text: "../shared_resources/2.shuf"
            width: parent.width
            onAccepted: ctrl.processDecode(filename.text, clustersList.text)
        }
        TextEdit {
            id: clustersList
            text: "0 1 2 3 4 5 6 7 8 9 a b c d e f 10 11 12 13"
            width: parent.width
            height: (parent.height - filename.height - clusters.height - status.height - 3*(parent.spacing)) // shitty code
            onTextChanged: updTimer.restart()
        }
        Text {
            id: status
            text: "Idle"
        }

    }

    Timer {
        id: updTimer
        repeat: false
        interval: 3000
        onTriggered: ctrl.processDecode(filename.text, clustersList.text)
    }

    Connections {
        target: ctrl
        onError: status.text = message
        onProcessStarted: status.text = "Processing.."
        onProcessEnd: status.text = "Idle"
    }
    
    Component.onCompleted: ctrl.processDecode(filename.text, clustersList.text)

    color: "white"
}

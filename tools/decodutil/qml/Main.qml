import QtQuick 1.1
import SalvUi 1.0

Rectangle {

    Image {
        id: image
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: controls.left
        fillMode: Image.PreserveAspectFit
        source: ctrl.image
        z: 1
    }
    Image {
        id: shade
        anchors.fill: image
        z: 2
        source: ctrl.shade
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

    color: "cyan"
}

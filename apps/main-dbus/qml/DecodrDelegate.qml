import QtQuick 1.1

Rectangle {

    color: checked ? "green" : "darkgray"
    Image {    
        id: preview
        anchors {  left: parent.left; top: parent.top; bottom:parent.botton; margins: 4 }
        width: height
        source: ""
    }

    Text {
        id: clusterCaption
        anchors { left: preview.right; top: parent.top; right: parent.right }
        text: cluster.toString(16)
    }
    
    Text {
        id: clustersDecodedCaption
        anchors { left: preview.right; top: clusterCaption.bottom; right: parent.right }
        color: connected ? "white" : "gray"
        text: "#C: " + clustersDecoded + " " + (decoding ? "running" : "check...")
    }
    
    ProgressBar {
        textVisible: false
        anchors { left: preview.right; top: clustersDecodedCaption.bottom; right: parent.right; bottom:parent.bottom }
        totalColor: parent.color
        currentColor: "black"
        current: blocksDecoded
        total: blocksTotal
    }
}

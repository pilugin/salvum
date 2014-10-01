import QtQuick 1.1

ListView {
    id: theView

    focus: false

    delegate: Rectangle {
        color: ListView.isCurrentItem ? "lightblue" : "white"
        height: 24
        width: 300
        Image {
            id: delegateIcon
            source: icon
        }
        Text {
            anchors.left: delegateIcon.right
            text: clusterNo + "\t| " + blockBegin + "\t| " + blockEnd
            anchors.verticalCenter: parent.verticalCenter
        }
        MouseArea {
            anchors.fill: parent
            onClicked: theView.currentIndex = index;
        }
        
        ListView.onIsCurrentItemChanged: if (ListView.isCurrentItem) {        
                                            theView.model.currentCluster = index
                                        }
    }
}

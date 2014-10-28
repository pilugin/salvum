import QtQuick 1.1

ListView {
    id: theView

    focus: false
    
    delegate: Rectangle {
        color: ListView.isCurrentItem ? "lightblue" : "white"
        height: 16
        width: 300
        Image {
            fillMode: Image.PreserveAspectFit
            height: parent.height
            id: delegateIcon
            source: icon
        }
        Text {
            anchors.left: delegateIcon.right
            text: clusterNo.toString(16) + "\t| " + blockBegin + "\t| " + blockEnd
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

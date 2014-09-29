import QtQuick 1.1

ListView {
    id: theView

    focus: false

    highlight: Rectangle {
        border.color: "red"
        width: parent ? parent.width : 0
        z: 10
        color: "#00000000"
    }

    highlightFollowsCurrentItem: true
    highlightMoveSpeed: 4000000

    delegate: Item {
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
            onClicked: {
                theView.model.currentCluster = index;
                theView.currentIndex = index;
            }
        }
    }
}

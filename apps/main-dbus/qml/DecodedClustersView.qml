import QtQuick 1.1

ListView {
    id: theView

    focus: false

    highlight: Rectangle {
        border.color: "red"
        z: 10
        color: "#00000000"
    }

    highlightFollowsCurrentItem: true
    highlightMoveSpeed: 4000000

    delegate: Text {
        text: clusterNo + "\t | " + blockBegin + "\t | " + blockEnd
        MouseArea {
            anchors.fill: parent
            onClicked: {
                theView.model.currentCluster = index;
                theView.currentIndex = index;
            }
        }
    }
}

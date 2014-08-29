import QtQuick 1.1

Rectangle {
    width: 500
    height: 300

    color: "#C0C0C0"

    GridView {
        id: theView

        anchors.fill: parent
        cellWidth: 110
        cellHeight: 110

        signal clicked(int row, variant clusters);

        model: salvJpegModel
        delegate: Rectangle {
            color: "black"
            width: 100
            height: 100

            MouseArea {
                anchors.fill: parent
                onClicked: theView.clicked(index, decodedClusters);
            }
        }

    }

    Connections {
        target: theView
        onClicked: console.log("HERE" + row + "  " + clusters);
    }
}

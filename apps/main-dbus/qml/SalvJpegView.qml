import QtQuick 1.1

Rectangle {
    width: 500
    height: 300

    color: "#C0C0C0"


    Component {
        id: salvJpegDelegate

        Rectangle {
            color: "black"
            width: 100
            height: 100
        }
    }

    GridView {
        anchors.fill: parent
        cellWidth: 110
        cellHeight: 110

        model: salvJpegModel
        delegate: salvJpegDelegate
    }
}

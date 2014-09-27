import QtQuick 1.1

GridView {
    id: theView

    property double imageSize: 300

    anchors.fill: parent
    cellWidth: imageSize * 1.1
    cellHeight: imageSize * 1.1

    signal clicked(int row, variant clusters);

    focus: true

    highlight: Rectangle {
        border.color: "red"
        z: 10
        color: "#00000000"
    }

    highlightFollowsCurrentItem: true

    onCurrentIndexChanged: model.currentSalvIndex = currentIndex

    delegate: Rectangle {
        id: theDelegate

        width: theView.imageSize
        height: theView.imageSize
        z: 5

        Image {
            anchors.fill: parent
            source: image
        }
//        Image {
//            //visible: false
//            anchors.fill: parent
//            source: shade
//        }
        Item {
            anchors.fill: parent
//            Rectangle {
//                color: "red"
//                x: 
//            }
//            Text { text: shadePath }
//            Component.onCompleted: console.log(shadePath)
        }
        

        MouseArea {
            anchors.fill: parent
            onClicked: theView.currentIndex = index
        }
    }

}

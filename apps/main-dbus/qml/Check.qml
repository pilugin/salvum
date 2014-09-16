import QtQuick 1.1
import SalvUi 1.0

Rectangle {
    SalvJpegView {
        id: salvView
        model: salvJpegModel

        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: theFooter.top
        anchors.right: clustersView.left
        height: parent.height
        clip: true
    }

    DecodedClustersView {
        id: clustersView

        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: theFooter.top
        width: 250
        height: parent.height

        property variant salv: salvJpegModel.currentSalv
        model: salv ? salv.decodedClusters : null
    }

/*
    Rectangle {
        id: theFooter
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        height: 10
        color: "cyan"
        MouseArea {
            anchors.fill: parent
            onClicked: {
                if (mouse.button==Qt.LeftButton)
                    salvView.imageSize += 10;
                else if (mouse.button==Qt.RightButton)
                    salvView.imageSize -= 10;
            }
        }
    }
*/
    ProgressBar {
        id: theFooter
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        height: 15
        
    }    
}

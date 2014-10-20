import QtQuick 1.1
import SalvUi 1.0

Rectangle {
    id: checkView

    function setDecoderInfo(image, decodedClusters) {
        imageView.image = image
        clustersView.model = decodedClusters
        imageView.rect1 = decodedClusters.shadeRect1
        imageView.rect2 = decodedClusters.shadeRect2
    }


    Flickable {
        id: imageView
        anchors { left: parent.left; top: parent.top }
        width: 500
        height: 500
        contentHeight: innerImageView.height * innerImageView.scale
        contentWidth: innerImageView.width * innerImageView.scale

        property variant rect1: null
        property variant rect2: null
        property string image: ""

        clip: true

        Image {
            id: innerImageView
            transformOrigin: Item.TopLeft
            scale: 0.25

            fillMode: Image.PreserveAspectFit
            source: imageView.image

            Rectangle {
                x: imageView.rect1 ? imageView.rect1.x : 0
                y: imageView.rect1 ? imageView.rect1.y : 0
                width: imageView.rect1 ? imageView.rect1.width : 0
                height: imageView.rect1 ? imageView.rect1.height : 0
                color: "#808080"
                opacity: 0.7
            }
            Rectangle {
                x: imageView.rect2 ? imageView.rect2.x : 0
                y: imageView.rect2 ? imageView.rect2.y : 0
                width: imageView.rect2 ? imageView.rect2.width : 0
                height: imageView.rect2 ? imageView.rect2.height : 0
                color: "#808080"
                opacity: 0.7
            }

            onStatusChanged: if (status == Image.Ready) {
                                 imageView.contentX = 0//width/2
                                 imageView.contentY = 0//height/2
                             }
        }
    }
    Rectangle {
        id: controls
        color: "green"
        anchors { left: parent.left; right: clustersView.left; top: imageView.bottom }
        height: 35
    }

    DecodedClustersView {
        id: clustersView
        anchors { left: imageView.right; right: parent.right; top: parent.top; bottom: decoderView.top }

    }

    GridView {
        id: decoderView
        anchors { top: controls.bottom; left: parent.left; right: parent.right; bottom: parent.bottom }
        model: decoderHub
        cellWidth: 160
        cellHeight: 45

        delegate: DecodrDelegate {
            width: 150
            height: 40

            MouseArea {
                anchors.fill: parent
                onClicked: {

                    checkView.setDecoderInfo(imagePath, decodedClusters)

                }
            }
        }
    }

}

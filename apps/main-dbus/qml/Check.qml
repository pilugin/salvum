import QtQuick 1.1
import SalvUi 1.0

Rectangle {
    id: checkView

    function setDecoderInfo(image, decodedClusters) {
        imageView.image = image
        clustersView.currentIndex =-1
        clustersView.model = decodedClusters
        imageView.rect1 = decodedClusters.shadeRect1
        imageView.rect2 = decodedClusters.shadeRect2
        imageView.rejImg = decodedClusters.rejectedImage
    }

    Flickable {
        id: imageView
        anchors { left: parent.left; top: parent.top; right: clustersView.left; bottom: controls.top }
        contentHeight: innerImageView.height * contentScale
        contentWidth: innerImageView.width * contentScale
        
        property bool scaledMode : false
        property real contentScale : (scaledMode ? 6 : 0.25)
        
        function positionViewOnEdge() {
            var x = rect1 ? rect1.x : 0
            var y = rect1 ? rect1.y : 0
            if (scaledMode) {
                contentX = x*contentScale - width*0.75
                contentY = y*contentScale - height*0.75
            }   
        }            
        
        onScaledModeChanged: positionViewOnEdge()
        
        property variant rect1: null
        property variant rect2: null
        property variant rejImg: null
        property string image: ""

        clip: true

        Image {
            id: innerImageView
            transformOrigin: Item.TopLeft
            scale: imageView.contentScale

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
                width: imageView.rect2 ? imageView.rect2.width-1 : 0
                height: imageView.rect2 ? imageView.rect2.height : 0
                color: "#808080"
                opacity: 0.7
            }
            Image {
                x: 0
                y: imageView.rejImg ? imageView.rejImg.offset : 0
                source: imageView.rejImg ? imageView.rejImg.source : ""
                onSourceChanged: console.log("SRC = " + source )
            }

            onStatusChanged: if (status == Image.Ready) {
                                 imageView.contentX = 0
                                 imageView.contentY = 0
                             }
        }
    }
    Row {
        id: controls
        
        property bool allDecodersChecked: false
        
        anchors { left: parent.left; right: clustersView.left; bottom: decoderView.top }
        height: 35
        anchors.margins: 3
        property real childHeight: height - 2*anchors.margins
        
        spacing: 3
        Rectangle {
            color: "darkgreen"
            height: parent.childHeight
            width: 60
            Text { 
                text: "Prev"
                anchors.centerIn: parent
            }
            MouseArea {
                anchors.fill: parent
                onClicked: decoderView.currentIndex = (decoderView.currentIndex+decoderView.count-1)%decoderView.count
            }
        }
        Rectangle {
            color: "darkgreen"
            height: parent.childHeight
            width: 60
            Text { 
                text: "Next"
                anchors.centerIn: parent
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    var i = decoderHub.getUncheckedItemIndex( decoderView.currentIndex+1 );
                    if (i != -1)
                        decoderView.currentIndex = i;
                }
            }
        }
        Rectangle {
            color: "darkgreen"
            height: parent.childHeight
            width: 60
            Text {
                text: "Scale:" + (imageView.scaledMode ? "On" : "Off")
                anchors.centerIn: parent
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    imageView.scaledMode = !imageView.scaledMode;
                }
            }
        }
        Rectangle {
            color: "lightgray"
            height: parent.childHeight
            width: height * imageView.contentWidth / imageView.contentHeight
            clip: true
            Rectangle {
                id: currentViewportIndicator
                color: "green"
                x: imageView.contentX / imageView.contentWidth * parent.width
                y: imageView.contentY / imageView.contentHeight * parent.height
                width:  imageView.width  / imageView.contentWidth  * parent.width
                height: imageView.height / imageView.contentHeight * parent.height
            }
            Rectangle {
                color: "darkgray"
                x: 0
                y: currentViewportIndicator.y + currentViewportIndicator.height
                width: parent.width
                height: parent.height
            }
        }
        Rectangle {
            color: "darkgreen"
            height: parent.childHeight
            width: 140                
        }
        Rectangle {
            color: controls.allDecodersChecked ? "green" : "lightgray"
            height: parent.childHeight
            width: 100
            Text {
                text: "Proceed"
                color: controls.allDecodersChecked ? "black" : "gray"
                anchors.centerIn: parent
            }
            MouseArea {
                anchors.fill: parent
                enabled: controls.allDecodersChecked
                onClicked: {
                    decoderHub.baselineDecoders();
                    supervisor.proceedDecode();
                }
            }
        }
    }
    
    Connections {
        target: decoderHub
        onAllDecodersChecked: controls.allDecodersChecked = true
    }    

    DecodedClustersView {
        id: clustersView
        clip: true
        anchors { right: parent.right; top: parent.top; bottom: decoderView.top }
        width: 300
        onModelChanged: currentIndex = model.currentCluster
        
        Component.onCompleted: positionViewAtEnd()
        
        onCurrentIndexChanged: imageView.positionViewOnEdge()
    }

    GridView {
        id: decoderView
        anchors { left: parent.left; right: parent.right; bottom: parent.bottom }
        height: 200
        flow: GridView.LeftToRight
        model: decoderHub
        cellWidth: 210
        cellHeight: 55
        clip: true
        
        delegate: DecodrDelegate {
            width: 205
            height: 50
            border.color: GridView.isCurrentItem ? "blue" : color
            
            MouseArea {
                anchors.fill: parent
                onClicked: decoderView.currentIndex = index
            }
            
            GridView.onIsCurrentItemChanged: if (GridView.isCurrentItem) {
                                                checkView.setDecoderInfo(imagePath, decodedClusters)            
                                            }
        }
    }

}

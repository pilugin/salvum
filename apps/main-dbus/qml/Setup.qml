import QtQuick 1.1

Rectangle {

    Rectangle {
        id: paths
        height: 20
        anchors { left: parent.left; right: parent.right; top: parent.top }
        border.color: "black"

        TextInput {
            id: mediaPath
            width: parent.width*0.4
            anchors { left: parent.left; top: parent.top; bottom: parent.bottom }
        
            Rectangle {
                color: "black"
                anchors { right: parent.right; top: parent.top; bottom: parent.bottom }
                width: 1
            }
        
            text: "/dev/urandom"
        }
    
        TextInput {
            id: wspacePath
            anchors { left: mediaPath.right; top: parent.top; bottom: parent.bottom  }
            width: parent.width*0.4
        
            text: "./shared_resources/WSPACE"
        }
    
        Rectangle {
            id: setSourceBtn
            anchors { right: parent.right; top: parent.top; bottom: parent.bottom }
            width: parent.width*0.2        
            color: "#A0A0FF"
            Text { 
                anchors.fill: parent
                text: "load workspace" 
                verticalAlignment: Text.AlignVCenter; horizontalAlignment: Text.AlignHCenter
            }
        
            MouseArea {
                anchors.fill: parent
                onClicked: wspaceModel.loadWspace(wspacePath.text)
            }
        }
        
        Component.onCompleted: wspaceModel.loadWspace(wspacePath.text)
    }
    
    Connections {
        target: wspaceModel
        onBitmapPathUpdated: {
            bcast.stop()
            bcast.setSource( mediaPath.text, wspaceModel.bitmapPath )
        }          
    }
    
    Rectangle {
        id: info
        anchors { right: parent.right; left: parent.left; top: paths.bottom; bottom: parent.bottom } 
        
        Column {
            id: bitmapInfoView
            width: 190
            anchors { top: parent.top; left: parent.left }
        
            Text { text: "JpegHeads: " + bitmapInfo.jpegHeads }
            Text { text: "GoodHeads: " + bitmapInfo.goodHeads }
            Text { text: "Zeros: " + bitmapInfo.zeros }
            Text { text: "Goods: " + bitmapInfo.goods }
            Text { text: "Decodable: " + bitmapInfo.decodable }
            Text { text: "Starts: " + bitmapInfo.starts }            
            
        }
        
        ListView {
            id: selectedHeadsView
            anchors { left: parent.left; right: jpegHeadsView.left; top: bitmapInfoView.bottom; bottom: parent.bottom  }
            model: selectedHeadsModel
            header: Rectangle {        
                height: 15
                width: parent.width
                color: "gray"
                Text {
                    id: caption
                    width: 70
                    text: "CLUSTER"
                }               
                Text {
                    id: clustersDecodedCaption
                    anchors.left: caption.right
                    width: 40
                    text: "CNT"
                }
                Text {
                    anchors.left: clustersDecodedCaption.right
                    width: 50
                    text: "IMAGE"
                }
            }
            footer: Rectangle { 
                color: "gray"
                width: parent.width
                height: 15
                Text { text: "TOTAL#: " + selectedHeadsView.count }
            }
            
            delegate: Rectangle {        
                id: delegate      
                height: 15
                width: parent.width
                color: (index%2 == 0) ? "#A0A0FF" : "#9090FF"
                Text {
                    id: caption
                    width: 70
                    text: cluster.toString(16) 
                    color: isGood ? "#505050" : "black"
                }               
                Text {
                    id: clustersDecodedCaption
                    anchors.left: caption.right
                    width: 40
                    text: clustersDecoded>0 ? clustersDecoded : ""
                }
                Text {
                    anchors.left: clustersDecodedCaption.right
                    width: 50
                    text: blocksTotal > 0 ? (blocksDecoded.toString() + "/" + blocksTotal.toString()) : ""
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        goodHeadsModel.toggleSelected(cluster)
                        jpegHeadsModel.toggleSelected(cluster)
                    }
                }
            }
        }
         
        ListView {
            id: jpegHeadsView
            anchors { left: bitmapInfoView.right; top: parent.top; bottom: parent.bottom }
            width: 100
        
            header: Text { text: "JPEG HEADS" }
            model: jpegHeadsModel
            delegate: Text { 
                text: cluster.toString(16)                
                color: selected ? "red" : "black"
                
                MouseArea {
                    anchors.fill: parent
                    onClicked: jpegHeadsModel.toggleSelected(cluster)
                }
            }            
        }
        
        ListView {
            id: goodHeadsView
            anchors { left: jpegHeadsView.right; top: parent.top; bottom: parent.bottom }
            width: 100
        
            header: Text { text: "GOOD HEADS" }
            model: goodHeadsModel
            delegate: Text { 
                text: cluster.toString(16)                
                color: selected ? "green" : "black"
                
                MouseArea {
                    anchors.fill: parent
                    onClicked: goodHeadsModel.toggleSelected(cluster)
                }
            }            
        }
        
        Rectangle {
            id: startButton
            color: "#9090FF"
            anchors { left: goodHeadsView.right; right: parent.right; bottom: parent.bottom }
            height: 50
            Text {
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter; horizontalAlignment: Text.AlignHCenter
                text: selectedHeadsView.count>0 ? "press here to START" : "select clusters"
            }
            MouseArea {
                anchors.fill: parent
                enabled: selectedHeadsView.count>0
                onClicked: { 
                    supervisor.startDecode( selectedHeadsModel.selectedHeads )
                }
            }
        }
        
        
    }        
}    

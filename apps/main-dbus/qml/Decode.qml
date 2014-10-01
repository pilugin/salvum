import QtQuick 1.1
import SalvUi 1.0

Rectangle {
    GridView {
        anchors { top: parent.top; left: parent.left; right: parent.right; bottom: bcastProgress.bottom }
        model: decoderHub
        flow: GridView.LeftToRight
        cellWidth: 210
        cellHeight: 55
        
        delegate: DecodrDelegate {
            width: 205
            height: 50            
        }
    }
    
    ProgressBar {
        id: bcastProgress
        anchors { left: parent.left; right: parent.right; bottom: parent.bottom }
        height: 14
        
        current: bcast.currentCluster
        total: bcast.clustersCount
    }    
}

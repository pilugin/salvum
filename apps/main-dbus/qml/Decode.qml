import QtQuick 1.1
import SalvUi 1.0

Rectangle {
    GridView {
        anchors { top: parent.top; left: parent.left; right: parent.right; bottom: bcastProgress.bottom }
        model: decoderHub
        cellWidth: 160
        cellHeight: 45
        
        delegate: DecodrDelegate {
            width: 150
            height: 40            
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

import QtQuick 1.1
import SalvUi 1.0

Rectangle {
    color: "green"    
    GridView {
        anchors.fill: parent
        model: decodrModel
        cellWidth: 170
        cellHeight: 70
        
        delegate: DecodrDelegate {
            width: 150
            height: 50
        }
    }
    
}
import QtQuick 1.1
import SalvUi 1.0

Rectangle {
    color: "green"
    ListView {
        anchors.fill: parent
        model: clientsHub.clientIds
        delegate: Text { text: clientsHub.clientIds[index] }
    }
    
    Component.onCompleted: {
        clientsHub.startDecoders( supervisor.decodedHeads );
        console.log( supervisor.decodedHeads );        
    }
}
import QtQuick 1.1

Loader {
    id: loader
    source: supervisor.qmlScreen
    onSourceChanged: console.log("Main.qml::loader::source changed: " + source);
    
    Connections {
        target: supervisor
        onSetupStateExited: bcast.start();
        onDecodeStateEntered: clientsHub.startDecoders( bcast.shmemPath, supervisor.decodedHeads )
        onBroadcastStateEntered: clientsHub.startProcessing() 
        
    }
    
    Connections {
        target: clientsHub
        onAllDecodersConnected: supervisor.allDecodersConnected();
    }
    
    Component.onCompleted: supervisor.init()        
}
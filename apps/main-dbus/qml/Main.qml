import QtQuick 1.1

Loader {
    id: loader
    source: supervisor.qmlScreen
    onSourceChanged: console.log("Main.qml::loader::source changed: " + source);
    
    Connections {
        target: supervisor
//        onDecodeStateEntered: {
        onSetupStateExited: {        
            clientsHub.startDecoders( bcast.shmemPath, supervisor.decodedHeads )
            bcast.start( clientsHub.getRewindCluster() )
        }            
        onBroadcastStateEntered: clientsHub.startProcessing() 
        
    }
    
    Connections {
        target: clientsHub
        onAllDecodersConnected: supervisor.allDecodersConnected();
    }
    
    Component.onCompleted: supervisor.init()        
}
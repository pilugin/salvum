import QtQuick 1.1

Loader {
    id: loader
    source: supervisor.qmlScreen
    onSourceChanged: console.log("Main.qml::loader::source changed: " + source);
    
    Connections {
        target: supervisor
        onPrepareDecodersStateEntered: {        
            decoderHub.createDecoders( bcast.shmemPath, supervisor.decodedHeads )
            bcast.start( decoderHub.getRewindCluster() )
        }            
        onPrepareDecodersStateExited: decoderHub.startProcessing()
        
    }
    
    Connections {
        target: decoderHub
        onAllDecodersConnected: supervisor.allDecodersConnected();
        onAllDecodersWaitForCheck: supervisor.allDecodersWaitForCheck();
        onAllDecodersChecked: console.log("ALL_DEC_CHECKD")
    }
    
    Component.onCompleted: supervisor.init()        
}

import QtQuick 1.1

Loader {
    id: loader
    source: supervisor.qmlScreen
    onSourceChanged: console.log("Main.qml::loader::source changed: " + source);
    
    Connections {
        target: supervisor
        onDecodeStateEntered: {
            clientsHub.startDecoders( supervisor.decodedHeads );
            console.log( supervisor.decodedHeads );        
        }
        
    }
    
    Component.onCompleted: supervisor.init()        
}
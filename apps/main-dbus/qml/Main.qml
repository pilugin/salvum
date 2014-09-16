import QtQuick 1.1

Loader {
    id: loader
    source: supervisor.qmlScreen
    onSourceChanged: console.log("Main.qml::loader::source changed: " + source);
    
    Component.onCompleted: supervisor.init()
}
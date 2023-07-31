import QtQuick
import QtQuick.Window
import QtQuick.Layouts

Item{
    id:rootItem
    property var containerItem: container.children
    property string name: ""

    StackLayout {
        id: container
        anchors.fill: parent
    }

    state: "docked"
    states: [
        State {
            name: "undocked"
            PropertyChanges{ target: wnd; visible: true; title: name}
            PropertyChanges{ target: rootItem; visible: false }
            ParentChange{ target: container; parent: undockedContainer }
        },
        State {
            name: "docked"
            PropertyChanges { target: wnd; visible: false }
            PropertyChanges { target: rootItem; visible: true }
            ParentChange { target: container; parent: rootItem }
        }
    ]
    Window {
        id:wnd
        visible: false
        width: 200
        height: 400
        Item {
            id: undockedContainer
            anchors.fill: parent
        }

        onVisibleChanged: {
            if(visible){
                show()
            }
            else{
                close();
            }
        }
        onClosing: {
            rootItem.state = "docked"
        }

    }
}

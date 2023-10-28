import QtQuick
import QtQuick.Window
import QtQuick.Layouts

Item {
    id: rootItem

    property alias containerItem: container.children
    property alias windowTitle: wnd.title

    property bool isWindow: false

    onIsWindowChanged: {
//        print("onIsWindowChanged: ", isWindow)
        if(isWindow){
            wnd.visible = true
            rootItem.visible = false
            container.parent = undockedContainer
        }
        else{
            wnd.visible = false
            rootItem.visible = true
            container.parent = rootItem
        }
    }


    signal windowClose

    StackLayout {
        id: container
        anchors.fill: parent
    }

    //    state: "docked"
//    states: [
//        State {
//            name: "undocked"
//            PropertyChanges{ target: wnd; visible: true; /*title: name */}
//            PropertyChanges{ target: rootItem; visible: false }
//            ParentChange{ target: container; parent: undockedContainer }
//        },
//        State {
//            name: "docked"
//            PropertyChanges { target: wnd; visible: false }
//            PropertyChanges { target: rootItem; visible: true }
//            ParentChange { target: container; parent: rootItem }
//        }
//    ]

    Window {
        id: wnd

        visible: false
        width: 300
        height: 500
        x: mapToGlobal(10, 30).x
        y: mapToGlobal(10, 30).y

        Item {
            id: undockedContainer
            anchors.fill: parent
        }

        onVisibleChanged: {
            if(visible){
                show()
            } else {
                close();
            }
        }

        onClosing: {
            windowClose()
//            rootItem.state = "docked"
            isWindow = false
        }
    }
}

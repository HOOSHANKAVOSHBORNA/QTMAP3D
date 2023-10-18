import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material
import Crystal
import "style"

ColumnLayout {
    id: rootItem

    property ListModel sideModel: ListModel{}
    property alias currentItemIndex: tabBar.currentIndex

    signal modelEmpty

    function isInModel(objectName) {
        for (var i = 0; i < sideModel.count; i++) {
            if (sideModel.get(i).name === objectName)
                return i
        }

        return -1
    }

    function toggleToolbox() {
        var i = isInModel("Toolbox")
        if (i !== -1) {
            sideModel.remove(i)
            if (sideModel.count === 0) modelEmpty()
            currentItemIndex = 0
            stackLayout.currentIndex = stackLayout.getIndex(sideModel.get(0).name)
        } else {
            sideModel.append({name: "Toolbox"})
            currentItemIndex = sideModel.count - 1
            stackLayout.currentIndex = 0
        }

        toolbox.listModel = ToolboxInstance
    }

    function toggleLocationManager() {
        var i = isInModel("LocationManager");
        if (i !== -1) {
            sideModel.remove(i)
            if (sideModel.count === 0) modelEmpty()
            currentItemIndex = 0
            stackLayout.currentIndex = stackLayout.getIndex(sideModel.get(0).name)
        } else {
            sideModel.append({name: "LocationManager"})
            currentItemIndex = sideModel.count - 1
            stackLayout.currentIndex = 1
        }

        locationManager.listModel = LocatoinManagerInstance
    }

    function toggleLayers() {
        var i = isInModel("Layers");
        if (i !== -1) {
            sideModel.remove(i)
            if (sideModel.count === 0) modelEmpty()
            currentItemIndex = 0
            stackLayout.currentIndex = stackLayout.getIndex(sideModel.get(0).name)
        } else {
            sideModel.append({name: "Layers"})
            currentItemIndex = sideModel.count - 1
            stackLayout.currentIndex = 2
        }

        layers.layersModell = LayersInstance
    }

    function toggleBookmark() {
        var i = isInModel("Bookmark");
        if (i !== -1) {
            sideModel.remove(i)
            if (sideModel.count === 0) modelEmpty()
            currentItemIndex = 0
            stackLayout.currentIndex = stackLayout.getIndex(sideModel.get(0).name)
        } else {
            sideModel.append({name: "Bookmark"})
            currentItemIndex = sideModel.count - 1
            stackLayout.currentIndex = 3
        }

        bookmark.model = BookmarkInstance
    }

    TabBar {
        id: tabBar

        contentWidth: rootItem.sideModel.count ? parent.width - 40 / Style.monitorRatio : 0
        Layout.leftMargin: rootItem.sideModel.count ? 18 / Style.monitorRatio : 0
        Material.accent: Style.foregroundColor

        background: Rectangle {
            color: Style.disableColor
            height: 2
            anchors.bottom: parent.bottom
        }

        Repeater {
            id: repeater

            model: rootItem.sideModel

            TabButton {
                id: tb

                width: tabBar.width / rootItem.sideModel.count

                contentItem: Text {
                    id: txt

                    text: name ?? "unknown"
                    font: Style.fontFamily
                    opacity: enabled ? 1.0 : 0.3
                    color: tabBar.currentIndex === index ? Style.foregroundColor : Style.disableColor
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }

                background: Rectangle {
                    color: "transparent"
                }

                onDoubleClicked: {
                    stackLayout.data[currentItemIndex].parent = windowContainer
                    wnd.show()
                }
            }
        }
    }

    StackLayout {
        id: stackLayout
        Layout.fillHeight: true
        currentIndex: 0

        function getIndex(i) {
            console.log(i)
            if (i === "Toolbox") {
                return 0
            } else if (i === "LocationManager") {
                return 1
            } else if (i === "Layers") {
                return 2
            } else {
                return 3
            }
        }

        ToolboxView { id: toolbox }
        LocationManager { id: locationManager }
        LayersWidget { id: layers }
        BookmarkItem { id: bookmark }
    }

    Window {
        id: wnd

        visible: false
        width: 300
        height: 500
        x: mapToGlobal(10, 30).x
        y: mapToGlobal(10, 30).y

        Item {
            id: windowContainer
            anchors.fill: parent
        }

        onVisibleChanged: {
            if(visible){
                show()
            } else {
                windowContainer.data[0].parent = stackLayout
                windowContainer.data = []
                close();
            }
        }
    }
}

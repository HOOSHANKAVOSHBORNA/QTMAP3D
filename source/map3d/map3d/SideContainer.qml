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
    property int visibleCount: 0
    //    property var names: {0: "Toolbox", 1: "LocationManager", 2: "Layers", 3: "Bookmark"}
    //    property var indexes: {"Toolbox": 0, "LocationManager": 1, "Layers": 2, "Bookmark": 3}

    //    signal modelEmpty

    //    function isInModel(objectName) {
    //        for (var i = 0; i < sideModel.count; i++) {
    //            if (sideModel.get(i).name === objectName)
    //                return i
    //        }

    //        return -1
    //    }

    //    function toggleItem(itemName) {
    //        var index = indexes[itemName]

    //        var i = isInModel(itemName)
    //        if (i !== -1) {
    //            sideModel.remove(i)
    //            if (sideModel.count === 0) modelEmpty()
    //            currentItemIndex = 0
    //            stackLayout.currentIndex = indexes[sideModel.get(0).name]
    //        } else {
    //            sideModel.append({name: itemName})
    //            currentItemIndex = sideModel.count - 1
    //            stackLayout.currentIndex = index
    //        }
    //        modelsInitialize();
    //    }


    //    function modelsInitialize() {
    //        toolbox.listModel = ToolboxInstance
    //        locationManager.listModel = LocatoinManagerInstance
    //        layers.layersModell = LayersInstance
    //        bookmark.model = BookmarkInstance
    //    }

//clip: true

    TabBar {
        id: tabBar
//        contentWidth: rootItem.sideModel.count ? parent.width - 40 / Style.monitorRatio : 0
//        visible: /*count? true:*/ false
        Material.accent: Style.foregroundColor

        background: Rectangle {
            color: Style.disableColor
            height: 2
            anchors.bottom: parent.bottom
        }

        Repeater {
            id: repeater
            model: sideModel

            TabButton {
                id: tb


                width: visible? txt.implicitWidth + 10: 0
                implicitHeight: visible? 30/Style.monitorRatio: 0
                visible: model.checked

                contentItem: Text {
                    id: txt

                    text: model.name ?? "unknown"
                    font: Style.fontFamily
                    opacity: enabled ? 1.0 : 0.3
                    color: tabBar.currentIndex === index ? Style.foregroundColor : Style.disableColor
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }

                background: Rectangle {
                    color: "transparent"
                }
                onVisibleChanged: {
                    if (visible) {
                        tabBar.currentIndex = model.index
                    } else {
                        tabBar.currentIndex = 0
                    }

                    var count = 0
                    for(var i = 0; i <  sideModel.count; ++i){
                        if(sideModel.get(i).checked)
                            count++
                    }
                    visibleCount = count
                }
            }
        }
    }

    StackLayout {
        id: stackLayout
        Layout.fillHeight: true
        currentIndex: tabBar.currentIndex
        visible: visibleCount? true: false
        ToolboxView {
            id: toolbox
            listModel:ToolboxInstance
//            visible: sideModel.get(0).checked
        }
        LayersWidget {
            id: layers
            //            layersModell: LayersInstance
//            visible: sideModel? sideModel.get(1).checked: false
        }
        BookmarkItem {
            id: bookmark
            model: BookmarkInstance
//            visible:  sideModel.get(2).checked
        }
        LocationManager {
            id: locationManager
            listModel: LocatoinManagerInstance
//            visible: sideModel? sideModel.get(3).checked: false
        }
    }
}

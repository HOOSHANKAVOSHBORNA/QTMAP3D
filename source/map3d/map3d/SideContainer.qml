import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material
import Crystal
import "style"

ColumnLayout {
    id: rootItem

    property ListModel model: ListModel{}
    property int visibleCount: 0

    property alias isOpen: stackLayout.visible
    property alias currentItemIndex: stackLayout.currentIndex
    property int previousItemIndex: -1

//    function setCurrentIndex(index){
//        tabBar.currentIndex = index
//    }

    function toggleIndex(index) {
        if (isOpen && currentItemIndex === index) {
            if (previousItemIndex !== -1) {
                stackLayout.currentIndex = previousItemIndex
            } else {
                isOpen = false
            }
        } else {
            if (!isOpen) {
                isOpen = true
            }
            previousItemIndex = stackLayout.currentIndex
            stackLayout.currentIndex = index
        }
    }

    function toggleToolbox() {
        toggleIndex(0)
        toolbox.listModel = ToolboxInstance
    }

    function toggleLocationManager() {
        toggleIndex(1)
        locationManager.listModel = LocatoinManagerInstance
    }

    function toggleLayers() {
        toggleIndex(2)
        layers.layersModell = LayersInstance
    }

    function toggleBookmark() {
        toggleIndex(3)
        bookmark.model = BookmarkInstance
    }

//    TabBar {
//        id: tabBar
//        contentWidth: rootItem.model.count ?parent.width - 40 /Style.monitorRatio: 0
//        Layout.leftMargin: rootItem.model.count ? 18 / Style.monitorRatio : 0
//        Material.accent: Style.foregroundColor
//        background:
//            Rectangle{
//            color:Style.disableColor
//            height: 2
//            anchors.bottom: parent.bottom

//        }
////        clip: true

//        Repeater {
//            id: repeater
//            model: rootItem.model

//            TabButton {
//                id:tb

//                width:{
//                    if (rootItem.model.count === 1){

//                        return implicitWidth
//                    }else{
//                        if(tabBar.currentIndex === index){
//                            return implicitWidth
//                        }
////                        else return (tabBar.width - implicitWidth*2) / rootItem.model.count
//                    }
//                }

//                contentItem: Text {
//                    id:txt
//                    text: name ?? "unknown"
//                    font: Style.fontFamily
//                    opacity: enabled ? 1.0 : 0.3
//                    color: tabBar.currentIndex === index ? Style.foregroundColor : Style.disableColor
//                    horizontalAlignment: Text.AlignHCenter
//                    verticalAlignment: Text.AlignVCenter
//                    elide: Text.ElideRight
//                }

//                background:Rectangle{
//                    color:"transparent"                  }


//                onDoubleClicked: {
//                    var docItem = stackLayout.data[index]
//                    docItem.state = "undocked"

//                    for(var i = 1; i<rootItem.model.count; i++){
//                        var mindex = (i + index) % rootItem.model.count
//                        if(repeater.itemAt(mindex).visible){
//                            tabBar.currentIndex = mindex
//                            break
//                        }
//                    }
//                }

//                onVisibleChanged: {
//                    tabBar.currentIndex = visible ? index: tabBar.currentIndex
//                }
//            }

//        }
//    }

    StackLayout {
        id: stackLayout
        Layout.fillHeight: true
        currentIndex: 0
        visible: false

        ToolboxView { id: toolbox }
        LocationManager { id: locationManager }
        LayersWidget { id: layers }
        BookmarkItem { id: bookmark }
    }
}

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material
import Crystal
import "style"


Item {
    id: rootItem

    property ListModel sideModel: ListModel{}
    property alias currentItemIndex: tabBar.currentIndex
    property int visibleCount: 0

    //clip: true
    ColumnLayout{
        id:columnLayout
        anchors.fill: parent
        TabBar {
            id: tabBar
            Layout.preferredHeight: 30 / Style.monitorRatio
            Layout.fillWidth: true
            Material.accent: Style.foregroundColor

            background: Rectangle {
                color: "transparent"
                //                height: 2
                anchors.bottom: parent.bottom
            }

            Repeater {
                id: repeater
                model: sideModel
                TabButton {
                    clip: true
                    background: Rectangle {
                        color: "transparent"
                    }
                    contentItem: Text {
                        id: txt

                        text: model.name ?? "unknown"
                        font: Style.fontFamily
                        opacity: enabled ? 1.0 : 0.3
                        color: tabBar.currentIndex === index ? Style.foregroundColor : Style.disableColor
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                    }
                    visible: model.checked
                    width: visible? columnLayout.width/visibleCount:0
                    onVisibleChanged: {
                        if (visible) {
                            tabBar.currentIndex = model.index
                        } else {
                            if (model.index === tabBar.currentIndex)
                                for (var j = 0; j < sideModel.count; ++j) {
                                    if (sideModel.get(j).checked) {
                                        tabBar.currentIndex = j
                                        break
                                    }
                                }
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
            Layout.fillWidth: true
            currentIndex: tabBar.currentIndex
            visible: visibleCount? true: false
            ToolboxView {
                id: toolbox
                listModel:ToolboxInstance
            }
            LayersWidget {
                id: layers
                //            layersModell: LayersInstance
            }
            BookmarkItem {
                id: bookmark
                model: BookmarkInstance
            }
            LocationManager {
                id: locationManager
                listModel: LocatoinManagerInstance
            }
        }
    }
}

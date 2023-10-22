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
        anchors.topMargin: 5 / Style.monitorRatio
        anchors.rightMargin: 15 / Style.monitorRatio
        anchors.leftMargin: 15 / Style.monitorRatio
        TabBar {
            id: tabBar
//            Layout.preferredHeight: 30 / Style.monitorRatio
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
                    background: Rectangle {
                        color: "transparent"
                    }
                    contentItem: Rectangle {
                        anchors.fill: parent
                        color: 'transparent'

                        Rectangle {
                            height: 2
                            width: parent.width
                            radius: 1
                            color: Style.disableColor
                            anchors.bottom: parent.bottom
                        }

                        Rectangle {
                            color: 'transparent'
                            anchors.fill: parent
                            anchors.margins: 3 / Style.monitorRatio
                            clip: true

                            Text {
                                id: txt

                                text: model.name ?? "unknown"
                                font.family: Style.fontFamily
                                font.pixelSize: 17 / Style.monitorRatio
                                opacity: enabled ? 1.0 : 0.3
                                color: tabBar.currentIndex === index ? Style.foregroundColor : Style.disableColor
                                anchors.verticalCenter: parent.verticalCenter
    //                            verticalAlignment: Text.AlignVCenter
                                //                            horizontalAlignment: Text.AlignHCenter
                            }
                        }



                        // TODO: replace rectangle and mouse area with Button
                        Rectangle {
                            visible: tabBar.currentIndex === model.index
                            width: 22 / Style.monitorRatio
                            height: 22 / Style.monitorRatio
                            anchors.right: parent.right
                            anchors.verticalCenter: parent.verticalCenter
                            color: 'transparent'

//                            padding: 0

                            Image {
                                source: "qrc:/Resources/undocker.png"
                                width: 22 / Style.monitorRatio
                                height: 22 / Style.monitorRatio
                            }

                            MouseArea {
                                anchors.fill: parent

                                onClicked: {
                                    stackLayout.data[tabBar.currentIndex].state = 'undocked'
                                    model.checked = false
                                }
                            }

//                            background: Rectangle {
//                                color: 'transparent'
//                            }

//                            display: AbstractButton.IconOnly
                        }
                    }


                    //                        Button {
                    //                            id: btnPin

                    //                            padding: 0

                    //                            visible: tabBar.currentIndex === model.index

                    //                            icon {
                    //                                source: "qrc:/Resources/undocker.png"
                    //                                width: 22 / Style.monitorRatio
                    //                                height: 22 / Style.monitorRatio
                    //                            }

                    //                            background: Rectangle {
                    //                                color: 'transparent'
                    //                            }

                    //                            display: AbstractButton.IconOnly
                    //                        }


                    visible: model.checked
                    width: {
                        if (visible) {
                            if (visibleCount < 3) {
                                return columnLayout.width / visibleCount
                            } else if (visibleCount >= 3 && tabBar.currentIndex === model.index) {
                                return 100
                            } else {
                                return (columnLayout.width - 100) / (visibleCount - 1)
                            }
                        } else {
                            return 0
                        }
                    }

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
            visible: visibleCount ? true: false

            DockWindow {
                containerItem: ToolboxView {
                    id: toolbox
                    listModel:ToolboxInstance
                }

                onWindowClose: {
                    sideModel.get(0).checked = true
                }
            }

            DockWindow {
                containerItem: LayersWidget {
                    id: layers
                    //            layersModell: LayersInstance
                }

                onWindowClose: {
                    sideModel.get(1).checked = true
                }
            }

            DockWindow {
                containerItem: BookmarkItem {
                    id: bookmark
                    model: BookmarkInstance
                }

                onWindowClose: {
                    sideModel.get(2).checked = true
                }
            }

            DockWindow {
                containerItem: LocationManager {
                    id: locationManager
                    listModel: LocatoinManagerInstance
                }

                onWindowClose: {
                    sideModel.get(3).checked = true
                }
            }
        }
    }
}

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "style"

Rectangle {
    id:container

    property alias sideContainerWidth: leftContainer.implicitWidth

    required property var mainWindow
    required property var unpinContainer
    required property var pinContainer

    readonly property color     _colorHover : "#01AED6"
    readonly property color     _colorPresed : "#003569"

    function addToLeftContainer(item, name) {
        leftContainer.model.append({item:item, name:name})
    }

    function removeFromLeftContainer(item) {
        var indx = -1
        for (var i = 0; i < leftContainer.model.count; ++i){
            if (leftContainer.model.get(i).item === item){
                indx = i
                break
            }
        }
        if (indx > -1)
            leftContainer.model.remove(indx)
    }

    state: "unpin"
    states: [
        State {
            name: "unpin"
            ParentChange { target: container; parent: unpinContainer }
        },
        State {
            name: "pin"
            ParentChange { target: container; parent: pinContainer }
        }
    ]


    radius: 10
    anchors.fill: parent

    gradient: Gradient{
     GradientStop { position: 0.0; color: Style.topGradient }
     GradientStop { position: 1.0; color: Style.bottomGradient }
    }
    border.color: Style.borderColor
    border.width: 2
    RowLayout {
        anchors.fill: parent
        Rectangle {
            id: toolBar
            Layout.leftMargin: 3
            Layout.preferredWidth: 80/Style.monitorRatio - 6
            Layout.preferredHeight: parent.height -5
            color: Style.backgroundColor
            border.color: "gray"
            border.width: 3
            radius: 10
            ColumnLayout{
                id: centerItemUp
                anchors.fill: parent
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                spacing: 0
                Button {
                    width: 20
                    icon {
                        source: "qrc:/Resources/toolbox.png"
                        width: 28
                        height: 28
                        color : hovered ? (pressed ? _colorPresed: _colorHover) :
                                          (pressed ? _colorHover : _colorPresed);
                    }
                    background: Rectangle {
                        color: "transparent"
                    }

                    display: AbstractButton.IconOnly
                    checkable: true
                    checked: false
                    property var toolboxItem
                    onReleased: {
                        if (checked && mainWindow.toolbox ) {
                            var toolboxx = Qt.createComponent("ToolboxView.qml");
                            if (toolboxx.status === Component.Ready) {
                                toolboxItem = toolboxx.createObject(null, {});
                                toolboxItem.listModel = mainWindow.toolbox
                                addToLeftContainer(toolboxItem, "Toolbox")
                            }
                            else{
                                print("can not load toolbox.");
                            }
                        }
                        else {
                            removeFromLeftContainer(toolboxItem)
                        }
                    }
                }
                Button {
                    width: 28
                    icon {
                        source: "qrc:/Resources/location.png"
                        width: 28
                        height: 28
                        color : hovered ? (pressed ? _colorPresed: _colorHover) :
                                          (pressed ? _colorHover : _colorPresed);
                    }

                    background: Rectangle { color: "transparent" }

                    display: AbstractButton.IconOnly

                    checkable: true
                    checked: false
                    property var locationManagerItem
                    onReleased: {
                        if (checked && mainWindow.toolbox) {
                            var locationManager = Qt.createComponent("LocationManager.qml");
                            if (locationManager.status === Component.Ready) {
                                locationManagerItem = locationManager.createObject(null, {});
                                locationManagerItem.listModel = mainWindow.locationManagerProxyModel
                                addToLeftContainer(locationManagerItem, "Location Manager")
                            } else {
                                print("can not load LocationManager.qml.");
                            }
                        } else {
                            removeFromLeftContainer(locationManagerItem)
                        }
                    }
                }

                Button {
                    width: 28
                    display: AbstractButton.TextUnderIcon
                    icon {
                        source: "qrc:/Resources/settings.png"
                        width: 28
                        height: 28
                        color : hovered ? (pressed ? _colorPresed: _colorHover) :
                                          (pressed ? _colorHover : _colorPresed);
                    }
                    background: Rectangle {
                        color: "transparent"
                    }
                }
                Button {
                    id: layerButton
                    width: 28
                    icon {
                        source: "qrc:/Resources/layers.png"
                        width: 28
                        height: 28
                        color : hovered ? (pressed ? _colorPresed: _colorHover) :
                                          (pressed ? _colorHover : _colorPresed);
                    }
                    background: Rectangle {
                        color: "transparent"
                    }
                    display: AbstractButton.TextUnderIcon
                    checkable: true
                    checked: false
                    property var layerItem
                    onReleased: {
                        if (checked && mainWindow.layersModel ) {
                            var layersWidget = Qt.createComponent("LayersWidget.qml");
                            if (layersWidget.status === Component.Ready) {
                                layerItem = layersWidget.createObject(null, {});
                                layerItem.layersModell = mainWindow.layersModel
                                addToLeftContainer(layerItem, "Layers")
                            } else {
                                print("can not load Layer Widget")
                            }
                        } else {
                            removeFromLeftContainer(layerItem)
                        }
                    }
                }
                Button {
                    width: 28
                    icon {
                        source: "qrc:/Resources/list.png"
                        width: 28
                        height: 28
                        color : hovered ? (pressed ? _colorPresed: _colorHover) :
                                          (pressed ? _colorHover : _colorPresed);
                    }
                    background: Rectangle {
                        color: "transparent"
                    }
                    display: AbstractButton.TextUnderIcon

                    onReleased: {
                        mainWindow.showListWindow()
                    }
                }

                Button {
                    width: 28
                    icon {
                        source: "qrc:/Resources/hand.png"
                        width: 28
                        height: 28
                        color : hovered ? (pressed ? _colorPresed: _colorHover) :
                                          (pressed ? _colorHover : _colorPresed);
                    }
                    background: Rectangle {
                        color: "transparent"
                    }
                    display: AbstractButton.TextUnderIcon

                    onReleased: {
                        if (container.state === "pin") {
                            container.state = "unpin"
                        } else {
                            container.state = "pin"
                        }
                    }
                }
                Item {
                    // spacer item
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Rectangle { anchors.fill: parent; color: "transparent" } // to visualize the spacer
                }
            }
        }

        //-------------------------------------left dock-----------------
        SideContainer {
            id: leftContainer
            Layout.fillWidth: true
            //                Layout.preferredWidth: visibleCount > 0 ?  implicitWidth : 0
            //                Layout.maximumWidth: visibleCount > 0 ?  parent.width/3.5 : 0
        }
    }
}

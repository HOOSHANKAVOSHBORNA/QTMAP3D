import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "style"

Rectangle {
    id: container

    property alias sideContainerWidth: leftContainer.implicitWidth

    // TODO: you should not pass mainwindow to here :)
    required property var mainWindow
    required property var unpinContainer
    required property var pinContainer

    readonly property color _colorHover: "#01AED6"
    readonly property color _colorPresed: "#003569"

    readonly property color fg50: Qt.rgba(Style.foregroundColor.r, Style.foregroundColor.g, Style.foregroundColor.b, 0.50)


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

    radius: 20 / Style.monitorRatio

    anchors {
        fill: parent
    }

    gradient: Gradient{
        GradientStop { position: 0.0; color: Style.topGradient }
        GradientStop { position: 1.0; color: Style.bottomGradient }
    }

    border {
        color: Style.borderColor
        width: 2
    }

    RowLayout {
        anchors.fill: parent

        Rectangle {
            id: toolBar

            Layout.preferredWidth: 65 / Style.monitorRatio
            Layout.fillHeight: true
            Layout.leftMargin: 5 / Style.monitorRatio
            Layout.bottomMargin: 5 / Style.monitorRatio
            Layout.topMargin: 5 / Style.monitorRatio

            //            color: Style.backgroundColor
            color: "pink"

            border {
                width: 3 / Style.monitorRatio
                color: "gray"
            }

            radius: 15 / Style.monitorRatio

            Column {
                anchors.top: parent.top
                anchors.topMargin: 30 / Style.monitorRatio
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 80 / Style.monitorRatio

                Rectangle {
                    width: 45 / Style.monitorRatio
                    height: 45 / Style.monitorRatio
                    radius: width / 2
                }

                ColumnLayout {
                    id: centerItemUp

                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: 20 / Style.monitorRatio

                    Rectangle {
                        width: 40 / Style.monitorRatio
                        height: 40 / Style.monitorRatio
                        radius: 10 / Style.monitorRatio
                        color: fg50

                        Button {
                            anchors.centerIn: parent
                            padding: 0

                            icon {
                                source: "qrc:/Resources/toolbox.png"
                                width: 35 / Style.monitorRatio
                                height: 35 / Style.monitorRatio
                                color: hovered ? (pressed ? _colorPresed: _colorHover) :
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
                                if (checked && mainWindow.toolbox) {
                                    var toolboxx = Qt.createComponent("ToolboxView.qml");
                                    if (toolboxx.status === Component.Ready) {
                                        toolboxItem = toolboxx.createObject(null, {});
                                        toolboxItem.listModel = mainWindow.toolbox
                                        addToLeftContainer(toolboxItem, "Toolbox")
                                    } else {
                                        print("can not load toolbox.");
                                    }
                                } else {
                                    removeFromLeftContainer(toolboxItem)
                                }
                            }
                        }
                    }

                    Button {
                        padding: 0

                        icon {
                            source: "qrc:/Resources/location.png"
                            width: 35 / Style.monitorRatio
                            height: 35 / Style.monitorRatio
                            color: hovered ? (pressed ? _colorPresed: _colorHover) :
                                              (pressed ? _colorHover : _colorPresed);
                        }

                        background: Rectangle {
                            color: "transparent"
                        }

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
                        padding: 0

                        icon {
                            source: "qrc:/Resources/settings.png"
                            width: 35 / Style.monitorRatio
                            height: 35 / Style.monitorRatio
                            color: hovered ? (pressed ? _colorPresed: _colorHover) :
                                              (pressed ? _colorHover : _colorPresed);
                        }

                        background: Rectangle {
                            color: "transparent"
                        }
                    }

                    Button {
                        padding: 0

                        icon {
                            source: "qrc:/Resources/layers.png"
                            width: 35 / Style.monitorRatio
                            height: 35 / Style.monitorRatio
                            color: hovered ? (pressed ? _colorPresed: _colorHover) :
                                              (pressed ? _colorHover : _colorPresed);
                        }

                        background: Rectangle {
                            color: "transparent"
                        }

                        display: AbstractButton.IconOnly
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
                        padding: 0

                        icon {
                            source: "qrc:/Resources/list.png"
                            width: 35 / Style.monitorRatio
                            height: 35 / Style.monitorRatio
                            color: hovered ? (pressed ? _colorPresed: _colorHover) :
                                              (pressed ? _colorHover : _colorPresed);
                        }

                        background: Rectangle {
                            color: "transparent"
                        }

                        display: AbstractButton.IconOnly
                        checkable: true
                        checked: false

                        onReleased: {
                            mainWindow.showListWindow()
                        }
                    }

                    Button {
                        padding: 0

                        icon {
                            source: "qrc:/Resources/hand.png"
                            width: 35 / Style.monitorRatio
                            height: 35 / Style.monitorRatio
                            color: hovered ? (pressed ? _colorPresed: _colorHover) :
                                              (pressed ? _colorHover : _colorPresed);
                        }

                        background: Rectangle {
                            color: "transparent"
                        }

                        display: AbstractButton.IconOnly
                        checkable: true
                        checked: false

                        onReleased: {
                            if (container.state === "pin") {
                                container.state = "unpin"
                            } else {
                                container.state = "pin"
                            }
                        }
                    }
                }
            }
        }

        SideContainer {
            id: leftContainer

            Layout.fillWidth: true
        }
    }
}
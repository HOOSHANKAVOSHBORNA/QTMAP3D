import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects

import "style"

Rectangle {
    id: rootRect

    property alias sideContainer: sideContainer

    property alias listWindowVisible: btnList.checked
    property int minWidth

    readonly property color fg50: Qt.rgba(Style.foregroundColor.r,
                                          Style.foregroundColor.g,
                                          Style.foregroundColor.b, 0.50)
    readonly property color bg50: Qt.rgba(Style.backgroundColor.r,
                                          Style.backgroundColor.g,
                                          Style.backgroundColor.b, 0.50)

    property bool pin: btnPin.checked

    //    property var bookmarkItem: null

    //        state: btnPin.checked? "pin": "unpin"
    //        states: [
    //            State {
    //                name: "unpin"
    //            },
    //            State {
    //                name: "pin"
    //            }
    //        ]

    //    width: 350 / Style.monitorRatio
    //    radius: 20 / Style.monitorRatio

    //    PropertyAnimation on width {
    //        id: openAnimation
    //        running: false
    //        from: container.width
    //        to: 350 / Style.monitorRatio
    //    }

    //    PropertyAnimation on width {
    //        id: closeAnimation
    //        running: false
    //        from: 350 / Style.monitorRatio
    //        to: 75 / Style.monitorRatio
    //    }

    //    gradient: Gradient{
    //        GradientStop { position: 0.0; color: Style.topGradient }
    //        GradientStop { position: 1.0; color: Style.bottomGradient }
    //    }
    border {
        color: "white"
        width: 3
    }
    radius: Math.ceil(15 / Style.monitorRatio)
    color: bg50

    minWidth: sideContainer.currentItemIndex
              !== -1 ? Math.ceil((360 + 75) / Style.monitorRatio) : Math.ceil(
                           75 / Style.monitorRatio)

    RowLayout {
        id: rowLayout
        anchors.fill: parent
        Item {
            Layout.preferredWidth: Math.ceil(65 / Style.monitorRatio)
            Layout.minimumWidth: Math.ceil(65 / Style.monitorRatio)
            Layout.maximumWidth: Math.ceil(65 / Style.monitorRatio)
            Layout.margins: Math.ceil(5 / Style.monitorRatio)
            Layout.fillHeight: true
            Rectangle {
                id: toolBar

                anchors.fill: parent
                color: Style.backgroundColor

                radius: rootRect.radius

                Rectangle {
                    id: logo
                    anchors.topMargin: Math.ceil(35 / Style.monitorRatio)
                    anchors.top: parent.top
                    anchors.horizontalCenter: parent.horizontalCenter
                    width: Math.ceil(45 / Style.monitorRatio)
                    height: Math.ceil(45 / Style.monitorRatio)
                    radius: Math.ceil(width / 2)

                    Image {
                        width: Math.ceil(37 / Style.monitorRatio)
                        height: Math.ceil(37 / Style.monitorRatio)
                        source: "qrc:/Resources/Qarch.png"
                        anchors.centerIn: parent
                    }
                }

                ColumnLayout {
                    anchors.top: logo.bottom
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.topMargin: Math.ceil(80 / Style.monitorRatio)
                    spacing: Math.ceil(20 / Style.monitorRatio)

                    Repeater {
                        id: sideBarRep
                        model: sideBarModel
                        property int checkedIndex: -1

                        Button {
                            id: btnDelegate
                            padding: 0
                            icon {
                                source: model.iconSource
                                width: Math.ceil(35 / Style.monitorRatio)
                                height: Math.ceil(35 / Style.monitorRatio)
                            }

                            background: Rectangle {
                                width: Math.ceil(39 / Style.monitorRatio)
                                height: Math.ceil(39 / Style.monitorRatio)
                                radius: Math.ceil(10 / Style.monitorRatio)
                                color: model.index
                                       === sideBarRep.checkedIndex ? fg50 : "transparent"
                                anchors.centerIn: btnDelegate
                            }

                            hoverEnabled: true

                            display: AbstractButton.IconOnly
                            checkable: true
                            checked: false
                            onToggled: {
                                if (sideBarModel.get(model.index).isWindow) {
                                    sideContainer.focusWidnow(model.index)
                                } else if (sideBarRep.checkedIndex === model.index
                                           && !sideBarModel.get(
                                               model.index).isWindow) {
                                    sideBarRep.checkedIndex = -1
                                    sideContainer.setCurrentItemIndex(-1)
                                } else if (sideBarRep.checkedIndex !== model.index
                                           && !sideBarModel.get(
                                               model.index).isWindow) {
                                    sideBarRep.checkedIndex = model.index
                                    sideContainer.setCurrentItemIndex(
                                                model.index)
                                }
                            }

                            ToolTip {
                                y: 0
                                x: 35
                                visible: btnDelegate.hovered
                                contentItem: Text {
                                    id: tooltipText
                                    anchors.margins: 7 / Style.monitorRatio
                                    text: model.name
                                    font.pixelSize: Style.regularFontSize
                                    color: 'white'
                                }

                                background: Rectangle {
                                    visible: btnDelegate.hovered
                                    height: btnDelegate.height
                                    width: tooltipText.contentWidth + 14
                                    radius: 10 / Style.monitorRatio

                                    color: "#01AED6"
                                }
                            }
                        }
                    }

                    Button {
                        id: btnList
                        padding: 0
                        icon {
                            source: "qrc:/Resources/list.png"
                            width: Math.ceil(35 / Style.monitorRatio)
                            height: Math.ceil(35 / Style.monitorRatio)
                        }
                        background: Rectangle {
                            radius: Math.ceil(10 / Style.monitorRatio)
                            color: btnList.checked ? fg50 : "transparent"
                        }

                        display: AbstractButton.IconOnly
                        //onClicked: {
                        //    print(applicationWindow.connectionConfigCpp.isConnected)
                        //}
                        checkable: true

                        //                        checked: this take value by alias property
                        //                        onToggled: {
                        //                            listWindowVisible = !listWindowVisible
                        //                        }
                        ToolTip {
                            y: 0
                            x: 35

                            visible: btnList.hovered

                            contentItem: Text {
                                id: tooltipText2
                                anchors.margins: 7 / Style.monitorRatio
                                text: 'List'
                                font.pixelSize: Style.regularFontSize
                                color: 'white'
                            }

                            background: Rectangle {
                                visible: btnList.hovered
                                height: btnList.height
                                width: tooltipText2.contentWidth + 14
                                radius: 10 / Style.monitorRatio

                                color: "#01AED6"
                            }
                        }
                    }
                }

                Button {
                    id: btnPin
                    anchors.bottom: parent.bottom
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottomMargin: Math.ceil(100 / Style.monitorRatio)
                    padding: 0
                    icon {
                        source: pin ? "qrc:/Resources/sidebar-unpin.png" : "qrc:/Resources/sidebar-pin.png"
                        width: Math.ceil(35 / Style.monitorRatio)
                        height: Math.ceil(35 / Style.monitorRatio)
                    }
                    background: Rectangle {
                        radius: Math.ceil(10 / Style.monitorRatio)
                        color: btnPin.checked ? fg50 : "transparent"
                    }

                    display: AbstractButton.IconOnly
                    checkable: true
                    checked: false
                    onToggled: {

                    }

                    ToolTip {
                        y: 0
                        x: 35
                        visible: btnPin.hovered

                        contentItem: Text {
                            id: tooltipText3
                            anchors.margins: 7 / Style.monitorRatio
                            text: pin ? "Pin" : "Unpin"
                            font.pixelSize: Style.regularFontSize
                            color: 'white'
                        }

                        background: Rectangle {
                            visible: btnPin.hovered
                            height: btnPin.height
                            width: tooltipText3.contentWidth + 14
                            radius: 10 / Style.monitorRatio

                            color: "#01AED6"
                        }
                    }
                }
            }
            MultiEffect {
                source: toolBar
                enabled: true
                anchors.fill: toolBar
                shadowColor: Style.foregroundColor
                shadowEnabled: true
                shadowVerticalOffset: 0
                shadowHorizontalOffset: 2
                shadowBlur: 0.1
                shadowOpacity: .6
                shadowScale: 1.0
            }
        }

        SideContainer {
            id: sideContainer

            Layout.fillWidth: true
            Layout.fillHeight: true

            currentItemIndex: -1

            sideModel: sideBarModel

            onNoItem: {
                sideBarRep.checkedIndex = -1
            }

            onCurrentItemIndexChanged: sideBarRep.checkedIndex = currentItemIndex

            Component.onCompleted: currentItemIndex = -1
        }
    }

    //--------------------------------------------------------------------------------------
    ListModel {
        id: sideBarModel
        signal toggleItem(var index, var checked)
        ListElement {
            name: "Profile"
            iconSource: "qrc:/Resources/user.png"
            checked: false
            isWindow: false
        }

        ListElement {
            name: "Toolbox"
            iconSource: "qrc:/Resources/toolbox.png"
            checked: false
            isWindow: false
        }

        ListElement {
            name: "Layer"
            iconSource: "qrc:/Resources/layers.png"
            checked: false
            isWindow: false
        }

        ListElement {
            name: "Bookmark"
            iconSource: "qrc:/Resources/bookmark.png"
            checked: false
            isWindow: false
        }

        ListElement {
            name: "Location"
            iconSource: "qrc:/Resources/location.png"
            checked: false
            isWindow: false
        }

        ListElement {
            //            iconSource: connectionConfigCpp.isConnected ? "qrc:/Resources/plugged.png" : "qrc:/Resources/unplugged.png"
            name: "Connection Configuration"
            iconSource: "qrc:/Resources/plugged.png"
            checked: false
            isWindow: false
        }

        ListElement {
            name: "Settings"
            iconSource: "qrc:/Resources/settings.png"
            checked: false
            isWindow: false
        }
    }
}

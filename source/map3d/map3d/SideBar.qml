import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects
import Crystal
import "style"

Rectangle {
    id: rootRect

    property int minWidth
    //    property alias sideContainerWidth: leftContainer.implicitWidth

    //    // TODO: you should not pass mainwindow to here :)
    //    required property var mainWindow
    //    required property var unpinContainer
    //    required property var pinContainer

    //    readonly property color _colorHover: "#01AED6"
    //    readonly property color _colorPresed: "#003569"

    readonly property color fg50: Qt.rgba(Style.foregroundColor.r, Style.foregroundColor.g, Style.foregroundColor.b, 0.50)
    readonly property color bg50: Qt.rgba(Style.backgroundColor.r, Style.backgroundColor.g, Style.backgroundColor.b, 0.50)

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
    radius: 15 / Style.monitorRatio
    color: bg50

    minWidth: sideContainer.visibleCount? (300+75) / Style.monitorRatio: 75 / Style.monitorRatio

    RowLayout {
        id:rowLayout
        anchors.fill: parent
        Item {
            Layout.preferredWidth: 65 / Style.monitorRatio
            Layout.minimumWidth: 65 / Style.monitorRatio
            Layout.maximumWidth: 65 / Style.monitorRatio
            Layout.margins: 5/Style.monitorRatio
            Layout.fillHeight: true
            Rectangle {
                id: toolBar

                anchors.fill: parent
                color: Style.backgroundColor

                radius: rootRect.radius

                Rectangle {
                    id: logo
                    anchors.topMargin: 35 / Style.monitorRatio
                    anchors.top: parent.top
                    anchors.horizontalCenter: parent.horizontalCenter
                    width: 45 / Style.monitorRatio
                    height: 45 / Style.monitorRatio
                    radius: width / 2

                    Image {
                        width: 37 / Style.monitorRatio
                        height: 37 / Style.monitorRatio
                        source: "qrc:/Resources/Qarch.png"
                        anchors.centerIn: parent
                    }
                }

                ColumnLayout {
                    anchors.top: logo.bottom
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.topMargin: 80 / Style.monitorRatio
                    spacing: 20 / Style.monitorRatio

                    Repeater {
                        model: sideBarModel

                        Button {
                            id: btnDelegate
                            padding: 0
                            icon {
                                source: model.iconSource
                                width: 35 / Style.monitorRatio
                                height: 35 / Style.monitorRatio
                            }
                            background: Rectangle {
                                radius: 10 / Style.monitorRatio
                                color: btnDelegate.checked ? fg50 : "transparent"
                            }

                            display: AbstractButton.IconOnly
                            checkable: true
                            checked: false
                            onToggled: {
                                model.checked = checked;
                            }
                        }
                    }
                    Button {
                        id: btnList
                        padding: 0
                        icon {
                            source: "qrc:/Resources/list.png"
                            width: 35 / Style.monitorRatio
                            height: 35 / Style.monitorRatio
                        }
                        background: Rectangle {
                            radius: 10 / Style.monitorRatio
                            color: btnList.checked ? fg50 : "transparent"
                        }

                        display: AbstractButton.IconOnly
                        checkable: true
                        checked: false
                        onToggled: {

                        }
                    }
                }
                Button {
                    id: btnPin
                    anchors.bottom: parent.bottom
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottomMargin: 100 / Style.monitorRatio
                    padding: 0
                    icon {
                        source: "qrc:/Resources/hand.png"
                        width: 35 / Style.monitorRatio
                        height: 35 / Style.monitorRatio
                    }
                    background: Rectangle {
                        radius: 10 / Style.monitorRatio
                        color: btnPin.checked ? fg50 : "transparent"
                    }

                    display: AbstractButton.IconOnly
                    checkable: true
                    checked: false
                    onToggled: {

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
            sideModel: sideBarModel

        }
    }

    //--------------------------------------------------------------------------------------
    ListModel {
        id: sideBarModel
        signal toggleItem(var index, var checked)
        ListElement {
            name: "Toolbox"
            iconSource: "qrc:/Resources/toolbox.png"
            checked: false
        }

        ListElement {
            name: "Layer"
            iconSource: "qrc:/Resources/layers.png"
            checked: false
        }

        ListElement {
            name: "Bookmark"
            iconSource: "qrc:/Resources/bookmark.png"
            checked: false
        }

        ListElement {
            name: "Location"
            iconSource: "qrc:/Resources/location.png"
            checked: false
        }

        ListElement {
            name: "Setting"
            iconSource: "qrc:/Resources/settings.png"
            checked: false
        }

    }
}

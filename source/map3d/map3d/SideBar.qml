import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Crystal
import "style"

Rectangle {
    id: container

    //    property alias sideContainerWidth: leftContainer.implicitWidth

    //    // TODO: you should not pass mainwindow to here :)
    //    required property var mainWindow
    //    required property var unpinContainer
    //    required property var pinContainer

    //    readonly property color _colorHover: "#01AED6"
    //    readonly property color _colorPresed: "#003569"

    readonly property color fg50: Qt.rgba(Style.foregroundColor.r, Style.foregroundColor.g, Style.foregroundColor.b, 0.50)

    readonly property bool pin: btnPin.checked
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

    //    border {
    //        color: Style.borderColor
    //        width: 2
    //    }
    color: Style.backgroundColor

    width: sideContainer.visibleCount? rowLayout.implicitWidth: 0
    RowLayout {
        id:rowLayout
        anchors.fill: parent
        Rectangle {
            id: toolBar

            width: 80 / Style.monitorRatio
            Layout.fillHeight: true
            Layout.leftMargin: 5 / Style.monitorRatio
            Layout.bottomMargin: 5 / Style.monitorRatio
            Layout.topMargin: 5 / Style.monitorRatio

            color: Style.backgroundColor

            border {
                width: 3 / Style.monitorRatio
                color: "gray"
            }

            radius: 15 / Style.monitorRatio

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
                            //                                    if (!leftContainer.sideModel.count) openAnimation.running = true
//                            sideBarModel.toggleItem(index, checked)
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

        SideContainer {
            id: sideContainer
//            Layout.preferredHeight: 200
//            Layout.fillWidth: true
            Layout.minimumWidth: 300/Style.monitorRatio
            Layout.maximumWidth: 300/Style.monitorRatio
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

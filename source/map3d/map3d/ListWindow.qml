import QtQuick 2.13
import QtQuick.Layouts 1.13
import QtQuick.Controls 2.13
import Crystal 1.0
import QtQuick.Window 2.13
import "style"

CListWindow {
    id: rootItem
    width: 1000
    height: 600

    //    color: 'transparent'
    //    flags: Qt.FramelessWindowHint
    Rectangle {
        color: Style.backgroundColor
        anchors.fill: parent
        radius: 20

        ColumnLayout {
            anchors.fill: parent
            anchors.leftMargin: 25
            anchors.rightMargin: 25
            anchors.topMargin: 20
            anchors.bottomMargin: 25

            spacing: 0

            RowLayout {
                Layout.fillWidth: true
                Layout.preferredHeight: 25
                Layout.bottomMargin: 20

                Text {
                    Layout.fillWidth: true
                    text: 'Object list'
                    font.family: Style.fontFamily
                    font.pixelSize: 23
                    font.weight: 500
                    color: Style.foregroundColor
                }

                Button {
                    Layout.preferredWidth: 25
                    Layout.preferredHeight: width
                    padding: 0

                    background: Rectangle {
                        color: 'transparent'
                    }

                    display: AbstractButton.IconOnly

                    icon {
                        width: 25
                        height: 25
                        source: 'qrc:/Resources/add-place-close.png'
                    }

                    onClicked: {
                        rootItem.close()
                    }
                }
            }

            TabBar {
                id: listTabbar
                Layout.fillWidth: true
                Layout.preferredHeight: 26
                Layout.bottomMargin: 20
                padding: 0
                spacing: 5

                background: Rectangle {
                    //                color: 'red'
                    color: 'transparent'
                }

                Repeater {
                    id: rep
                    model: tabbarModel

                    TabButton {
                        required property var model

                        padding: 0
                        anchors.top: parent.top
                        width: titleTxt.width + 2 * 15
                        height: 26

                        background: Rectangle {
                            color: 'transparent'
                            anchors.fill: parent
                        }

                        contentItem: Rectangle {
                            anchors.fill: parent
                            border.width: 1
                            border.color: (model.index === listTabbar.currentIndex
                                           || hovered) ? Style.hoverColor : Style.foregroundColor
                            radius: 15
                            color: 'transparent'

                            Text {
                                id: titleTxt
                                anchors.centerIn: parent
                                text: model.title
                                color: (model.index === listTabbar.currentIndex
                                        || hovered) ? Style.hoverColor : Style.foregroundColor
                                font.family: Style.fontFamily
                                font.pixelSize: 17
                            }
                        }
                    }

                    onItemAdded: (index, modelItem) => {
                                     stackLayout.data.push(modelItem.model.item)
                                 }
                }
            }

            StackLayout {
                id: stackLayout
                Layout.fillWidth: true
                Layout.fillHeight: true
                currentIndex: listTabbar.currentIndex
            }
        }

        // --------------------- just for test
        //        RowLayout {
        //            height: 50
        //            anchors.bottom: parent.bottom
        //            anchors.left: parent.left
        //            anchors.right: parent.right
        //            anchors.margins: 10

        //            TextField {
        //                id: addNewTitle
        //                Layout.fillHeight: true
        //                Layout.fillWidth: true
        //                text: 'newTitle'
        //            }

        //            Button {
        //                Layout.fillHeight: true
        //                Layout.fillWidth: true
        //                text: 'add'

        //                onClicked: {
        //                    appendTest(addNewTitle.text)
        //                }
        //            }
        //        }
        // just for test
    }

    Component.onCompleted: {

        //        appendTest('ayfirst')
        //        appendTest('aysecond')
        //        appendTest('aythird')
        //        appendTest('ayforth')
    }
}

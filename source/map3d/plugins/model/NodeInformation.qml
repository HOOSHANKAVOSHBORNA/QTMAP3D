import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import "style"
Window {
    id:window
    readonly property color verticalBarColor: Qt.rgba(Style.foregroundColor.r, Style.foregroundColor.g, Style.foregroundColor.b, 0.10)
    readonly property color parentColor: Qt.rgba(Style.foregroundColor.r, Style.foregroundColor.g, Style.foregroundColor.b, 0.20)
    property int previousX
    property int previousY
    flags: Qt.Window | Qt.FramelessWindowHint
    width:310/Style.monitorRatio
    height:612/Style.monitorRatio
    color: "transparent"
    property var nodeinfo
    Rectangle{
        color: Style.backgroundColor
        anchors.fill: parent
        radius: 15
    }

    ColumnLayout{
        anchors.fill: parent
        spacing: 0
        Item{
            id:windowframe
            implicitHeight: 30/Style.monitorRatio
            Layout.fillWidth:true
            RowLayout{
                anchors.fill: parent
                spacing:0

                IconImage{
                    Layout.preferredHeight: 22/Style.monitorRatio
                    Layout.preferredWidth: 22/Style.monitorRatio
                    Layout.leftMargin: 15
                    source: nodeinfo ? nodeinfo.icnUrl : ""
                }
                Text{
                    text: nodeinfo ? nodeinfo.title : ""
                    color: Style.foregroundColor
                    font.pixelSize: 17/Style.monitorRatio
                    Layout.fillWidth: true
                }
                Button{
                    id: bookBtn
                    Layout.preferredWidth: 22/Style.monitorRatio
                    Layout.preferredHeight: 22/Style.monitorRatio
                    background:IconImage{
                        width: 22/Style.monitorRatio
                        height: 22/Style.monitorRatio
                        source: bookBtn.checked ? "qrc:/Resources/filled-bookmark.png" : "qrc:/Resources/bookmark.png"
                    }
                    checkable: true
                    onClicked: {
                        nodeinfo.bookmarkChecked(checked)
                    }
                }
                Button{
                    Layout.preferredWidth: 22/Style.monitorRatio
                    Layout.preferredHeight: 22/Style.monitorRatio
                    Layout.rightMargin: 15
                    background:IconImage{
                        width: 22/Style.monitorRatio
                        height: 22/Style.monitorRatio
                        source: "qrc:/Resources/multiply.png"
                    }
                    onClicked:{
                        window.close()
                    }
                }
            }
            MouseArea {
                anchors.fill: parent

                onPressed: {
                    previousX = mouseX
                    previousY = mouseY
                }

                onMouseXChanged: {
                    var dx = mouseX - previousX
                    window.setX(window.x + dx)
                }

                onMouseYChanged: {
                    var dy = mouseY - previousY
                    window.setY(window.y + dy)
                }
               z:-1
            }
}
        Item{
            id:mainImageItem
            Layout.fillWidth: true
            implicitHeight: 175/Style.monitorRatio
            Image{
                id:image
                anchors.fill: parent

                source:nodeinfo ? nodeinfo.imageUrl :""

            }
            Rectangle{
                color: "silver"
                anchors.bottom: mainImageItem.bottom
                width: mainImageItem.width
                height: 30/Style.monitorRatio

                RowLayout{
                    anchors.fill: parent
                    spacing: 0
                    Button{
                        Layout.preferredHeight: 28
                        Layout.preferredWidth: 28
                        Layout.leftMargin: 44
                        background:IconImage{
                            width: 28/Style.monitorRatio
                            height: 28/Style.monitorRatio
                            source: "qrc:/Resources/track-icon.png"
                        }
                    }

                    Text{
                        text: "Track"
                        color: Style.foregroundColor
                        font.pixelSize: 13/Style.monitorRatio
                        Layout.fillWidth: true
                        Layout.leftMargin: 2
                        font.family: Style.fontFamily

                    }
                    Rectangle{
                        id:verticalbar
                        color: verticalBarColor
                        Layout.preferredHeight: 30/Style.monitorRatio
                        Layout.preferredWidth: 2/Style.monitorRatio
                    }
                    Button{
                        Layout.preferredHeight: 28/Style.monitorRatio
                        Layout.preferredWidth: 28/Style.monitorRatio
                        Layout.leftMargin: 44
                        background:IconImage{
                            width: 28/Style.monitorRatio
                            height: 28/Style.monitorRatio
                            source: "qrc:/Resources/easy-to-find.png"
                        }
                        onClicked: {
                            nodeinfo.goToPosition()
                        }
                    }
                    Text{
                        text: "Go to"
                        color: Style.foregroundColor
                        font.pixelSize: 13/Style.monitorRatio
                        Layout.fillWidth: true
                        Layout.leftMargin: 2
                        font.family: Style.fontFamily
                    }
                }
            }
        }
        ScrollView{
            id:treeViewItem
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.topMargin: 5
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

            TreeView{
                id:treeView
                anchors.fill: parent
                anchors.centerIn: parent
                model: nodeinfo
                rowSpacing: 10

                delegate: Item {
                    id: treeDelegate
                    required property TreeView treeView
                    required property bool isTreeNode
                    required property bool expanded
                    required property int hasChildren
                    required property int depth
                    required property bool selected
                    required property bool current


                    implicitWidth: treeView.width
                    implicitHeight: 20/Style.monitorRatio

                    Rectangle{
                        id:parenttext
                        color:treeDelegate.hasChildren ? parentColor : "transparent"
                        radius: 15
                        width: 300/Style.monitorRatio
                        height: 25/Style.monitorRatio
                        anchors.horizontalCenter: parent.horizontalCenter
                        RowLayout{
                            anchors.fill: parent
                            spacing: 0

                            IconImage{
                                Layout.preferredHeight: 19/Style.monitorRatio
                                Layout.preferredWidth: 19/Style.monitorRatio
                                source: iconImageSource ?? ""
                                Layout.alignment: Qt.AlignVCenter
                                Layout.leftMargin: 10
                                visible: treeDelegate.hasChildren
                            }

                            Text{
                                text:display ?? ""
                                font.pixelSize: 18/Style.monitorRatio
                                color: Style.foregroundColor
                                Layout.alignment: Qt.AlignVCenter
                                Layout.fillWidth: true
                            }
                            IconImage{
                                id:indicator
                                Layout.preferredHeight: 15
                                Layout.preferredWidth: 15
                                source: "qrc:/Resources/down.png"
                                Layout.alignment: Qt.AlignVCenter
                                Layout.rightMargin: 10
                                rotation: treeDelegate.expanded ? 180 : 0
                                visible: treeDelegate.hasChildren
                            }
                        }

                        TapHandler{
                            onTapped: {
                                treeView.toggleExpanded(row)
                            }
                        }
                    }
                    Item{
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        RowLayout{
                            anchors.fill: parent
                            spacing: 0
                            Rectangle{
                                color:"transparent"
                                Layout.preferredHeight: 10/Style.monitorRatio
                                Layout.preferredWidth: 160/Style.monitorRatio
                                Text{
                                    text: nameText ?? ""
                                    font.pixelSize: 12/Style.monitorRatio
                                    font.family: Style.fontFamily
                                    color: Style.foregroundColor
                                    anchors.left: parent.left
                                    anchors.leftMargin: 15
                                }
                            }
                            Rectangle{
                                color:"transparent"
                                Layout.preferredHeight: 10/Style.monitorRatio
                                Layout.preferredWidth: 160/Style.monitorRatio
                                Text{
                                    text: valueText ?? ""
                                    font.pixelSize: 12/Style.monitorRatio
                                    font.family: Style.fontFamily
                                    color: Style.foregroundColor

                                    anchors.left: parent.left
                                    anchors.leftMargin: 15
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

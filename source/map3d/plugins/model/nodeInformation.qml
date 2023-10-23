import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
Window {
    id:backgroundRec
    width:310
    height: 612
    color: "#DEE3E6"
    property var nodeinfo
    property color projectcolor: "#003569"
    ColumnLayout{
        anchors.fill: parent
        spacing: 0
        Item{
            id:windowframe
            implicitHeight: 30
            Layout.fillWidth:true
            RowLayout{
                anchors.fill: parent
                spacing:0
                Button{
                    Layout.preferredHeight: 22
                    Layout.preferredWidth: 22
                    Layout.leftMargin: 15
                    background:IconImage{
                        width: 22
                        height: 22
//                        source: nodeinfo ? nodeinfo.getHeaderImageUrl() : ""
                    }
                }
                Text{
                    text: nodeinfo ? nodeinfo.getWindowName() : ""
                    color: projectcolor
                    font.pixelSize: 17
                    Layout.fillWidth: true
                }
                Button{
                    Layout.preferredWidth: 22
                    Layout.preferredHeight: 22
                    background:IconImage{
                        width: 22
                        height: 22
                        source: /*nodeinfo ? (nodeinfo.bookmarkStatus() ? "qrc:/icons/filled-bookmark.png" :*/ "qrc:/Resources/bookmark.png"/*) : ""*/
                    }
                    onClicked: {

                    }
                }
                IconImage{
                    Layout.preferredWidth: 22
                    Layout.preferredHeight: 22
                    source: "qrc:/Resources/multiply.png"
                    Layout.rightMargin: 15
                }
            }
        }

        Item{
            id:mainImageItem
            Layout.fillWidth: true
            implicitHeight: 175
            Image{
                id:image
                anchors.fill: parent
                source:nodeinfo ? nodeinfo.getMainImageUrl() :""
            }
            Rectangle{
                color: "silver"
                anchors.bottom: mainImageItem.bottom
                width: mainImageItem.width
                height: 30

                RowLayout{
                    anchors.fill: parent
                    spacing: 0
                    Button{
                        Layout.preferredHeight: 28
                        Layout.preferredWidth: 28
                        Layout.leftMargin: 44
                        background:IconImage{
                            width: 28
                            height: 28
                            source: "qrc:/Resources/track-icon.png"
                        }
                    }

                    Text{
                        text: "Track"
                        color: projectcolor
                        font.pixelSize: 13
                        Layout.fillWidth: true
                        Layout.leftMargin: 2
                        // font.family: Roboto
                    }
                    Rectangle{
                        id:verticalbar
                        color: Qt.rgba(0,53,105,0.1)
                        Layout.preferredHeight: 30
                        Layout.preferredWidth: 2
                    }
                    Button{
                        Layout.preferredHeight: 28
                        Layout.preferredWidth: 28
                        Layout.leftMargin: 44
                        background:IconImage{
                            width: 28
                            height: 28
                            source: "qrc:/Resources/easy-to-find.png"
                        }
                    }
                    Text{
                        text: "Go to"
                        color: projectcolor
                        font.pixelSize: 13
                        Layout.fillWidth: true
                        Layout.leftMargin: 2
                        // font.family: Roboto
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
                    implicitHeight: 20

                    Rectangle{
                        id:parenttext
                        color:treeDelegate.hasChildren ? Qt.rgba(0, 53, 105, 0.2) : "transparent"
                        radius: 15
                        width: 300
                        height: 25
                        anchors.horizontalCenter: parent.horizontalCenter
                        RowLayout{
                            anchors.fill: parent
                            spacing: 0

                            IconImage{
                                Layout.preferredHeight: 19
                                Layout.preferredWidth: 19
                                source: iconImageSource ?? ""
                                Layout.alignment: Qt.AlignVCenter
                                Layout.leftMargin: 10
                                visible: treeDelegate.hasChildren
                            }

                            Text{
                                text:display ?? ""
                                font.pixelSize: 18
                                color: projectcolor
                                Layout.alignment: Qt.AlignVCenter
                                Layout.fillWidth: true
                            }
                            IconImage{
                                id:indicator
                                Layout.preferredHeight: 15
                                Layout.preferredWidth: 15
                                source: "qrc:/icons/down.png"
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
                                Layout.preferredHeight: 10
                                Layout.preferredWidth: 160
                                Text{
                                    text: propertyText ?? ""
                                    font.pixelSize: 12
                                    // font.family: Roboto
                                    color: projectcolor
                                    anchors.left: parent.left
                                    anchors.leftMargin: 15
                                }
                            }
                            Rectangle{
                                color:"transparent"
                                Layout.preferredHeight: 10
                                Layout.preferredWidth: 160
                                Text{
                                    text: valueText ?? ""
                                    font.pixelSize: 12
                                    // font.family: Roboto
                                    color: projectcolor
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

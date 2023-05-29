
import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13
import QtQuick.Effects
import Crystal 1.0

Item {
    id: rootItem
    implicitWidth:  rect.implicitWidth
    implicitHeight: rect.implicitHeight

    property ListModel itemsModel
    property var clickCallback

    signal listsButtonClicked()
    signal layersButtonClicked()

    Rectangle {
        id: rect
        anchors.fill: parent
        implicitWidth:  rowLayout.implicitWidth + 20
        implicitHeight: rowLayout.implicitHeight + 15

        color: "transparent"
        radius: 10

        Rectangle {
            anchors.fill: parent
            color: "#404040"
            opacity: 0.8
            radius: 10
        }

        MouseArea {
            anchors.fill: parent
        }


        RowLayout {
            id: rowLayout
            anchors.fill: parent
            anchors.margins: 5
            anchors.leftMargin: 10
            anchors.rightMargin: 10
            anchors.topMargin: 10
            spacing: 15

            Repeater {
                model: rootItem.itemsModel
                delegate: Item {
                    Layout.alignment: Qt.AlignCenter
                    Layout.preferredWidth: 64
                    Layout.preferredHeight: 64

                    ColumnLayout {
                        id: col
                        anchors.centerIn: parent
                        Image {
                            id: img
                            Layout.alignment: Qt.AlignCenter
                            Layout.preferredWidth: 24
                            Layout.preferredHeight: 24
                            source: icon_url
                            width: 24
                            height: 24
                        }

                        Label {
                            Layout.alignment: Qt.AlignCenter
                            Layout.preferredWidth: implicitWidth
                            Layout.preferredHeight: implicitHeight
                            text: title_text
                            color: "white"
                            font.family: _fontFamily
                            font.pointSize: _fontPointSize
                        }
                        visible: false
                    }

                    MouseArea {
                        id: mouseArea
                        anchors.fill: col
                        hoverEnabled: true

                        property bool isMouseOnItem: false

                        onEntered: function() {
                            isMouseOnItem = true;
                        }
                        onExited: function() {
                            isMouseOnItem = false;
                        }

                        onClicked: function() {
                            rootItem.clickCallback(index);
                        }
                    }

                    MultiEffect{
                        anchors.fill: col
                        source: col
                        colorization: 1
                        colorizationColor: mouseArea.isMouseOnItem ?
                                   (mouseArea.pressed ? _colorPresed: _colorHover) :
                                   (mouseArea.pressed ? _colorHover : "#FFFFFF");
                    }
                }
            }


            // independent buttons
            Item {
                Layout.alignment: Qt.AlignCenter
                Layout.preferredWidth: 64
                Layout.preferredHeight: 64

                ColumnLayout {
                    id: col2
                    anchors.centerIn: parent
                    Image {
                        id: img2
                        Layout.alignment: Qt.AlignCenter
                        Layout.preferredWidth: 24
                        Layout.preferredHeight: 24
                        source: "qrc:/Resources/Layers.png"
                        width: 24
                        height: 24
                    }

                    Label {
                        Layout.alignment: Qt.AlignCenter
                        Layout.preferredWidth: implicitWidth
                        Layout.preferredHeight: implicitHeight
                        text: "Layers"
                        color: "white"
                        font.family: _fontFamily
                        font.pointSize: _fontPointSize
                    }
                    visible: false
                }

                MouseArea {
                    id: mouseArea2
                    anchors.fill: col2
                    hoverEnabled: true

                    property bool isMouseOnItem: false

                    onEntered: function() {
                        isMouseOnItem = true;
                    }
                    onExited: function() {
                        isMouseOnItem = false;
                    }

                    onClicked: function() {
                        rootItem.layersButtonClicked();
                    }
                }

                MultiEffect {
                    anchors.fill: col2
                    source: col2
                    colorization: 1
                    colorizationColor: mouseArea2.isMouseOnItem ?
                               (mouseArea2.pressed ? _colorPresed: _colorHover) :
                               (mouseArea2.pressed ? _colorHover : "#FFFFFF");
                }
            }


            Item {
                Layout.alignment: Qt.AlignCenter
                Layout.preferredWidth: 64
                Layout.preferredHeight: 64

                ColumnLayout {
                    id: col1
                    anchors.centerIn: parent
                    Image {
                        id: img1
                        Layout.alignment: Qt.AlignCenter
                        Layout.preferredWidth: 24
                        Layout.preferredHeight: 24
                        source: "qrc:///Resources/Outliner.png"
                        width: 24
                        height: 24
                    }

                    Label {
                        Layout.alignment: Qt.AlignCenter
                        Layout.preferredWidth: implicitWidth
                        Layout.preferredHeight: implicitHeight
                        text: "List"
                        color: "white"
                        font.family: _fontFamily
                        font.pointSize: _fontPointSize
                    }
                    visible: false
                }

                MouseArea {
                    id: mouseArea1
                    anchors.fill: col1
                    hoverEnabled: true

                    property bool isMouseOnItem: false

                    onEntered: function() {
                        isMouseOnItem = true;
                    }
                    onExited: function() {
                        isMouseOnItem = false;
                    }

                    onClicked: function() {
                        rootItem.listsButtonClicked();
                        //rootItem.clickCallback(index);
                    }
                }

                MultiEffect {
                    anchors.fill: col1
                    source: col1
                    colorization: 1
                    colorizationColor: mouseArea1.isMouseOnItem ?
                               (mouseArea1.pressed ? _colorPresed: _colorHover) :
                               (mouseArea1.pressed ? _colorHover : "#FFFFFF");
                }
            }
//            Item {
//                Layout.alignment: Qt.AlignCenter
//                Layout.preferredWidth: 64
//                Layout.preferredHeight: 64

//                ColumnLayout {
//                    id: col3
//                    anchors.centerIn: parent
//                    Image {
//                        id: img3
//                      Layout.alignment: Qt.AlignCenter
//                        Layout.preferredWidth: 24
//                        Layout.preferredHeight: 24
//                        source: "qrc:///Resources/close.png"
//                        width: 24
//                        height: 24
//                    }

//                    Label {
//                        Layout.alignment: Qt.AlignCenter
//                        Layout.preferredWidth: implicitWidth
//                        Layout.preferredHeight: implicitHeight
//                        text: "Exit"
//                        color: "white"
//                        font.family: _fontFamily
//                        font.pointSize: 11
//                    }
//                    visible: false
//                }

//                MouseArea {
//                    id: mouseArea3
//                    anchors.fill: col3
//                    hoverEnabled: true

//                    property bool isMouseOnItem: false

//                    onEntered: function() {
//                        isMouseOnItem = true;
//                    }
//                    onExited: function() {
//                        isMouseOnItem = false;
//                    }

//                    onClicked: function() {
//                        wnd.close();
//                        //rootItem.clickCallback(index);
//                    }
//                }

//                ColorOverlay {
//                    anchors.fill: col3
//                    source: col3
//                    color: mouseArea3.isMouseOnItem ?
//                               (mouseArea3.pressed ? _colorPresed: _colorHover) :
//                               (mouseArea3.pressed ? _colorHover : "#FFFFFF");
//                }
//            }
//            Item {
//                Layout.alignment: Qt.AlignCenter
//                Layout.preferredWidth: 64
//                Layout.preferredHeight: 64

//                ColumnLayout {
//                    id: col4
//                    anchors.centerIn: parent
//                    Image {
//                        id: img4
//                        Layout.alignment: Qt.AlignCenter
//                        Layout.preferredWidth: 24
//                        Layout.preferredHeight: 24
//                        source: "qrc:///Resources/collapse.png"
//                        width: 24
//                        height: 24
//                    }

//                    Label {
//                        Layout.alignment: Qt.AlignCenter
//                        Layout.preferredWidth: implicitWidth
//                        Layout.preferredHeight: implicitHeight
//                        text: "Mininnize"
//                        color: "white"
//                        font.family: _fontFamily
//                        font.pointSize: 11
//                    }
//                    visible: false
//                }

//                MouseArea {
//                    id: mouseArea4
//                    anchors.fill: col4
//                    hoverEnabled: true

//                    property bool isMouseOnItem: false

//                    onEntered: function() {
//                        isMouseOnItem = true;
//                    }
//                    onExited: function() {
//                        isMouseOnItem = false;
//                    }

//                    onClicked: function() {
//                        wnd.showMinimized()
//                        //rootItem.clickCallback(index);
//                    }
//                }

//                ColorOverlay {
//                    anchors.fill: col4
//                    source: col4
//                    color: mouseArea4.isMouseOnItem ?
//                               (mouseArea4.pressed ? _colorPresed: _colorHover) :
//                               (mouseArea4.pressed ? _colorHover : "#FFFFFF");
//                }
//            }







        }
    }
}

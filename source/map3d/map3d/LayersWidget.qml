import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13
import Crystal 1.0
import QtGraphicalEffects 1.12




Item {
    id: rootItem


    implicitWidth: 300
    property CLayersModel layersModel

    property bool layersItemVisible: false

    function menuWidgetLayersButtonClicked() {
        layersItemHideAnimation.stop();
        layersItemShowAnimation.stop();

        if (rootItem.layersItemVisible == true) {
            layersItemHideAnimation.to = 310 + (widgetsMargins * 2.0);
//            layersItemHideAnimation.from = 0;
            layersItemHideAnimation.duration = 200;
            rootItem.layersItemVisible = false;
            layersItemHideAnimation.start();
        } else {
            layersItemShowAnimation.from = 310 + (widgetsMargins * 2.0);
            layersItemShowAnimation.to = 0;
            layersItemShowAnimation.duration = 200;
            rootItem.layersItemVisible = true;
            layersItemShowAnimation.start();
        }
    }

    signal toggleLayerEnabled(int layerIndex)

    PropertyAnimation {
        id: layersItemShowAnimation
        target: layersItem
        property: 'x'
        easing.type: Easing.OutQuint
    }
    PropertyAnimation {
        id: layersItemHideAnimation
        target: layersItem
        property: 'x'
        easing.type: Easing.InQuint
    }

    Item {
        id: layersItem
        clip: false

        y: 0
        height:parent.height
        width: 300
        x: 310 + (widgetsMargins * 2)

        MouseArea {
            anchors.fill: parent
        }




              }

        Rectangle {
            anchors.fill: parent
            radius: 10
            color: "#404040"
            opacity: 0.8
        }

        ColumnLayout {
            anchors.fill: parent

            Label {
                color: "white"
                text: "Layers"
                font.family: "SourceSansPro"
                font.pointSize: 14
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignCenter
                Layout.minimumHeight: implicitHeight + 16
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter

            }


            Item {
                clip:true
                Layout.fillWidth: true
                Layout.fillHeight: true
                id: containerItem
                Layout.margins: 10


                ScrollView {
                    anchors.fill: parent
                    ColumnLayout {
                        width: containerItem.width
                        id: col


                        Repeater {
                            model: rootItem.layersModel

                            Item {
                                id: row
                                Layout.fillWidth: true
                                Layout.minimumHeight: lay.implicitHeight
                                RowLayout {
                                    id:lay
                                    anchors.fill: parent

                                    Item {
                                        id: img
                                        Layout.preferredWidth: 24
                                        Layout.preferredHeight: 24
                                        Layout.alignment: Qt.AlignCenter

                                        Image {
                                            anchors.fill: parent
                                            source: "qrc:/Resources/eye_open.png"
                                            sourceSize: Qt.size(24,24)
                                            visible: layer_enabled
                                        }
                                        Image {
                                            anchors.fill: parent
                                            source: "qrc:/Resources/eye_close.png"
                                            sourceSize: Qt.size(24,24)
                                            visible: layer_enabled == false
                                        }
                                    }


                                    Label {
                                        id: label
                                        text: display
                                        color: "white"

                                        Layout.alignment: Qt.AlignCenter
                                        Layout.fillWidth: true

                                    }

                                }

                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: function() {
                                        rootItem.toggleLayerEnabled(layer_index);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
//}


import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13
import Crystal 1.0

Item {
    id: rootItem

    implicitWidth: 300
    property CLayersModel layersModel

    signal toggleLayerEnabled(int layerIndex)

    Item {
        id: layersItem

        y: 0
        height:parent.height
        width: 300
        x: 0

        Rectangle {
            anchors.fill: parent
            radius: 10
            color: "#404040"
            opacity: 0.8
        }
        ColumnLayout {
            anchors.margins: 10
            anchors.fill: parent

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

            Item {
                Layout.fillHeight: true
            }
        }
    }
}

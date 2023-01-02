import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13

Item {
    clip:true
    id:rootItem


    property ListModel detectionSystemsModel: ListModel {
    }
    property ListModel sendsModel: ListModel {
    }

    function updateData(jsonObject) {


        rootItem.detectionSystemsModel.clear();
        for (var idx1 in jsonObject.DetectionSystem) {
            detectionSystemsModel.append({"systemName": jsonObject.DetectionSystem[idx1]});
        }

        rootItem.sendsModel.clear();
        for (var idx2 in jsonObject.Send) {
            sendsModel.append({"sendName": jsonObject.Send[idx2]});
        }


        if (jsonObject.TN)                   tnLabel.text = jsonObject.TN;
        if (jsonObject.IFFCode)              iffCodeLabel.text = jsonObject.IFFCode;
        if (jsonObject.CallSign)             callSignLabel.text = jsonObject.CallSign;
        if (jsonObject.Type)                 typeLabel.text = jsonObject.Type;
        if (jsonObject.MasterRadar)          masterLabel.text = jsonObject.MasterRadar;
        if (jsonObject.Identification)       identificationLabel.text = jsonObject.Identification;
        if (jsonObject.IdentificationMethod) identificationMethodLabel.text = jsonObject.IdentificationMethod;
        if (jsonObject.Time)                 timeLabel.text = jsonObject.Time;
        if (jsonObject.Pos)                  posLabel.text = jsonObject.Pos;


        if (jsonObject.Latitude)  latitudeLabel.text = jsonObject.Latitude;
        if (jsonObject.Longitude) longitudeLabel.text = jsonObject.Longitude;
        if (jsonObject.Altitude)  altitudeLabel.text = jsonObject.Altitude;
        if (jsonObject.Heading)   headingLabel.text = jsonObject.Heading;
        if (jsonObject.Speed)     speedLabel.text = jsonObject.Speed;
    }

    ScrollView {
        anchors.fill: parent
        contentWidth: lay.width + 30
        contentHeight: lay.height + 15

        ColumnLayout {
            id:lay
            spacing: 10

            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            width: rootItem.width - 30


            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: width * (img.sourceSize.height / img.sourceSize.width)
                Image {
                    id: img
                    anchors.fill: parent
                    source: "qrc:///Resources/airplan.jpg"
                    fillMode: Image.Stretch
                }
            }


            Rectangle {
                Layout.preferredWidth: rootItem.width - 30
                Layout.preferredHeight: 2
                color: "white"

            }

            GridLayout {
                rows: 9
                columns: 2
                Layout.fillWidth: true
                Layout.preferredHeight: implicitHeight

                Label {
                    text: "TN: "
                    color: "white"
                    Layout.preferredWidth: implicitWidth
                }
                Label {
                    id: tnLabel
                    text: "0.0"
                    color: "white"
                    Layout.fillWidth: true
                    horizontalAlignment: Qt.AlignRight
                }


                Label {
                    text: "IFFCode: "
                    color: "white"
                    Layout.preferredWidth: implicitWidth
                }
                Label {
                    id: iffCodeLabel
                    text: "0.0"
                    color: "white"
                    Layout.fillWidth: true
                    horizontalAlignment: Qt.AlignRight
                }

                Label {
                    text: "CallSign: "
                    color: "white"
                    Layout.preferredWidth: implicitWidth
                }
                Label {
                    id: callSignLabel
                    text: "0.0"
                    color: "white"
                    Layout.fillWidth: true
                    horizontalAlignment: Qt.AlignRight
                }


                Label {
                    text: "Type: "
                    color: "white"
                    Layout.preferredWidth: implicitWidth
                }
                Label {
                    id: typeLabel
                    text: "0.0"
                    color: "white"
                    Layout.fillWidth: true
                    horizontalAlignment: Qt.AlignRight
                }


                Label {
                    text: "Master: "
                    color: "white"
                    Layout.preferredWidth: implicitWidth
                }
                Label {
                    id: masterLabel
                    text: "0.0"
                    color: "white"
                    Layout.fillWidth: true
                    horizontalAlignment: Qt.AlignRight
                }

                Label {
                    text: "Identification: "
                    color: "white"
                    Layout.preferredWidth: implicitWidth
                }
                Label {
                    id: identificationLabel
                    text: "0.0"
                    color: "white"
                    Layout.fillWidth: true
                    horizontalAlignment: Qt.AlignRight
                }

                Label {
                    text: "Identification Method: "
                    color: "white"
                    Layout.preferredWidth: implicitWidth
                }
                Label {
                    id: identificationMethodLabel
                    text: "0.0"
                    color: "white"
                    Layout.fillWidth: true
                    horizontalAlignment: Qt.AlignRight
                }

                Label {
                    text: "Time: "
                    color: "white"
                    Layout.preferredWidth: implicitWidth
                }
                Label {
                    id: timeLabel
                    text: "0.0"
                    color: "white"
                    Layout.fillWidth: true
                    horizontalAlignment: Qt.AlignRight
                }

                Label {
                    text: "Pos: "
                    color: "white"
                    Layout.preferredWidth: implicitWidth
                }
                Label {
                    id: posLabel
                    text: "0.0"
                    color: "white"
                    Layout.fillWidth: true
                    horizontalAlignment: Qt.AlignRight
                }

            }

            Rectangle {
                Layout.preferredWidth: rootItem.width - 30
                Layout.preferredHeight: 2
                color: "white"

            }

            GridLayout {
                rows: 5
                columns: 2
                Layout.fillWidth: true
                Layout.preferredHeight: implicitHeight

                Label {
                    text: "Latitude: "
                    color: "white"
                    Layout.preferredWidth: implicitWidth
                }
                Label {
                    id: latitudeLabel
                    text: "0.0"
                    color: "white"
                    Layout.fillWidth: true
                    horizontalAlignment: Qt.AlignRight
                }



                Label {
                    text: "Longitude: "
                    color: "white"
                    Layout.preferredWidth: implicitWidth
                }
                Label {
                    id: longitudeLabel
                    text: "0.0"
                    color: "white"
                    Layout.fillWidth: true
                    horizontalAlignment: Qt.AlignRight
                }


                Label {
                    text: "Altitude: "
                    color: "white"
                    Layout.preferredWidth: implicitWidth
                }
                Label {
                    id: altitudeLabel
                    text: "0.0"
                    color: "white"
                    Layout.fillWidth: true
                    horizontalAlignment: Qt.AlignRight
                }



                Label {
                    text: "Heading: "
                    color: "white"
                    Layout.preferredWidth: implicitWidth
                }
                Label {
                    id: headingLabel
                    text: "0.0"
                    color: "white"
                    Layout.fillWidth: true
                    horizontalAlignment: Qt.AlignRight
                }


                Label {
                    text: "Speed: "
                    color: "white"
                    Layout.preferredWidth: implicitWidth
                }
                Label {
                    id: speedLabel
                    text: "0.0"
                    color: "white"
                    Layout.fillWidth: true
                    horizontalAlignment: Qt.AlignRight
                }

            }

            Rectangle {
                Layout.preferredWidth: rootItem.width - 30
                Layout.preferredHeight: 2
                color: "white"

            }

            Label {
                Layout.preferredWidth: rootItem.width - 30
                Layout.preferredHeight: implicitHeight
                color: "white"
                text: "Detection Systems"
                horizontalAlignment: Qt.AlignHCenter
            }

            Repeater {
                model: rootItem.detectionSystemsModel
                Label {
                    Layout.preferredWidth: rootItem.width - 30
                    Layout.preferredHeight: implicitHeight
                    color: "white"
                    text: systemName
                    horizontalAlignment: Qt.AlignLeft
                }

            }

            Rectangle {
                Layout.preferredWidth: rootItem.width - 30
                Layout.preferredHeight: 2
                color: "white"

            }

            Label {
                Layout.preferredWidth: rootItem.width - 30
                Layout.preferredHeight: implicitHeight
                color: "white"
                text: "Sends"
                horizontalAlignment: Qt.AlignHCenter
            }

            Repeater {
                model: rootItem.sendsModel
                Label {
                    Layout.preferredWidth: rootItem.width - 30
                    Layout.preferredHeight: implicitHeight
                    color: "white"
                    text: sendName
                    horizontalAlignment: Qt.AlignLeft
                }

            }


        }
    }
}

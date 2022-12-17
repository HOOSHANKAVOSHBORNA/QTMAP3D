import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13

Item {
    clip:true
    id:rootItem


    function updateData(jsonObject) {

        console.log(jsonObject);

        if (jsonObject.Data.TN)                   tnLabel.text = jsonObject.Data.TN;
        if (jsonObject.Data.IFFCode)              iffCodeLabel.text = jsonObject.Data.IFFCode;
        if (jsonObject.Data.CallSign)             callSignLabel.text = jsonObject.Data.CallSign;
        if (jsonObject.Data.Type)                 typeLabel.text = jsonObject.Data.Type;
        if (jsonObject.Data.MasterRadar)          masterLabel.text = jsonObject.Data.MasterRadar;
        if (jsonObject.Data.Identification)       identificationLabel.text = jsonObject.Data.Identification;
        if (jsonObject.Data.IdentificationMethod) identificationMethodLabel.text = jsonObject.Data.IdentificationMethod;
        if (jsonObject.Data.Time)                 timeLabel.text = jsonObject.Data.Time;
        if (jsonObject.Data.Pos)                  posLabel.text = jsonObject.Data.Pos;


        if (jsonObject.Data.Latitude)  latitudeLabel.text = jsonObject.Data.Latitude;
        if (jsonObject.Data.Longitude) longitudeLabel.text = jsonObject.Data.Longitude;
        if (jsonObject.Data.Altitude)  altitudeLabel.text = jsonObject.Data.Altitude;
        if (jsonObject.Data.Heading)   headingLabel.text = jsonObject.Data.Heading;
        if (jsonObject.Data.Speed)     speedLabel.text = jsonObject.Data.Speed;
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



        }
    }
}

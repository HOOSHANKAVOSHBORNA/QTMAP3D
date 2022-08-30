import QtQuick 2.0
import QtQuick.Layouts 1.12

Item {
                property string title: "teste"
                height:column.implicitHeight +6
                width: column.implicitWidth

                ColumnLayout{
                    id:column
                    anchors.top: parent.top
                    anchors.topMargin: 3
                    width: laout_back.width
                     Rectangle {
                        color: "#AA000000"
                        border.color: "#ffffff"
                        border.width: 1
                        radius: 5
                        width: 200
                        height: 30
                        Layout.alignment: Qt.AlignHCenter


                    }



                }

            }

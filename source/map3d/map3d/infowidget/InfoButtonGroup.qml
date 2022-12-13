
import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13

Item {
    id:rootItem

    signal view2D3DButtonClicked();
    signal routeButtonClicked();
    signal followButtonClicked();
    signal moreButtonClicked();

    ListModel {
        id: buttonsModel

        ListElement {
            buttonText: "2D/3D"
            iconUrl: "qrc:///Resources/3D.png"
            clickCallback: function() { rootItem.view2D3DButtonClicked(); }
        }

        ListElement {
            buttonText: "Route"
            iconUrl:"qrc:///Resources/route.png"
            clickCallback: function() { rootItem.routeButtonClicked(); }
        }

        ListElement {
            buttonText: "Follow"
            iconUrl:"qrc:///Resources/tracking.png"
            clickCallback: function() { rootItem.followButtonClicked(); }
        }

        ListElement {
            buttonText: "More"
            iconUrl: "qrc:///Resources/more.png"
            clickCallback: function() { rootItem.moreButtonClicked(); }
        }
    }

    RowLayout {
        anchors.fill: parent
        spacing: 0

        Repeater {
            model: buttonsModel
            Button {
                id: control
                Layout.fillWidth:  true
                Layout.fillHeight: true
                text: buttonText
                font.pointSize: 10
                font.weight: Font.Normal
                icon.source: iconUrl
                display: AbstractButton.TextUnderIcon
                background: Rectangle {
                    color: "#FFFFFF"
                    opacity: 0.0
                }
                contentItem: Item {

                    Column {
                        anchors.centerIn: parent
                        spacing: 5
                        Image {
                            anchors.horizontalCenter: parent.horizontalCenter

                            source: iconUrl
                            sourceSize: Qt.size(24,24)
                            width: 24
                            height:24
                        }

                        Text {
                            anchors.horizontalCenter: parent.horizontalCenter
                            text: control.text
                            font: control.font
                            opacity: enabled ? 1.0 : 0.3
                            color: control.down ? "#17a81a" : "white"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            elide: Text.ElideRight
                        }

                    }
                }

                onClicked: function() {
                    clickCallback()
                }

            }
        }

    }

}

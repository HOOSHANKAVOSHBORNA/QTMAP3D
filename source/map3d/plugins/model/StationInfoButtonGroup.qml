
import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13

Item {
    id:rootItem

    signal gotoButtonClicked();
    signal routeButtonClicked(bool check);
    signal trackButtonClicked(bool check);
    signal moreButtonClicked();

    property var _checked: "#908000"
    property var _colorHover: "#FFCC00"
    property var activeButton: "Active"
    ListModel {
        id: buttonsModel

        ListElement {
            buttonText: "Range"
            iconUrl: "qrc:///Resources/goto.png"
            checkable: true
            checked: false
            clickCallback: function(check) { rootItem.gotoButtonClicked();}
        }

        ListElement {
            buttonText: "Visibility"
            iconUrl:"qrc:///Resources/route.png"
            checkable: true
            checked: false
            clickCallback: function(check) { rootItem.routeButtonClicked(check);}
        }

        ListElement {
            buttonText: activeButton
            iconUrl:"qrc:///Resources/tracking.png"
            checkable: true
            checked: false
            clickCallback: function(check) { rootItem.trackButtonClicked(check);}
        }

        ListElement {
            buttonText: "More"
            iconUrl: "qrc:///Resources/more.png"
            checkable: false
            checked: false
            clickCallback: function(check) { rootItem.moreButtonClicked();}
        }
    }

    RowLayout {
        anchors.fill: parent
        spacing: 0

        Repeater {
            id: rep
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
                            color: buttonsModel.get(index).checkable ? (buttonsModel.get(index).checked ? _checked :
                                                                              (hovered ?  _colorHover : "#FFFFFF")) : (hovered ? "#FFCC00" :  "#FFFFFF")
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            elide: Text.ElideRight
                        }

                    }
                }

                onClicked: function() {
                    clickCallback(!buttonsModel.get(index).checked)
                    buttonsModel.setProperty(index, "checked", buttonsModel.get(index).checked ? false : true)

                }

            }
        }

    }

}

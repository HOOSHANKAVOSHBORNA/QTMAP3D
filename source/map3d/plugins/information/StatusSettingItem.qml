import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root
    anchors.fill: parent


    property var filterModel



    ListView{
        id:listView
        anchors.fill: parent
        // anchors.fill: parent
        model: filterModel
        delegate: RowLayout{

            CheckBox {
                id: control
                text: fieldName
                checked: fieldIsActive
                checkable:true
                indicator: Rectangle {
                    implicitWidth: 20
                    implicitHeight: 20
                    anchors.verticalCenter: parent.verticalCenter
                    radius: 3
                    border.color: control.checked ?  "#01AED6" : "#003569"

                    Rectangle {
                        width: 10
                        height: 10
                        anchors.centerIn: parent
                        radius: 2
                        color: control.checked ?  "#01AED6" : "#003569"
                        visible: control.checked
                    }
                }

                contentItem: Text {
                    text: control.text
                    font: control.font
                    opacity: enabled ? 1.0 : 0.3
                    color: control.checked ? "#01AED6" : "#003569"
                    verticalAlignment: Text.AlignVCenter
                    leftPadding: control.indicator.width + control.spacing

                }
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        filterModel.toggleCheck(root.filterModel.index(index, 0),!fieldIsActive)
                    }
                }
            }
            //                Label{

            //                    Layout.leftMargin: -15
            //                    text:modelData
            //                    color: control.checked ?  "#01AED6" : "#003569"
            //                    verticalAlignment: Text.AlignVCenter

            ////                    font.family: "Roboto"
            //                    font.pixelSize : 15
            //                    MouseArea{
            //                        anchors.fill: parent
            //                        onClicked: {
            //                            print(currentIndex , index )
            //                            listView.currentIndex = index;
            //                            print(index)
            //                        }
            //                }

            //                }
        }
    }
}


import QtQuick 2.13
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.13
import QtQuick.Controls.Styles 1.4
import QtQuick.Extras 1.4
import QtQml 2.13
import Crystal 1.0

Item {
    id: root
    anchors.fill: parent
    property var nowIndex: 0
    property var nowText: ""
    property ContextMenumodel model: ContextMenumodel {

    }

    function addMenuItem(listToShow) {
        for (var i = 0; i < listToShow.length; i++){
            pieMenu.addItem(listToShow[i])
            pieMenu.menuItems[i].triggered.connect(itemClicked)

        }

    }

    function itemClicked() {
        root.model.itemClicked(nowIndex, nowText)
    }

    PieMenu {
        id: pieMenu
        anchors.centerIn: parent
        style: PieMenuStyle {
            id: pieStyle
            shadowRadius: 20
            endAngle: -270
            startAngle: -90
            menuItem: Item {
                id: item
                rotation: -90 + sectionCenterAngle(styleData.index)

                property bool sh: styleData.hovered
                onShChanged: {
                    nowIndex = styleData.index
                    nowText = control.menuItems[styleData.index].text
                }

                Rectangle {
                    id: rect
                    PropertyAnimation {
                        target: rect
                        property: "width"
                        from: 0
                        to: parent.height * 0.25
                        duration: 300
                        running: true
                    }


                    width: styleData.hovered ? parent.height * 0.35 : parent.height * 0.25
                    height: width
                    color:  "#505050"
                    opacity: 0.4
                    radius: width / 2

                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter

                }
                Text {
                    id: textItem
                    text: control.menuItems[styleData.index]? control.menuItems[styleData.index].text : ""
                    opacity: 1
                    anchors.centerIn: rect
                    color: styleData.hovered ? "#FFCC00" : "white"
                    rotation: -item.rotation

                }

            }

        }
    }
    Component.onCompleted: pieMenu.popup(0, 0)

}

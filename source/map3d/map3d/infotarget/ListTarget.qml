import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.13

Item {



    width: parent.width - _margin
    anchors.horizontalCenter: parent.horizontalCenter
    height: 300
    Rectangle{
        anchors.fill: parent
        border.width: 1
        border.color: _colorRec
        radius: _radius
        layer.enabled: true
}
    ScrollView {
        id: scroller
        anchors.fill: parent
        contentHeight: columnLayout.implicitHeight
        ScrollBar.vertical.interactive : false
        clip : true
        Item {
            id: tmp
            anchors.fill: parent
        }
        Column {
            id :columnLayout
            anchors.fill: parent
            spacing: 2
            Repeater {
                id: repeater
                model: rootItem.toolboxModel
            }
        }
    }
}

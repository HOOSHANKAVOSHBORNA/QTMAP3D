import QtQuick 2.15
import QtQuick.Shapes
import QtQuick.Controls.Material
import QtQuick.Layouts

Item {
    id: rootItem
    // ----------------------------------------- properties
    property var listModel // {name, iconUrl, checkable, checked}
    property double outerRadius: 200
    property double innerRadius: 150
    property double startAngle: -90 - currentCutLen * listModel.rowCount() / 2
    property double currentCutLen: 45
    property double animationDuration: 2000

    readonly property double scaleNameLen: 10

    // ----------------------------------------- signals
    signal clicked(int index)
    signal toggled(int index, bool checked)

    anchors.fill: parent


    // ----------------------------------------- animation
    PropertyAnimation on outerRadius {
        id: outerAnimation
        easing.type: Easing.OutElastic
        from: 0
        to: outerRadius
        duration: parseInt(animationDuration)
    }

    PropertyAnimation on innerRadius {
        id: innerAnimation
        easing.type: Easing.OutElastic
        from: 0
        to: innerRadius
        duration: parseInt(animationDuration)
    }

    // ----------------------------------------- fake background
    Rectangle {
        id: fakeOuterCircle

        visible: false

        width: outerRadius * 2
        height: outerRadius * 2
        radius: outerRadius
        opacity: 0.5
        color: 'transparent'
        //        color: 'pink'
        anchors.centerIn: parent

        Rectangle {
            id: fakeInnerCircle

            width: innerRadius * 2
            height: innerRadius * 2
            radius: innerRadius
            opacity: 0.6

            color: 'gold'
            anchors.centerIn: parent
        }
    }

    // ----------------------------------------- Circular Menu
    Repeater {
//        id:         anchors.fill: parent
        model: listModel

        CircularCut {
            required property int index
            required property var model
            checkable: model.checkable
            checked: model.checked
            cutText: model.name
            cutOuterRadius: outerRadius
            cutInnerRadius: innerRadius
            cutStartAngle: startAngle + model.index * currentCutLen
            cutLen: currentCutLen
            cutIconSource: model.iconUrl
            onClicked: rootItem.clicked(model.index)
            onToggled: rootItem.toggled(model.index, checked)
        }
    }
}

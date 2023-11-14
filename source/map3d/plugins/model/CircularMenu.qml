import QtQuick 2.15
import QtQuick.Shapes
import QtQuick.Controls.Material
import QtQuick.Layouts

Item {
    // ----------------------------------------- properties
    property ListModel listModel: ListModel{} // {name, color, iconSource}
    property double outerRadius: 200
    property double innerRadius: 150
    property double startAngle: -90 - currentCutLen * listModel.count / 2
    property double currentCutLen: 45
    property double animationDuration: 2000

    readonly property double scaleNameLen: 10

    // ----------------------------------------- signals
    signal clickedIndex(int index)

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
        anchors.fill: parent
        model: listModel.count

        CircularCut {
            required property var model

            cutText: listModel.get(model.index).name
            cutOuterRadius: outerRadius
            cutInnerRadius: innerRadius
            cutStartAngle: startAngle + model.index * currentCutLen
            cutLen: currentCutLen
            cutIconSource: listModel.get(model.index).iconSource
            cutIconColor: listModel.get(model.index).color
            onCutClicked: clickedIndex(model.index)
        }
    }
}

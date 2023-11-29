import QtQuick 2.15
import QtQuick.Shapes
import QtQuick.Controls.Material
import QtQuick.Layouts
import "style"

Item {
    id: rootItem
    // ----------------------------------------- properties
    property var listModel // {name, iconUrl, checkable, checked}
    property double outerRadius: 200
    property double innerRadius: 150
    property double midAngle: -90
    property double startAngle: midAngle - currentCutLen * listModel.rowCount() / 2
//    property double startAngle: -180
    property double currentCutLen: 45
    property double animationDuration: 2000
    property bool animationStarter: false
    property bool animationEnder: false
    property color styleHoverColor: Style.hoverColor
    property color hover20: Qt.rgba(styleHoverColor.r, styleHoverColor.g, styleHoverColor.b, 0.20)

    readonly property double scaleNameLen: 10

    // ----------------------------------------- signals
    signal clicked(int index)
    signal toggled(int index, bool checked)

    anchors.fill: parent


    // ----------------------------------------- animation
//    PropertyAnimation on outerRadius {
//        id: outerAnimation
//        easing.type: Easing.OutElastic
//        from: 0
//        to: outerRadius
//        duration: animationDuration
//        running: animationStarter
//    }

//    PropertyAnimation on innerRadius {
//        id: innerAnimation
//        easing.type: Easing.OutElastic
//        from: 0
//        to: innerRadius
//        duration: animationDuration
//        running: animationStarter
//    }

    // ----------------------------------------- fake background
    //    Rectangle {
    //        id: fakeOuterCircle
    //        visible: false
    //        width: outerRadius * 2
    //        height: outerRadius * 2
    //        radius: outerRadius
    //        opacity: 0.5
    //        color: 'transparent'
    //        anchors.centerIn: parent

    //        Rectangle {
    //            id: fakeInnerCircle
    //            width: innerRadius * 2
    //            height: innerRadius * 2
    //            radius: innerRadius
    //            opacity: 0.6
    //            color: 'transparent'
    //            anchors.centerIn: parent
    //        }
    //    }

    // ----------------------------------------- real background
    Item {
        anchors.fill: parent

        Shape {
            anchors.centerIn: parent
            containsMode: Shape.FillContains

            ShapePath {
                strokeColor: Qt.rgba(0, 0.68, 0.84, 0.20)
                strokeWidth: 25
                fillColor: 'transparent'
                capStyle: ShapePath.RoundCap

                PathAngleArc {
                    centerX: 0
                    centerY: 0
                    radiusX: outerRadius - 25/2
                    radiusY: outerRadius - 25/2
                    startAngle: rootItem.startAngle
                    sweepAngle: (midAngle - startAngle) * 2
                }
            }
        }
    }


    // ----------------------------------------- Circular Menu
    Repeater {
        onVisibleChanged: {
            if (visible) {
                animationStarter = true
//                startAngle = midAngle - currentCutLen * listModel.rowCount() / 2
//                currentCutLen = 180 / listModel.rowCount()
                startAngle = midAngle - currentCutLen * listModel.rowCount() / 2
            } else {
                animationStarter = false
            }
        }
        model: listModel

        CircularCut {
            required property int index
            required property var model
            checkable: model.checkable
            checked: model.checked
            text: model.name
            outerRadius: rootItem.outerRadius
            innerRadius: rootItem.innerRadius
            startAngle: rootItem.startAngle + model.index * currentCutLen
            len: currentCutLen
            iconSource: model.iconUrl
            iconSize: 35
            onClicked: rootItem.clicked(model.index)
            onToggled: rootItem.toggled(model.index, checked)
        }
    }
}

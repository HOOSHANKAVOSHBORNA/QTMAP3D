import QtQuick 2.13
import QtQuick.Window 2.12
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.12
import QtQuick.Dialogs
import Crystal 1.0
//import "style"

Item {
    anchors.fill: parent
    id: rootItem
    implicitHeight: parent ? parent.height : 0

    property string _headerTitleSTR: "Properties"
    property CProperty model
    property int lblWidth: 100
    property int valHeight: 25
    //////////////////////////Main Content////////////////////////
    Rectangle {
        id: item
        visible: true
        width: parent.width *0.9
        height: rootItem.height
        anchors.fill: parent
        color: "#404040"
        opacity: 0.9
        anchors.margins: 6
        radius: 10

        ScrollView {
            id: frame
            clip: true
            contentWidth: parent.width
            anchors.fill: parent
            /////////////////////// components Grid ////////////////////////////
            ColumnLayout{
                id: mainLayout
                anchors.fill: parent
                anchors.margins: 10
                ////////////////////////////////// Name ////////////////////////////////////////////////
                RowLayout{
                    id:nameSec
                    Layout.fillWidth: true
                    Text {
                        Layout.preferredWidth: lblWidth
                        text: "name : "
                        font.pointSize: 10
                        color: "white"
                    }
                    TextField {
                        id: namevalue
                        Layout.preferredWidth: 150
                        Layout.minimumWidth: 100
                        Layout.fillWidth: true
                        height: valHeight
                        text: rootItem.model ? rootItem.model.name : ""
                        font.pointSize: 10
                        color: "black"
                        onAccepted: {
                            rootItem.model.name = namevalue.displayText
                        }
                    }
                }
                ////////////////////////////////////fill Color Property//////////////////////////////////

                //---------------------------------fill color----------------------------//////////
                Rectangle{
                    color: "white"
                    Layout.fillWidth: true
                    height: 2
                    visible: fillcolorSec.visible
                }
                RowLayout{
                    id:fillcolorSec
                    Layout.fillWidth: true
                    visible: rootItem.model ? rootItem.model.fillColorStatus : false

                    Text {
                        Layout.preferredWidth: lblWidth
                        text: "Fill Color: "
                        font.pointSize: 10
                        color: "white"
                    }

                    Rectangle{
                        width: valHeight
                        height: valHeight
                        color: rootItem.model ? rootItem.model.fillColor : false
                        border.width: 2
                        border.color: "#c9c9c9"
                        radius: 5
                        MouseArea{
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            onClicked: fillColorDialog.visible = true
                        }
                    }
                    SpinBox {
                        id: opacityValue
                        Layout.fillWidth: true
                        Layout.minimumWidth: 80
                        height: valHeight
                        from : 0
                        to : 100
                        onValueChanged: {
                            rootItem.model.fillColor.a = value/100
                        }
                    }
                    Binding{
                        target: opacityValue
                        property: "value"
                        value: rootItem.model ? rootItem.model.fillColor.a * 100 : 0
                        delayed: true
                    }

                    ColorDialog {
                        visible: false
                        id:  fillColorDialog
                        title: "Please choose a color"
                        onAccepted: {
                            var currentColor = fillColorDialog.selectedColor
                            currentColor.a = opacityValue.value / 100
                            rootItem.model.fillColor = currentColor
                        }
                    }
                }
                ///////////////////////////// Stroke ///////////////////////////////////
                Rectangle{
                    color: "white"
                    Layout.fillWidth: true
                    height: 2
                    visible: strokeSec.visible
                }
                GroupBox{
                    id: strokeSec
                    visible:rootItem.model ? rootItem.model.strokeStatus : false

                    padding: 0
                    Layout.fillWidth: true
                    Layout.margins: 0
                    background: Rectangle{
                        color: "transparent"
                        border.color: "transparent"
                    }
                    title: "Stroke:"

                    GridLayout{
                        anchors.fill: parent
                        columnSpacing: 1
                        rowSpacing: 1
                        columns:2

                        Text{
                            Layout.preferredWidth: lblWidth
                            text: "Color: "
                            font.pointSize: 10
                            color: "white"
                        }
                        RowLayout{
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            Rectangle{
                                width: valHeight
                                height: valHeight
                                color: rootItem.model ? rootItem.model.strokeColor : "#202020"
                                border.width: 2
                                border.color: "#c9c9c9"
                                radius: 5
                                MouseArea{
                                    anchors.fill: parent
                                    cursorShape: Qt.PointingHandCursor
                                    onClicked: strokeColorDialog.visible = true

                                }
                            }

                            SpinBox {
                                id: strokeOpacityValue
                                to : 100
                                from : 0
                                Layout.fillWidth: true
                                Layout.minimumWidth: 80
                                height: valHeight
                                onValueChanged: {
                                    rootItem.model.strokeColor.a= value/100
                                }
                            }
                            Binding{
                                target: strokeOpacityValue
                                property: "value"
                                value: rootItem.model ? rootItem.model.strokeColor.a * 100 : 0
                                delayed: true
                            }

                            ColorDialog {
                                visible: false
                                id:  strokeColorDialog
                                title: "Please choose a color"
                                onAccepted: {
                                    var currentStrokeColor = strokeColorDialog.selectedColor
                                    currentStrokeColor.a = strokeOpacityValue.value / 100
                                    rootItem.model.strokeColor = currentStrokeColor
                                }
                            }
                        }

                        Text {
                            Layout.preferredWidth: lblWidth
                            text: "Width: "
                            font.pointSize: 10
                            color: "white"
                        }
                        SpinBox {
                            id: strokeWidthValue
                            Layout.fillWidth: true
                            Layout.minimumWidth: 100
                            height: valHeight
                            from: 0
                            onValueChanged: {
                                if (rootItem.model)
                                    rootItem.model.strokeWidth = value
                            }
                        }
                        Binding{
                            target: strokeWidthValue
                            property: "value"
                            value: rootItem.model ? rootItem.model.strokeWidth : 0
                            delayed: true
                        }
                    }
                }
                ////////////---------------------Location--------------------/////////////
                Rectangle{
                    color: "white"
                    Layout.fillWidth: true
                    height: 2
                    visible: locationSec.visible
                }
                GroupBox{
                    id: locationSec
                    visible:rootItem.model ? rootItem.model.locationStatus : false
                    Layout.fillWidth: true
                    padding: 0
                    title:"Location: "

                    background: Rectangle{
                        color: "transparent"
                        border.color: "transparent"
                    }

                    GridLayout{
                        anchors.fill: parent
                        columnSpacing: 1
                        rowSpacing: 1
                        columns:2
                        Text {
                            Layout.preferredWidth: lblWidth
                            text: "X: "
                            font.pointSize: 10
                            color: "white"
                        }
                        FloatSpinBox {
                            id: xLocationValue
                            Layout.fillWidth: true
                            Layout.minimumWidth: 100
                            height: valHeight
                            decimals: 4
                            from: -2000000000
                            to: 2000000000
                            onValueChanged: {
                                rootItem.model.location.x = realValue
                            }
                        }
                        Binding{
                            target: xLocationValue
                            property: "realValue"
                            value: rootItem.model ? rootItem.model.location.x : 0
                            delayed: true
                        }

                        Text {
                            Layout.preferredWidth: lblWidth
                            text: "Y: "
                            font.pointSize: 10
                            color: "white"
                        }
                        FloatSpinBox {
                            id: yLocationValue
                            Layout.fillWidth: true
                            Layout.minimumWidth: 100
                            height: valHeight
                            decimals: 4
                            from: -2000000000
                            to: 2000000000
                            onRealValueChanged: {
                                rootItem.model.location.y = realValue
                            }
                        }
                        Binding{
                            target: yLocationValue
                            property: "realValue"
                            value: rootItem.model ? rootItem.model.location.y : 0
                            delayed: true
                        }

                        Text {
                            Layout.preferredWidth: lblWidth
                            text: "Z: "
                            font.pointSize: 10
                            color: "white"
                        }
                        FloatSpinBox {

                            id: zLocationValue
                            Layout.fillWidth: true
                            Layout.minimumWidth: 100
                            height: valHeight
                            decimals: 4
                            from: -2000000000
                            to: 2000000000
                            onValueChanged: {
                                rootItem.model.location.z = realValue
                            }
                        }
                        Binding{
                            target: zLocationValue
                            property: "realValue"
                            value: rootItem.model ? rootItem.model.location.z : 0
                            delayed: true
                        }
                        CheckBox {
                            id: relative
                            text: "Relative"
                            font.pointSize: 10
                            checked: false

                            onCheckStateChanged: if(checked === true){
                                                     rootItem.model.locationRelative =true
                                                 }
                                                 else{
                                                     rootItem.model.locationRelative = false
                                                 }

                            indicator: Rectangle {
                                implicitWidth: 15
                                implicitHeight: 15
                                x: relative.leftPadding
                                y: parent.height / 2 - height / 2
                                radius: 3
                                border.color: relative.down ? "black" : "#313131"

                                Rectangle {
                                    width: 10
                                    height: 10
                                    x: 3
                                    y: 3
                                    radius: 2
                                    color: relative.down ? "black" : "dark green"
                                    visible: relative.checked
                                }
                            }
                            contentItem: Text {
                                text: relative.text
                                font: relative.font
                                opacity: enabled ? 1.0 : 0.3
                                color: relative.down ? "#black" : "white"
                                verticalAlignment: Text.AlignVCenter
                                leftPadding: relative.indicator.width + relative.spacing
                            }
                        }
                    }
                }
                //                }
                ///////////-----------------------center-------------------//////////////
                Rectangle{
                    color: "white"
                    Layout.fillWidth: true
                    height: 2
                    visible: centerSec.visible
                }
                GroupBox{
                    id: centerSec
                    visible:rootItem.model ? rootItem.model.centerStatus : false
                    padding: 0
                    Layout.fillWidth: true
                    title: "Center: "
                    background: Rectangle{
                        color: "transparent"
                        border.color: "transparent"
                    }
                    GridLayout{
                        anchors.fill: parent
                        columnSpacing: 1
                        rowSpacing: 1
                        columns:2

                        Text {
                            Layout.preferredWidth: lblWidth
                            text: "X: "
                            font.pointSize: 10
                            color: "white"
                        }
                        FloatSpinBox {
                            id: xCenterValue
                            Layout.fillWidth: true
                            Layout.minimumWidth: 100
                            height: valHeight
                            decimals: 2
                            from: -2000000000
                            to: 2000000000
                            onValueChanged: {
                                rootItem.model.center.x = realValue
                            }
                        }
                        Binding{
                            target: xCenterValue
                            property: "realValue"
                            value: rootItem.model ? rootItem.model.center.x : 0
                            delayed: true
                        }

                        Text {
                            Layout.preferredWidth: lblWidth
                            text: "Y: "
                            font.pointSize: 10
                            color: "white"
                        }
                        FloatSpinBox {
                            id: yCenterValue
                            Layout.fillWidth: true
                            Layout.minimumWidth: 100
                            height: valHeight
                            decimals: 2
                            from: -2000000000
                            to: 2000000000
                            onValueChanged: {
                                rootItem.model.center.y =realValue
                            }
                        }
                        Binding{
                            target: yCenterValue
                            property: "realValue"
                            value: rootItem.model ? rootItem.model.center.y : 0
                            delayed: true
                        }

                        Text {
                            Layout.preferredWidth: lblWidth
                            text: "Z: "
                            font.pointSize: 10
                            color: "white"
                        }
                        FloatSpinBox {
                            id: zCenterValue
                            Layout.fillWidth: true
                            Layout.minimumWidth: 100
                            height: valHeight
                            decimals: 2
                            from: -2000000000
                            to: 2000000000
                            onValueChanged: {
                                rootItem.model.center.z =realValue
                            }
                        }
                        Binding{
                            target: zCenterValue
                            property: "realValue"
                            value: rootItem.model ? rootItem.model.center.z : 0
                            delayed: true
                        }
                    }
                }
                ////////// -------------------- Arc ----------------------//////////////
                Rectangle{
                    color: "white"
                    Layout.fillWidth: true
                    height: 2
                    visible: arcSec.visible
                }
                GroupBox{
                    id: arcSec
                    visible:rootItem.model ? rootItem.model.arcStatus : false
                    padding: 0
                    Layout.fillWidth: true
                    title: "Arc: "
                    background: Rectangle{
                        color: "transparent"
                        border.color: "transparent"
                    }

                    GridLayout{
                        anchors.fill: parent
                        columnSpacing: 1
                        rowSpacing: 1
                        columns:2

                        Text{
                            Layout.preferredWidth: lblWidth
                            text: "Start: "
                            font.pointSize: 10
                            color: "white"
                        }
                        SpinBox {
                            id: startArcValue
                            Layout.fillWidth: true
                            Layout.minimumWidth: 100
                            height: valHeight
                            value: 0
                            from : 0
                            to : 360
                            onValueChanged: {
                                if (rootItem.model)
                                    rootItem.model.arc.x = value
                            }
                        }
                        Binding{
                            target: startArcValue
                            property: "value"
                            value: rootItem.model ? rootItem.model.arc.x : 0
                            delayed: true
                        }
                        Text {
                            Layout.preferredWidth: lblWidth
                            text: "End: "
                            font.pointSize: 10
                            color: "white"
                        }
                        SpinBox {
                            id: endArcValue
                            Layout.fillWidth: true
                            Layout.minimumWidth: 100
                            height: valHeight
                            value: 360
                            from: 0
                            to: 360
                            onValueChanged: {
                                if (rootItem.model){
                                    rootItem.model.arc.y = value
                                }
                            }
                        }
                        Binding{
                            target: endArcValue
                            property: "value"
                            value: rootItem.model ? rootItem.model.arc.y : 0
                            delayed: true
                        }
                    }
                }
                ////////------------------------- radius------------------//////////////
                Rectangle{
                    color: "white"
                    Layout.fillWidth: true
                    height: 2
                    visible: radiusSec.visible
                }
                RowLayout{
                    id:radiusSec
                    visible:rootItem.model ? rootItem.model.radiusStatus : false
                    Layout.fillWidth: true
                    Text {
                        Layout.preferredWidth: lblWidth
                        text: "Radius: "
                        font.pointSize: 10
                        color: "white"
                    }
                    FloatSpinBox {
                        id: radiusValue
                        Layout.fillWidth: true
                        Layout.minimumWidth: 100
                        height: valHeight
                        decimals: 2
                        from : 0
                        to: 2000000000
                        onValueChanged: {
                            rootItem.model.radius = realValue
                        }
                    }
                    Binding{
                        target: radiusValue
                        property: "realValue"
                        value: rootItem.model ? rootItem.model.radius :0
                        delayed: true
                    }
                }
                ////////------------------------- height------------------//////////////
                Rectangle{
                    color: "white"
                    Layout.fillWidth: true
                    height: 2
                    visible: heightSec.visible
                }
                RowLayout{
                    id:heightSec
                    visible:rootItem.model ? rootItem.model.heightStatus : false
                    Layout.fillWidth: true
                    Text {
                        Layout.preferredWidth: lblWidth
                        text: "Height: "
                        font.pointSize: 10
                        color: "white"
                    }

                    FloatSpinBox {
                        id:heightValue
                        Layout.fillWidth: true
                        Layout.minimumWidth: 100
                        height: valHeight
                        decimals: 2
                        from : 0
                        to: 2000000000
                        onValueChanged: {
                            rootItem.model.heighT= realValue
                        }
                    }
                    Binding{
                        target: heightValue
                        property: "realValue"
                        value: rootItem.model? rootItem.model.heighT: 0
                        delayed: true
                    }
                }

                ////////------------------------- lenght ------------------//////////////
                Rectangle{
                    color: "white"
                    Layout.fillWidth: true
                    height: 2
                    visible: lenghtSec.visible
                }
                RowLayout{
                    id:lenghtSec
                    Layout.fillWidth: true
                    visible: rootItem.model ? rootItem.model.lenghtStatus : false
                    Text {
                        Layout.preferredWidth: lblWidth
                        text:  "lenght: "
                        font.pointSize: 10
                        color: "white"
                    }

                    FloatSpinBox {
                        id: lengthValue
                        Layout.fillWidth: true
                        Layout.minimumWidth: 100
                        height: valHeight
                        decimals: 2
                        from : 0
                        to: 2000000000
                        onValueChanged: {
                            rootItem.model.lenghT = realValue
                        }

                    }
                    Binding{
                        target: lengthValue
                        property: "realValue"
                        value: rootItem.model ? rootItem.model.lenghT : 0
                        delayed: true
                    }
                }
                ////////------------------------- width ------------------//////////////
                Rectangle{
                    color: "white"
                    Layout.fillWidth: true
                    height: 2
                    visible: widthSec.visible
                }
                RowLayout{
                    id:widthSec
                    Layout.fillWidth: true
                    visible:rootItem.model ? rootItem.model.widthStatus : false
                    Text {
                        Layout.preferredWidth: lblWidth
                        text: "Width: "
                        font.pointSize: 10
                        color: "white"
                    }
                    FloatSpinBox {
                        id:widthValue
                        Layout.fillWidth: true
                        Layout.minimumWidth: 100
                        height: valHeight
                        decimals: 2
                        from : 0
                        to: 2000000000
                        onValueChanged: {
                            rootItem.model.widtH = realValue
                        }
                    }
                    Binding{
                        target: widthValue
                        property: "realValue"
                        value: rootItem.model ? rootItem.model.widtH : 0
                        delayed: true
                    }
                }
                // --------------------------------- clamp -----------------------////////////
                Rectangle{
                    color: "white"
                    Layout.fillWidth: true
                    height: 2
                    visible: clampSec.visible
                }
                RowLayout{
                    id:clampSec
                    Layout.fillWidth: true
                    visible:rootItem.model ? rootItem.model.clampStatus : false
                    Text {
                        Layout.preferredWidth: lblWidth
                        text: "Clamp: "
                        font.pointSize: 10
                        color: "white"
                    }

                    ComboBox {
                        id: clampCombo
                        model: ["None", "Terrain", "Relative","Absolute"]

                        Layout.fillWidth: true
                        Layout.minimumWidth: 100
                        height: valHeight

                        onCurrentIndexChanged: {
                            if(rootItem.model)
                                rootItem.model.clamp = currentIndex
                        }

                        delegate: ItemDelegate {
                            width: clampCombo.width
                            contentItem: Text {
                                text: clampCombo.textRole
                                      ? (Array.isArray(clampCombo.model) ? modelData[clampCombo.textRole] : model[clampCombo.textRole])
                                      : modelData
                                color: "#5f5f5f"
                                font: clampCombo.font
                                elide: Text.ElideRight
                                verticalAlignment: Text.AlignVCenter
                            }
                            highlighted: clampCombo.highlightedIndex === index
                        }

                        indicator: Canvas {
                            id: canvas
                            x: clampCombo.width - width - clampCombo.rightPadding
                            y: clampCombo.topPadding + (clampCombo.availableHeight - height) / 2
                            width: 12
                            height: 8
                            contextType: "2d"

                            Connections {
                                target: clampCombo
                                function onPressedChanged() { canvas.requestPaint(); }
                            }
                        }

                        contentItem: Text {
                            leftPadding: 5
                            rightPadding: clampCombo.indicator.width + clampCombo.spacing

                            text: clampCombo.displayText
                            font: clampCombo.font
                            color: clampCombo.pressed ? "#5f5f5f" : "#404040"
                            verticalAlignment: Text.AlignVCenter
                            elide: Text.ElideRight
                        }

                        background: Rectangle {
                            implicitWidth: 100
                            implicitHeight: 22
                            border.color: clampCombo.pressed ? "#5f5f5f" : "#404040"
                            border.width: clampCombo.visualFocus ? 2 : 1
                            radius: 5
                        }

                        popup: Popup {
                            y: clampCombo.height - 1
                            width: clampCombo.width
                            implicitHeight: contentItem.implicitHeight
                            padding: 1

                            contentItem: ListView {
                                clip: true
                                implicitHeight: contentHeight
                                model: clampCombo.popup.visible ? clampCombo.delegateModel : null
                                currentIndex: clampCombo.highlightedIndex

                                ScrollIndicator.vertical: ScrollIndicator { }
                            }

                            background: Rectangle {
                                border.color: "#404040"
                                radius: 5
                            }
                        }
                    }
                }
                ////////-------------------------/tesselation/------------------//////////////
                Rectangle{
                    color: "white"
                    Layout.fillWidth: true
                    height: 2
                    visible: tesselationSec.visible
                }
                RowLayout{
                    id:tesselationSec
                    visible:rootItem.model ? rootItem.model.tesselationStatus : false
                    Layout.fillWidth: true
                    Text {
                        Layout.preferredWidth: lblWidth
                        text: "Tesselation: "
                        font.pointSize: 10
                        color: "white"
                    }
                    SpinBox {
                        id: tesselationValue
                        from : 0
                        to: 2000000000
                        Layout.fillWidth: true
                        Layout.minimumWidth: 100
                        height: valHeight
                        onValueChanged: {
                            rootItem.model.tesselation = value
                        }
                    }
                    Binding{
                        target: tesselationValue
                        property: "value"
                        value: rootItem.model ? rootItem.model.tesselation: 0
                        delayed: true
                    }
                }
                ////------------------------ Show lenght -------------------- ///////////////
                Rectangle{
                    color: "white"
                    Layout.fillWidth: true
                    height: 2
                    visible: showLenghtSec.visible
                }
                RowLayout{
                    id:showLenghtSec
                    visible:rootItem.model ? rootItem.model.showLenStatus : false
                    Layout.fillWidth: true

                    Text {
                        Layout.preferredWidth: lblWidth
                        text: "Show Length: "
                        font.pointSize: 10
                        color: "white"
                    }
                    Switch {
                        id: showLenghtValue
                        padding: 0
                        width: 100
                        height: valHeight
                        checked: false
                        onToggled: function() {
                            rootItem.model.showLen = showLenghtValue.checked
                        }
                    }
                    Binding{
                        target: showLenghtValue
                        property: "checked"
                        value: rootItem.model ? rootItem.model.showLen: 0
                        delayed: true
                    }
                }
                ////------------------------ Bearing -------------------- ///////////////
                Rectangle{
                    color: "white"
                    Layout.fillWidth: true
                    height: 2
                    visible: bearingSec.visible
                }

                RowLayout{
                    id:bearingSec
                    visible:rootItem.model ? rootItem.model.bearingStatus : false
                    Layout.fillWidth: true
                    Text {
                        Layout.preferredWidth: lblWidth
                        text: "Bearing: "
                        font.pointSize: 10
                        color: "white"
                    }
                    Switch {
                        id: bearingValue
                        padding: 0
                        width: 100
                        height: valHeight
                        checked: false
                        onToggled: function() {
                            rootItem.model.showBearing = bearingValue.checked
                        }
                    }
                    Binding{
                        target: bearingValue
                        property: "checked"
                        value: rootItem.model ? rootItem.model.showBearing: 0
                        delayed: true
                    }
                }

                ////------------------------ Show Slope -------------------- ///////////////
                Rectangle{
                    color: "white"
                    Layout.fillWidth: true
                    height: 2
                    visible: slopeSec.visible
                }

                RowLayout{
                    id:slopeSec
                    visible:rootItem.model ? rootItem.model.showSlopStatus : false
                    Layout.fillWidth: true
                    Text {
                        Layout.preferredWidth: lblWidth
                        text: "Show Slope: "
                        font.pointSize: 10
                        color: "white"
                    }
                    Switch {
                        id: slopeValue
                        padding: 0
                        width: 100
                        height: valHeight
                        checked: false
                        onToggled: function() {
                            rootItem.model.showSlop  = slopeValue.checked
                        }
                    }
                    Binding{
                        target: slopeValue
                        property: "checked"
                        value: rootItem.model ? rootItem.model.showSlop: 0
                        delayed: true
                    }
                }
                ////------------------------ Points  -------------------- ///////////////
                Rectangle{
                    color: "white"
                    Layout.fillWidth: true
                    height: 2
                    visible: pointSec.visible
                }
                GroupBox{
                    id: pointSec
                    visible: rootItem.model ? rootItem.model.pointsStatus : false
                    padding: 0
                    Layout.fillWidth: true

                    background: Rectangle{
                        color: "transparent"
                        border.color: "transparent"
                    }

                    label: RowLayout{
                        anchors.left: parent.left
                        anchors.top: parent.top
                        Text {
                            Layout.preferredWidth: lblWidth
                            text: "Points: "
                            font.pointSize: 10
                            color: "gray"
                        }
                        Switch {
                            id:pointVisible
                            padding: 0
                            width: 100
                            height: valHeight
                            checked: false
                            onToggled: function() {
                                rootItem.model.pointsVisible  = pointVisible.checked
                            }
                        }
                        Binding{
                            target: pointVisible
                            property: "checked"
                            value: rootItem.model ? rootItem.model.pointsVisible: 0
                            delayed: true
                        }
                    }
                    GridLayout{
                        anchors.fill: parent
                        anchors.topMargin: 10
                        columnSpacing: 1
                        rowSpacing: 1
                        columns:2

                        Text{
                            Layout.preferredWidth: lblWidth
                            text: "Point Color: "
                            font.pointSize: 10
                            color: "white"
                        }
                        RowLayout{
                            Layout.fillWidth: true
                            enabled: pointVisible.checked
                            Rectangle{
                                width: valHeight
                                height: valHeight
                                color: rootItem.model ? rootItem.model.pointsColor : "#202020"
                                border.width: 2
                                border.color: "#c9c9c9"
                                radius: 5
                                MouseArea{
                                    anchors.fill: parent
                                    cursorShape: Qt.PointingHandCursor
                                    onClicked: pointColorDialog.visible = true
                                }
                            }

                            SpinBox {
                                id: pointOpacityValue
                                Layout.fillWidth: true
                                Layout.minimumWidth: 80
                                height: valHeight
                                from : 0
                                to : 100
                                onValueChanged: {
                                    rootItem.model.pointsColor.a = value/100
                                }
                            }
                            Binding{
                                target: pointOpacityValue
                                property: "value"
                                value: rootItem.model ? rootItem.model.pointsColor.a * 100 : 0
                                delayed: true
                            }
                            ColorDialog {
                                visible: false
                                id:  pointColorDialog
                                title: "Please choose a color"
                                onAccepted: {
                                    var currentColor = pointColorDialog.selectedColor
                                    currentColor.a = pointOpacityValue.value / 100
                                    rootItem.model.pointsColor = currentColor
                                }
                            }
                        }

                        Text {
                            Layout.preferredWidth: lblWidth
                            text: "Point Width: "
                            font.pointSize: 10
                            color: "white"
                        }
                        SpinBox {
                            id: pointWidthValue
                            enabled: pointVisible.checked
                            Layout.fillWidth: true
                            Layout.minimumWidth: 100
                            height: valHeight
                            from: 0
                            onValueChanged: {
                                rootItem.model.pointsWidth = value
                            }
                        }
                        Binding{
                            target: pointWidthValue
                            property: "value"
                            value: rootItem.model ? rootItem.model.pointsWidth: 0
                            delayed: true
                        }

                        Text {
                            Layout.preferredWidth: lblWidth
                            text: "Point Smooth: "
                            font.pointSize: 10
                            color: "white"
                        }
                        Switch {
                            id:pointSmoothValue
                            enabled: pointVisible.checked
                            padding: 0
                            width: 100
                            height: valHeight
                            checked: false
                            onToggled: function() {
                                rootItem.model.pointsSmooth  = pointSmoothValue.checked
                            }
                        }
                        Binding{
                            target: pointSmoothValue
                            property: "checked"
                            value: rootItem.model ? rootItem.model.pointsSmooth: 0
                            delayed: true
                        }
                    }
                }
            }
        }
    }
}


import QtQuick 2.13
import QtQuick.Window 2.12
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.12
import QtQuick.Dialogs
import Crystal 1.0
import "style"

Item {
    anchors.fill: parent
    id: rootItem
    implicitHeight: parent ? parent.height : 0

    property string _headerTitleSTR: "Properties"
    property CProperty model
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
        ColumnLayout{
            anchors.fill: parent
            Text {
                id: headerTitle
                Layout.fillWidth: true
                Layout.preferredHeight: 50
                text: rootItem._headerTitleSTR
                font.family: "SourceSansPro"
                font.pointSize: 14
                color: "white"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            ScrollView {
                id: frame
                clip: true
                contentWidth: frame.width

                Layout.fillWidth: true
                Layout.fillHeight: true
                ScrollBar.vertical.policy: ScrollBar.AlwaysOn


                /////////////////////// components Grid ////////////////////////////
                ColumnLayout{

                    id: props
                    width: parent.width - 18
                    x:5
                    spacing: 2
                    ////////////////////////////////// Name ////////////////////////////////////////////////

                    ColumnLayout{
                        width:parent.width
                        visible:true


                        RowLayout{
                            id:nM
                            Layout.fillWidth: true
                            Text {
                                id: names1
                                Layout.preferredWidth: 100
                                text: "name : "
                                font.pointSize: 10
                                color: "white"
                            }
                            TextField {
                                id: names
                                Layout.preferredWidth: 100
                                Layout.fillWidth: true
                                Layout.leftMargin: 25
                                text: rootItem.model ? rootItem.model.name : ""
                                font.pointSize: 10
                                color: "white"
                                onAccepted: {

                                    var currentName = names.displayText

                                    rootItem.model.name = currentName
                                }
                            }



                        }

                    }

                    ////////////////////////////////////fill Color Property//////////////////////////////////

                    //---------------------------------fill colore----------------------------//////////
                    ColumnLayout{
                        width:parent.width
                        visible: rootItem.model ? rootItem.model.fillColorStatus : false
                        Rectangle{
                            color: "white"
                            Layout.fillWidth: true
                            Layout.preferredHeight: 2
                        }
                        RowLayout{
                            id:fillcolor
                            Layout.fillWidth: true

                            Text {
                                id: fillColorCircle
                                Layout.preferredWidth: 100
                                text: "Fill Color: "
                                font.pointSize: 10
                                color: "white"
                            }

                            RowLayout{
                                Layout.fillWidth: true
                                Layout.leftMargin: -2
                                Rectangle{
                                    id: colorThumbnail
                                    Layout.preferredHeight: 20
                                    Layout.preferredWidth: 20
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
                                    to : 100
                                    from : 0
                                    Layout.fillWidth: true
                                    Layout.preferredHeight: 28
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
                        }
                    }
                    ///////////////////////////// Stroke ///////////////////////////////////
                    ColumnLayout{
                        width:parent.width
                        visible:rootItem.model ? rootItem.model.strokeStatus : false
                        Rectangle{
                            color: "white"
                            Layout.fillWidth: true
                            Layout.preferredHeight: 2
                        }
                        GroupBox{
                            id: stroke
                            padding: 0
                            Layout.fillWidth: true
                            Layout.preferredHeight: 100
                            Layout.margins: 0
                            background: Rectangle{
                                color: "transparent"
                                border.color: "transparent"
                            }

                            label: Rectangle {
                                y: 5
                                color: "transparent"

                                Text {
                                    text: " Stroke :"
                                }
                            }

                            GridLayout{
                                id:strockopacitycolor
                                anchors.fill: parent
                                columnSpacing: 1
                                rowSpacing: 1
                                columns:2

                                Text{
                                    Layout.topMargin: 30
                                    Layout.preferredWidth: 100
                                    Layout.preferredHeight: 35
                                    Layout.leftMargin: -3

                                    text: "Color: "
                                    font.pointSize: 10
                                    color: "white"
                                }
                                RowLayout{
                                    Layout.fillWidth: true
                                    Layout.leftMargin: 5
                                    Rectangle{
                                        id: colorThumbnail1
                                        Layout.preferredHeight: 20
                                        Layout.preferredWidth: 20
                                        color: rootItem.model ? rootItem.model.stroke : "#202020"
                                        border.width: 2
                                        border.color: "#c9c9c9"
                                        radius: 5
                                        MouseArea{
                                            anchors.fill: parent
                                            cursorShape: Qt.PointingHandCursor
                                            onClicked: lineColorDialog.visible = true

                                        }
                                    }

                                    FloatSpinBox {
                                        id: strkopacityValue
                                        to : 100
                                        from : 0
                                        Layout.fillWidth: true
                                        Layout.preferredHeight: 28


                                        onValueChanged: {
                                            rootItem.model.stroke.a= realValue/100
                                        }
                                    }
                                    Binding{
                                        target: strkopacityValue

                                        property: "realValue"
                                        value: rootItem.model ? rootItem.model.stroke.a * 100 : 0
                                        delayed: true
                                    }

                                    ColorDialog {
                                        visible: false
                                        id:  lineColorDialog
                                        title: "Please choose a color"
                                        onAccepted: {
                                            var currentstrkColor = lineColorDialog.selectedColor
                                            currentstrkColor.a = strkopacityValue.value / 100
                                            rootItem.model.stroke = currentstrkColor
                                        }
                                    }
                                }

                                Text {
                                    Layout.preferredWidth: 100
                                    Layout.preferredHeight: 35
                                    Layout.leftMargin: -3
                                    text: "Width: "
                                    font.pointSize: 10
                                    color: "white"
                                }
                                FloatSpinBox {
                                    id: strkWidth
                                    Layout.topMargin: -12
                                    Layout.fillWidth: true
                                    Layout.preferredHeight: 28
                                    Layout.leftMargin: 28
                                    from: 0
                                    to: 10000000
                                    onValueChanged: {
                                        if (rootItem.model)
                                            rootItem.model.strokeWidth = value
                                    }
                                }
                                Binding{
                                    target: strkWidth

                                    property: "realValue"
                                    value: rootItem.model ? rootItem.model.strokeWidth : 0
                                }
                            }
                        }
                    }
                    ////////////---------------------Location--------------------/////////////
                    ColumnLayout{
                        width:parent.width
                        visible:rootItem.model ? rootItem.model.locationStatus : false

                        Rectangle{
                            color: "white"
                            Layout.fillWidth: true
                            Layout.preferredHeight: 2
                        }
                        GroupBox{
                            id: location
                            padding: 0
                            Layout.fillWidth: true
                            Layout.preferredHeight: 120
                            Layout.margins: 10

                            background: Rectangle{
                                color: "transparent"
                                border.color: "transparent"
                            }

                            label: Rectangle {
                                y: -8
                                x:-10
                                color: "transparent"

                                Text {
                                    text: "Location :"
                                }
                            }

                            GridLayout{
                                id:locprops
                                anchors.fill: parent
                                columnSpacing: 1
                                rowSpacing: 1
                                columns:2
                                Text {
                                    Layout.preferredWidth: 104
                                    Layout.preferredHeight: 10
                                    Layout.leftMargin: -12
                                    text: "X: "
                                    font.pointSize: 10
                                    color: "white"
                                }
                                FloatSpinBox {
                                    id: mlocationX
                                    Layout.fillWidth: true
                                    Layout.preferredHeight: 28
                                    Layout.leftMargin: 25
                                    Layout.rightMargin: -9
                                    decimals: 6
                                    from: 0
                                    to: 100000000
                                    onValueChanged: {
                                        rootItem.model.location.x = realValue
                                    }
                                }
                                Binding{
                                    target: mlocationX

                                    property: "realValue"
                                    value: rootItem.model ? rootItem.model.location.x : 0
                                    delayed: true
                                }

                                Text {
                                    Layout.preferredWidth: 104
                                    Layout.preferredHeight: 10
                                    Layout.leftMargin: -12
                                    text: "Y: "
                                    font.pointSize: 10
                                    color: "white"
                                }
                                FloatSpinBox {
                                    id: mlocationY
                                    Layout.fillWidth: true
                                    Layout.preferredHeight: 28
                                    Layout.leftMargin: 25
                                    Layout.rightMargin: -9
                                    decimals: 6
                                    from: 0
                                    to: 100000000
                                    onRealValueChanged: {
                                        rootItem.model.location.y = realValue
                                    }
                                }
                                Binding{
                                    target: mlocationY
                                    property: "realValue"
                                    value: rootItem.model ? rootItem.model.location.y : 0
                                    delayed: true
                                }

                                Text {
                                    Layout.preferredWidth: 104
                                    Layout.preferredHeight: 10
                                    Layout.leftMargin: -12
                                    text: "Z: "
                                    font.pointSize: 10
                                    color: "white"
                                }
                                FloatSpinBox {

                                    id: mlocationZ
                                    Layout.fillWidth: true
                                    Layout.preferredHeight: 28
                                    Layout.leftMargin: 25
                                    Layout.rightMargin: -9
                                    decimals: 6
                                    from: 0
                                    to: 100000000
                                    onValueChanged: {
                                        rootItem.model.location.z = realValue
                                    }
                                }
                                Binding{
                                    target: mlocationZ

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
                    }
                    ///////////-----------------------center-------------------//////////////

                    ColumnLayout{
                        width:parent.width
                        visible:rootItem.model ? rootItem.model.centerStatus : false

                        Rectangle{
                            color: "white"
                            Layout.fillWidth: true
                            Layout.preferredHeight: 2
                        }
                        GroupBox{
                            id: cenTer
                            padding: 0
                            Layout.fillWidth: true
                            Layout.preferredHeight: 100
                            Layout.margins: 10

                            background: Rectangle{
                                color: "transparent"
                                border.color: "transparent"
                            }

                            label: Rectangle {
                                y: -8
                                x:-10
                                color: "transparent"
                                Text {
                                    text: "Center :"
                                }
                            }

                            GridLayout{
                                id:centerprops
                                anchors.fill: parent
                                columnSpacing: 1
                                rowSpacing: 1
                                columns:2

                                Text {
                                    Layout.preferredWidth: 104
                                    Layout.preferredHeight: 10
                                    Layout.leftMargin: -12
                                    text: "X: "
                                    font.pointSize: 10
                                    color: "white"
                                }
                                FloatSpinBox {

                                    id: mcenterX
                                    Layout.fillWidth: true
                                    Layout.preferredHeight: 28
                                    Layout.leftMargin: 25
                                    Layout.rightMargin: -9

                                    decimals: 7
                                    from: 0
                                    to: 10000000
                                    onValueChanged: {
                                        rootItem.model.center.x = realValue
                                    }
                                }
                                Binding{
                                    target: mcenterX

                                    property: "realValue"
                                    value: rootItem.model ? rootItem.model.center.x : 0
                                }

                                Text {
                                    Layout.preferredWidth: 104
                                    Layout.preferredHeight: 10
                                    Layout.leftMargin: -12
                                    text: "Y: "
                                    font.pointSize: 10
                                    color: "white"
                                }
                                FloatSpinBox {
                                    id: mcenterY
                                    Layout.fillWidth: true
                                    Layout.preferredHeight: 28
                                    Layout.leftMargin: 25
                                    Layout.rightMargin: -9
                                    decimals: 7
                                    from: 0
                                    to: 10000000
                                    onValueChanged: {
                                        rootItem.model.center.y =realValue
                                    }
                                }
                                Binding{
                                    target: mcenterY

                                    property: "realValue"
                                    value: rootItem.model ? rootItem.model.center.y : 0
                                }

                                Text {
                                    Layout.preferredWidth: 104
                                    Layout.preferredHeight: 10
                                    Layout.leftMargin: -12
                                    text: "Z: "
                                    font.pointSize: 10
                                    color: "white"
                                }
                                FloatSpinBox {
                                    id: mcenterZ
                                    Layout.fillWidth: true
                                    Layout.preferredHeight: 28
                                    Layout.leftMargin: 25
                                    Layout.rightMargin: -9
                                    decimals: 7
                                    from: 0
                                    to: 10000000
                                    onValueChanged: {
                                        rootItem.model.center.z =realValue
                                    }
                                }
                                Binding{
                                    target: mcenterZ

                                    property: "realValue"
                                    value: rootItem.model ? rootItem.model.center.z : 0
                                }
                            }
                        }
                    }
                    ////////// -------------------- Arc ----------------------//////////////

                    ColumnLayout{
                        width:parent.width
                        visible:rootItem.model ? rootItem.model.arcStatus : false

                        Rectangle{
                            color: "white"
                            Layout.fillWidth: true
                            Layout.preferredHeight: 2
                        }
                        GroupBox{
                            id: arc
                            padding: 0
                            Layout.fillWidth: true
                            Layout.preferredHeight: 85
                            Layout.margins: 0
                            background: Rectangle{
                                color: "transparent"
                                border.color: "transparent"
                            }

                            label: Rectangle {
                                y: 5
                                color: "transparent"
                                Text {
                                    text: " Arc :"
                                }
                            }

                            GridLayout{
                                id:arclayout
                                anchors.fill: parent
                                columnSpacing: 1
                                rowSpacing: 1
                                columns:2

                                Text{
                                    Layout.topMargin: 25
                                    Layout.preferredWidth: 100
                                    Layout.preferredHeight: 10

                                    text: "Start: "
                                    font.pointSize: 10
                                    color: "white"
                                }
                                RowLayout{
                                    Layout.fillWidth: true
                                    FloatSpinBox {
                                        id: startarc
                                        value: 0
                                        to : 360
                                        from : 0
                                        decimals: 0
                                        Layout.fillWidth: true
                                        Layout.topMargin: 30
                                        Layout.preferredHeight: 28
                                        Layout.leftMargin: 25
                                        Layout.rightMargin: 0

                                        onValueChanged: {
                                            if (rootItem.model)
                                                rootItem.model.arc.x =realValue
                                        }
                                    }
                                }
                                Text {
                                    Layout.preferredWidth: 104
                                    Layout.preferredHeight: 10
                                    text: "End: "
                                    font.pointSize: 10
                                    color: "white"
                                }
                                FloatSpinBox {
                                    id: endarc
                                    Layout.fillWidth: true
                                    Layout.preferredHeight: 28
                                    decimals: 0
                                    Layout.leftMargin: 25
                                    value: 360
                                    from: 0
                                    to: 360
                                    onValueChanged: {
                                        if (rootItem.model){
                                            rootItem.model.arc.y = realValue
                                            print(realValue)
                                        }
                                    }
                                }
                            }
                        }
                    }
                    ////////------------------------- radius------------------//////////////

                    ColumnLayout{
                        width:parent.width
                        visible:rootItem.model ? rootItem.model.radiusStatus : false

                        Rectangle{
                            color: "white"
                            Layout.fillWidth: true
                            Layout.preferredHeight: 2
                        }
                        RowLayout{
                            id:radius
                            Layout.fillWidth: true
                            Text {
                                id: radiustitle
                                Layout.preferredWidth: 100
                                text: "Radius: "
                                font.pointSize: 10
                                color: "white"
                            }

                            RowLayout{
                                Layout.fillWidth: true
                                FloatSpinBox {
                                    id: radiusspinbox
                                    //                                        value: rootItem.radius
                                    realValue: rootItem.model ? rootItem.model.radius : false
                                    from : 0
                                    to: 10000000
                                    Layout.fillWidth: true
                                    Layout.preferredHeight: 28
                                    Layout.leftMargin: 25
                                    onValueChanged: {
                                        rootItem.model.radius = value
                                    }
                                }
                            }
                        }
                    }

                    ////////------------------------- height------------------//////////////

                    ColumnLayout{
                        width:parent.width
                        visible:rootItem.model ? rootItem.model.heightStatus : false

                        Rectangle{
                            color: "white"
                            Layout.fillWidth: true
                            Layout.preferredHeight: 2
                        }
                        RowLayout{
                            id:rheight
                            Layout.fillWidth: true
                            Text {
                                id: heighttitle
                                Layout.preferredWidth: 100
                                text: "Height: "
                                font.pointSize: 10
                                color: "white"
                            }

                            RowLayout{
                                Layout.fillWidth: true


                                FloatSpinBox {
                                    id:mheight

                                    from : 0
                                    to: 1000000000
                                    Layout.fillWidth: true
                                    Layout.preferredHeight: 28
                                    Layout.leftMargin: 25
                                    onValueChanged: {
                                        rootItem.model.heighT= realValue
                                    }
                                }
                                Binding{
                                    target: mheight

                                    property: "realValue"
                                    value: rootItem.model? rootItem.model.heighT: 0
                                    delayed: true
                                }

                            }
                        }
                    }

                    ////////------------------------- lenght ------------------//////////////

                    ColumnLayout{
                        width:parent.width
                        visible:rootItem.model ? rootItem.model.lenghtStatus : false

                        Rectangle{
                            color: "white"
                            Layout.fillWidth: true
                            Layout.preferredHeight: 2
                        }
                        RowLayout{
                            id:lenght
                            Layout.fillWidth: true
                            Text {
                                id: rlentitle
                                Layout.preferredWidth: 100
                                text:  "lenght: "
                                font.pointSize: 10
                                color: "white"
                            }

                            RowLayout{
                                Layout.fillWidth: true


                                FloatSpinBox {
                                    id: lengthValue
                                    from : 0
                                    to: 10000000
                                    Layout.fillWidth: true
                                    Layout.preferredHeight: 28
                                    Layout.leftMargin: 25
                                    height: 28

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
                        }
                    }
                    ////////------------------------- width ------------------//////////////

                    ColumnLayout{
                        width:parent.width
                        visible:rootItem.model ? rootItem.model.widthStatus : false

                        Rectangle{
                            color: "white"
                            Layout.fillWidth: true
                            Layout.preferredHeight: 2
                        }
                        RowLayout{
                            id:widthl
                            Layout.fillWidth: true
                            Text {
                                id: widthtitle
                                Layout.preferredWidth: 100
                                //                                    Layout.preferredHeight: 28
                                text: "Width: "
                                font.pointSize: 10
                                color: "white"
                            }

                            RowLayout{
                                Layout.fillWidth: true


                                FloatSpinBox {
                                    id:widthValue


                                    from : 0
                                    to: 10000000
                                    Layout.fillWidth: true
                                    Layout.preferredHeight: 28
                                    Layout.leftMargin: 25
                                    height: 28
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
                        }
                    }


                    // --------------------------------- clamp -----------------------////////////
                    ColumnLayout{
                        width:parent.width
                        visible:rootItem.model ? rootItem.model.clampStatus : false

                        Rectangle{
                            color: "white"
                            Layout.fillWidth: true
                            Layout.preferredHeight: 2
                        }
                        RowLayout{
                            id:clamp
                            Layout.fillWidth: true
                            Text {
                                id: ctitle
                                Layout.preferredWidth: 100
                                text: "Clamp: "
                                font.pointSize: 10
                                color: "white"
                            }

                            RowLayout{
                                Layout.fillWidth: true


                                ComboBox {
                                    id: control
                                    model: ["None", "Terrain", "Relative","Absolute"]

                                    Layout.fillWidth: true
                                    Layout.leftMargin: 25

                                    onCurrentIndexChanged: {
                                        if(rootItem.model)
                                            rootItem.model.clamp = currentIndex
                                    }

                                    delegate: ItemDelegate {
                                        width: control.width
                                        contentItem: Text {
                                            text: control.textRole
                                                  ? (Array.isArray(control.model) ? modelData[control.textRole] : model[control.textRole])
                                                  : modelData
                                            color: "#5f5f5f"
                                            font: control.font
                                            elide: Text.ElideRight
                                            verticalAlignment: Text.AlignVCenter
                                        }
                                        highlighted: control.highlightedIndex === index
                                    }

                                    indicator: Canvas {
                                        id: canvas
                                        x: control.width - width - control.rightPadding
                                        y: control.topPadding + (control.availableHeight - height) / 2
                                        width: 12
                                        height: 8
                                        contextType: "2d"

                                        Connections {
                                            target: control
                                            function onPressedChanged() { canvas.requestPaint(); }
                                        }
                                    }

                                    contentItem: Text {
                                        leftPadding: 5
                                        rightPadding: control.indicator.width + control.spacing

                                        text: control.displayText
                                        font: control.font
                                        color: control.pressed ? "#5f5f5f" : "#404040"
                                        verticalAlignment: Text.AlignVCenter
                                        elide: Text.ElideRight
                                    }

                                    background: Rectangle {
                                        implicitWidth: 100
                                        implicitHeight: 22
                                        border.color: control.pressed ? "#5f5f5f" : "#404040"
                                        border.width: control.visualFocus ? 2 : 1
                                        radius: 5
                                    }

                                    popup: Popup {
                                        y: control.height - 1
                                        width: control.width
                                        implicitHeight: contentItem.implicitHeight
                                        padding: 1

                                        contentItem: ListView {
                                            clip: true
                                            implicitHeight: contentHeight
                                            model: control.popup.visible ? control.delegateModel : null
                                            currentIndex: control.highlightedIndex

                                            ScrollIndicator.vertical: ScrollIndicator { }
                                        }

                                        background: Rectangle {
                                            border.color: "#404040"
                                            radius: 5
                                        }
                                    }
                                }
                            }
                        }
                    }
                    ////////-------------------------/tesselation/------------------//////////////

                    ColumnLayout{
                        width:parent.width
                        visible:rootItem.model ? rootItem.model.tesselationStatus : false

                        Rectangle{
                            color: "white"
                            Layout.fillWidth: true
                            Layout.preferredHeight: 2
                        }
                        RowLayout{
                            id:tesselation
                            Layout.fillWidth: true
                            Text {
                                id: tesselationSphere
                                Layout.preferredWidth: 100
                                text: "Tesselation: "
                                font.pointSize: 10
                                color: "white"
                            }

                            RowLayout{
                                Layout.fillWidth: true
                                FloatSpinBox {
                                    to: 100000
                                    from : 0
                                    Layout.fillWidth: true
                                    Layout.preferredHeight: 28
                                    Layout.leftMargin: 25
                                    onValueChanged: {
                                        rootItem.model.tesselation = value
                                    }
                                }
                            }
                        }
                    }
                    ////------------------------ Show lenght -------------------- ///////////////
                    ColumnLayout{
                        width:parent.width
                        visible:rootItem.model ? rootItem.model.showLenStatus : false

                        Rectangle{
                            color: "white"
                            Layout.fillWidth: true
                            Layout.preferredHeight: 2
                        }
                        RowLayout{
                            id:tesselation1
                            Layout.fillWidth: true
                            Text {
                                Layout.preferredWidth: 100
                                text: "Show Length: "
                                font.pointSize: 10
                                color: "white"
                            }
                            Switch {
                                id: len
                                Layout.alignment:  Qt.AlignCenter
                                Layout.preferredWidth: 70
                                Layout.preferredHeight: 20
                                Layout.leftMargin: 25
                                checked: false
                                onToggled: function() {
                                    rootItem.model.showLen = len.checked
                                }
                            }
                        }
                    }
                    ////------------------------ Bearing -------------------- ///////////////

                    ColumnLayout{
                        width:parent.width
                        visible:rootItem.model ? rootItem.model.bearingStatus : false

                        Rectangle{
                            color: "white"
                            Layout.fillWidth: true
                            Layout.preferredHeight: 2
                        }

                        RowLayout{
                            id:beearing
                            Layout.fillWidth: true
                            Text {
                                Layout.preferredWidth: 100
                                text: "Bearing: "
                                font.pointSize: 10
                                color: "white"
                            }
                            Switch {
                                id: bae
                                Layout.alignment:  Qt.AlignCenter
                                Layout.preferredWidth: 70
                                Layout.preferredHeight: 20
                                Layout.leftMargin: 25
                                checked: false
                                onToggled: function() {
                                    rootItem.model.bearing = bae.checked
                                }
                            }
                        }
                    }

                    ////------------------------ Show Slope -------------------- ///////////////
                    ColumnLayout{
                        width:parent.width
                        visible:rootItem.model ? rootItem.model.showSlopStatus : false

                        Rectangle{
                            color: "white"
                            Layout.fillWidth: true
                            Layout.preferredHeight: 2
                        }

                        RowLayout{
                            id:slope
                            Layout.fillWidth: true
                            Text {
                                Layout.preferredWidth: 100
                                text: "Show Slope: "
                                font.pointSize: 10
                                color: "white"
                            }
                            Switch {
                                id: slp
                                Layout.alignment:  Qt.AlignCenter
                                Layout.preferredWidth: 70
                                Layout.preferredHeight: 20
                                Layout.leftMargin: 25
                                checked: false
                                onToggled: function() {
                                    rootItem.model.showSlop  = slp.checked
                                }
                            }
                        }
                    }


                    ////------------------------ Points  -------------------- ///////////////
                    Rectangle{
                        visible: rootItem.model ? rootItem.model.pointsStatus : false
                        color: "white"
                        Layout.fillWidth: true
                        Layout.preferredHeight: 2
                    }
                    GroupBox{
                        visible: rootItem.model ? rootItem.model.pointsStatus : false
                        id: pointColorSecR
                        padding: 0
                        Layout.fillWidth: true
                        Layout.preferredHeight: 150
                        Layout.margins: 0

                        background: Rectangle{
                            color: "transparent"
                            border.color: "transparent"
                        }

                        label: Rectangle {
                            y: 5
                            color: "transparent"
                            Text {
                                text: " Points :"
                            }
                        }

                        GridLayout{
                            id:pointgrid
                            anchors.fill: parent
                            columnSpacing: 1
                            rowSpacing: 1
                            columns:2
                            Text{
                                Layout.topMargin: 30
                                Layout.preferredWidth: 100
                                Layout.preferredHeight: 35
                                text: "Point Color: "
                                font.pointSize: 10
                                color: "white"
                            }
                            RowLayout{
                                Layout.fillWidth: true
                                Layout.leftMargin: 5
                                Rectangle{
                                    id: cptclr
                                    Layout.preferredHeight: 20
                                    Layout.preferredWidth: 20
                                    color: rootItem.model ? rootItem.model.points : "#202020"
                                    border.width: 2
                                    border.color: "#c9c9c9"
                                    radius: 5
                                    MouseArea{
                                        anchors.fill: parent
                                        cursorShape: Qt.PointingHandCursor
                                        onClicked: pointColorDialog.visible = true
                                    }
                                }

                                Rectangle{
                                    Layout.fillWidth: true
                                    Layout.preferredHeight: 20
                                    color: "#c9c9c9"
                                    radius: 5
                                    Text {
                                        text: rootItem.model ? rootItem.model.points : ""
                                        anchors.centerIn: parent
                                        font.pointSize: 10
                                    }
                                }
                                ColorDialog {
                                    visible: false
                                    id:  pointColorDialog
                                    title: "Please choose a color"
                                    onAccepted: {
                                        rootItem.model.points = pointColorDialog.selectedColor
                                    }
                                }
                            }

                            Text {
                                Layout.preferredWidth: 100
                                Layout.preferredHeight: 35
                                text: "Point Width: "
                                font.pointSize: 10
                                color: "white"
                            }
                            FloatSpinBox {
                                id: strkWidth1
                                Layout.topMargin: -12
                                Layout.fillWidth: true
                                Layout.preferredHeight: 28
                                Layout.leftMargin: 28
                                value: 360
                                from: 0
                                to: 360
                                onValueChanged: {
                                    rootItem.model.pointsWidth = value
                                }
                            }

                            Text {
                                Layout.preferredWidth: 100
                                text: "Point Smooth: "
                                font.pointSize: 10
                                color: "white"
                            }
                            Switch {
                                id:psmt
                                Layout.preferredWidth: 70
                                Layout.preferredHeight: 20
                                Layout.leftMargin: 25
                                checked: false
                                onToggled: function() {
                                    rootItem.model.pointsSmooth  = psmt.checked
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}


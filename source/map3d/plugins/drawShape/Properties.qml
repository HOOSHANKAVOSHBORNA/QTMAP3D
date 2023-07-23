import QtQuick 2.13
import QtQuick.Window 2.12
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.12
import QtQuick.Dialogs
import Crystal 1.0
import "style"

CProperty {
    anchors.fill: parent
    id: rootItem
    implicitHeight: parent ? parent.height : 0

    property string _headerTitleSTR: "Properties"

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

                    ////////////////////////////////////fill Color Property//////////////////////////////////

                    //---------------------------------fill colore----------------------------//////////
                    ColumnLayout{
                        width:parent.width
                        visible: rootItem.fillColorStatus
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
                                    color: rootItem.fillColor
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
                                        rootItem.fillColor.a = value/100
                                    }
                                }
                                Binding{
                                    target: opacityValue

                                    property: "value"
                                    value: rootItem.fillColor.a * 100
                                }

                                ColorDialog {
                                    visible: false
                                    id:  fillColorDialog
                                    title: "Please choose a color"
                                    onAccepted: {
                                        var currentColor = fillColorDialog.selectedColor
                                        currentColor.a = opacityValue.value / 100
                                        rootItem.fillColor = currentColor
                                    }
                                }
                            }
                        }
                    }
                    ///////////////////////////// Stroke ///////////////////////////////////
                    ColumnLayout{
                        width:parent.width
                        visible:rootItem.strokeStatus
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
                                        color: rootItem.stroke
                                        border.width: 2
                                        border.color: "#c9c9c9"
                                        radius: 5
                                        MouseArea{
                                            anchors.fill: parent
                                            cursorShape: Qt.PointingHandCursor
                                            onClicked: lineColorDialog.visible = true

                                        }
                                    }

                                    QSpinBox {
                                        id: strkopacityValue
                                        to : 100
                                        from : 0
                                        Layout.fillWidth: true
                                        Layout.preferredHeight: 28


                                        onValueChanged: {
                                            rootItem.stroke.a= realValue/100
                                        }
                                    }
                                    Binding{
                                        target: strkopacityValue

                                        property: "realValue"
                                        value: rootItem.stroke.a * 100
                                        delayed: true
                                    }

                                    ColorDialog {
                                        visible: false
                                        id:  lineColorDialog
                                        title: "Please choose a color"
                                        onAccepted: {
                                            var currentstrkColor = lineColorDialog.selectedColor
                                            currentstrkColor.a = strkopacityValue.value / 100
                                            rootItem.stroke = currentstrkColor
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
                                QSpinBox {
                                    id: strkWidth
                                    Layout.topMargin: -12
                                    Layout.fillWidth: true
                                    Layout.preferredHeight: 28
                                    Layout.leftMargin: 28
                                    value: 1
                                    from: 0
                                    onValueChanged: {
                                        rootItem.strokeWidth = value
                                    }
                                }
                                Binding{
                                    target: strkWidth

                                    property: "value"
                                    value: rootItem.strokeWidth
                                }
                            }
                        }
                    }
                    ////////////---------------------Location--------------------/////////////
                    ColumnLayout{
                        width:parent.width
                        visible:rootItem.locationStatus

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
                                QSpinBox {
                                    id: mlocationX
                                    Layout.fillWidth: true
                                    Layout.preferredHeight: 28
                                    Layout.leftMargin: 25
                                    Layout.rightMargin: -9
                                    decimals: 6
                                    onValueChanged: {
                                        rootItem.location.x = realValue
                                    }
                                }
                                Binding{
                                    target: mlocationX

                                    property: "realValue"
                                    value: rootItem.location.x
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
                                QSpinBox {
                                    id: mlocationY
                                    Layout.fillWidth: true
                                    Layout.preferredHeight: 28
                                    Layout.leftMargin: 25
                                    Layout.rightMargin: -9
                                    decimals: 6
                                    onRealValueChanged: {
                                        rootItem.location.y = realValue
                                    }
                                }
                                Binding{
                                    target: mlocationY
                                    property: "realValue"
                                    value: rootItem.location.y
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
                                QSpinBox {

                                    id: mlocationZ
                                    Layout.fillWidth: true
                                    Layout.preferredHeight: 28
                                    Layout.leftMargin: 25
                                    Layout.rightMargin: -9
                                    decimals: 4
                                    onValueChanged: {
                                        rootItem.location.z = realValue
                                    }
                                }
                                Binding{
                                    target: mlocationZ

                                    property: "realValue"
                                    value: rootItem.location.z
                                    delayed: true
                                }
                                CheckBox {
                                    id: relative
                                    text: "Relative"
                                    font.pointSize: 10
                                    checked: false

                                    onCheckStateChanged: if(checked === true){
                                                             rootItem.locationRelative =true
                                                         }
                                                         else{
                                                             rootItem.locationRelative = false
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
                        visible:rootItem.centerStatus

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
                                QSpinBox {

                                    id: mcenterX
                                    Layout.fillWidth: true
                                    Layout.preferredHeight: 28
                                    Layout.leftMargin: 25
                                    Layout.rightMargin: -9

                                    decimals: 7
                                    onValueChanged: {
                                        rootItem.center.x = realValue
                                    }
                                }
                                Binding{
                                    target: mcenterX

                                    property: "realValue"
                                    value: rootItem.center.x
                                }

                                Text {
                                    Layout.preferredWidth: 104
                                    Layout.preferredHeight: 10
                                    Layout.leftMargin: -12
                                    text: "Y: "
                                    font.pointSize: 10
                                    color: "white"
                                }
                                QSpinBox {
                                    id: mcenterY
                                    Layout.fillWidth: true
                                    Layout.preferredHeight: 28
                                    Layout.leftMargin: 25
                                    Layout.rightMargin: -9
                                    decimals: 7
                                    onValueChanged: {
                                        rootItem.center.y =realValue
                                    }
                                }
                                Binding{
                                    target: mcenterY

                                    property: "realValue"
                                    value: rootItem.center.y
                                }

                                Text {
                                    Layout.preferredWidth: 104
                                    Layout.preferredHeight: 10
                                    Layout.leftMargin: -12
                                    text: "Z: "
                                    font.pointSize: 10
                                    color: "white"
                                }
                                QSpinBox {
                                    id: mcenterZ
                                    Layout.fillWidth: true
                                    Layout.preferredHeight: 28
                                    Layout.leftMargin: 25
                                    Layout.rightMargin: -9
                                    decimals: 7
                                    onValueChanged: {
                                        rootItem.center.z =realValue
                                    }
                                }
                                Binding{
                                    target: mcenterZ

                                    property: "realValue"
                                    value: rootItem.center.z
                                }
                            }
                        }
                    }
                    ////////// -------------------- Arc ----------------------//////////////

                    ColumnLayout{
                        width:parent.width
                        visible:rootItem.arcStatus

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
                                    QSpinBox {
                                        id: startarc
                                        value: 0
                                        to : 360
                                        from : 0
                                        Layout.fillWidth: true
                                        Layout.topMargin: 30
                                        Layout.preferredHeight: 28
                                        Layout.leftMargin: 25
                                        Layout.rightMargin: 0

                                        onValueChanged: {

                                            rootItem.arc.x =value
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
                                QSpinBox {
                                    id: endarc
                                    Layout.fillWidth: true
                                    Layout.preferredHeight: 28
                                    Layout.leftMargin: 25
                                    value: 360
                                    from: 0
                                    to: 360
                                    onValueChanged: {
                                        rootItem.arc.y = value
                                    }
                                }
                            }
                        }
                    }
                    ////////------------------------- radius------------------//////////////

                    ColumnLayout{
                        width:parent.width
                        visible:rootItem.radiusStatus

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
                                QSpinBox {
                                    id: radiusspinbox
                                    //                                        value: rootItem.radius
                                    to : 10000000
                                    from : 0
                                    Layout.fillWidth: true
                                    Layout.preferredHeight: 28
                                    Layout.leftMargin: 25
                                    onValueChanged: {
                                        rootItem.radius = value
                                    }
                                }
                            }
                        }
                    }

                    ////////------------------------- height------------------//////////////

                    ColumnLayout{
                        width:parent.width
                        visible:rootItem.heightStatus

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


                                QSpinBox {
                                    id:mheight
                                    to : 1000000
                                    from : 0
                                    Layout.fillWidth: true
                                    Layout.preferredHeight: 28
                                    Layout.leftMargin: 25
                                    onValueChanged: {
                                        rootItem.heighT = realValue
                                    }
                                }
                                Binding{
                                    target: mheight

                                    property: "realValue"
                                    value: rootItem.heighT
                                    delayed: true
                                }
                            }
                        }
                    }
                    ////////------------------------- lenght ------------------//////////////

                    ColumnLayout{
                        width:parent.width
                        visible:rootItem.lenghtStatus

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


                                QSpinBox {
                                    id: lengthValue
                                    from : 0
                                    Layout.fillWidth: true
                                    Layout.preferredHeight: 28
                                    Layout.leftMargin: 25
                                    height: 28

                                    onValueChanged: {
                                        rootItem.lenghT = realValue
                                    }
                                }
                                Binding{
                                    target: lengthValue

                                    property: "realValue"
                                    value: rootItem.lenghT
                                    delayed: true

                                }
                            }
                        }
                    }
                    ////////------------------------- width ------------------//////////////

                    ColumnLayout{
                        width:parent.width
                        visible:rootItem.widthStatus

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


                                QSpinBox {
                                    id:widthValue

                                    realValue: rootItem.widtH
                                    from : 0
                                    Layout.fillWidth: true
                                    Layout.preferredHeight: 28
                                    Layout.leftMargin: 25

                                }
                                Binding{
                                    target: rootItem

                                    property: "widtH"
                                    value: widthValue.realValue
                                    delayed: true
                                }
                            }
                        }
                    }


                    // --------------------------------- clamp -----------------------////////////
                    ColumnLayout{
                        width:parent.width
                        visible:rootItem.clampStatus

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
                                        rootItem.clamp = currentIndex
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
                        visible:rootItem.tesselationStatus

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
                                QSpinBox {
                                    to: 100000
                                    from : 0
                                    Layout.fillWidth: true
                                    Layout.preferredHeight: 28
                                    Layout.leftMargin: 25
                                    onValueChanged: {
                                        rootItem.tesselation = value
                                    }
                                }
                            }
                        }
                    }
                    ////------------------------ Show lenght -------------------- ///////////////
                    ColumnLayout{
                        width:parent.width
                        visible:rootItem.showLenStatus

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
                                    rootItem.showLen = len.checked
                                }
                            }
                        }
                    }
                    ////------------------------ Bearing -------------------- ///////////////

                    ColumnLayout{
                        width:parent.width
                        visible:rootItem.bearingStatus

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
                                    rootItem.bearing = bae.checked
                                }
                            }
                        }
                    }

                    ////------------------------ Show Slope -------------------- ///////////////
                    ColumnLayout{
                        width:parent.width
                        visible:rootItem.showSlopStatus

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
                                    rootItem.showSlop  = slp.checked
                                }
                            }
                        }
                    }


                    ////------------------------ Points  -------------------- ///////////////
                    Rectangle{
                        visible: rootItem.pointsStatus
                        color: "white"
                        Layout.fillWidth: true
                        Layout.preferredHeight: 2
                    }
                    GroupBox{
                        visible: rootItem.pointsStatus
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
                                    color: rootItem.points
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
                                        text: rootItem.points
                                        anchors.centerIn: parent
                                        font.pointSize: 10
                                    }
                                }
                                ColorDialog {
                                    visible: false
                                    id:  pointColorDialog
                                    title: "Please choose a color"
                                    onAccepted: {
                                        rootItem.points = pointColorDialog.selectedColor
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
                            QSpinBox {
                                id: strkWidth1
                                Layout.topMargin: -12
                                Layout.fillWidth: true
                                Layout.preferredHeight: 28
                                Layout.leftMargin: 28
                                value: 360
                                from: 0
                                to: 360
                                onValueChanged: {
                                    rootItem.pointsWidth = value
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
                                    rootItem.pointsSmooth  = psmt.checked
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}


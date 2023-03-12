import QtQuick 2.13
import QtQuick.Window 2.12
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.2
import Crystal 1.0

Item {
    id: rootItem
    implicitHeight: parent.height



    property CircleProperties circleProperties
    property bool rulerCondition : lineProperties.ruler
    property string headerTitleSTR: "circle Properties"
    property string fillColor: "#91001d"
    property string lineColor: "#001191"



    Item {
        id: dialog
        width: 250
        height: rootItem.height


        //////////////////////////Main Content////////////////////////
        Rectangle {
            id: item
            visible: true
            width: 240
            height: rootItem.height
            anchors.fill: parent
            color: "#404040"
            opacity: 0.9
            anchors.margins: 6
            radius: 10

            Rectangle{
                id:header
                width: parent.width -2
                anchors.horizontalCenter: parent.horizontalCenter
                y: 2
                height: 30
                radius: 0
                color: "#404040"

                Text {
                    id: headerTitle
                    text: headerTitleSTR
                    anchors.centerIn: parent
                    font.family: "SourceSansPro"
                    font.pointSize: 14
                    color: "white"
                }

            }

            ScrollView {
                id: frame
                clip: true
                anchors.top: header.bottom
                padding: 5
                width: parent.width
                height: parent.height - header.height
                ScrollBar.vertical.policy: ScrollBar.AlwaysOn
                Flickable {
                    contentHeight: 350
                    width: 240

                    Rectangle {
                        id : innerContainer
                        color: "transparent"
                        anchors.fill: parent

                        /////////////////////// components Grid ////////////////////////////
                        GridLayout{
                            id: props
                            width: 235
                            y: innerContainer.y +3
                            anchors.horizontalCenter: parent.horizontalCenter
                            columnSpacing: 0
                            rowSpacing: 1
                            columns:2
                            rows: 6
                            layoutDirection: Qt.RightToLeft

                            ////////////////////////////////////fill Color Property//////////////////////////////////
                            Rectangle{
                                Layout.fillWidth: true
                                color: "#404040"
                                height: 35


                                Rectangle{
                                    height: 20
                                    width: 20
                                    color: fillColor
                                    border.width: 2
                                    border.color: "#c9c9c9"
                                    x: 5
                                    radius: 5
                                    anchors.verticalCenter: parent.verticalCenter

                                    MouseArea{
                                        anchors.fill: parent
                                        cursorShape: Qt.PointingHandCursor
                                        onClicked: fillColorDialog.visible = true

                                    }
                                }
                                Rectangle{
                                    height: 20
                                    width: 75
                                    color: "#c9c9c9"
                                    x: 30
                                    anchors.verticalCenter: parent.verticalCenter
                                    radius: 5

                                    Text {
                                        id: fillolorField
                                        text: fillColor
                                        anchors.centerIn: parent
                                        font.pointSize: 10

                                    }
                                }
                                ColorDialog {
                                    visible: false
                                    id:  fillColorDialog
                                    title: "Please choose a color"
                                    onAccepted: {
                                        fillColor = fillColorDialog.color
                                        circleProperties.fillcolor= fillColor.toString()
                                    }

                                }
                            }
                            Rectangle{
                                Layout.fillWidth: true
                                color: "#404040"
                                height: 35


                                Text {
                                    id: fillColorCircle
                                    text: qsTr("Fill Color :")
                                    font.pointSize: 10
                                    color: "white"
                                    anchors.verticalCenter:  parent.verticalCenter
                                    x:7
                                }
                            }

                            ///////////////////////////////////line Color Property//////////////////////////////////
                            Rectangle{
                                id: lineColorSecR
                                Layout.fillWidth: true
                                color: "#404040"
                                height: 35
                                //                                border.color: "#5f5f5f"
                                //                                border.width: 1


                                Rectangle{
                                    height: 20
                                    width: 20
                                    color: lineColor
                                    //                                    border.width: 2
                                    //                                    border.color: "#c9c9c9"
                                    x: 5
                                    radius: 5
                                    anchors.verticalCenter: parent.verticalCenter

                                    MouseArea{
                                        anchors.fill: parent
                                        cursorShape: Qt.PointingHandCursor
                                        onClicked: lineColorDialog.visible = true

                                    }
                                }
                                Rectangle{
                                    height: 20
                                    width: 75
                                    color: "#c9c9c9"
                                    x: 30
                                    anchors.verticalCenter: parent.verticalCenter
                                    radius: 5

                                    Text {
                                        id: lineColorField
                                        text: lineColor
                                        anchors.centerIn: parent
                                        font.pointSize: 10

                                    }
                                }
                                ColorDialog {
                                    visible: false
                                    id: lineColorDialog
                                    title: "Please choose a color"
                                    onAccepted: {
                                        //                                    lineColor = lineColorDialog.color
                                        lineColor = lineColorDialog.color
                                        circleProperties.linecolor = lineColor
                                    }

                                }
                            }

                            Rectangle{
                                id: lineColorSecL
                                Layout.fillWidth: true
                                color: "#404040"
                                height: 35
                                //                                border.color: "#5f5f5f"
                                //                                border.width: 1

                                Text {
                                    id: lineColors
                                    text: qsTr("Line Color:")
                                    font.pointSize: 10
                                    color: "white"
                                    anchors.verticalCenter:  parent.verticalCenter
                                    x:7
                                }
                            }

                            ///////////////////////////////////Location Property///////////////////////////////
                            Rectangle{
                                id:locationContainer
                                Layout.fillWidth: true
                                color: "#404040"
                                height: 80
                                border.color: "#5f5f5f"
                                border.width: 1

                                Column{
                                    Row{
                                        spacing: 13
                                        leftPadding: 5
                                        topPadding: 5
                                        anchors.horizontalCenter: parent.horizontalCenter

                                        Text {
                                            color: "#e5e5e5"
                                            text: qsTr("X:")
                                            font.pointSize: 10
                                        }
                                        Rectangle{
                                            height: 20
                                            width: 75
                                            color: "#c9c9c9"
                                            clip:  true
                                            radius: 5
                                            Layout.fillWidth: true
                                            TextInput {
                                                id: mlocationX
                                                anchors.fill: parent
                                                //                                                text: circleProperties.location.x
                                                font.pointSize: 10
                                                selectByMouse: true
                                                selectionColor: "dark green"
                                                validator: DoubleValidator {
                                                    decimals: 8;
                                                    notation: DoubleValidator.StandardNotation
                                                    locale: "insert x"
                                                }
                                                onTextChanged: {
                                                    circleProperties.location.x = text
                                                }
                                            }
                                        }
                                    }
                                    Row{
                                        spacing: 13
                                        leftPadding: 5
                                        topPadding: 5
                                        anchors.horizontalCenter: parent.horizontalCenter

                                        Text {
                                            color: "#e5e5e5"
                                            text: qsTr("Y:")
                                            font.pointSize: 10
                                        }
                                        Rectangle{
                                            height: 20
                                            width: 75
                                            color: "#c9c9c9"
                                            clip:  true
                                            radius: 5

                                            TextInput {
                                                id: mlocationY
                                                anchors.fill: parent
                                                font.pointSize: 10
                                                //                                                text: circleProperties.location.y
                                                selectByMouse: true
                                                selectionColor: "dark green"
                                                validator: DoubleValidator {
                                                    decimals: 8;
                                                    notation: DoubleValidator.StandardNotation
                                                    locale: "insert y"
                                                }
                                                onTextChanged: {
                                                    circleProperties.location.y = text
                                                }
                                            }
                                        }
                                    }
                                    Row{
                                        spacing: 13
                                        leftPadding: 5
                                        topPadding: 5
                                        anchors.horizontalCenter: parent.horizontalCenter

                                        Text {
                                            color: "#e5e5e5"
                                            text: qsTr("Z:")
                                            font.pointSize: 10
                                        }
                                        Rectangle{
                                            height: 20
                                            width: 75
                                            color: "#c9c9c9"
                                            clip:  true
                                            radius: 5

                                            TextInput {
                                                id: mlocationZ
                                                anchors.fill: parent
                                                font.pointSize: 10
                                                //                                                text: circleProperties.location.z
                                                selectByMouse: true
                                                selectionColor: "dark green"
                                                validator: DoubleValidator {
                                                    decimals: 8;
                                                    notation: DoubleValidator.StandardNotation
                                                    locale: "insert z"
                                                }
                                                onTextChanged: {
                                                    circleProperties.location.z = text
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                            Rectangle{
                                id: locationTitle
                                Layout.fillWidth: true
                                color: "#404040"
                                height: 80
                                border.color: "#5f5f5f"
                                border.width: 1

                                Text {
                                    id: locationCircle
                                    text: qsTr("Location :")
                                    font.pointSize: 10
                                    color: "white"
                                    y: 10
                                    x:7
                                }
                                CheckBox {
                                    id: relative
                                    text: qsTr("Relative")
                                    checked: flase
                                    anchors.bottom: locationTitle.bottom
                                    onCheckStateChanged: if(checked === true){
                                                             circleProperties.relative = true
                                                         }
                                                         else{
                                                             circleProperties.relative = false
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

                            ///////////////////////////////////ARC Property///////////////////////////////
                            Rectangle{
                                id:arcContainer
                                Layout.fillWidth: true
                                color: "#404040"
                                height: 55
                                border.color: "#5f5f5f"
                                border.width: 1

                                Column{
                                    Row{
                                        spacing: 3
                                        leftPadding: 2
                                        topPadding: 5
                                        x:1

                                        Text {
                                            color: "#e5e5e5"
                                            text: qsTr("Start:")
                                            font.pointSize: 10
                                        }
                                        Rectangle{
                                            height: 20
                                            width: 67
                                            color: "#c9c9c9"
                                            clip:  true
                                            radius: 5

                                            TextInput {
                                                id: startArc
                                                anchors.fill: parent
                                                font.pointSize: 10
//                                                text: "0"
                                                selectByMouse: true
                                                selectionColor: "dark green"
                                                validator: DoubleValidator {
                                                    decimals: 8;
                                                    notation: DoubleValidator.StandardNotation
                                                    locale: "insert start"
                                                }
                                                onTextChanged: {
                                                    circleProperties.arcstart = text
                                                }
                                            }
                                        }
                                    }
                                    Row{
                                        spacing: 9
                                        leftPadding: 2
                                        topPadding: 5
                                        //                                        anchors.horizontalCenter: parent.horizontalCenter
                                        x: 1

                                        Text {
                                            color: "#e5e5e5"
                                            text: qsTr("End:")
                                            font.pointSize: 10
                                        }
                                        Rectangle{
                                            height: 20
                                            width: 67
                                            color: "#c9c9c9"
                                            clip:  true
                                            radius: 5

                                            TextInput {
                                                id: endArc
                                                anchors.fill: parent
                                                font.pointSize: 10
//                                                text: "360"
                                                selectByMouse: true
                                                selectionColor: "dark green"
                                                validator: DoubleValidator {
                                                    decimals: 8;
                                                    notation: DoubleValidator.StandardNotation
                                                    locale: "insert end"
                                                }
                                                onTextChanged: {
                                                    circleProperties.arcend = text
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                            Rectangle{
                                id: arcTitle
                                Layout.fillWidth: true
                                color: "#404040"
                                height: 55
                                border.color: "#5f5f5f"
                                border.width: 1

                                Text {
                                    id: arcCircle
                                    text: qsTr("Arc :")
                                    font.pointSize: 10
                                    color: "white"
                                    y: 10
                                    x:7
                                }
                            }


                            ///////////////////////////////////radius/////////////////////////////////////

                            Rectangle{
                                id: radiusContainer
                                Layout.fillWidth: true
                                color: "#404040"
                                height: 30
                                //                                border.color: "#5f5f5f"
                                //                                border.width: 1

                                SpinBox {
                                    id: radiusValue
                                    stepSize: 500
                                    value: 200000
                                    to : 10000000
                                    from : 0
                                    validator: DoubleValidator {
                                        bottom: 0
                                        top:  100
                                    }
                                    editable: true
                                    anchors.centerIn: parent
                                    height: 20
                                    //                                    onDataChanged: lineProperties.width = widthValue.valueFromText

                                    contentItem: TextInput {
                                        id:valueInput
                                        z: 2
                                        text: radiusValue.value
                                        font: radiusValue.font
                                        color: "#404040"
                                        horizontalAlignment: Qt.AlignHCenter
                                        verticalAlignment: Qt.AlignVCenter
                                        readOnly: !radiusValue.editable
                                        validator: radiusValue.validator
                                        inputMethodHints: Qt.ImhFormattedNumbersOnly
                                        topPadding: 13
//                                        leftPadding: 30
                                        rightPadding: -25
                                        selectByMouse: true
                                        selectionColor: "dark green"
                                        onTextChanged: {
                                            if(circleProperties){
                                                radiusValue.value = valueInput.text
                                                circleProperties.radius = radiusValue.value
                                            }
                                        }
                                    }
                                    up.indicator: Rectangle {
                                        x: radiusValue.mirrored ? 0 : parent.width - width
                                        height: parent.height
                                        implicitWidth: 20
                                        implicitHeight: 20
                                        color: radiusValue.up.pressed ? "#5f5f5f" : "#404040"
                                        border.color: enabled ? "#404040" : "#5f5f5f"

                                        Text {
                                            text: "+"
                                            font.pixelSize: radiusValue.font.pixelSize * 2
                                            color: "white"
                                            anchors.fill: parent
                                            fontSizeMode: Text.Fit
                                            horizontalAlignment: Text.AlignHCenter
                                            verticalAlignment: Text.AlignVCenter
                                        }
                                    }
                                    down.indicator: Rectangle {
                                        x: radiusValue.mirrored ? parent.width - width : 0
                                        height: parent.height
                                        implicitWidth: 20
                                        implicitHeight: 20
                                        color: radiusValue.down.pressed ? "#5f5f5f" : "#404040"
                                        border.color: enabled ? "#404040" : "#5f5f5f"

                                        Text {
                                            text: "-"
                                            font.pixelSize: radiusValue.font.pixelSize * 2
                                            color: "white"
                                            anchors.fill: parent
                                            fontSizeMode: Text.Fit
                                            horizontalAlignment: Text.AlignHCenter
                                            verticalAlignment: Text.AlignVCenter
                                        }
                                    }
                                    background: Rectangle {
                                        id:radiusHolder
                                        implicitWidth: radiusContainer.width - 10
                                        color: "#c9c9c9"
                                        border.color: "#bdbebf"
                                    }
                                }
                            }
                            Rectangle{
                                Layout.fillWidth: true
                                color: "#404040"
                                height: 30
                                //                                border.color: "#5f5f5f"
                                //                                border.width: 1

                                Text {
                                    id: radiuscircle
                                    text: qsTr("Radius:")
                                    font.pointSize: 10
                                    color: "white"
                                    anchors.verticalCenter:  parent.verticalCenter
                                    x:7
                                }
                            }



                            ///////////////////////////////////height/////////////////////////////////////
                            Rectangle{
                                id: transContainer
                                Layout.fillWidth: true
                                color: "#404040"
                                height: 30
                                //                                border.color: "#5f5f5f"
                                //                                border.width: 1

                                SpinBox {
                                    id: transValue
                                    stepSize: 100
                                    value: 0
                                    to : 1000000
                                    from : 0
                                    validator: DoubleValidator {
                                        bottom: 0
                                        top:  100
                                    }
                                    editable: true
                                    anchors.centerIn: parent
                                    height: 20

                                    contentItem: TextInput {
                                        id: heightInput
                                        z: 2
                                        //                                        text: transValue.textFromValue(transValue.value, transValue.locale)
                                        text: transValue.value
                                        font: transValue.font
                                        color: "#404040"
                                        horizontalAlignment: Qt.AlignHCenter
                                        verticalAlignment: Qt.AlignVCenter +10
                                        readOnly: !transValue.editable
                                        validator: transValue.validator
                                        inputMethodHints: Qt.ImhFormattedNumbersOnly
                                        topPadding: 13
                                        selectByMouse: true
                                        selectionColor: "dark green"
                                        onTextChanged: {
                                            if(transValue && transValue.value){
                                                transValue.value = heightInput.text
                                                circleProperties.circleheight = transValue.value
                                            }
                                        }
                                    }
                                    up.indicator: Rectangle {
                                        x: transValue.mirrored ? 0 : parent.width - width
                                        height: parent.height
                                        implicitWidth: 20
                                        implicitHeight: 20
                                        color: transValue.up.pressed ? "#5f5f5f" : "#404040"
                                        border.color: enabled ? "#404040" : "#5f5f5f"

                                        Text {
                                            text: "+"
                                            font.pixelSize: transValue.font.pixelSize * 2
                                            color: "white"
                                            anchors.fill: parent
                                            fontSizeMode: Text.Fit
                                            horizontalAlignment: Text.AlignHCenter
                                            verticalAlignment: Text.AlignVCenter
                                        }
                                    }
                                    down.indicator: Rectangle {
                                        x: transValue.mirrored ? parent.width - width : 0
                                        height: parent.height
                                        implicitWidth: 20
                                        implicitHeight: 20
                                        color: transValue.down.pressed ? "#5f5f5f" : "#404040"
                                        border.color: enabled ? "#404040" : "#5f5f5f"

                                        Text {
                                            text: "-"
                                            font.pixelSize: transValue.font.pixelSize * 2
                                            color: "white"
                                            anchors.fill: parent
                                            fontSizeMode: Text.Fit
                                            horizontalAlignment: Text.AlignHCenter
                                            verticalAlignment: Text.AlignVCenter
                                        }
                                    }
                                    background: Rectangle {
                                        implicitWidth: transContainer.width -10
                                        color: "#c9c9c9"
                                        border.color: "#bdbebf"
                                    }
                                }
                            }
                            Rectangle{
                                id:heightContainer
                                Layout.fillWidth: true
                                color: "#404040"
                                height: 30
                                //                                border.color: "#5f5f5f"
                                //                                border.width: 1

                                Text {
                                    id: transSphere
                                    text: qsTr("Height:")
                                    font.pointSize: 10
                                    color: "white"
                                    anchors.verticalCenter:  parent.verticalCenter
                                    x:7
                                }
                            }

                            ///////////////////////////////////opacity/////////////////////////////////////

                            Rectangle{
                                id: opacityContainer
                                Layout.fillWidth: true
                                color: "#404040"
                                height: 30
                                //                                border.color: "#5f5f5f"
                                //                                border.width: 1

                                SpinBox {
                                    id: opacityValue
                                    stepSize: 5
                                    value: 50
                                    to : 100
                                    from : 0
                                    validator: DoubleValidator {
                                        bottom: 0
                                        top:  100
                                    }
                                    editable: true
                                    anchors.centerIn: parent
                                    height: 20


                                    contentItem: TextInput {
                                        id:opacityValueInput
                                        z: 2
                                        //                                        text: pointwidthValue.textFromValue(pointwidthValue.value, pointwidthValue.locale)
                                        text: opacityValue.value
                                        font: opacityValue.font
                                        color: "#404040"
                                        horizontalAlignment: Qt.AlignHCenter
                                        verticalAlignment: Qt.AlignVCenter +10
                                        readOnly: !opacityValue.editable
                                        validator: opacityValue.validator
                                        inputMethodHints: Qt.ImhFormattedNumbersOnly
                                        topPadding: 13
                                        selectByMouse: true
                                        selectionColor: "dark green"
                                        onTextChanged: {
                                            if(circleProperties){
                                                opacityValue.value = opacityValueInput.text
                                                circleProperties.transparency = opacityValue.value
                                            }
                                        }
                                    }
                                    up.indicator: Rectangle {
                                        x: opacityValue.mirrored ? 0 : parent.width - width
                                        height: parent.height
                                        implicitWidth: 20
                                        implicitHeight: 20
                                        color: opacityValue.up.pressed ? "#5f5f5f" : "#404040"
                                        border.color: enabled ? "#404040" : "#5f5f5f"

                                        Text {
                                            text: "+"
                                            font.pixelSize: opacityValue.font.pixelSize * 2
                                            color: "white"
                                            anchors.fill: parent
                                            fontSizeMode: Text.Fit
                                            horizontalAlignment: Text.AlignHCenter
                                            verticalAlignment: Text.AlignVCenter
                                        }
                                    }
                                    down.indicator: Rectangle {
                                        x: opacityValue.mirrored ? parent.width - width : 0
                                        height: parent.height
                                        implicitWidth: 20
                                        implicitHeight: 20
                                        color: opacityValue.down.pressed ? "#5f5f5f" : "#404040"
                                        border.color: enabled ? "#404040" : "#5f5f5f"

                                        Text {
                                            text: "-"
                                            font.pixelSize: opacityValue.font.pixelSize * 2
                                            color: "white"
                                            anchors.fill: parent
                                            fontSizeMode: Text.Fit
                                            horizontalAlignment: Text.AlignHCenter
                                            verticalAlignment: Text.AlignVCenter
                                        }
                                    }
                                    background: Rectangle {
                                        implicitWidth: opacityContainer.width -10
                                        color: "#c9c9c9"
                                        border.color: "#bdbebf"
                                    }
                                }
                            }
                            Rectangle{
                                id: opacityTitle
                                Layout.fillWidth: true
                                color: "#404040"
                                height: 30
                                //                                border.color: "#5f5f5f"
                                //                                border.width: 1

                                Text {
                                    id: opacityCircle
                                    text: qsTr("Opacity:")
                                    font.pointSize: 10
                                    color: "white"
                                    anchors.verticalCenter:  parent.verticalCenter
                                    x:7
                                }
                            }


                            /////////////////////////////////// Clamp ///////////////////////////////////////////
                            Rectangle{
                                id: clampContainer
                                Layout.fillWidth: true
                                color: "#404040"
                                height: 30


                                ComboBox {
                                    id: control
                                    model: ["None", "Terrain", "Relative","Absolute"]
                                    anchors.centerIn: parent
                                    onCurrentIndexChanged:   {
                                        if (circleProperties)
                                            circleProperties.clamp = currentIndex
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
                                        implicitHeight: 20
                                        border.color: control.pressed ? "#5f5f5f" : "#404040"
                                        border.width: control.visualFocus ? 2 : 1
                                        radius: 5
                                        color: "#c9c9c9"

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

                            Rectangle{
                                Layout.fillWidth: true
                                color: "#404040"
                                height: 30
                                //                                border.color: "#5f5f5f"
                                //                                border.width: 1

                                Text {
                                    text: qsTr("Clamp :")
                                    font.pointSize: 10
                                    color: "white"
                                    anchors.verticalCenter:  parent.verticalCenter
                                    x:7
                                }
                            }

                        }
                    }
                }
            }
        }
    }
}

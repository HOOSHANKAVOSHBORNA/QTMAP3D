import QtQuick 2.13
import QtQuick.Window 2.13
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13
import QtQuick.Dialogs 1.2
import Crystal 1.0

Item {
    id: rootItem
    function show() {
        dialog.open()
    }

    function hide() {
        dialog.close();
    }


    property CircleProperties circleProperties


    Dialog {
        id:dialog
        //   flags: Qt.FramelessWindowHint | Qt.Window
        width: 250
        height: 350
        title: qsTr("Circle Properties")



        //    x:0
        //    y:0


        ///////////////////properties////////////////////////////
        property color fillColor: "#004C05"
        property color lineColor: "black"
//        property int previousX
//        property int previousY

        ///////////////////Main Holder///////////////////////////
        //    Rectangle{
        //        id: background
        //        color: "#313131"
        //        anchors.fill: parent
        //        radius: 10
        //        border.width: 1
        //        border.color: "gray"

        //        MouseArea {
        //            anchors.fill: parent

        //            onPressed: {
        //                previousX = mouseX
        //                previousY = mouseY
        //            }

        //            onMouseXChanged: {
        //                var dx = mouseX - previousX
        //                root.setX(root.x + dx)
        //            }

        //            onMouseYChanged: {
        //                var dy = mouseY - previousY
        //                root.setY(root.y + dy)
        //            }
        //        }

        ///////////////////////////Header//////////////////////////////
        //        Rectangle{
        //            width: parent.width -2
        //            anchors.horizontalCenter: parent.horizontalCenter
        //            y: 2
        //            height: 35
        //            radius: 10
        //            gradient:Gradient{
        //                GradientStop{position:1.0 ; color: "#313131"}
        //                GradientStop{position:0.0 ; color: "#5f5f5f"}
        //            }
        //            Text {
        //                id: headerTitle
        //                text: qsTr("Circle Properties")
        //                anchors.centerIn: parent
        //                font.bold: true
        //                font.pointSize: 10
        //                color: "white"
        //            }

        //            Image {
        //                id: closeBtn
        //                source: "close.png"
        //                sourceSize: "15x15"
        //                x: parent.width -27
        //                y: 10

        //                MouseArea{
        //                    anchors.fill: parent
        //                    cursorShape: Qt.PointingHandCursor
        //                    onClicked: {
        //                        Qt.quit()
        //                    }
        //                }
        //            }
        //        }
        //////////////////////////Main Content////////////////////////
        contentItem:  Rectangle {
            id: item
            visible: true
            width: parent.width
            //            height: 350
            //            anchors.horizontalCenter: parent.horizontalCenter
            anchors.fill: parent
            color: "#3e3e3e"
            //            y:43
            ScrollView {
                id: frame
                clip: true
                anchors.fill: parent
                ScrollBar.vertical.policy: ScrollBar.AlwaysOn
                Flickable {
                    contentHeight: 320
                    width: 250
                    Rectangle {
                        id : innerContainer
                        color: "#4b4b4b"
                        anchors.fill: parent

                        /////////////////////// components Grid ////////////////////////////
                        GridLayout{
                            id: props
                            width: 235
                            y: innerContainer.y +3
                            anchors.horizontalCenter: parent.horizontalCenter
                            columnSpacing: 0
                            rowSpacing: 2
                            columns:2
                            rows: 6
                            layoutDirection: Qt.RightToLeft
                            anchors.fill: parent

                            ///////////////////////////////////Fill Color Property//////////////////////////////////
                            Rectangle{
                                Layout.fillWidth: true
                                color: "#313131"
                                height: 35
                                border.color: "#5f5f5f"
                                border.width: 1


                                Rectangle{
                                    height: 20
                                    width: 20
                                    color: circleProperties.fillcolor
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
                                        id: fillColorField
                                        text: circleProperties.fillcolor
                                        anchors.centerIn: parent
                                        font.pointSize: 10

                                    }
                                }
                                ColorDialog {
                                    visible: false
                                    id: fillColorDialog
                                    title: "Please choose a color"
                                    onAccepted: {
                                        circleProperties.fillcolor = fillColorDialog.color
                                    }

                                }
                            }
                            Rectangle{
                                Layout.fillWidth: true
                                color: "#313131"
                                height: 35
                                border.color: "#5f5f5f"
                                border.width: 1

                                Text {
                                    id: fillColorCircle
                                    text: qsTr("FillColor :")
                                    font.pointSize: 10
                                    color: "white"
                                    anchors.verticalCenter:  parent.verticalCenter
                                    x:7
                                }
                            }

                            ///////////////////////////////////Line Color Property//////////////////////////////////
                            Rectangle{
                                Layout.fillWidth: true
                                color: "#313131"
                                height: 35
                                border.color: "#5f5f5f"
                                border.width: 1


                                Rectangle{
                                    height: 20
                                    width: 20
                                    color: lineColor
                                    border.width: 2
                                    border.color: "#c9c9c9"
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
                                        lineColor = lineColorDialog.color
                                        circleProperties.linecolor = lineColorDialog.color
                                    }

                                }
                            }
                            Rectangle{
                                Layout.fillWidth: true
                                color: "#313131"
                                height: 35
                                border.color: "#5f5f5f"
                                border.width: 1

                                Text {
                                    id: lineColorCircle
                                    text: qsTr("LineColor :")
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
                                color: "#313131"
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
                                                text: circleProperties.location.x
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
                                                text: circleProperties.location.y
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
                                                text: circleProperties.location.z
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
                                color: "#313131"
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
                                    checked: circleProperties.relative
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
                                color: "#313131"
                                height: 55
                                border.color: "#5f5f5f"
                                border.width: 1

                                Column{
                                    Row{
                                        spacing: 3
                                        leftPadding: 2
                                        topPadding: 5
                                        //                                        anchors.horizontalCenter: parent.horizontalCenter
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
                                                text: circleProperties.arcstart
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
                                                text: circleProperties.arcend
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
                                color: "#313131"
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

                            ///////////////////////////////////Radius///////////////////////////////////////////
                            Rectangle{
                                Layout.fillWidth: true
                                color: "#313131"
                                height: 30
                                border.color: "#5f5f5f"
                                border.width: 1

                                Rectangle{
                                    height: 20
                                    width: 75
                                    color: "#c9c9c9"
                                    clip:  true
                                    radius: 5
                                    anchors.verticalCenter:  parent.verticalCenter
                                    x: 28

                                    TextInput {
                                        id: radiusValue
                                        anchors.fill: parent
                                        font.pointSize: 10
                                        text: circleProperties.radius
                                        selectByMouse: true
                                        selectionColor: "dark green"
                                        validator: DoubleValidator {
                                            decimals: 8;
                                            notation: DoubleValidator.StandardNotation
                                            locale: "insert radius"
                                        }
                                        onTextChanged: {
                                            circleProperties.radius = text
                                        }
                                    }
                                }
                            }
                            Rectangle{
                                Layout.fillWidth: true
                                color: "#313131"
                                height: 30
                                border.color: "#5f5f5f"
                                border.width: 1

                                Text {
                                    id: radiusCircle
                                    text: qsTr("Radius :")
                                    font.pointSize: 10
                                    color: "white"
                                    anchors.verticalCenter:  parent.verticalCenter
                                    x:7
                                }
                            }
                            /////////////////////////////////// Height ///////////////////////////////////////////
                            Rectangle{
                                Layout.fillWidth: true
                                color: "#313131"
                                height: 30
                                border.color: "#5f5f5f"
                                border.width: 1

                                Rectangle{
                                    height: 20
                                    width: 75
                                    color: "#c9c9c9"
                                    clip:  true
                                    radius: 5
                                    anchors.verticalCenter:  parent.verticalCenter
                                    x: 28

                                    TextInput {
                                        id: heightValue
                                        anchors.fill: parent
                                        font.pointSize: 10
                                        text: circleProperties.circleheight
                                        selectByMouse: true
                                        selectionColor: "dark green"
                                        validator: DoubleValidator {
                                            decimals: 8;
                                            notation: DoubleValidator.StandardNotation
                                            locale: "insert height"
                                        }
                                        onTextChanged: {
                                            circleProperties.circleheight = text
                                        }
                                    }
                                }
                            }
                            Rectangle{
                                Layout.fillWidth: true
                                color: "#313131"
                                height: 30
                                border.color: "#5f5f5f"
                                border.width: 1

                                Text {
                                    id: heightCircle
                                    text: qsTr("Height :")
                                    font.pointSize: 10
                                    color: "white"
                                    anchors.verticalCenter:  parent.verticalCenter
                                    x:7
                                }
                            }
                            ///////////////////////////////////Transparency/////////////////////////////////////
                            Rectangle{
                                id: transContainer
                                Layout.fillWidth: true
                                color: "#313131"
                                height: 30
                                border.color: "#5f5f5f"
                                border.width: 1

                                SpinBox {
                                    id: transValue
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
                                        z: 2
                                        text: transValue.textFromValue(transValue.value, transValue.locale)
                                        font: transValue.font
                                        color: "#313131"
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
                                           circleProperties.transparency = transValue.value
                                            }
                                        }
                                    }
                                    up.indicator: Rectangle {
                                        x: transValue.mirrored ? 0 : parent.width - width
                                        height: parent.height
                                        implicitWidth: 20
                                        implicitHeight: 20
                                        color: transValue.up.pressed ? "#5f5f5f" : "#313131"
                                        border.color: enabled ? "#313131" : "#5f5f5f"

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
                                        color: transValue.down.pressed ? "#5f5f5f" : "#313131"
                                        border.color: enabled ? "#313131" : "#5f5f5f"

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
                                Layout.fillWidth: true
                                color: "#313131"
                                height: 30
                                border.color: "#5f5f5f"
                                border.width: 1

                                Text {
                                    id: transCircle
                                    text: qsTr("Transparency :")
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
                                color: "#313131"
                                height: 30
                                border.color: "#5f5f5f"
                                border.width: 1

                                ComboBox {
                                    id: control
                                    model: ["None", "Terrain", "Relative","Absolute"]
                                    anchors.centerIn: parent
                                    onCurrentIndexChanged:   {
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

                                        onPaint: {
                                            context.reset();
                                            context.moveTo(0, 0);
                                            context.lineTo(width, 0);
                                            context.lineTo(width / 2, height);
                                            context.closePath();
                                            context.fillStyle = control.pressed ? "#5f5f5f" : "#313131";
                                            context.fill();
                                        }
                                    }

                                    contentItem: Text {
                                        leftPadding: 5
                                        rightPadding: control.indicator.width + control.spacing

                                        text: control.displayText
                                        font: control.font
                                        color: control.pressed ? "#5f5f5f" : "#313131"
                                        verticalAlignment: Text.AlignVCenter
                                        elide: Text.ElideRight
                                    }

                                    background: Rectangle {
                                        implicitWidth: 100
                                        implicitHeight: 20
                                        border.color: control.pressed ? "#5f5f5f" : "#313131"
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
                                            border.color: "#313131"
                                            radius: 5
                                        }
                                    }
                                }
                            }

                            Rectangle{
                                Layout.fillWidth: true
                                color: "#313131"
                                height: 30
                                border.color: "#5f5f5f"
                                border.width: 1

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


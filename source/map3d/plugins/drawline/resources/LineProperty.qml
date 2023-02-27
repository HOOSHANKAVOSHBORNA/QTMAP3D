import QtQuick 2.13
import QtQuick.Window 2.12
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.12
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


    property LineProperties lineProperties

    Dialog {
        id: dialog
        width: 250
        height: 300
        title: qsTr("Line Properties")



        //////////////////////////Main Content////////////////////////
        contentItem:  Rectangle {
            id: item
            visible: true
            width: 240
            height: 300
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
                    contentHeight: 300
                    width: 240
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

                            ////////////////////////////////////line Color Property//////////////////////////////////
                            Rectangle{
                                Layout.fillWidth: true
                                color: "#313131"
                                height: 35
                                border.color: "#5f5f5f"
                                border.width: 1


                                Rectangle{
                                    height: 20
                                    width: 20
                                    color: lineProperties.color
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
                                        text: "#356898"
                                        anchors.centerIn: parent
                                        font.pointSize: 10

                                    }
                                }
                                ColorDialog {
                                    visible: false
                                    id:  lineColorDialog
                                    title: "Please choose a color"
                                    onAccepted: {
                                        lineProperties.color = lineColorDialog.color
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

                            ///////////////////////////////////point Color Property//////////////////////////////////
                            Rectangle{
                                Layout.fillWidth: true
                                color: "#313131"
                                height: 35
                                border.color: "#5f5f5f"
                                border.width: 1


                                Rectangle{
                                    height: 20
                                    width: 20
                                    color: lineProperties.pointColor
                                    border.width: 2
                                    border.color: "#c9c9c9"
                                    x: 5
                                    radius: 5
                                    anchors.verticalCenter: parent.verticalCenter

                                    MouseArea{
                                        anchors.fill: parent
                                        cursorShape: Qt.PointingHandCursor
                                        onClicked: pointColorDialog.visible = true

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
                                        id: pointColorField
                                        text: "lineColor"
                                        anchors.centerIn: parent
                                        font.pointSize: 10

                                    }
                                }
                                ColorDialog {
                                    visible: false
                                    id: pointColorDialog
                                    title: "Please choose a color"
                                    onAccepted: {
                                        //                                    lineColor = lineColorDialog.color
                                        lineProperties.pointColor = pointColorDialog.color
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
                                    id: pointColor
                                    text: qsTr("pointColor :")
                                    font.pointSize: 10
                                    color: "white"
                                    anchors.verticalCenter:  parent.verticalCenter
                                    x:7
                                }
                            }

                            ///////////////////////////////////width/////////////////////////////////////

                            Rectangle{
                                id: transContainer
                                Layout.fillWidth: true
                                color: "#313131"
                                height: 30
                                border.color: "#5f5f5f"
                                border.width: 1

                                SpinBox {
                                    id: transValue
                                    stepSize: 1
                                    value: 10
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
                                                lineProperties.width = transValue.value
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
                                    id: transSphere
                                    text: qsTr("Width:")
                                    font.pointSize: 10
                                    color: "white"
                                    anchors.verticalCenter:  parent.verticalCenter
                                    x:7
                                }
                            }

                            ///////////////////////////////////pointwidth/////////////////////////////////////

                            Rectangle{
                                id: pointwidthContainer
                                Layout.fillWidth: true
                                color: "#313131"
                                height: 30
                                border.color: "#5f5f5f"
                                border.width: 1

                                SpinBox {
                                    id: pointwidthValue
                                    stepSize: 1
                                    value: 10
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
                                        text: pointwidthValue.textFromValue(pointwidthValue.value, pointwidthValue.locale)
                                        font: pointwidthValue.font
                                        color: "#313131"
                                        horizontalAlignment: Qt.AlignHCenter
                                        verticalAlignment: Qt.AlignVCenter +10
                                        readOnly: !pointwidthValue.editable
                                        validator: pointwidthValue.validator
                                        inputMethodHints: Qt.ImhFormattedNumbersOnly
                                        topPadding: 13
                                        selectByMouse: true
                                        selectionColor: "dark green"
                                        onTextChanged: {
                                            if(pointwidthValue && pointwidthValue.value){
                                                lineProperties.pointwidth = pointwidthValue.value
                                            }
                                        }
                                    }
                                    up.indicator: Rectangle {
                                        x: pointwidthValue.mirrored ? 0 : parent.width - width
                                        height: parent.height
                                        implicitWidth: 20
                                        implicitHeight: 20
                                        color: pointwidthValue.up.pressed ? "#5f5f5f" : "#313131"
                                        border.color: enabled ? "#313131" : "#5f5f5f"

                                        Text {
                                            text: "+"
                                            font.pixelSize: pointwidthValue.font.pixelSize * 2
                                            color: "white"
                                            anchors.fill: parent
                                            fontSizeMode: Text.Fit
                                            horizontalAlignment: Text.AlignHCenter
                                            verticalAlignment: Text.AlignVCenter
                                        }
                                    }
                                    down.indicator: Rectangle {
                                        x: pointwidthValue.mirrored ? parent.width - width : 0
                                        height: parent.height
                                        implicitWidth: 20
                                        implicitHeight: 20
                                        color: pointwidthValue.down.pressed ? "#5f5f5f" : "#313131"
                                        border.color: enabled ? "#313131" : "#5f5f5f"

                                        Text {
                                            text: "-"
                                            font.pixelSize: pointwidthValue.font.pixelSize * 2
                                            color: "white"
                                            anchors.fill: parent
                                            fontSizeMode: Text.Fit
                                            horizontalAlignment: Text.AlignHCenter
                                            verticalAlignment: Text.AlignVCenter
                                        }
                                    }
                                    background: Rectangle {
                                        implicitWidth: pointwidthContainer.width -10
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
                                    id: pointwidth
                                    text: qsTr("Point Width:")
                                    font.pointSize: 10
                                    color: "white"
                                    anchors.verticalCenter:  parent.verticalCenter
                                    x:7
                                }
                            }
                            ///////////////////////////////////tesselation///////////////////////////////////////////
                            Rectangle{
                                Layout.fillWidth: true
                                color: "#313131"
                                height: 30
                                border.color: "#5f5f5f"
                                border.width: 1


                                Rectangle{
                                    height: 20
                                    width: 97
                                    color: "#c9c9c9"
                                    clip:  true
                                    radius: 5
                                    anchors.verticalCenter:  parent.verticalCenter
                                    x: 10

                                    TextInput {
                                        id: tesselationValue
                                        text: lineProperties.tesselation
                                        anchors.fill: parent
                                        font.pointSize: 10
                                        selectByMouse: true
                                        selectionColor: "dark green"
                                        validator: DoubleValidator {
                                            decimals: 0;
                                            notation: DoubleValidator.StandardNotation
                                            locale: "insert tesseletaion"
                                        }
                                        onTextChanged: {
                                            lineProperties.tesselation = text
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
                                    id: tesselation
                                    text: qsTr("Tesselation:")
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
                                        lineProperties.clamp = currentIndex
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
                            /////////////////////////////////// type///////////////////////////////////////////
                            //                            Rectangle{
                            //                                id: typeContainer
                            //                                Layout.fillWidth: true
                            //                                color: "#313131"
                            //                                height: 30
                            //                                border.color: "#5f5f5f"
                            //                                border.width: 1

                            //                                ComboBox {
                            //                                    id: typeControl
                            //                                    model: ["Line", "Strip"]
                            //                                    anchors.centerIn: parent
                            //                                    onCurrentIndexChanged:   {
                            //                                        //                                        circleProperties.clamp = currentIndex
                            //                                    }

                            //                                    delegate: ItemDelegate {
                            //                                        width: typeControl.width
                            //                                        contentItem: Text {
                            //                                            text: typeControl.textRole
                            //                                                  ? (Array.isArray(typeControl.model) ? modelData[typeControl.textRole] : model[typeControl.textRole])
                            //                                                  : modelData
                            //                                            color: "#5f5f5f"
                            //                                            font: typeControl.font
                            //                                            elide: Text.ElideRight
                            //                                            verticalAlignment: Text.AlignVCenter
                            //                                        }
                            //                                        highlighted: typeControl.highlightedIndex === index
                            //                                    }

                            //                                    indicator: Canvas {
                            //                                        id: typeCanvas
                            //                                        x: typeControl.width - width - typeControl.rightPadding
                            //                                        y: typeControl.topPadding + (typeControl.availableHeight - height) / 2
                            //                                        width: 12
                            //                                        height: 8
                            //                                        contextType: "2d"

                            //                                        Connections {
                            //                                            target: typeControl
                            //                                            function onPressedChanged() { typeCanvas.requestPaint(); }
                            //                                        }

                            //                                        onPaint: {
                            //                                            context.reset();
                            //                                            context.moveTo(0, 0);
                            //                                            context.lineTo(width, 0);
                            //                                            context.lineTo(width / 2, height);
                            //                                            context.closePath();
                            //                                            context.fillStyle = typeControl.pressed ? "#5f5f5f" : "#313131";
                            //                                            context.fill();
                            //                                        }
                            //                                    }

                            //                                    contentItem: Text {
                            //                                        leftPadding: 5
                            //                                        rightPadding: typeControl.indicator.width + typeControl.spacing

                            //                                        text: typeControl.displayText
                            //                                        font: typeControl.font
                            //                                        color: typeControl.pressed ? "#5f5f5f" : "#313131"
                            //                                        verticalAlignment: Text.AlignVCenter
                            //                                        elide: Text.ElideRight
                            //                                    }

                            //                                    background: Rectangle {
                            //                                        implicitWidth: 100
                            //                                        implicitHeight: 20
                            //                                        border.color: typeControl.pressed ? "#5f5f5f" : "#313131"
                            //                                        border.width: typeControl.visualFocus ? 2 : 1
                            //                                        radius: 5
                            //                                        color: "#c9c9c9"

                            //                                    }

                            //                                    popup: Popup {
                            //                                        y: typeControl.height - 1
                            //                                        width: typeControl.width
                            //                                        implicitHeight: contentItem.implicitHeight
                            //                                        padding: 1

                            //                                        contentItem: ListView {
                            //                                            clip: true
                            //                                            implicitHeight: contentHeight
                            //                                            model: typeControl.popup.visible ? typeControl.delegateModel : null
                            //                                            currentIndex: typeControl.highlightedIndex

                            //                                            ScrollIndicator.vertical: ScrollIndicator { }
                            //                                        }

                            //                                        background: Rectangle {
                            //                                            border.color: "#313131"
                            //                                            radius: 5
                            //                                        }
                            //                                    }
                            //                                }
                            //                            }

                            //                            Rectangle{
                            //                                Layout.fillWidth: true
                            //                                color: "#313131"
                            //                                height: 30
                            //                                border.color: "#5f5f5f"
                            //                                border.width: 1

                            //                                Text {
                            //                                    text: qsTr("Line Type :")
                            //                                    font.pointSize: 10
                            //                                    color: "white"
                            //                                    anchors.verticalCenter:  parent.verticalCenter
                            //                                    x:7
                            //                                }


                            //                            }
                            /////////////////////////////////////////////////////point//////////////////////////////////////////////////
                            Rectangle{
                                id: pointContainer
                                Layout.fillWidth: true
                                color: "#313131"
                                height: 30
                                border.color: "#5f5f5f"
                                border.width: 1

                                ComboBox {
                                    id: pointControl
                                    model: ["Visibile", "Unvisible"]
                                    anchors.centerIn: parent
                                    currentIndex: 1
                                    onCurrentIndexChanged:   {
                                        lineProperties.visible = currentIndex
                                    }

                                    delegate: ItemDelegate {
                                        width: pointControl.width
                                        contentItem: Text {
                                            text: pointControl.textRole
                                                  ? (Array.isArray(pointControl.model) ? modelData[pointControl.textRole] : model[pointControl.textRole])
                                                  : modelData
                                            color: "#5f5f5f"
                                            font: pointControl.font
                                            elide: Text.ElideRight
                                            verticalAlignment: Text.AlignVCenter
                                        }
                                        highlighted: pointControl.highlightedIndex === index
                                    }

                                    indicator: Canvas {
                                        id: pointCanvas
                                        x: pointControl.width - width - pointControl.rightPadding
                                        y: pointControl.topPadding + (pointControl.availableHeight - height) / 2
                                        width: 12
                                        height: 8
                                        contextType: "2d"

                                        Connections {
                                            target: pointControl
                                            function onPressedChanged() { pointCanvas.requestPaint(); }
                                        }

                                        onPaint: {
                                            context.reset();
                                            context.moveTo(0, 0);
                                            context.lineTo(width, 0);
                                            context.lineTo(width / 2, height);
                                            context.closePath();
                                            context.fillStyle = pointControl.pressed ? "#5f5f5f" : "#313131";
                                            context.fill();
                                        }
                                    }

                                    contentItem: Text {
                                        leftPadding: 5
                                        rightPadding: pointControl.indicator.width + pointControl.spacing

                                        text: pointControl.displayText
                                        font: pointControl.font
                                        color: pointControl.pressed ? "#5f5f5f" : "#313131"
                                        verticalAlignment: Text.AlignVCenter
                                        elide: Text.ElideRight
                                    }

                                    background: Rectangle {
                                        implicitWidth: 100
                                        implicitHeight: 20
                                        border.color: pointControl.pressed ? "#5f5f5f" : "#313131"
                                        border.width: pointControl.visualFocus ? 2 : 1
                                        radius: 5
                                        color: "#c9c9c9"

                                    }

                                    popup: Popup {
                                        y: pointControl.height - 1
                                        width: pointControl.width
                                        implicitHeight: contentItem.implicitHeight
                                        padding: 1

                                        contentItem: ListView {
                                            clip: true
                                            implicitHeight: contentHeight
                                            model: pointControl.popup.visible ? pointControl.delegateModel : null
                                            currentIndex: pointControl.highlightedIndex

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
                                    text: qsTr("Points :")
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

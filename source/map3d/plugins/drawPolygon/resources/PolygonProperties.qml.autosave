import QtQuick 2.13
import QtQuick.Window 2.12
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.2
import Crystal 1.0

Item {
    id: rootItem
    implicitHeight: parent.height



    property PolygonProperties polygonProperties
    property string headerTitleSTR: "Polygon Properties"
    property string fillColor: "#91001d"
    property string lineColor: "#ffffff"


    onVisibleChanged: {
        polygonProperties.fillcolor = fillColor
        polygonProperties.linecolor = lineColor
        polygonProperties.height = transValue.value
        polygonProperties .lineWidth = lineValue.value
        polygonProperties.lineopacity = lineopacityValue.value
        polygonProperties.transparency = opacityValue.value
    }

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
//                ScrollBar.vertical.policy: ScrollBar.AlwaysOn
                Flickable {
                    contentHeight: 400
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
                                width: parent.width/1.9
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
                                    width: 100
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
                                        polygonProperties.fillcolor = fillColor.toString()
                                    }

                                }
                            }
                            Rectangle{
                                Layout.fillWidth: true
                                width: parent.width/3
                                color: "#404040"
                                height: 35


                                Text {
                                    id: fillColorpolygon
                                    text: qsTr("Fill Color :")
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
                                            if(polygonProperties){
                                                opacityValue.value = opacityValueInput.text
                                                polygonProperties.transparency = opacityValue.value
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
                                    id: opacitypolygon
                                    text: qsTr("Opacity:")
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
                                    width: 100
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
                                        polygonProperties.linecolor = lineColor
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
                                    text: qsTr("Stroke Color:")
                                    font.pointSize: 10
                                    color: "white"
                                    anchors.verticalCenter:  parent.verticalCenter
                                    x:7
                                }
                            }
                            ///////////////////////////////////opacity/////////////////////////////////////

                            Rectangle{
                                id: lineopacityContainer
                                Layout.fillWidth: true
                                color: "#404040"
                                height: 30
                                //                                border.color: "#5f5f5f"
                                //                                border.width: 1

                                SpinBox {
                                    id: lineopacityValue
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
                                        id:lineopacityValueInput
                                        z: 2
                                        //                                        text: pointwidthValue.textFromValue(pointwidthValue.value, pointwidthValue.locale)
                                        text: lineopacityValue.value
                                        font: lineopacityValue.font
                                        color: "#404040"
                                        horizontalAlignment: Qt.AlignHCenter
                                        verticalAlignment: Qt.AlignVCenter +10
                                        readOnly: !lineopacityValue.editable
                                        validator: lineopacityValue.validator
                                        inputMethodHints: Qt.ImhFormattedNumbersOnly
                                        topPadding: 13
                                        selectByMouse: true
                                        selectionColor: "dark green"
                                        onTextChanged: {
                                            if(polygonProperties){
                                                lineopacityValue.value = lineopacityValueInput.text
                                                polygonProperties.lineopacity = lineopacityValue.value
                                            }
                                        }
                                    }
                                    up.indicator: Rectangle {
                                        x: lineopacityValue.mirrored ? 0 : parent.width - width
                                        height: parent.height
                                        implicitWidth: 20
                                        implicitHeight: 20
                                        color: lineopacityValue.up.pressed ? "#5f5f5f" : "#404040"
                                        border.color: enabled ? "#404040" : "#5f5f5f"

                                        Text {
                                            text: "+"
                                            font.pixelSize: lineopacityValue.font.pixelSize * 2
                                            color: "white"
                                            anchors.fill: parent
                                            fontSizeMode: Text.Fit
                                            horizontalAlignment: Text.AlignHCenter
                                            verticalAlignment: Text.AlignVCenter
                                        }
                                    }
                                    down.indicator: Rectangle {
                                        x: lineopacityValue.mirrored ? parent.width - width : 0
                                        height: parent.height
                                        implicitWidth: 20
                                        implicitHeight: 20
                                        color: lineopacityValue.down.pressed ? "#5f5f5f" : "#404040"
                                        border.color: enabled ? "#404040" : "#5f5f5f"

                                        Text {
                                            text: "-"
                                            font.pixelSize: lineopacityValue.font.pixelSize * 2
                                            color: "white"
                                            anchors.fill: parent
                                            fontSizeMode: Text.Fit
                                            horizontalAlignment: Text.AlignHCenter
                                            verticalAlignment: Text.AlignVCenter
                                        }
                                    }
                                    background: Rectangle {
                                        implicitWidth: lineopacityContainer.width -10
                                        color: "#c9c9c9"
                                        border.color: "#bdbebf"
                                    }
                                }
                            }
                            Rectangle{
                                id: lineopacityTitle
                                Layout.fillWidth: true
                                color: "#404040"
                                height: 30
                                //                                border.color: "#5f5f5f"
                                //                                border.width: 1

                                Text {
                                    id: lineopacitypolygon
                                    text: qsTr("Stroke Opacity:")
                                    font.pointSize: 10
                                    color: "white"
                                    anchors.verticalCenter:  parent.verticalCenter
                                    x:7
                                }
                            }


                            //////////////////////////////////linewidth////////////////////////////////////
                            Rectangle{
                                id: lineContainer
                                Layout.fillWidth: true
                                color: "#404040"
                                height: 30
                                //                                border.color: "#5f5f5f"
                                //                                border.width: 1

                                SpinBox {
                                    id: lineValue
                                    stepSize: 1
                                    value: 0
                                    to : 100000
                                    from : 0
                                    validator: DoubleValidator {
                                        bottom: 0
                                        top:  10000
                                    }
                                    editable: true
                                    anchors.centerIn: parent
                                    height: 20

                                    contentItem: TextInput {
                                        id: widthInput
                                        z: 2
                                        //                                        text: transValue.textFromValue(transValue.value, transValue.locale)
                                        text: lineValue.value
                                        font: lineValue.font
                                        color: "#404040"
                                        horizontalAlignment: Qt.AlignHCenter
                                        verticalAlignment: Qt.AlignVCenter +10
                                        readOnly: !lineValue.editable
                                        validator: lineValue.validator
                                        inputMethodHints: Qt.ImhFormattedNumbersOnly
                                        topPadding: 13
                                        selectByMouse: true
                                        selectionColor: "dark green"
                                        onTextChanged: {
                                            if(polygonProperties && lineValue && (lineValue.value == 0 || lineValue.value)){
                                                lineValue.value = widthInput.text
                                                polygonProperties.lineWidth = lineValue.value/10
                                            }
                                        }
                                    }
                                    up.indicator: Rectangle {
                                        x: lineValue.mirrored ? 0 : parent.width - width
                                        height: parent.height
                                        implicitWidth: 20
                                        implicitHeight: 20
                                        color: lineValue.up.pressed ? "#5f5f5f" : "#404040"
                                        border.color: enabled ? "#404040" : "#5f5f5f"

                                        Text {
                                            text: "+"
                                            font.pixelSize: lineValue.font.pixelSize * 2
                                            color: "white"
                                            anchors.fill: parent
                                            fontSizeMode: Text.Fit
                                            horizontalAlignment: Text.AlignHCenter
                                            verticalAlignment: Text.AlignVCenter
                                        }
                                    }
                                    down.indicator: Rectangle {
                                        x: lineValue.mirrored ? parent.width - width : 0
                                        height: parent.height
                                        implicitWidth: 20
                                        implicitHeight: 20
                                        color: lineValue.down.pressed ? "#5f5f5f" : "#404040"
                                        border.color: enabled ? "#404040" : "#5f5f5f"

                                        Text {
                                            text: "-"
                                            font.pixelSize: lineValue.font.pixelSize * 2
                                            color: "white"
                                            anchors.fill: parent
                                            fontSizeMode: Text.Fit
                                            horizontalAlignment: Text.AlignHCenter
                                            verticalAlignment: Text.AlignVCenter
                                        }
                                    }
                                    background: Rectangle {
                                        implicitWidth: lineContainer.width -10
                                        color: "#c9c9c9"
                                        border.color: "#bdbebf"
                                    }
                                }
                            }
                            Rectangle{
                                id:weightContainer
                                Layout.fillWidth: true
                                color: "#404040"
                                height: 30
                                //                                border.color: "#5f5f5f"
                                //                                border.width: 1

                                Text {
                                    //                                    id: transSphere
                                    text: qsTr("Stroke Width:")
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
                                            if(polygonProperties && transValue && (transValue.value == 0 || transValue.value)){
                                                transValue.value = heightInput.text
                                                polygonProperties.polygonheight = transValue.value
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



//                            ///////////////////////////////////tesselation///////////////////////////////////////////
//                            Rectangle{
//                                id: tesselationContainer
//                                Layout.fillWidth: true
//                                color: "#404040"
//                                height: 30
//                                //                                border.color: "#5f5f5f"
//                                //                                border.width: 1

//                                SpinBox {
//                                    id: tesselationValue
//                                    stepSize: 1
//                                    value: 10
//                                    to : 100000
//                                    from : 0
//                                    validator: DoubleValidator {
//                                        bottom: 0
//                                        top:  100
//                                    }
//                                    editable: true
//                                    anchors.centerIn: parent
//                                    height: 20

//                                    contentItem: TextInput {
//                                        id:tesselInput
//                                        z: 2
//                                        //                                        text: tesselationValue.textFromValue(tesselationValue.value, tesselationValue.locale)
//                                        text: tesselationValue.value
//                                        font: tesselationValue.font
//                                        color: "#404040"
//                                        horizontalAlignment: Qt.AlignHCenter
//                                        verticalAlignment: Qt.AlignVCenter +10
//                                        readOnly: !tesselationValue.editable
//                                        validator: tesselationValue.validator
//                                        inputMethodHints: Qt.ImhFormattedNumbersOnly
//                                        topPadding: 13
//                                        selectByMouse: true
//                                        selectionColor: "dark green"
//                                        onTextChanged: {
//                                            if(polygonProperties){
//                                                tesselationValue.value = tesselInput.text
//                                                polygonProperties.tesselation = tesselationValue.value
//                                            }
//                                        }
//                                    }
//                                    up.indicator: Rectangle {
//                                        x: tesselationValue.mirrored ? 0 : parent.width - width
//                                        height: parent.height
//                                        implicitWidth: 20
//                                        implicitHeight: 20
//                                        color: tesselationValue.up.pressed ? "#5f5f5f" : "#404040"
//                                        border.color: enabled ? "#404040" : "#5f5f5f"

//                                        Text {
//                                            text: "+"
//                                            font.pixelSize: tesselationValue.font.pixelSize * 2
//                                            color: "white"
//                                            anchors.fill: parent
//                                            fontSizeMode: Text.Fit
//                                            horizontalAlignment: Text.AlignHCenter
//                                            verticalAlignment: Text.AlignVCenter
//                                        }
//                                    }
//                                    down.indicator: Rectangle {
//                                        x: tesselationValue.mirrored ? parent.width - width : 0
//                                        height: parent.height
//                                        implicitWidth: 20
//                                        implicitHeight: 20
//                                        color: tesselationValue.down.pressed ? "#5f5f5f" : "#404040"
//                                        border.color: enabled ? "#404040" : "#5f5f5f"

//                                        Text {
//                                            text: "-"
//                                            font.pixelSize: tesselationValue.font.pixelSize * 2
//                                            color: "white"
//                                            anchors.fill: parent
//                                            fontSizeMode: Text.Fit
//                                            horizontalAlignment: Text.AlignHCenter
//                                            verticalAlignment: Text.AlignVCenter
//                                        }
//                                    }
//                                    background: Rectangle {
//                                        implicitWidth: tesselationContainer.width -10
//                                        color: "#c9c9c9"
//                                        border.color: "#bdbebf"
//                                    }
//                                }
//                            }
//                            Rectangle{
//                                id:tesselationContainerTitle
//                                Layout.fillWidth: true
//                                color: "#404040"
//                                height: 30
//                                //                                border.color: "#5f5f5f"
//                                //                                border.width: 1

//                                Text {
//                                    id: tesselationSphere
//                                    text: qsTr("Tesselation:")
//                                    font.pointSize: 10
//                                    color: "white"
//                                    anchors.verticalCenter:  parent.verticalCenter
//                                    x:7
//                                }
//                            }



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
                                        if (polygonProperties)
                                            polygonProperties.clamp = currentIndex
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

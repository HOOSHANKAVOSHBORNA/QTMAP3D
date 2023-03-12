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
                                        circleProperties.color = fillColor.toString()
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
                                        circleProperties.pointColor = lineColor
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
                                    id: lineColor
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
//                                                text: circleProperties.arcstart
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
//                                                text: circleProperties.arcend
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
                            ///////////////////////////////////width/////////////////////////////////////

                            Rectangle{
                                id: widthContainer
                                Layout.fillWidth: true
                                color: "#404040"
                                height: 30
                                //                                border.color: "#5f5f5f"
                                //                                border.width: 1

                                SpinBox {
                                    id: widthValue
                                    stepSize: 1
                                    value: 10
                                    to : 10000
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
                                        //                                        text: widthValue.textFromValue(widthValue.value, widthValue.locale)
                                        text: widthValue.value
                                        font: widthValue.font
                                        color: "#404040"
                                        horizontalAlignment: Qt.AlignHCenter
                                        verticalAlignment: Qt.AlignVCenter +10
                                        readOnly: !widthValue.editable
                                        validator: widthValue.validator
                                        inputMethodHints: Qt.ImhFormattedNumbersOnly
                                        topPadding: 13
                                        selectByMouse: true
                                        selectionColor: "dark green"
                                        onTextChanged: {
                                            if(lineProperties){
                                                widthValue.value = valueInput.text
                                                lineProperties.width = widthValue.value
                                            }
                                        }
                                    }
                                    up.indicator: Rectangle {
                                        x: widthValue.mirrored ? 0 : parent.width - width
                                        height: parent.height
                                        implicitWidth: 20
                                        implicitHeight: 20
                                        color: widthValue.up.pressed ? "#5f5f5f" : "#404040"
                                        border.color: enabled ? "#404040" : "#5f5f5f"

                                        Text {
                                            text: "+"
                                            font.pixelSize: widthValue.font.pixelSize * 2
                                            color: "white"
                                            anchors.fill: parent
                                            fontSizeMode: Text.Fit
                                            horizontalAlignment: Text.AlignHCenter
                                            verticalAlignment: Text.AlignVCenter
                                        }
                                    }
                                    down.indicator: Rectangle {
                                        x: widthValue.mirrored ? parent.width - width : 0
                                        height: parent.height
                                        implicitWidth: 20
                                        implicitHeight: 20
                                        color: widthValue.down.pressed ? "#5f5f5f" : "#404040"
                                        border.color: enabled ? "#404040" : "#5f5f5f"

                                        Text {
                                            text: "-"
                                            font.pixelSize: widthValue.font.pixelSize * 2
                                            color: "white"
                                            anchors.fill: parent
                                            fontSizeMode: Text.Fit
                                            horizontalAlignment: Text.AlignHCenter
                                            verticalAlignment: Text.AlignVCenter
                                        }
                                    }
                                    background: Rectangle {
                                        implicitWidth: widthContainer.width -10
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
                                    id: widthSphere
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
                                color: "#404040"
                                height: 30
                                //                                border.color: "#5f5f5f"
                                //                                border.width: 1

                                SpinBox {
                                    id: pointwidthValue
                                    stepSize: 1
                                    value: 10
                                    to : 10000
                                    from : 0
                                    validator: DoubleValidator {
                                        bottom: 0
                                        top:  100
                                    }
                                    editable: true
                                    anchors.centerIn: parent
                                    height: 20


                                    contentItem: TextInput {
                                        id:pointValueInput
                                        z: 2
                                        //                                        text: pointwidthValue.textFromValue(pointwidthValue.value, pointwidthValue.locale)
                                        text: pointwidthValue.value
                                        font: pointwidthValue.font
                                        color: "#404040"
                                        horizontalAlignment: Qt.AlignHCenter
                                        verticalAlignment: Qt.AlignVCenter +10
                                        readOnly: !pointwidthValue.editable
                                        validator: pointwidthValue.validator
                                        inputMethodHints: Qt.ImhFormattedNumbersOnly
                                        topPadding: 13
                                        selectByMouse: true
                                        selectionColor: "dark green"
                                        onTextChanged: {
                                            if(lineProperties){
                                                pointwidthValue.value = pointValueInput.text
                                                lineProperties.pointwidth = pointwidthValue.value
                                            }
                                        }
                                    }
                                    up.indicator: Rectangle {
                                        x: pointwidthValue.mirrored ? 0 : parent.width - width
                                        height: parent.height
                                        implicitWidth: 20
                                        implicitHeight: 20
                                        color: pointwidthValue.up.pressed ? "#5f5f5f" : "#404040"
                                        border.color: enabled ? "#404040" : "#5f5f5f"

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
                                        color: pointwidthValue.down.pressed ? "#5f5f5f" : "#404040"
                                        border.color: enabled ? "#404040" : "#5f5f5f"

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
                                id: pointWtitle
                                Layout.fillWidth: true
                                color: "#404040"
                                height: 30
                                //                                border.color: "#5f5f5f"
                                //                                border.width: 1

                                Text {
                                    id: pointwidth
                                    text: qsTr("Point Width:")
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
                                    stepSize: 1
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
                                                lineProperties.height = transValue.value
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

                            ///////////////////////////////////tesselation///////////////////////////////////////////
                            Rectangle{
                                id: tesselationContainer
                                Layout.fillWidth: true
                                color: "#404040"
                                height: 30
                                //                                border.color: "#5f5f5f"
                                //                                border.width: 1

                                SpinBox {
                                    id: tesselationValue
                                    stepSize: 1
                                    value: 10
                                    to : 100000
                                    from : 0
                                    validator: DoubleValidator {
                                        bottom: 0
                                        top:  100
                                    }
                                    editable: true
                                    anchors.centerIn: parent
                                    height: 20

                                    contentItem: TextInput {
                                        id:tesselInput
                                        z: 2
                                        //                                        text: tesselationValue.textFromValue(tesselationValue.value, tesselationValue.locale)
                                        text: tesselationValue.value
                                        font: tesselationValue.font
                                        color: "#404040"
                                        horizontalAlignment: Qt.AlignHCenter
                                        verticalAlignment: Qt.AlignVCenter +10
                                        readOnly: !tesselationValue.editable
                                        validator: tesselationValue.validator
                                        inputMethodHints: Qt.ImhFormattedNumbersOnly
                                        topPadding: 13
                                        selectByMouse: true
                                        selectionColor: "dark green"
                                        onTextChanged: {
                                            if(lineProperties){
                                                tesselationValue.value = tesselInput.text
                                                lineProperties.tesselation = tesselationValue.value
                                            }
                                        }
                                    }
                                    up.indicator: Rectangle {
                                        x: tesselationValue.mirrored ? 0 : parent.width - width
                                        height: parent.height
                                        implicitWidth: 20
                                        implicitHeight: 20
                                        color: tesselationValue.up.pressed ? "#5f5f5f" : "#404040"
                                        border.color: enabled ? "#404040" : "#5f5f5f"

                                        Text {
                                            text: "+"
                                            font.pixelSize: tesselationValue.font.pixelSize * 2
                                            color: "white"
                                            anchors.fill: parent
                                            fontSizeMode: Text.Fit
                                            horizontalAlignment: Text.AlignHCenter
                                            verticalAlignment: Text.AlignVCenter
                                        }
                                    }
                                    down.indicator: Rectangle {
                                        x: tesselationValue.mirrored ? parent.width - width : 0
                                        height: parent.height
                                        implicitWidth: 20
                                        implicitHeight: 20
                                        color: tesselationValue.down.pressed ? "#5f5f5f" : "#404040"
                                        border.color: enabled ? "#404040" : "#5f5f5f"

                                        Text {
                                            text: "-"
                                            font.pixelSize: tesselationValue.font.pixelSize * 2
                                            color: "white"
                                            anchors.fill: parent
                                            fontSizeMode: Text.Fit
                                            horizontalAlignment: Text.AlignHCenter
                                            verticalAlignment: Text.AlignVCenter
                                        }
                                    }
                                    background: Rectangle {
                                        implicitWidth: tesselationContainer.width -10
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
                                    id: tesselationSphere
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
                                color: "#404040"
                                height: 30
                                //                                border.color: "#5f5f5f"
                                //                                border.width: 1

                                ComboBox {
                                    id: control
                                    model: ["None", "Terrain", "Relative","Absolute"]
                                    anchors.centerIn: parent
                                    onCurrentIndexChanged:   {
                                        if (lineProperties)
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

                                        //                                        onPaint: {
                                        //                                            context.reset();
                                        //                                            context.moveTo(0, 0);
                                        //                                            context.lineTo(width, 0);
                                        //                                            context.lineTo(width / 2, height);
                                        //                                            context.closePath();
                                        //                                            context.fillStyle = control.pressed ? "#5f5f5f" : "#404040";
                                        //                                            context.fill();
                                        //                                        }
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
                            /////////////////////////////////////////////////////point//////////////////////////////////////////////////
                            Rectangle{
                                id: pointContainer
                                Layout.fillWidth: true
                                color: "#404040"
                                height: 30
                                //                                border.color: "#5f5f5f"
                                //                                border.width: 1

                                Switch {
                                    id: visibleSwitch
                                    anchors.centerIn: parent
                                    checked: true
                                    onCheckedChanged:      if(visibleSwitch.checked === false){
                                                               smooth.checkable = false;
                                                               pointwidthValue.enabled = false;
                                                           }else{
                                                               smooth.checkable = true;
                                                               pointwidthValue.enabled = true;
                                                           }

                                    ToolTip {
                                        parent: visibleSwitch
                                        y: visibleSwitch.y + visibleSwitch.height
                                        Text{
                                            text: visibleSwitch.checked ? "Click to deactive" : "Click to active"
                                            color: "white"
                                        }

                                        background: Rectangle {
                                            color: "#404040"
                                            radius: 4
                                        }

                                        visible:  visibleSwitch.hovered
                                    }
                                    onToggled: function() {
                                        lineProperties.visible = visibleSwitch.checked
                                    }

                                    Layout.alignment: Qt.AlignRight
                                    indicator: Rectangle {
                                        implicitWidth: 70
                                        implicitHeight: 20
                                        x: visibleSwitch.leftPadding
                                        y: parent.height / 2 - height / 2
                                        radius: 13
                                        color: visibleSwitch.checked ? "#3f9173" : "#383838"
                                        border.color: visibleSwitch.checked ? "#17a81a" : "#a8171a"

                                        Rectangle {
                                            x: visibleSwitch.checked ? parent.width - width : 0
                                            width: 20
                                            height: 20
                                            radius: 13
                                            color: visibleSwitch.down ? "#cccccc" : "#ffffff"
                                            border.color: visibleSwitch.checked ? (visibleSwitch.down ? "#17a81a" : "#21be2b") : "#999999"
                                        }
                                    }
                                }
                            }

                            Rectangle{
                                id: pointContainerTitle
                                Layout.fillWidth: true
                                color: "#404040"
                                height: 30
                                //                                border.color: "#5f5f5f"
                                //                                border.width: 1

                                Text {
                                    text: qsTr("Points :")
                                    font.pointSize: 10
                                    color: "white"
                                    anchors.verticalCenter:  parent.verticalCenter
                                    x:7
                                }
                            }

                            /////////////////////////////////////////////////////smooth//////////////////////////////////////////////////
                            Rectangle{
                                id: smoothContainer
                                Layout.fillWidth: true
                                color: "#404040"
                                height: 30
                                //                                border.color: "#5f5f5f"
                                //                                border.width: 1

                                Switch {
                                    id: smooth
                                    anchors.centerIn: parent
                                    checked: true

                                    ToolTip {
                                        parent: smooth
                                        y: smooth.y + smooth.height
                                        Text{
                                            text: smooth.checked ? "Click to deactive" : "Click to active"
                                            color: "white"
                                        }

                                        background: Rectangle {
                                            color: "#404040"
                                            radius: 4
                                        }

                                        visible:  smooth.hovered
                                    }
                                    onToggled: function() {
                                        lineProperties.smooth = smooth.checked
                                    }

                                    Layout.alignment: Qt.AlignRight
                                    indicator: Rectangle {
                                        implicitWidth: 70
                                        implicitHeight: 20
                                        x: smooth.leftPadding
                                        y: parent.height / 2 - height / 2
                                        radius: 13
                                        color: smooth.checked ? "#3f9173" : "#383838"
                                        border.color: smooth.checked ? "#17a81a" : "#a8171a"

                                        Rectangle {
                                            x: smooth.checked ? parent.width - width : 0
                                            width: 20
                                            height: 20
                                            radius: 13
                                            color: smooth.down ? "#cccccc" : "#ffffff"
                                            border.color: smooth.checked ? (smooth.down ? "#17a81a" : "#21be2b") : "#999999"
                                        }
                                    }
                                }
                            }

                            Rectangle{
                                id: smoothContainerTitle
                                Layout.fillWidth: true
                                color: "#404040"
                                height: 30
                                //                                border.color: "#5f5f5f"
                                //                                border.width: 1

                                Text {
                                    text: qsTr("Smooth :")
                                    font.pointSize: 10
                                    color: "white"
                                    anchors.verticalCenter:  parent.verticalCenter
                                    x:7
                                }
                            }

                            /////////////////////////////////////////////////////showlen//////////////////////////////////////////////////
                            Rectangle{
                                id: lenContainer
                                Layout.fillWidth: true
                                color: "#404040"
                                height: 30
                                //                                border.color: "#5f5f5f"
                                //                                border.width: 1

                                Switch {
                                    id: len
                                    anchors.centerIn: parent
                                    checked: false

                                    ToolTip {
                                        parent: len
                                        y: len.y + len.height
                                        Text{
                                            text: len.checked ? "Click to deactive" : "Click to active"
                                            color: "white"
                                        }

                                        background: Rectangle {
                                            color: "#404040"
                                            radius: 4
                                        }

                                        visible:  len.hovered
                                    }
                                    onToggled: function() {
                                        lineProperties.showLen = len.checked
                                    }

                                    Layout.alignment: Qt.AlignRight
                                    indicator: Rectangle {
                                        implicitWidth: 70
                                        implicitHeight: 20
                                        x: len.leftPadding
                                        y: parent.height / 2 - height / 2
                                        radius: 13
                                        color: len.checked ? "#3f9173" : "#383838"
                                        border.color: len.checked ? "#17a81a" : "#a8171a"

                                        Rectangle {
                                            x: len.checked ? parent.width - width : 0
                                            width: 20
                                            height: 20
                                            radius: 13
                                            color: len.down ? "#cccccc" : "#ffffff"
                                            border.color: len.checked ? (len.down ? "#17a81a" : "#21be2b") : "#999999"
                                        }
                                    }
                                }
                            }

                            Rectangle{
                                id:lenContainerTitle
                                Layout.fillWidth: true
                                color: "#404040"
                                height: 30
                                //                                border.color: "#5f5f5f"
                                //                                border.width: 1

                                Text {
                                    text: qsTr("Show Length:")
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

    onRulerConditionChanged:   {
        if(rulerCondition === true){
            pointColorSecR.visible = false
            pointColorSecL.visible = false
            pointwidthContainer.visible = false
            pointWtitle.visible = false
            pointContainer.visible = false
            pointContainerTitle.visible = false
            smoothContainer.visible = false
            smoothContainerTitle.visible = false
            lenContainer.visible = false
            lenContainerTitle.visible = false
            transContainer.visible = false
            heightContainer.visible = false
            lineProperties.showLen = true
            lineProperties.visible = false
            headerTitleSTR = "Ruler Properties"

        }
        else if(rulerCondition === false){
            pointColorSecR.visible = true
            pointColorSecL.visible = true
            pointwidthContainer.visible = true
            pointWtitle.visible = true
            pointContainer.visible = true
            pointContainerTitle.visible = true
            smoothContainer.visible = true
            smoothContainerTitle.visible = true
            lenContainer.visible = true
            lenContainerTitle.visible = true
            transContainer.visible = true
            heightContainer.visible = true
            lineProperties.showLen = false
            lineProperties.visible = true
            headerTitleSTR = "Line Properties"
        }
    }


}

















//import QtQuick 2.13
//import QtQuick.Window 2.13
//import QtQuick.Controls 2.13
//import QtQuick.Layouts 1.13
//import QtQuick.Dialogs 1.2
//import Crystal 1.0


//Item {
//    id: rootItem
//    implicitHeight: parent.height

//    property CircleProperties circleProperties
//    property string headerTitleSTR: "circle Properties"
//    property string fillColor: "#91001d"
//    property string lineColor: "#001191"


//    Item {
//        id:dialog
//        width: 250
//        height: 350



//        ///////////////////Main Holder/////////////////////////

//        Rectangle{
//            id:header
//            width: parent.width -2
//            anchors.horizontalCenter: parent.horizontalCenter
//            y: 2
//            height: 30
//            radius: 0
//            color: "#404040"

//            Text {
//                id: headerTitle
//                text: headerTitleSTR
//                anchors.centerIn: parent
//                font.family: "SourceSansPro"
//                font.pointSize: 14
//                color: "white"
//            }

//        }

//        //////////////////////////Main Content////////////////////////
//         Rectangle {
//            id: item
//            visible: true
//            width: parent.width
//            //            height: 350
//            //            anchors.horizontalCenter: parent.horizontalCenter
//            anchors.fill: parent
//            color: "#3e3e3e"
//            //            y:43
//            ScrollView {
//                id: frame
//                clip: true
//                anchors.fill: parent
//                ScrollBar.vertical.policy: ScrollBar.AlwaysOn
//                Flickable {
//                    contentHeight: 320
//                    width: 250
//                    Rectangle {
//                        id : innerContainer
//                        color: "#4b4b4b"
//                        anchors.fill: parent

//                        /////////////////////// components Grid ////////////////////////////
//                        GridLayout{
//                            id: props
//                            width: 235
//                            y: innerContainer.y +3
//                            anchors.horizontalCenter: parent.horizontalCenter
//                            columnSpacing: 0
//                            rowSpacing: 2
//                            columns:2
//                            rows: 6
//                            layoutDirection: Qt.RightToLeft
//                            anchors.fill: parent

//                            ///////////////////////////////////Fill Color Property//////////////////////////////////
//                            Rectangle{
//                                Layout.fillWidth: true
//                                color: "#313131"
//                                height: 35
//                                border.color: "#5f5f5f"
//                                border.width: 1


//                                Rectangle{
//                                    height: 20
//                                    width: 20
//                                    color: circleProperties.fillcolor
//                                    border.width: 2
//                                    border.color: "#c9c9c9"
//                                    x: 5
//                                    radius: 5
//                                    anchors.verticalCenter: parent.verticalCenter

//                                    MouseArea{
//                                        anchors.fill: parent
//                                        cursorShape: Qt.PointingHandCursor
//                                        onClicked: fillColorDialog.visible = true

//                                    }
//                                }
//                                Rectangle{
//                                    height: 20
//                                    width: 75
//                                    color: "#c9c9c9"
//                                    x: 30
//                                    anchors.verticalCenter: parent.verticalCenter
//                                    radius: 5

//                                    Text {
//                                        id: fillColorField
//                                        text: circleProperties.fillcolor
//                                        anchors.centerIn: parent
//                                        font.pointSize: 10

//                                    }
//                                }
//                                ColorDialog {
//                                    visible: false
//                                    id: fillColorDialog
//                                    title: "Please choose a color"
//                                    onAccepted: {
//                                        circleProperties.fillcolor = fillColorDialog.color
//                                    }

//                                }
//                            }
//                            Rectangle{
//                                Layout.fillWidth: true
//                                color: "#313131"
//                                height: 35
//                                border.color: "#5f5f5f"
//                                border.width: 1

//                                Text {
//                                    id: fillColorCircle
//                                    text: qsTr("FillColor :")
//                                    font.pointSize: 10
//                                    color: "white"
//                                    anchors.verticalCenter:  parent.verticalCenter
//                                    x:7
//                                }
//                            }

//                            ///////////////////////////////////Line Color Property//////////////////////////////////
//                            Rectangle{
//                                Layout.fillWidth: true
//                                color: "#313131"
//                                height: 35
//                                border.color: "#5f5f5f"
//                                border.width: 1


//                                Rectangle{
//                                    height: 20
//                                    width: 20
//                                    color: lineColor
//                                    border.width: 2
//                                    border.color: "#c9c9c9"
//                                    x: 5
//                                    radius: 5
//                                    anchors.verticalCenter: parent.verticalCenter

//                                    MouseArea{
//                                        anchors.fill: parent
//                                        cursorShape: Qt.PointingHandCursor
//                                        onClicked: lineColorDialog.visible = true

//                                    }
//                                }
//                                Rectangle{
//                                    height: 20
//                                    width: 75
//                                    color: "#c9c9c9"
//                                    x: 30
//                                    anchors.verticalCenter: parent.verticalCenter
//                                    radius: 5

//                                    Text {
//                                        id: lineColorField
//                                        text: lineColor
//                                        anchors.centerIn: parent
//                                        font.pointSize: 10

//                                    }
//                                }
//                                ColorDialog {
//                                    visible: false
//                                    id: lineColorDialog
//                                    title: "Please choose a color"
//                                    onAccepted: {
//                                        lineColor = lineColorDialog.color
//                                        circleProperties.linecolor = lineColorDialog.color
//                                    }

//                                }
//                            }
//                            Rectangle{
//                                Layout.fillWidth: true
//                                color: "#313131"
//                                height: 35
//                                border.color: "#5f5f5f"
//                                border.width: 1

//                                Text {
//                                    id: lineColorCircle
//                                    text: qsTr("LineColor :")
//                                    font.pointSize: 10
//                                    color: "white"
//                                    anchors.verticalCenter:  parent.verticalCenter
//                                    x:7
//                                }
//                            }
//                            ///////////////////////////////////Location Property///////////////////////////////
//                            Rectangle{
//                                id:locationContainer
//                                Layout.fillWidth: true
//                                color: "#313131"
//                                height: 80
//                                border.color: "#5f5f5f"
//                                border.width: 1

//                                Column{
//                                    Row{
//                                        spacing: 13
//                                        leftPadding: 5
//                                        topPadding: 5
//                                        anchors.horizontalCenter: parent.horizontalCenter

//                                        Text {
//                                            color: "#e5e5e5"
//                                            text: qsTr("X:")
//                                            font.pointSize: 10
//                                        }
//                                        Rectangle{
//                                            height: 20
//                                            width: 75
//                                            color: "#c9c9c9"
//                                            clip:  true
//                                            radius: 5
//                                            Layout.fillWidth: true
//                                            TextInput {
//                                                id: mlocationX
//                                                anchors.fill: parent
//                                                text: circleProperties.location.x
//                                                font.pointSize: 10
//                                                selectByMouse: true
//                                                selectionColor: "dark green"
//                                                validator: DoubleValidator {
//                                                    decimals: 8;
//                                                    notation: DoubleValidator.StandardNotation
//                                                    locale: "insert x"
//                                                }
//                                                onTextChanged: {
//                                                    circleProperties.location.x = text
//                                                }
//                                            }
//                                        }
//                                    }
//                                    Row{
//                                        spacing: 13
//                                        leftPadding: 5
//                                        topPadding: 5
//                                        anchors.horizontalCenter: parent.horizontalCenter

//                                        Text {
//                                            color: "#e5e5e5"
//                                            text: qsTr("Y:")
//                                            font.pointSize: 10
//                                        }
//                                        Rectangle{
//                                            height: 20
//                                            width: 75
//                                            color: "#c9c9c9"
//                                            clip:  true
//                                            radius: 5

//                                            TextInput {
//                                                id: mlocationY
//                                                anchors.fill: parent
//                                                font.pointSize: 10
//                                                text: circleProperties.location.y
//                                                selectByMouse: true
//                                                selectionColor: "dark green"
//                                                validator: DoubleValidator {
//                                                    decimals: 8;
//                                                    notation: DoubleValidator.StandardNotation
//                                                    locale: "insert y"
//                                                }
//                                                onTextChanged: {
//                                                    circleProperties.location.y = text
//                                                }
//                                            }
//                                        }
//                                    }
//                                    Row{
//                                        spacing: 13
//                                        leftPadding: 5
//                                        topPadding: 5
//                                        anchors.horizontalCenter: parent.horizontalCenter

//                                        Text {
//                                            color: "#e5e5e5"
//                                            text: qsTr("Z:")
//                                            font.pointSize: 10
//                                        }
//                                        Rectangle{
//                                            height: 20
//                                            width: 75
//                                            color: "#c9c9c9"
//                                            clip:  true
//                                            radius: 5

//                                            TextInput {
//                                                id: mlocationZ
//                                                anchors.fill: parent
//                                                font.pointSize: 10
//                                                text: circleProperties.location.z
//                                                selectByMouse: true
//                                                selectionColor: "dark green"
//                                                validator: DoubleValidator {
//                                                    decimals: 8;
//                                                    notation: DoubleValidator.StandardNotation
//                                                    locale: "insert z"
//                                                }
//                                                onTextChanged: {
//                                                    circleProperties.location.z = text
//                                                }
//                                            }
//                                        }
//                                    }
//                                }
//                            }
//                            Rectangle{
//                                id: locationTitle
//                                Layout.fillWidth: true
//                                color: "#313131"
//                                height: 80
//                                border.color: "#5f5f5f"
//                                border.width: 1

//                                Text {
//                                    id: locationCircle
//                                    text: qsTr("Location :")
//                                    font.pointSize: 10
//                                    color: "white"
//                                    y: 10
//                                    x:7
//                                }
//                                CheckBox {
//                                    id: relative
//                                    text: qsTr("Relative")
//                                    checked: circleProperties.relative
//                                    anchors.bottom: locationTitle.bottom
//                                    onCheckStateChanged: if(checked === true){
//                                                             circleProperties.relative = true
//                                                         }
//                                                         else{
//                                                             circleProperties.relative = false
//                                                         }

//                                    indicator: Rectangle {
//                                        implicitWidth: 15
//                                        implicitHeight: 15
//                                        x: relative.leftPadding
//                                        y: parent.height / 2 - height / 2
//                                        radius: 3
//                                        border.color: relative.down ? "black" : "#313131"

//                                        Rectangle {
//                                            width: 10
//                                            height: 10
//                                            x: 3
//                                            y: 3
//                                            radius: 2
//                                            color: relative.down ? "black" : "dark green"
//                                            visible: relative.checked
//                                        }
//                                    }
//                                    contentItem: Text {
//                                        text: relative.text
//                                        font: relative.font
//                                        opacity: enabled ? 1.0 : 0.3
//                                        color: relative.down ? "#black" : "white"
//                                        verticalAlignment: Text.AlignVCenter
//                                        leftPadding: relative.indicator.width + relative.spacing
//                                    }
//                                }
//                            }

//                            ///////////////////////////////////ARC Property///////////////////////////////
//                            Rectangle{
//                                id:arcContainer
//                                Layout.fillWidth: true
//                                color: "#313131"
//                                height: 55
//                                border.color: "#5f5f5f"
//                                border.width: 1

//                                Column{
//                                    Row{
//                                        spacing: 3
//                                        leftPadding: 2
//                                        topPadding: 5
//                                        //                                        anchors.horizontalCenter: parent.horizontalCenter
//                                        x:1

//                                        Text {
//                                            color: "#e5e5e5"
//                                            text: qsTr("Start:")
//                                            font.pointSize: 10
//                                        }
//                                        Rectangle{
//                                            height: 20
//                                            width: 67
//                                            color: "#c9c9c9"
//                                            clip:  true
//                                            radius: 5

//                                            TextInput {
//                                                id: startArc
//                                                anchors.fill: parent
//                                                font.pointSize: 10
//                                                text: circleProperties.arcstart
//                                                selectByMouse: true
//                                                selectionColor: "dark green"
//                                                validator: DoubleValidator {
//                                                    decimals: 8;
//                                                    notation: DoubleValidator.StandardNotation
//                                                    locale: "insert start"
//                                                }
//                                                onTextChanged: {
//                                                    circleProperties.arcstart = text
//                                                }
//                                            }
//                                        }
//                                    }
//                                    Row{
//                                        spacing: 9
//                                        leftPadding: 2
//                                        topPadding: 5
//                                        //                                        anchors.horizontalCenter: parent.horizontalCenter
//                                        x: 1

//                                        Text {
//                                            color: "#e5e5e5"
//                                            text: qsTr("End:")
//                                            font.pointSize: 10
//                                        }
//                                        Rectangle{
//                                            height: 20
//                                            width: 67
//                                            color: "#c9c9c9"
//                                            clip:  true
//                                            radius: 5

//                                            TextInput {
//                                                id: endArc
//                                                anchors.fill: parent
//                                                font.pointSize: 10
//                                                text: circleProperties.arcend
//                                                selectByMouse: true
//                                                selectionColor: "dark green"
//                                                validator: DoubleValidator {
//                                                    decimals: 8;
//                                                    notation: DoubleValidator.StandardNotation
//                                                    locale: "insert end"
//                                                }
//                                                onTextChanged: {
//                                                    circleProperties.arcend = text
//                                                }
//                                            }
//                                        }
//                                    }
//                                }
//                            }
//                            Rectangle{
//                                id: arcTitle
//                                Layout.fillWidth: true
//                                color: "#313131"
//                                height: 55
//                                border.color: "#5f5f5f"
//                                border.width: 1

//                                Text {
//                                    id: arcCircle
//                                    text: qsTr("Arc :")
//                                    font.pointSize: 10
//                                    color: "white"
//                                    y: 10
//                                    x:7
//                                }
//                            }

//                            ///////////////////////////////////Radius///////////////////////////////////////////
//                            Rectangle{
//                                Layout.fillWidth: true
//                                color: "#313131"
//                                height: 30
//                                border.color: "#5f5f5f"
//                                border.width: 1

//                                Rectangle{
//                                    height: 20
//                                    width: 75
//                                    color: "#c9c9c9"
//                                    clip:  true
//                                    radius: 5
//                                    anchors.verticalCenter:  parent.verticalCenter
//                                    x: 28

//                                    TextInput {
//                                        id: radiusValue
//                                        anchors.fill: parent
//                                        font.pointSize: 10
//                                        text: circleProperties.radius
//                                        selectByMouse: true
//                                        selectionColor: "dark green"
//                                        validator: DoubleValidator {
//                                            decimals: 8;
//                                            notation: DoubleValidator.StandardNotation
//                                            locale: "insert radius"
//                                        }
//                                        onTextChanged: {
//                                            circleProperties.radius = text
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
//                                    id: radiusCircle
//                                    text: qsTr("Radius :")
//                                    font.pointSize: 10
//                                    color: "white"
//                                    anchors.verticalCenter:  parent.verticalCenter
//                                    x:7
//                                }
//                            }
//                            /////////////////////////////////// Height ///////////////////////////////////////////
//                            Rectangle{
//                                Layout.fillWidth: true
//                                color: "#313131"
//                                height: 30
//                                border.color: "#5f5f5f"
//                                border.width: 1

//                                Rectangle{
//                                    height: 20
//                                    width: 75
//                                    color: "#c9c9c9"
//                                    clip:  true
//                                    radius: 5
//                                    anchors.verticalCenter:  parent.verticalCenter
//                                    x: 28

//                                    TextInput {
//                                        id: heightValue
//                                        anchors.fill: parent
//                                        font.pointSize: 10
//                                        text: circleProperties.circleheight
//                                        selectByMouse: true
//                                        selectionColor: "dark green"
//                                        validator: DoubleValidator {
//                                            decimals: 8;
//                                            notation: DoubleValidator.StandardNotation
//                                            locale: "insert height"
//                                        }
//                                        onTextChanged: {
//                                            circleProperties.circleheight = text
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
//                                    id: heightCircle
//                                    text: qsTr("Height :")
//                                    font.pointSize: 10
//                                    color: "white"
//                                    anchors.verticalCenter:  parent.verticalCenter
//                                    x:7
//                                }
//                            }
//                            ///////////////////////////////////Transparency/////////////////////////////////////
//                            Rectangle{
//                                id: transContainer
//                                Layout.fillWidth: true
//                                color: "#313131"
//                                height: 30
//                                border.color: "#5f5f5f"
//                                border.width: 1

//                                SpinBox {
//                                    id: transValue
//                                    stepSize: 5
//                                    value: 50
//                                    to : 100
//                                    from : 0
//                                    validator: DoubleValidator {
//                                        bottom: 0
//                                        top:  100
//                                    }
//                                    editable: true
//                                    anchors.centerIn: parent
//                                    height: 20


//                                    contentItem: TextInput {
//                                        z: 2
//                                        text: transValue.textFromValue(transValue.value, transValue.locale)
//                                        font: transValue.font
//                                        color: "#313131"
//                                        horizontalAlignment: Qt.AlignHCenter
//                                        verticalAlignment: Qt.AlignVCenter +10
//                                        readOnly: !transValue.editable
//                                        validator: transValue.validator
//                                        inputMethodHints: Qt.ImhFormattedNumbersOnly
//                                        topPadding: 13
//                                        selectByMouse: true
//                                        selectionColor: "dark green"
//                                        onTextChanged: {
//                                            if(transValue && transValue.value){
//                                                circleProperties.transparency = transValue.value
//                                            }
//                                        }
//                                    }
//                                    up.indicator: Rectangle {
//                                        x: transValue.mirrored ? 0 : parent.width - width
//                                        height: parent.height
//                                        implicitWidth: 20
//                                        implicitHeight: 20
//                                        color: transValue.up.pressed ? "#5f5f5f" : "#313131"
//                                        border.color: enabled ? "#313131" : "#5f5f5f"

//                                        Text {
//                                            text: "+"
//                                            font.pixelSize: transValue.font.pixelSize * 2
//                                            color: "white"
//                                            anchors.fill: parent
//                                            fontSizeMode: Text.Fit
//                                            horizontalAlignment: Text.AlignHCenter
//                                            verticalAlignment: Text.AlignVCenter
//                                        }
//                                    }
//                                    down.indicator: Rectangle {
//                                        x: transValue.mirrored ? parent.width - width : 0
//                                        height: parent.height
//                                        implicitWidth: 20
//                                        implicitHeight: 20
//                                        color: transValue.down.pressed ? "#5f5f5f" : "#313131"
//                                        border.color: enabled ? "#313131" : "#5f5f5f"

//                                        Text {
//                                            text: "-"
//                                            font.pixelSize: transValue.font.pixelSize * 2
//                                            color: "white"
//                                            anchors.fill: parent
//                                            fontSizeMode: Text.Fit
//                                            horizontalAlignment: Text.AlignHCenter
//                                            verticalAlignment: Text.AlignVCenter
//                                        }
//                                    }
//                                    background: Rectangle {
//                                        implicitWidth: transContainer.width -10
//                                        color: "#c9c9c9"
//                                        border.color: "#bdbebf"
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
//                                    id: transCircle
//                                    text: qsTr("Transparency :")
//                                    font.pointSize: 10
//                                    color: "white"
//                                    anchors.verticalCenter:  parent.verticalCenter
//                                    x:7
//                                }
//                            }
//                            /////////////////////////////////// Clamp ///////////////////////////////////////////
//                            Rectangle{
//                                id: clampContainer
//                                Layout.fillWidth: true
//                                color: "#313131"
//                                height: 30
//                                border.color: "#5f5f5f"
//                                border.width: 1

//                                ComboBox {
//                                    id: control
//                                    model: ["None", "Terrain", "Relative","Absolute"]
//                                    anchors.centerIn: parent
//                                    onCurrentIndexChanged:   {
//                                        circleProperties.clamp = currentIndex
//                                    }

//                                    delegate: ItemDelegate {
//                                        width: control.width
//                                        contentItem: Text {
//                                            text: control.textRole
//                                                  ? (Array.isArray(control.model) ? modelData[control.textRole] : model[control.textRole])
//                                                  : modelData
//                                            color: "#5f5f5f"
//                                            font: control.font
//                                            elide: Text.ElideRight
//                                            verticalAlignment: Text.AlignVCenter
//                                        }
//                                        highlighted: control.highlightedIndex === index
//                                    }

//                                    indicator: Canvas {
//                                        id: canvas
//                                        x: control.width - width - control.rightPadding
//                                        y: control.topPadding + (control.availableHeight - height) / 2
//                                        width: 12
//                                        height: 8
//                                        contextType: "2d"

//                                        Connections {
//                                            target: control
//                                            function onPressedChanged() { canvas.requestPaint(); }
//                                        }

//                                        onPaint: {
//                                            context.reset();
//                                            context.moveTo(0, 0);
//                                            context.lineTo(width, 0);
//                                            context.lineTo(width / 2, height);
//                                            context.closePath();
//                                            context.fillStyle = control.pressed ? "#5f5f5f" : "#313131";
//                                            context.fill();
//                                        }
//                                    }

//                                    contentItem: Text {
//                                        leftPadding: 5
//                                        rightPadding: control.indicator.width + control.spacing

//                                        text: control.displayText
//                                        font: control.font
//                                        color: control.pressed ? "#5f5f5f" : "#313131"
//                                        verticalAlignment: Text.AlignVCenter
//                                        elide: Text.ElideRight
//                                    }

//                                    background: Rectangle {
//                                        implicitWidth: 100
//                                        implicitHeight: 20
//                                        border.color: control.pressed ? "#5f5f5f" : "#313131"
//                                        border.width: control.visualFocus ? 2 : 1
//                                        radius: 5
//                                        color: "#c9c9c9"

//                                    }

//                                    popup: Popup {
//                                        y: control.height - 1
//                                        width: control.width
//                                        implicitHeight: contentItem.implicitHeight
//                                        padding: 1

//                                        contentItem: ListView {
//                                            clip: true
//                                            implicitHeight: contentHeight
//                                            model: control.popup.visible ? control.delegateModel : null
//                                            currentIndex: control.highlightedIndex

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
//                                    text: qsTr("Clamp :")
//                                    font.pointSize: 10
//                                    color: "white"
//                                    anchors.verticalCenter:  parent.verticalCenter
//                                    x:7
//                                }
//                            }
//                        }
//                    }
//                }
//            }
//        }
//    }
//}


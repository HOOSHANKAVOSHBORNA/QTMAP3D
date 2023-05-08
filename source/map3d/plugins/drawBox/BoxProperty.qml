import QtQuick 2.13
import QtQuick.Window 2.12
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.2
import Crystal 1.0

Item {
    id: rootItem
    implicitHeight: parent.height



    property BoxProperties boxProperties
    property string headerTitleSTR: "box Properties"
    property string fillColor: "#91001d"
    property string lineColor: "#ffffff"
    property double unitsMulti
    property double stepSize


    onVisibleChanged: {
        boxProperties.color = fillColor
        boxProperties.opacity = opacityValue.value
        boxProperties.width = widthValue.value*unitsMulti
        boxProperties.height = heightValue.value*unitsMulti
        boxProperties.length = lengthValue.value*unitsMulti
    }

    onUnitsMultiChanged: {
        boxProperties.width = widthValue.value*unitsMulti
        boxProperties.height = heightValue.value*unitsMulti
        boxProperties.length = lengthValue.value*unitsMulti
    }

    onStepSizeChanged: {
        widthValue.stepSize = stepSize
        heightValue.stepSize = stepSize
        lengthValue.stepSize = stepSize
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
            ///////////////////////////////////////units///////////////////////////////////////////////
            Rectangle{
                id:units
                width: parent.width -2
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: header.bottom
                height: 25
                radius: 0
                color: "#303030"
                RowLayout{
                    spacing: 10
                    x:2
                    Text {
                        text: qsTr("Unit:")
                        color: "white"
                    }
                    RadioButton{
                        id:kmradio
                        text: "km"
                        contentItem: Text {
                            text: kmradio.text
                            color: "white"
                            leftPadding: kmradio.indicator.width + kmradio.spacing
                            verticalAlignment: Text.AlignVCenter
                        }
                        indicator: Rectangle {
                            id:kmbtn
                            implicitWidth: 15
                            implicitHeight: 15
                            x: kmradio.leftPadding
                            y: parent.height / 2 - height / 2
                            radius: 8
                            border.color: kmradio.down ? "#17a81a" : "#21be2b"
                            Rectangle {
                                width: 9
                                height: 9
                                anchors.centerIn: kmbtn
                                radius: 5
                                color: kmradio.down ? "#17a81a" : "#21be2b"
                                visible: kmradio.checked
                                onVisibleChanged: if(kmradio.checked){
                                                      unitsMulti = 1000
                                                  }
                            }
                        }
                    }
                    RadioButton{
                        id:mradio
                        text: "m"
                        checked: true
                        contentItem: Text {
                            text: mradio.text
                            color: "white"
                            leftPadding: mradio.indicator.width + mradio.spacing
                            verticalAlignment: Text.AlignVCenter
                        }
                        indicator: Rectangle {
                            id:mbtn
                            implicitWidth: 15
                            implicitHeight: 15
                            x: mradio.leftPadding
                            y: parent.height / 2 - height / 2
                            radius: 8
                            border.color: mradio.down ? "#17a81a" : "#21be2b"
                            Rectangle {
                                width: 9
                                height: 9
                                anchors.centerIn: mbtn
                                radius: 5
                                color: mradio.down ? "#17a81a" : "#21be2b"
                                visible: mradio.checked
                                onVisibleChanged: if(mradio.checked){
                                                      unitsMulti = 1
                                                  }
                            }
                        }
                    }RadioButton{
                        id:cmradio
                        text: "cm"
                        contentItem: Text {
                            text: cmradio.text
                            color: "white"
                            leftPadding: cmradio.indicator.width + cmradio.spacing
                            verticalAlignment: Text.AlignVCenter
                        }
                        indicator: Rectangle {
                            id:cmbtn
                            implicitWidth: 15
                            implicitHeight: 15
                            x: cmradio.leftPadding
                            y: parent.height / 2 - height / 2
                            radius: 8
                            border.color: cmradio.down ? "#17a81a" : "#21be2b"
                            Rectangle {
                                width: 9
                                height: 9
                                anchors.centerIn: cmbtn
                                radius: 5
                                color: cmradio.down ? "#17a81a" : "#21be2b"
                                visible: cmradio.checked
                                onVisibleChanged: if(cmradio.checked){
                                                      unitsMulti = 0.01
                                                  }
                            }
                        }
                    }
                }

            }

            ///////////////////////////////////////steps///////////////////////////////////////////////
            Rectangle{
                id:steps
                width: parent.width -2
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: units.bottom
                height: 30
                radius: 0
                color: "#303030"
                RowLayout{
                    spacing: 2
                    x:2
                    Text {
                        text: qsTr("Step:")
                        color: "white"
                    }
                    RadioButton{
                        id:aHund
                        text: "1000"
                        contentItem: Text {
                            text: aHund.text
                            color: "white"
//                            leftPadding: aHund.indicator.width + aHund.spacing
                            leftPadding: 10
                            verticalAlignment: Text.AlignVCenter
                        }
                        indicator: Rectangle {
                            id:aHundBtn
                            implicitWidth: 15
                            implicitHeight: 15
//                            x: aHund.leftPadding
                            x: 0
                            y: parent.height / 2 - height / 2
                            radius: 8
                            border.color: aHund.down ? "#17a81a" : "#21be2b"
                            Rectangle {
                                width: 9
                                height: 9
                                anchors.centerIn: aHundBtn
                                radius: 5
                                color: aHund.down ? "#17a81a" : "#21be2b"
                                visible: aHund.checked
                                onVisibleChanged: if(aHund.checked){
                                                      stepSize = 1000
                                                  }
                            }
                        }
                    }
                    RadioButton{
                        id:aOne
                        text: "100"
                        contentItem: Text {
                            text: aOne.text
                            color: "white"
//                            leftPadding: aOne.indicator.width + aOne.spacing
                            leftPadding: 10
                            verticalAlignment: Text.AlignVCenter
                        }
                        indicator: Rectangle {
                            id:aOneBtn
                            implicitWidth: 15
                            implicitHeight: 15
//                            x: aOne.leftPadding
                            x:0
                            y: parent.height / 2 - height / 2
                            radius: 8
                            border.color: aOne.down ? "#17a81a" : "#21be2b"
                            Rectangle {
                                width: 9
                                height: 9
                                anchors.centerIn: aOneBtn
                                radius: 5
                                color: aOne.down ? "#17a81a" : "#21be2b"
                                visible: aOne.checked
                                onVisibleChanged: if(aOne.checked){
                                                      stepSize = 100
                                                  }
                            }
                        }
                    }RadioButton{
                        id:adigit
                        text: "10"
                        contentItem: Text {
                            text: adigit.text
                            color: "white"
//                            leftPadding: adigit.indicator.width + adigit.spacing
                            leftPadding: 10
                            verticalAlignment: Text.AlignVCenter
                        }
                        indicator: Rectangle {
                            id:adigitBtn
                            implicitWidth: 15
                            implicitHeight: 15
//                            x: adigit.leftPadding
                            x:0
                            y: parent.height / 2 - height / 2
                            radius: 8
                            border.color: adigit.down ? "#17a81a" : "#21be2b"
                            Rectangle {
                                width: 9
                                height: 9
                                anchors.centerIn: adigitBtn
                                radius: 5
                                color: adigit.down ? "#17a81a" : "#21be2b"
                                visible: adigit.checked
                                onVisibleChanged: if(adigit.checked){
                                                      stepSize = 10
                                                  }
                            }
                        }
                    }RadioButton{
                        id:twodigit
                        text: "1"
                        checked: true
                        contentItem: Text {
                            text: twodigit.text
                            color: "white"
//                            leftPadding: twodigit.indicator.width + twodigit.spacing
                            leftPadding: 10
                            verticalAlignment: Text.AlignVCenter
                        }
                        indicator: Rectangle {
                            id:twodigitBtn
                            implicitWidth: 15
                            implicitHeight: 15
//                            x: twodigit.leftPadding
                            x:0
                            y: parent.height / 2 - height / 2
                            radius: 8
                            border.color: twodigit.down ? "#17a81a" : "#21be2b"
                            Rectangle {
                                width: 9
                                height: 9
                                anchors.centerIn: twodigitBtn
                                radius: 5
                                color: twodigit.down ? "#17a81a" : "#21be2b"
                                visible: twodigit.checked
                                onVisibleChanged: if(twodigit.checked){
                                                      stepSize = 1
                                                  }
                            }
                        }
                    }
                }

            }

            ScrollView {
                id: frame
                clip: true
                anchors.top: steps.bottom
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
                                        boxProperties.color= fillColor.toString()
                                    }

                                }
                            }
                            Rectangle{
                                Layout.fillWidth: true
                                width: parent.width/3
                                color: "#404040"
                                height: 35


                                Text {
                                    id: fillColorBox
                                    text: qsTr("Color :")
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
                                            if(boxProperties){
                                                opacityValue.value = opacityValueInput.text
                                                boxProperties.opacity = opacityValue.value
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
                                    id: opacityBox
                                    text: qsTr("Opacity:")
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
                                        spacing: 8
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
                                            width: 100
                                            color: "#c9c9c9"
                                            clip:  true
                                            radius: 5
                                            Layout.fillWidth: true
                                            TextInput {
                                                id: mlocationX
                                                padding: 2
                                                anchors.fill: parent
                                                text: boxProperties.location.x.toFixed(9)
                                                font.pointSize: 10
                                                selectByMouse: true
                                                selectionColor: "dark green"
                                                validator: DoubleValidator {
                                                    decimals: 13;
                                                    notation: DoubleValidator.StandardNotation
                                                    locale: "insert x"
                                                }
                                                onTextChanged: {
                                                    boxProperties.location.x = text

                                                }
                                            }
                                        }
                                    }
                                    Row{
                                        spacing: 8
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
                                            width: 100
                                            color: "#c9c9c9"
                                            clip:  true
                                            radius: 5

                                            TextInput {
                                                id: mlocationY
                                                padding: 2
                                                anchors.fill: parent
                                                font.pointSize: 10
                                                text: boxProperties.location.y.toFixed(9)
                                                selectByMouse: true
                                                selectionColor: "dark green"
                                                validator: DoubleValidator {
                                                    decimals: 13;
                                                    notation: DoubleValidator.StandardNotation
                                                    locale: "insert y"
                                                }
                                                onTextChanged: {
                                                    boxProperties.location.y = text
                                                }
                                            }
                                        }
                                    }
                                    Row{
                                        spacing: 8
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
                                            width: 100
                                            color: "#c9c9c9"
                                            clip:  true
                                            radius: 5

                                            TextInput {
                                                id: mlocationZ
                                                padding: 2
                                                anchors.fill: parent
                                                font.pointSize: 10
                                                text: boxProperties.location.z.toFixed(5)
                                                selectByMouse: true
                                                selectionColor: "dark green"
                                                validator: DoubleValidator {
                                                    decimals: 13;
                                                    notation: DoubleValidator.StandardNotation
                                                    locale: "insert z"
                                                }
                                                onTextChanged: {
                                                    boxProperties.location.z = text
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
                                    id: locationBox
                                    text: qsTr("Location :")
                                    font.pointSize: 10
                                    color: "white"
                                    y: 10
                                    x:7
                                }
                                CheckBox {
                                    id: relative
                                    text: qsTr("Relative")
                                    font.pointSize: 10
                                    checked: flase
                                    anchors.bottom: locationTitle.bottom
                                    onCheckStateChanged: if(checked === true){
                                                             boxProperties.relative = true
                                                         }
                                                         else{
                                                             boxProperties.relative = false
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

                            ///////////////////////////////////length/////////////////////////////////////
                            Rectangle{
                                id: lengthContainer
                                Layout.fillWidth: true
                                color: "#404040"
                                height: 30
                                //                                border.color: "#5f5f5f"
                                //                                border.width: 1

                                SpinBox {
                                    id: lengthValue
                                    stepSize: stepSize
                                    value: 40000.00
                                    to : 10000000
                                    from : 0
                                    validator: DoubleValidator {
                                            bottom: Math.min(spinbox.from, spinbox.to)
                                            top:  Math.max(spinbox.from, spinbox.to)
                                            decimals: 2
                                        }
                                    editable: true
                                    anchors.centerIn: parent
                                    height: 20

                                    contentItem: TextInput {
                                        id: lengthInput
                                        z: 2
                                        //                                        text: transValue.textFromValue(transValue.value, transValue.locale)
                                        text: lengthValue.value
                                        font: lengthValue.font

                                        color: "#404040"
                                        horizontalAlignment: Qt.AlignHCenter
                                        verticalAlignment: Qt.AlignVCenter +10
                                        readOnly: !lengthValue.editable
                                        validator: lengthValue.validator
                                        inputMethodHints: Qt.ImhFormattedNumbersOnly
                                        topPadding: 13
                                        selectByMouse: true
                                        selectionColor: "dark green"
                                        onTextChanged: {
                                            if(boxProperties && lengthValue && (lengthValue.value == 0 || lengthValue.value)){
                                                lengthValue.value = lengthInput.text
                                                boxProperties.length = lengthValue.value*unitsMulti
                                            }
                                        }
                                    }
                                    up.indicator: Rectangle {
                                        x: lengthValue.mirrored ? 0 : parent.width - width
                                        height: parent.height
                                        implicitWidth: 20
                                        implicitHeight: 20
                                        color: lengthValue.up.pressed ? "#5f5f5f" : "#404040"
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
                                        x: lengthValue.mirrored ? parent.width - width : 0
                                        height: parent.height
                                        implicitWidth: 20
                                        implicitHeight: 20
                                        color: lengthValue.down.pressed ? "#5f5f5f" : "#404040"
                                        border.color: enabled ? "#404040" : "#5f5f5f"

                                        Text {
                                            text: "-"
                                            font.pixelSize: lengthValue.font.pixelSize * 2
                                            color: "white"
                                            anchors.fill: parent
                                            fontSizeMode: Text.Fit
                                            horizontalAlignment: Text.AlignHCenter
                                            verticalAlignment: Text.AlignVCenter
                                        }
                                    }
                                    background: Rectangle {
                                        implicitWidth: lengthContainer.width -10
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
                                    text: qsTr("Length:")
                                    font.pointSize: 10
                                    color: "white"
                                    anchors.verticalCenter:  parent.verticalCenter
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
                                    stepSize: stepSize
                                    value: 20000
                                    to : 10000000
                                    from : 0
                                    validator: DoubleValidator {
                                        bottom: 0
                                        top:  100
                                    }
                                    editable: true
                                    anchors.centerIn: parent
                                    height: 20

                                    contentItem: TextInput {
                                        id: widthInput
                                        z: 2
                                        //                                        text: transValue.textFromValue(transValue.value, transValue.locale)
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
                                            if(boxProperties && widthValue && (widthValue.value == 0 || widthValue.value)){
                                                widthValue.value = widthInput.text
                                                boxProperties.width = widthValue.value*unitsMulti
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
                                            font.pixelSize: transValue.font.pixelSize * 2
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
                                    text: qsTr("Width:")
                                    font.pointSize: 10
                                    color: "white"
                                    anchors.verticalCenter:  parent.verticalCenter
                                    x:7
                                }
                            }


                            ///////////////////////////////////height/////////////////////////////////////
                            Rectangle{
                                id: heightContainer
                                Layout.fillWidth: true
                                color: "#404040"
                                height: 30
                                //                                border.color: "#5f5f5f"
                                //                                border.width: 1

                                SpinBox {
                                    id: heightValue
                                    stepSize: stepSize
                                    value: 20000
                                    to : 10000000
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
                                        text: heightValue.value
                                        font: heightValue.font
                                        color: "#404040"
                                        horizontalAlignment: Qt.AlignHCenter
                                        verticalAlignment: Qt.AlignVCenter +10
                                        readOnly: !heightValue.editable
                                        validator: heightValue.validator
                                        inputMethodHints: Qt.ImhFormattedNumbersOnly
                                        topPadding: 13
                                        selectByMouse: true
                                        selectionColor: "dark green"
                                        onTextChanged: {
                                            if(boxProperties && heightValue && (heightValue.value == 0 || heightValue.value)){
                                                heightValue.value = heightInput.text
                                                boxProperties.height = heightValue.value*unitsMulti
                                            }
                                        }
                                    }
                                    up.indicator: Rectangle {
                                        x: heightValue.mirrored ? 0 : parent.width - width
                                        height: parent.height
                                        implicitWidth: 20
                                        implicitHeight: 20
                                        color: heightValue.up.pressed ? "#5f5f5f" : "#404040"
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
                                        x: heightValue.mirrored ? parent.width - width : 0
                                        height: parent.height
                                        implicitWidth: 20
                                        implicitHeight: 20
                                        color: heightValue.down.pressed ? "#5f5f5f" : "#404040"
                                        border.color: enabled ? "#404040" : "#5f5f5f"

                                        Text {
                                            text: "-"
                                            font.pixelSize: heightValue.font.pixelSize * 2
                                            color: "white"
                                            anchors.fill: parent
                                            fontSizeMode: Text.Fit
                                            horizontalAlignment: Text.AlignHCenter
                                            verticalAlignment: Text.AlignVCenter
                                        }
                                    }
                                    background: Rectangle {
                                        implicitWidth: heightContainer.width -10
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
                                    text: qsTr("Height:")
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

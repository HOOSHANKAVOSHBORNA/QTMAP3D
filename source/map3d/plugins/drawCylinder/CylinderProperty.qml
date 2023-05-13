import QtQuick 2.13
import QtQuick.Window 2.12
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.2
import Crystal 1.0

Item {
    id: rootItem
    implicitHeight: parent.height



    property CylinderProperties cylinderProperties
    property string headerTitleSTR: "cylinder Properties"
    property string fillColor: "#91001d"
    property string lineColor: "#ffffff"
    property double unitsMulti
    property double stepSize


    onVisibleChanged: {
        cylinderProperties.fillcolor = fillColor
        cylinderProperties.transparency = opacityValue.value
        cylinderProperties.radius = radiusValue.value
        cylinderProperties.height = heightValue.value
    }

    onUnitsMultiChanged: {
        cylinderProperties.height = heightValue.value*unitsMulti
        cylinderProperties.radius = radiusValue.value*unitsMulti
    }

    onStepSizeChanged: {
        heightValue.stepSize = stepSize
        radiusValue.stepSize = stepSize
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
                    anchors.centerIn: parent
                    Text {
                        text: qsTr("Unit:")
                        color: "white"
                    }
                    ComboBox {
                        id: unitControl
                        currentIndex: 1
                        model: ["KM", "M", "CM"]
                        onCurrentIndexChanged:   {
                            if(currentIndex === 0){
                                unitsMulti = 1000
                            }else if(currentIndex === 1){
                                unitsMulti = 1
                            }else if(currentIndex === 2){
                                unitsMulti = 0.01
                            }
                        }
                        delegate: ItemDelegate {
                            width: unitControl.width
                            contentItem: Text {
                                text: unitControl.textRole
                                      ? (Array.isArray(unitControl.model) ? modelData[unitControl.textRole] : model[unitControl.textRole])
                                      : modelData
                                color: "#5f5f5f"
                                font: unitControl.font
                                elide: Text.ElideRight
                                verticalAlignment: Text.AlignVCenter
                            }
                            highlighted: unitControl.highlightedIndex === index
                        }
                        indicator: Canvas {
                            id: unitcanvas
                            x: unitControl.width - width - unitControl.rightPadding
                            y: unitControl.topPadding + (unitControl.availableHeight - height) / 2
                            width: 12
                            height: 8
                            contextType: "2d"
                            Connections {
                                target: unitControl
                                function onPressedChanged() { unitcanvas.requestPaint(); }
                            }
                        }
                        contentItem: Text {
                            leftPadding: 5
                            rightPadding: unitControl.indicator.width + unitControl.spacing
                            text: unitControl.displayText
                            font: unitControl.font
                            color: unitControl.pressed ? "#5f5f5f" : "#404040"
                            verticalAlignment: Text.AlignVCenter
                            elide: Text.ElideRight
                        }
                        background: Rectangle {
                            implicitWidth: 100
                            implicitHeight: 22
                            border.color: unitControl.pressed ? "#5f5f5f" : "#404040"
                            border.width: unitControl.visualFocus ? 2 : 1
                            radius: 5
                            color: "#c9c9c9"
                        }
                        popup: Popup {
                            y: unitControl.height - 1
                            width: unitControl.width
                            implicitHeight: contentItem.implicitHeight
                            padding: 1

                            contentItem: ListView {
                                clip: true
                                implicitHeight: contentHeight
                                model: unitControl.popup.visible ? unitControl.delegateModel : null
                                currentIndex: unitControl.highlightedIndex

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
                    anchors.centerIn: parent
                    Text {
                        text: qsTr("Step:")
                        color: "white"
                    }
                    ComboBox {
                        id: controls
                        currentIndex: 2
                        model: ["1000", "100", "10","1"]
                        onCurrentIndexChanged:   {
                            if(currentIndex === 0){
                                stepSize = 1000
                            }else if(currentIndex === 1){
                                stepSize = 100
                            }else if(currentIndex === 2){
                                stepSize = 10
                            }else if(currentIndex === 3){
                                stepSize = 1
                            }
                        }
                        delegate: ItemDelegate {
                            width: controls.width
                            contentItem: Text {
                                text: controls.textRole
                                      ? (Array.isArray(controls.model) ? modelData[controls.textRole] : model[controls.textRole])
                                      : modelData
                                color: "#5f5f5f"
                                font: controls.font
                                elide: Text.ElideRight
                                verticalAlignment: Text.AlignVCenter
                            }
                            highlighted: controls.highlightedIndex === index
                        }
                        indicator: Canvas {
                            id: canvass
                            x: controls.width - width - controls.rightPadding
                            y: controls.topPadding + (controls.availableHeight - height) / 2
                            width: 12
                            height: 8
                            contextType: "2d"
                            Connections {
                                target: controls
                                function onPressedChanged() { canvass.requestPaint(); }
                            }
                        }
                        contentItem: Text {
                            leftPadding: 5
                            rightPadding: controls.indicator.width + controls.spacing
                            text: controls.displayText
                            font: controls.font
                            color: controls.pressed ? "#5f5f5f" : "#404040"
                            verticalAlignment: Text.AlignVCenter
                            elide: Text.ElideRight
                        }
                        background: Rectangle {
                            implicitWidth: 100
                            implicitHeight: 22
                            border.color: controls.pressed ? "#5f5f5f" : "#404040"
                            border.width: controls.visualFocus ? 2 : 1
                            radius: 5
                            color: "#c9c9c9"
                        }
                        popup: Popup {
                            y: controls.height - 1
                            width: controls.width
                            implicitHeight: contentItem.implicitHeight
                            padding: 1

                            contentItem: ListView {
                                clip: true
                                implicitHeight: contentHeight
                                model: controls.popup.visible ? controls.delegateModel : null
                                currentIndex: controls.highlightedIndex

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

            ScrollView {
                id: frame
                clip: true
                anchors.top: steps.bottom
                padding: 5
                width: parent.width
                height: parent.height - header.height -55
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
                                        cylinderProperties.fillcolor= fillColor.toString()
                                    }

                                }
                            }
                            Rectangle{
                                Layout.fillWidth: true
                                width: parent.width/3
                                color: "#404040"
                                height: 35


                                Text {
                                    id: fillColorcylinder
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
                                            if(cylinderProperties){
                                                opacityValue.value = opacityValueInput.text
                                                cylinderProperties.transparency = opacityValue.value
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
                                    id: opacitycylinder
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
                                                text: cylinderProperties.location.x.toFixed(9)
                                                font.pointSize: 10
                                                selectByMouse: true
                                                selectionColor: "dark green"
                                                validator: DoubleValidator {
                                                    decimals: 13;
                                                    notation: DoubleValidator.StandardNotation
                                                    locale: "insert x"
                                                }
                                                onTextChanged: {
                                                    cylinderProperties.location.x = text
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
                                                text: cylinderProperties.location.y.toFixed(9)
                                                selectByMouse: true
                                                selectionColor: "dark green"
                                                validator: DoubleValidator {
                                                    decimals: 13;
                                                    notation: DoubleValidator.StandardNotation
                                                    locale: "insert y"
                                                }
                                                onTextChanged: {
                                                    cylinderProperties.location.y = text
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
                                                text: cylinderProperties.location.z.toFixed(5)
                                                selectByMouse: true
                                                selectionColor: "dark green"
                                                validator: DoubleValidator {
                                                    decimals: 13;
                                                    notation: DoubleValidator.StandardNotation
                                                    locale: "insert z"
                                                }
                                                onTextChanged: {
                                                    cylinderProperties.location.z = text
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
                                    id: locationcylinder
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
                                                             cylinderProperties.relative = true
                                                         }
                                                         else{
                                                             cylinderProperties.relative = false
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
                                        rightPadding: -10
                                        selectByMouse: true
                                        selectionColor: "dark green"
                                        onTextChanged: {
                                            if(cylinderProperties){
                                                radiusValue.value = valueInput.text
                                                cylinderProperties.radius = radiusValue.value*unitsMulti
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
                                    id: radiuscylinder
                                    text: qsTr("Radius:")
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
                                    value: 10000
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
                                            if(cylinderProperties && heightValue && (heightValue.value == 0 || heightValue.value)){
                                                heightValue.value = heightInput.text
                                                cylinderProperties.height = heightValue.value*unitsMulti
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

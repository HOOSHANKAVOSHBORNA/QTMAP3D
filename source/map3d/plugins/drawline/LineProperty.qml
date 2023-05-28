import QtQuick 2.13
import QtQuick.Window 2.12
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.2
import Crystal 1.0

Item {
    id: rootItem
    implicitHeight: parent ? parent.height : 0



    property LineProperties lineProperties
    property int rulerCondition : lineProperties ? lineProperties.ruler : 0
    property string headerTitleSTR: "Line Properties"
    property string lColor: "#000000"
    property string pColor: "#001191"



    Item {
        id: dialog
        width: 250
        height: rootItem.height


        //////////////////////////Main Content////////////////////////
        /*contentItem:*/  Rectangle {
            id: item
            visible: true
            width: 240
            height: rootItem.height
            //            anchors.horizontalCenter: parent.horizontalCenter
            anchors.fill: parent
            color: "#404040"
            opacity: 0.9
            anchors.margins: 6
            //            y:43
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
                    //                    text: qsTr("Line Properties")
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
                    contentHeight: 450
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

                            ////////////////////////////////////line Color Property//////////////////////////////////
                            Rectangle{
                                Layout.fillWidth: true
                                color: "#404040"
                                height: 35
                                //                                border.color: "#5f5f5f"
                                //                                border.width: 1


                                Rectangle{
                                    height: 20
                                    width: 20
                                    color: lColor
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
                                        text: lColor
                                        anchors.centerIn: parent
                                        font.pointSize: 10

                                    }
                                }
                                ColorDialog {
                                    visible: false
                                    id:  lineColorDialog
                                    title: "Please choose a color"
                                    onAccepted: {
                                        lColor = lineColorDialog.color
                                        lineProperties.color = lColor.toString()
                                    }

                                }
                            }
                            Rectangle{
                                Layout.fillWidth: true
                                color: "#404040"
                                height: 35
                                //                                border.color: "#5f5f5f"
                                //                                border.width: 1

                                Text {
                                    id: lineColorCircle
                                    text: qsTr("Color :")
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

                                QSpinBox {
                                    id: lineopacityValue
                                    value: 100
                                    to : 100
                                    from : 0
                                    anchors.fill: parent
                                    onValueChanged: {
                                        if(lineProperties){
                                            lineProperties.lineOpacity = value
                                        }
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
                                    id: lineopacityCircle
                                    text: qsTr("Opacity:")
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

                                QSpinBox {
                                    id: widthValue
                                    value: 10
                                    to : 10000
                                    from : 0
                                    anchors.fill: parent
                                    onValueChanged: {
                                        if(lineProperties){
                                            lineProperties.width = value
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
                                    id: widthSphere
                                    text: qsTr("Width:")
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

                                QSpinBox {
                                    id: transValue
                                    value: 0
                                    to : 1000000
                                    from : 0
                                    anchors.fill: parent
                                    onValueChanged: {
                                        if(lineProperties && transValue && (transValue.value == 0 || transValue.value)){
                                            lineProperties.height = value
                                        }
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

                                QSpinBox {
                                    id: tesselationValue
                                    value: 10
                                    to : 100000
                                    from : 0
                                    anchors.fill: parent
                                    onValueChanged: {
                                        if(lineProperties){
                                            lineProperties.tesselation = value
                                        }
                                    }
                                }
                            }
                            Rectangle{
                                id:tesselationContainerTitle
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
                                id:clampContainerTitle
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
                                        //                                        lineProperties.showLen = len.checked
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


                            Rectangle{
                                id: beaContainer
                                Layout.fillWidth: true
                                color: "#404040"
                                height: 30
                                //                                border.color: "#5f5f5f"
                                //                                border.width: 1

                                Switch {
                                    id: bea
                                    anchors.centerIn: parent
                                    checked: false

                                    ToolTip {
                                        parent: bea
                                        y: bea.y + bea.height
                                        Text{
                                            text: bea.checked ? "Click to deactive" : "Click to active"
                                            color: "white"
                                        }

                                        background: Rectangle {
                                            color: "#404040"
                                            radius: 4
                                        }

                                        visible:  bea.hovered
                                    }
                                    onToggled: function() {
                                        lineProperties.bearing = bea.checked
                                    }

                                    Layout.alignment: Qt.AlignRight
                                    indicator: Rectangle {
                                        implicitWidth: 70
                                        implicitHeight: 20
                                        x: bea.leftPadding
                                        y: parent.height / 2 - height / 2
                                        radius: 13
                                        color: bea.checked ? "#3f9173" : "#383838"
                                        border.color: bea.checked ? "#17a81a" : "#a8171a"

                                        Rectangle {
                                            x: bea.checked ? parent.width - width : 0
                                            width: 20
                                            height: 20
                                            radius: 13
                                            color: bea.down ? "#cccccc" : "#ffffff"
                                            border.color: bea.checked ? (bea.down ? "#17a81a" : "#21be2b") : "#999999"
                                        }
                                    }
                                }
                            }
                            Rectangle{
                                id:beaContainerTitle
                                Layout.fillWidth: true
                                color: "#404040"
                                height: 30
                                //                                border.color: "#5f5f5f"
                                //                                border.width: 1

                                Text {
                                    text: qsTr("Bearing :")
                                    font.pointSize: 10
                                    color: "white"
                                    anchors.verticalCenter:  parent.verticalCenter
                                    x:7
                                }
                            }


                            /////////////////////////////////////////////////////showslop//////////////////////////////////////////////////
                            Rectangle{
                                id: slopeContainer
                                Layout.fillWidth: true
                                color: "#404040"
                                height: 30
                                //                                border.color: "#5f5f5f"
                                //                                border.width: 1

                                Switch {
                                    id: slope
                                    anchors.centerIn: parent
                                    checked: false

                                    ToolTip {
                                        parent: slope
                                        y: slope.y + slope.height
                                        Text{
                                            text: slope.checked ? "Click to deactive" : "Click to active"
                                            color: "white"
                                        }

                                        background: Rectangle {
                                            color: "#404040"
                                            radius: 4
                                        }

                                        visible:  slope.hovered
                                    }
                                    onToggled: function() {
                                        lineProperties.showSlope = slope.checked
                                    }

                                    Layout.alignment: Qt.AlignRight
                                    indicator: Rectangle {
                                        implicitWidth: 70
                                        implicitHeight: 20
                                        x: slope.leftPadding
                                        y: parent.height / 2 - height / 2
                                        radius: 13
                                        color: slope.checked ? "#3f9173" : "#383838"
                                        border.color: slope.checked ? "#17a81a" : "#a8171a"

                                        Rectangle {
                                            x: slope.checked ? parent.width - width : 0
                                            width: 20
                                            height: 20
                                            radius: 13
                                            color: slope.down ? "#cccccc" : "#ffffff"
                                            border.color: slope.checked ? (slope.down ? "#17a81a" : "#21be2b") : "#999999"
                                        }
                                    }
                                }
                            }



                            Rectangle{
                                id:slopeContainerTitle
                                Layout.fillWidth: true
                                color: "#404040"
                                height: 30
                                //                                border.color: "#5f5f5f"
                                //                                border.width: 1

                                Text {
                                    text: qsTr("Show Slope:")
                                    font.pointSize: 10
                                    color: "white"
                                    anchors.verticalCenter:  parent.verticalCenter
                                    x:7
                                }
                            }
                        }

                        GroupBox{
                            id: pointGroup
                            width: parent.width
                            anchors.topMargin: 20
                            anchors.top: props.bottom
                            label: Rectangle{
                                id: labelContainer
                                color: "#404040"
                                opacity: 1
                                //                                border.color: "white"
                                //                                border.width: 1
                                //                                radius: 0
                                x: 10
                                height: 25
                                width: 125
                                y: -13
                                Text {
                                    id: pointLale
                                    text: qsTr("points:")
                                    color: "white"
                                    x:10

                                }
                                Rectangle{
                                    id: pointContainer
                                    x: 90
                                    anchors.verticalCenter: parent.verticalCenter
                                    color: "#404040"
                                    height: 30
                                    //                                border.color: "#5f5f5f"
                                    //                                border.width: 1

                                    Switch {
                                        id: visibleSwitch
                                        anchors.centerIn: parent
                                        checked: false
                                        onCheckedChanged:      if(visibleSwitch.checked === false){
                                                                   smoothContainer.opacity = 0.2;
                                                                   smooth.enabled = false
                                                                   pointwidthValue.enabled = false;
                                                                   pointwidthContainer.opacity = 0.2
                                                                   pointcolorbtn.enabled = false
                                                                   pointColorSecR.opacity = 0.2
                                                               }else{
                                                                   smooth.enabled = true;
                                                                   smoothContainer.opacity = 1
                                                                   pointwidthValue.enabled = true;
                                                                   pointwidthContainer.opacity = 1
                                                                   pointcolorbtn.enabled = true
                                                                   pointColorSecR.opacity = 1;

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
                                            implicitWidth: 50
                                            implicitHeight: 15
                                            x: visibleSwitch.leftPadding
                                            y: parent.height / 2 - height / 2
                                            radius: 13
                                            color: visibleSwitch.checked ? "#3f9173" : "#383838"
                                            border.color: visibleSwitch.checked ? "#17a81a" : "#a8171a"

                                            Rectangle {
                                                x: visibleSwitch.checked ? parent.width - width : 0
                                                width: 15
                                                height: 15
                                                radius: 13
                                                color: visibleSwitch.down ? "#cccccc" : "#ffffff"
                                                border.color: visibleSwitch.checked ? (visibleSwitch.down ? "#17a81a" : "#21be2b") : "#999999"
                                            }
                                        }
                                    }
                                }
                            }


                            GridLayout{
                                id: pointprops
                                width: parent.width
                                anchors.horizontalCenter: parent.horizontalCenter
                                anchors.topMargin: 20
                                columnSpacing: 0
                                rowSpacing: 1
                                columns:2
                                rows: 6
                                layoutDirection: Qt.RightToLeft

                                ///////////////////////////////////point Color Property//////////////////////////////////
                                Rectangle{
                                    id: pointColorSecR
                                    Layout.fillWidth: true
                                    color: "#404040"
                                    height: 35
                                    opacity : 0.2;
                                    //                                border.color: "#5f5f5f"
                                    //                                border.width: 1


                                    Rectangle{
                                        height: 20
                                        width: 20
                                        color: pColor
                                        //                                    border.width: 2
                                        //                                    border.color: "#c9c9c9"
                                        x: 5
                                        radius: 5
                                        anchors.verticalCenter: parent.verticalCenter

                                        MouseArea{
                                            id:pointcolorbtn
                                            enabled: false
                                            anchors.fill: parent
                                            cursorShape: Qt.PointingHandCursor
                                            onClicked: pointColorDialog.visible = true

                                        }
                                    }
                                    Rectangle{
                                        height: 20
                                        width: 65
                                        color: "#c9c9c9"
                                        x: 30
                                        anchors.verticalCenter: parent.verticalCenter
                                        radius: 5

                                        Text {
                                            id: pointColorField
                                            text: pColor
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
                                            pColor = pointColorDialog.color
                                            lineProperties.pointColor = pColor
                                        }

                                    }
                                }
                                Rectangle{
                                    id: pointColorSecL
                                    Layout.fillWidth: true
                                    color: "#404040"
                                    height: 35
                                    //                                border.color: "#5f5f5f"
                                    //                                border.width: 1

                                    Text {
                                        id: pointColor
                                        text: qsTr("point Color:")
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
                                    opacity : 0.2;
                                    //                                border.color: "#5f5f5f"
                                    //                                border.width: 1

                                    QSpinBox {
                                        id: pointwidthValue
                                        enabled: false
                                        value: 10
                                        to : 10000
                                        from : 0
                                        anchors.fill: parent
                                        onValueChanged: {
                                            if(lineProperties){
                                                lineProperties.pointwidth = value
                                            }
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




                                /////////////////////////////////////////////////////smooth//////////////////////////////////////////////////
                                Rectangle{
                                    id: smoothContainer
                                    Layout.fillWidth: true
                                    color: "#404040"
                                    height: 30
                                    opacity : 0.2;

                                    //                                border.color: "#5f5f5f"
                                    //                                border.width: 1

                                    Switch {
                                        id: smooth
                                        anchors.centerIn: parent
                                        checked: true
                                        enabled: false


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
                                        text: qsTr("Point Smooth :")
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

    onRulerConditionChanged:   {
        if(rulerCondition === 1){
            pointGroup.visible = false;
            smoothContainer.visible = false
            smoothContainerTitle.visible = false
            lenContainer.visible = false
            lenContainerTitle.visible = false
            beaContainer.visible = false
            beaContainerTitle.visible = false
            slopeContainer.visible = false
            slopeContainerTitle.visible = false
            transContainer.visible = false
            heightContainer.visible = false
            clampContainer.visible = false
            clampContainerTitle.visible = false
            tesselationContainer.visible = false
            tesselationContainerTitle.visible = false
            lineProperties.showLen = true
            lineProperties.bearing = false
            lineProperties.visible = false
            headerTitleSTR = "Ruler Properties"

        }
        else if(rulerCondition === 0){
            pointGroup.visible = true;
            smoothContainer.visible = true
            smoothContainerTitle.visible = true
            lenContainer.visible = true
            lenContainerTitle.visible = true
            transContainer.visible = true
            heightContainer.visible = true
            clampContainer.visible = true
            clampContainerTitle.visible = true
            tesselationContainer.visible = true
            tesselationContainerTitle.visible = true
            lineProperties.showLen = false
            lineProperties.bearing = false
            lineProperties.visible = false
            lineProperties.showSlope = false
            headerTitleSTR = "Line Properties"
        }
        else if(rulerCondition === 2){
            pointGroup.visible = false
            smoothContainer.visible = false
            smoothContainerTitle.visible = false
            lenContainer.visible = false
            lenContainerTitle.visible = false
            transContainer.visible = false
            heightContainer.visible = false
            clampContainer.visible = false
            clampContainerTitle.visible = false
            tesselationContainer.visible = false
            tesselationContainerTitle.visible = false
            slopeContainer.visible = false
            slopeContainerTitle.visible = false
            beaContainer.visible = false
            beaContainerTitle.visible = false
            lineProperties.showLen = false
            lineProperties.visible = false
            headerTitleSTR = "Height Measure"
        }

        else if(rulerCondition === 3){
            pointGroup.visible = false;
            smoothContainer.visible = false
            smoothContainerTitle.visible = false
            lenContainer.visible = false
            lenContainerTitle.visible = false
            beaContainer.visible = false
            beaContainerTitle.visible = false
            slopeContainer.visible = false
            slopeContainerTitle.visible = false
            transContainer.visible = false
            heightContainer.visible = false
            clampContainer.visible = false
            clampContainerTitle.visible = false
            tesselationContainer.visible = false
            tesselationContainerTitle.visible = false
            lineProperties.showLen = false
            lineProperties.showSlope = true
            lineProperties.bearing = false
            lineProperties.visible = false
            headerTitleSTR = "Slope Properties"

        }
    }
}

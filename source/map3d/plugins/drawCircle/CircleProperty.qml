import QtQuick 2.13
import QtQuick.Window 2.12
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.12
import QtQuick.Dialogs
import Crystal 1.0

Item {
    id: rootItem
    implicitHeight: parent ? parent.height : 0



    property CircleProperties circleProperties
    property string headerTitleSTR: "circle Properties"
    property string fillColor: "#91001d"
    property string lineColor: "#ffffff"



    onVisibleChanged: {
        circleProperties.fillcolor = fillColor
        circleProperties.transparency = opacityValue.value
        circleProperties.linecolor = lineColor;
        circleProperties.lineopacity = lineopacityValue.value
        circleProperties.radius = radiusValue.value
        circleProperties.circleheight = transValue.value;
        circleProperties.lineWidth = lineValue.value;
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

                            ////////////////////////////////////fill Color Property//////////////////////////////////
                            Rectangle{
                                Layout.fillWidth: true
                                width: parent.width/2
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
                                        fillColor = fillColorDialog.selectedColor
                                        circleProperties.fillcolor= fillColor.toString()
                                    }

                                }
                            }
                            Rectangle{
                                Layout.fillWidth: true
                                width: parent.width/2.7
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
                            ///////////////////////////////////opacity/////////////////////////////////////

                            Rectangle{
                                id: opacityContainer
                                Layout.fillWidth: true
                                color: "#404040"
                                height: 30
                                //                                border.color: "#5f5f5f"
                                //                                border.width: 1

                                QSpinBox {
                                    id: opacityValue
                                    value: 50
                                    to : 100
                                    from : 0
                                    anchors.fill: parent


                                    onValueChanged: {
                                        if(circleProperties){
                                            circleProperties.transparency = value
                                        }
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
                                        lineColor = lineColorDialog.selectedColor
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
                                    text: qsTr("Strk Color:")
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
                                    value: 50
                                    to : 100
                                    from : 0
                                    anchors.fill: parent



                                    onValueChanged: {
                                        if(circleProperties){
                                            circleProperties.lineopacity = value
                                        }
                                    }
                                }
                            }
                            Rectangle{
                                id: lineopacityTitle
                                Layout.fillWidth: true
                                color: "#404040"
                                height: 30
                                Text {
                                    id: lineopacityCircle
                                    text: qsTr("Strk Opacity:")
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

                                QSpinBox {
                                    id: lineValue
                                    value: 5
                                    to : 100000
                                    from : 0
                                    anchors.fill: parent
                                    onValueChanged: {
                                        if(circleProperties && lineValue && (lineValue.value == 0 || lineValue.value)){
                                            circleProperties.lineWidth = value/10
                                        }
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


                            ///////////////////////////////////Location Property///////////////////////////////
                            Rectangle{
                                id:locationContainer
                                Layout.fillWidth: true
                                color: "#404040"
                                height: 100
                                border.color: "#5f5f5f"
                                border.width: 1
                                z:9

                                Column{
                                    Row{
                                        spacing: 8
                                        //                                        leftPadding: 5
                                        topPadding: 2
                                        z:9
                                        anchors.horizontalCenter: parent.horizontalCenter

                                        //                                        Text {
                                        //                                            color: "#e5e5e5"
                                        //                                            text: qsTr("X:")
                                        //                                            font.pointSize: 10
                                        //                                        }
                                        Rectangle{
                                            height: 30
                                            width: locationContainer.width
                                            color: "transparent"
                                            //                                            clip:  true
                                            radius: 5
                                            Layout.fillWidth: true
                                            QSpinBox {
                                                id: mlocationX
                                                anchors.fill: parent
                                                decimals: 4
                                                //                                                step: 0.01
                                                //                                                value: xLoc


                                                showText:  circleProperties ? circleProperties.location.x  : 0
                                                onValueChanged: {
                                                    circleProperties.location.x = value
                                                }
                                            }
                                        }
                                    }
                                    Row{
                                        spacing: 8
                                        //                                        leftPadding: 5
                                        //                                        topPadding: 5
                                        z:8
                                        anchors.horizontalCenter: parent.horizontalCenter

                                        //                                        Text {
                                        //                                            color: "#e5e5e5"
                                        //                                            text: qsTr("Y:")
                                        //                                            font.pointSize: 10
                                        //                                        }
                                        Rectangle{
                                            height: 30
                                            width: locationContainer.width
                                            color: "transparent"
                                            //                                            clip:  true
                                            radius: 5

                                            QSpinBox {
                                                id: mlocationY
                                                anchors.fill: parent
                                                decimals: 4
                                                //                                                step: 0.01
                                                //                                                value: yLoc
                                                showText : circleProperties ? circleProperties.location.y  : 0
                                                onValueChanged: {
                                                    circleProperties.location.y = value
                                                }
                                            }
                                        }
                                    }
                                    Row{
                                        spacing: 8
                                        //                                        leftPadding: 5
                                        //                                        topPadding: 5
                                        z:7
                                        anchors.horizontalCenter: parent.horizontalCenter

                                        //                                        Text {
                                        //                                            color: "#e5e5e5"
                                        //                                            text: qsTr("Y:")
                                        //                                            font.pointSize: 10
                                        //                                        }
                                        Rectangle{
                                            height: 30
                                            width: locationContainer.width
                                            color: "transparent"
                                            //                                            clip:  true
                                            radius: 5

                                            QSpinBox {
                                                id: mlocationZ
                                                anchors.fill: parent
                                                decimals: 4
                                                //                                                step: 10
                                                showText : circleProperties ? circleProperties.location.z  : 0
                                                onValueChanged: {
                                                    circleProperties.location.z = value
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
                                height: 100
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
                                    checked: false
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
                                        spacing: 8
                                        leftPadding: 5
                                        topPadding: 5
                                        anchors.horizontalCenter: parent.horizontalCenter

                                        Text {
                                            color: "#e5e5e5"
                                            text: qsTr("A:")
                                            font.pointSize: 10
                                        }
                                        Rectangle{
                                            height: 20
                                            width: 100
                                            color: "transparent"
                                            clip:  true
                                            radius: 5

                                            QSpinBox {
                                                id: startArc
                                                anchors.fill: parent
                                                value: 0
                                                from: 0
                                                to: 360
                                                onValueChanged: {
                                                    circleProperties.arcstart = value
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
                                            text: qsTr("B:")
                                            font.pointSize: 10
                                        }
                                        Rectangle{
                                            height: 20
                                            width: 100
                                            color: "transparent"
                                            clip:  true
                                            radius: 5

                                            QSpinBox{
                                                id: endArc
                                                anchors.fill: parent
                                                value: 360
                                                from: 0
                                                to: 360
                                                onValueChanged: {
                                                    circleProperties.arcend = value
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

                                QSpinBox {
                                    id: radiusValue
                                    value: 20000
                                    to : 10000000
                                    from : 0
                                    anchors.fill: parent
                                    onValueChanged: {
                                        if(circleProperties){
                                            circleProperties.radius = value
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

                                QSpinBox {
                                    id: transValue
                                    value: 0
                                    to : 1000000
                                    from : 0
                                    anchors.fill: parent
                                    onValueChanged: {
                                        if(circleProperties && transValue && (transValue.value == 0 || transValue.value)){
                                            circleProperties.circleheight = value
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

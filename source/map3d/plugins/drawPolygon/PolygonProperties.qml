import QtQuick 2.13
import QtQuick.Window 2.12
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.12
import QtQuick.Dialogs
import Crystal 1.0

Item {
    id: rootItem
    implicitHeight: parent ? parent.height : 0



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
                                        fillColor = fillColorDialog.selectedColor
                                        polygonProperties.fillcolor = fillColor.toString()
                                    }

                                }
                            }
                            Rectangle{
                                Layout.fillWidth: true
                                width: parent.width/2.3
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

                                QSpinBox {
                                    id: opacityValue
                                    value: 50
                                    to : 100
                                    from : 0
                                    anchors.fill: parent
                                        onValueChanged: {
                                            if(polygonProperties){
                                                polygonProperties.transparency = value
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
                                        lineColor = lineColorDialog.selectedColor
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

                                QSpinBox {
                                    id: lineopacityValue
                                    value: 50
                                    to : 100
                                    from : 0
                                    anchors.fill: parent
                                        onValueChanged: {
                                            if(polygonProperties){
                                                polygonProperties.lineopacity = value
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

                                QSpinBox {
                                    id: lineValue
                                    value: 0
                                    to : 100000
                                    from : 0
                                    anchors.fill: parent
                                        onValueChanged: {
                                            if(polygonProperties && lineValue && (lineValue.value == 0 || lineValue.value)){
                                                polygonProperties.lineWidth = value
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
                                            if(polygonProperties && transValue && (transValue.value == 0 || transValue.value)){
                                                polygonProperties.height = value
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

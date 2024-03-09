import QtQuick 2.13
import QtQuick.Window 2.12
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.12
import QtQuick.Dialogs
// import Crystal 1.0

Item {
    id: rootItem
    implicitHeight: parent ? parent.height : 0



    property ConeProperties coneProperties
    property string headerTitleSTR: "cone Properties"
    property string fillColor: "#91001d"
    property string lineColor: "#ffffff"


    onVisibleChanged: {
        coneProperties.fillcolor = fillColor
        coneProperties.transparency = opacityValue.value
        coneProperties.radius = radiusValue.value
        coneProperties.height = heightValue.value
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

                                ColorBox{
                                    id: fillColorBox
                                    anchors.centerIn:  parent
                                    colorValue: "#AA05FF"
                                    opacityValue: 0.5
                                    onColorValueChanged: {
                                        if(coneProperties){
//                                            console.log(colorBoxValue)
//                                            console.log(colorValue)
//                                            coneProperties.fillcolor = colorBoxValue.rgb
                                        }
                                    }
                                }


                            }
                            Rectangle{
                                Layout.fillWidth: true
                                width: parent.width/3
                                color: "#404040"
                                height: 35


                                Text {
                                    id: fillColorCone
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

                                QSpinBox {
                                    id: opacityValue
                                    value: 50
                                    to : 100
                                    from : 0
                                    anchors.fill: parent
                                    onValueChanged: {
                                        if(coneProperties){
                                            coneProperties.transparency = value
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
                                    id: opacityCone
                                    text: qsTr("Opacity:")
                                    font.pointSize: 10
                                    color: "white"
                                    anchors.verticalCenter:  parent.verticalCenter
                                    x:7
                                }
                            }

                            ////////////////////////////////////Location Property///////////////////////////////
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


                                                showText:  coneProperties ? coneProperties.location.x  : 0
                                                onValueChanged: {
                                                    coneProperties.location.x = value
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
                                                showText : coneProperties ? coneProperties.location.y  : 0
                                                onValueChanged: {
                                                    coneProperties.location.y = value
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
                                                showText : coneProperties ? coneProperties.location.z  : 0
                                                onValueChanged: {
                                                    coneProperties.location.z = value
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
                                                             coneProperties.relative = true
                                                         }
                                                         else{
                                                             coneProperties.relative = false
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

                                QSpinBox {
                                    id: radiusValue
                                    value: 20000
                                    to : 10000000
                                    from : 0
                                    anchors.fill: parent
                                    onValueChanged: {
                                        if(coneProperties){
                                            coneProperties.radius = value
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
                                    id: radiusCone
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

                                QSpinBox {
                                    id: heightValue
                                    value: 10000
                                    to : 10000000
                                    from : 0
                                    anchors.fill: parent
                                    onValueChanged: {
                                        if(coneProperties && heightValue && (heightValue.value == 0 || heightValue.value)){
                                            coneProperties.height = value
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

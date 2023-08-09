import QtQuick 2.13
import QtQuick.Window 2.12
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.12
import QtQuick.Dialogs
import Crystal 1.0


Item {
    id: rootItem
    implicitHeight: parent ? parent.height : 0



    property SphereProperty sphereProperty
    property string headerTitleSTR: "sphere Property"
    property string fillColor: "#91001d"


    onVisibleChanged: {
        sphereProperty.color = fillColor
        sphereProperty.transparency = opacityValue.value
        sphereProperty.radius = radiusValue.value
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



                            ////////////////////////////////////sphere Color Property//////////////////////////////////
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
                                        sphereProperty.color = fillColor.toString()
                                    }

                                }
                            }
                            Rectangle{
                                Layout.fillWidth: true
                                width: parent.width/3
                                color: "#404040"
                                height: 35


                                Text {
                                    id: fillColorsphere
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
                                            if(sphereProperty){
                                                sphereProperty.transparency = value
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
                                    id: opacitysphere
                                    text: qsTr("Opacity:")
                                    font.pointSize: 10
                                    color: "white"
                                    anchors.verticalCenter:  parent.verticalCenter
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
                                            if(sphereProperty){
                                                sphereProperty.radius = value
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
                                    id: radiussphere
                                    text: qsTr("Radius:")
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


                                                showText:  sphereProperty ? sphereProperty.location.x  : 0
                                                onValueChanged: {
                                                    sphereProperty.location.x = value
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
                                                showText : sphereProperty ? sphereProperty.location.y  : 0
                                                onValueChanged: {
                                                    sphereProperty.location.y = value
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
                                                showText : sphereProperty ? sphereProperty.location.z  : 0
                                                onValueChanged: {
                                                    sphereProperty.location.z = value
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
                                                             sphereProperty.relative = true
                                                         }
                                                         else{
                                                             sphereProperty.relative = false
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

                            ///////////////////////////////////center Property///////////////////////////////
                            Rectangle{
                                id:centerContainer
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
                                            width: centerContainer.width
                                            color: "transparent"
                                            //                                            clip:  true
                                            radius: 5
                                            Layout.fillWidth: true
                                            QSpinBox {
                                                id: mcenterX
                                                anchors.fill: parent
                                                decimals: 4
                                                //                                                step: 0.01
                                                //                                                value: xLoc


                                                showText:  sphereProperty ? sphereProperty.center.x  : 0
                                                onValueChanged: {
                                                    sphereProperty.center.x = value
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
                                            width: centerContainer.width
                                            color: "transparent"
                                            //                                            clip:  true
                                            radius: 5

                                            QSpinBox {
                                                id: mcenterY
                                                anchors.fill: parent
                                                decimals: 4
                                                //                                                step: 0.01
                                                //                                                value: yLoc
                                                showText : sphereProperty ? sphereProperty.center.y  : 0
                                                onValueChanged: {
                                                    sphereProperty.center.y = value
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
                                            width: centerContainer.width
                                            color: "transparent"
                                            //                                            clip:  true
                                            radius: 5

                                            QSpinBox {
                                                id: mcenterZ
                                                anchors.fill: parent
                                                decimals: 4
                                                //                                                step: 10
                                                showText : sphereProperty ? sphereProperty.center.z  : 0
                                                onValueChanged: {
                                                    sphereProperty.center.z = value
                                                }
                                            }
                                        }
                                    }
                                }
                            }

                            Rectangle{
                                id: centerTitle
                                Layout.fillWidth: true
                                color: "#404040"
                                height: 100
                                border.color: "#5f5f5f"
                                border.width: 1

                                Text {
                                    id: centerBox
                                    text: qsTr("Center:")
                                    font.pointSize: 10
                                    color: "white"
                                    y: 10
                                    x:7
                                }

                            }

                            ///////////////////////////////////Type Icons ///////////////////////////////////////
                            Rectangle{
                                Layout.fillWidth: true
                                color: "#404040"
                                height: 30

                                Row{
                                    spacing: 5
                                    anchors.centerIn: parent
                                    Rectangle{
                                        id: fullglobebtn
                                        width: 20
                                        height: 20
                                        color: "#5f5f5f"
                                        radius: 4
                                        border.width: 1
                                        border.color: "dark green"

                                        Image {
                                            id: globe
                                            source: "qrc:/resources/globe.png"
                                            sourceSize: "15 x 15"
                                            anchors.centerIn: parent
                                        }
                                        MouseArea{
                                            anchors.fill: parent
                                            cursorShape: Qt.PointingHandCursor
                                            onClicked: {
                                                sphereProperty.shape = 0
                                                fullglobebtn.border.color = "dark green"
                                                tophalfbtn.border.color = "gray"
                                                bothalfbtn.border.color = "gray"
                                            }
                                        }
                                    }
                                    Rectangle{
                                        id: tophalfbtn
                                        width: 20
                                        height: 20
                                        color: "#5f5f5f"
                                        radius: 4
                                        border.width: 1
                                        border.color: "gray"

                                        Image {
                                            id: tophalf
                                            source: "qrc:/resources/topHalf.png"
                                            sourceSize: "15 x 15"
                                            anchors.centerIn: parent
                                        }
                                        MouseArea{
                                            anchors.fill: parent
                                            cursorShape: Qt.PointingHandCursor
                                            onClicked: {
                                                sphereProperty.shape = 1
                                                fullglobebtn.border.color = "gray"
                                                tophalfbtn.border.color = "dark green"
                                                bothalfbtn.border.color = "gray"
                                            }
                                        }
                                    }
                                    Rectangle{
                                        id: bothalfbtn
                                        width: 20
                                        height: 20
                                        color: "#5f5f5f"
                                        radius: 4
                                        border.width: 1
                                        border.color: "gray"

                                        Image {
                                            id: bothalf
                                            source: "qrc:/resources/botHalf.png"
                                            sourceSize: "15 x 15"
                                            anchors.centerIn: parent
                                        }
                                        MouseArea{
                                            anchors.fill: parent
                                            cursorShape: Qt.PointingHandCursor
                                            onClicked: {
                                                sphereProperty.shape = 2
                                                fullglobebtn.border.color = "gray"
                                                tophalfbtn.border.color = "gray"
                                                bothalfbtn.border.color = "dark green"
                                            }
                                        }
                                    }
                                }
                            }
                            Rectangle{
                                Layout.fillWidth: true
                                color: "#404040"
                                height: 30

                                Text {
                                    id: shapeSphere
                                    text: qsTr("Shape:")
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


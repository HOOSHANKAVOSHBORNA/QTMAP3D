import QtQuick 2.13
import QtQuick.Window 2.12
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.12
import QtQuick.Dialogs
import Crystal 1.0
import "style"

Item {
    anchors.fill: parent
    id: rootItem
    implicitHeight: parent ? parent.height : 0
    readonly property color foregroundColor: Qt.rgba(Style.foregroundColor.r, Style.foregroundColor.g, Style.foregroundColor.b, 0.20)
    readonly property color backgroundColor: Qt.rgba(Style.backgroundColor.r, Style.backgroundColor.g, Style.backgroundColor.b, 0.30)
    property string _headerTitleSTR: "Properties"
    property CProperty model
    //property int lblWidth: 100
    property int swtchWidth: 80
    property int lblWidth: 50
    property int valHeight: 30
    //////////////////////////Main Content////////////////////////
    Rectangle {
        id: item
        visible: true
        width: parent.width *0.9
        height: rootItem.height
        anchors.fill: parent
        color: "transparent"
     //   opacity: 0.9
        anchors.margins: 6 / Style.monitorRatio
        radius: 10 / Style.monitorRatio

        ScrollView {
            id: frame
            clip: true
            contentWidth: parent.width
            anchors.fill: parent
            /////////////////////// components Grid ////////////////////////////
            ColumnLayout{
                id: mainLayout
                anchors.fill: parent
                anchors.margins: 5 / Style.monitorRatio
                ////////////////////////////////// Name ////////////////////////////////////////////////
                RowLayout{
                    id:nameSec
                    Layout.fillWidth: true
                    Text {
                        Layout.preferredWidth: lblWidth  / Style.monitorRatio
                        text: "Name"
                        font.pointSize: 10 / Style.monitorRatio
                        color: Style.foregroundColor
                    }
                    TextField {
                        id: namevalue
                        Layout.preferredWidth: 200 / Style.monitorRatio
                        Layout.minimumWidth: 100 / Style.monitorRatio
                        Layout.fillWidth: true
                        height: valHeight
                        text: rootItem.model ? rootItem.model.name : ""
                        font.pointSize: 10  / Style.monitorRatio
                        color: "black"
                        background: Rectangle{
                            color: foregroundColor
                            radius: height/2
                        }
                        onAccepted: {
                            rootItem.model.name = namevalue.displayText
                        }
                    }
                }

                ColorBoxInput{
                    id:testInput
                    visible: false
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }

                RowLayout{
                    id:fillcolorSec
                    Layout.fillWidth: true
                    visible: rootItem.model ? rootItem.model.fillColorStatus : false

                    Text {
                        Layout.preferredWidth: lblWidth
                        text: "Color"
                        font.pointSize: 10 / Style.monitorRatio
                        color: Style.foregroundColor
                    }
                    RowLayout{
                        spacing:3
                        Repeater{
                            model: ["red","orange","yellow","green","blue","white"]
                            Rectangle{
                                implicitWidth: 20 / Style.monitorRatio
                                implicitHeight: 20 / Style.monitorRatio
                                radius: width
                                color: modelData
                                MouseArea{
                                    anchors.fill: parent
                                    onClicked: {

                                    }
                                }
                            }
                        }

                        Rectangle{
                            implicitWidth: 20 / Style.monitorRatio
                            implicitHeight: 20 / Style.monitorRatio
                            radius: width
                            border.width:1 / Style.monitorRatio
                            border.color:Style.foregroundColor
                            IconImage{
                                Layout.alignment: Qt.AlignCenter
                                width: 20 / Style.monitorRatio
                                height: 20 / Style.monitorRatio
                                source: "qrc:/Resources/location-add.png"
                            }
                            MouseArea{
                                anchors.fill: parent
                               // onClicked: fillColorDialog.visible = true
                                onClicked: testInput.visible = true

                            }
                        }
                    }

//                    Rectangle{
//                        width: valHeight
//                        height: valHeight
//                        color: rootItem.model ? rootItem.model.fillColor : false
//                        border.width: 2
//                        border.color: "#c9c9c9"
//                        radius: 5
//                        MouseArea{
//                            anchors.fill: parent
//                            cursorShape: Qt.PointingHandCursor
//                            onClicked: fillColorDialog.visible = true
//                        }
//                    }
//                    SpinBox {
//                        id: opacityValue
//                        Layout.fillWidth: true
//                        Layout.minimumWidth: 80
//                        height: valHeight
//                        from : 0
//                        to : 100
//                        onValueChanged: {
//                            rootItem.model.fillColor.a = value/100
//                        }
//                    }
//                    Binding{
//                        target: opacityValue
//                        property: "value"
//                        value: rootItem.model ? rootItem.model.fillColor.a * 100 : 0
//                        delayed: true
//                    }

                    ColorDialog {
                        visible: false
                        id:  fillColorDialog
                        title: "Please choose a color"
                        onAccepted: {
                            var currentColor = fillColorDialog.selectedColor
                            currentColor.a = opacityValue.value / 100
                            rootItem.model.fillColor = currentColor
                        }
                    }
                }
                RowLayout{
                    Text{
                        text: "Stroke"
                        color: Style.foregroundColor
                        font.pointSize: 10 / Style.monitorRatio
                        Layout.alignment: Qt.AlignTop
                        Layout.preferredWidth: lblWidth / Style.monitorRatio
                        visible:rootItem.model ? rootItem.model.strokeStatus : false
                    }
                    GroupBox{
                        id: strokeSec
                        visible:rootItem.model ? rootItem.model.strokeStatus : false

                        padding: 0
                        Layout.fillWidth: true
                        Layout.margins: 0

                        background: Rectangle{
                            color: foregroundColor
                            radius: 10 / Style.monitorRatio
                            border.color: "transparent"
                        }

                        ColumnLayout{
                            anchors.fill: parent

                            RowLayout{
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                Layout.topMargin: 2 / Style.monitorRatio

                                RowLayout{
                                    spacing:3
                                    Layout.leftMargin: 2 / Style.monitorRatio
                                    Repeater{
                                        model: ["red","orange","yellow","green","blue","white"]
                                        Rectangle{
                                            implicitWidth: 20 / Style.monitorRatio
                                            implicitHeight: 20 / Style.monitorRatio
                                            radius: width
                                            color: modelData
                                            MouseArea{
                                                anchors.fill: parent
                                                onClicked: {

                                                }
                                            }
                                        }
                                    }

                                    Rectangle{
                                        implicitWidth: 20 / Style.monitorRatio
                                        implicitHeight: 20 / Style.monitorRatio
                                        radius: width
                                        border.width:1 / Style.monitorRatio
                                        border.color:Style.foregroundColor
                                        IconImage{
                                            Layout.alignment: Qt.AlignCenter
                                            width: 20 / Style.monitorRatio
                                            height: 20 / Style.monitorRatio
                                            source: "qrc:/Resources/location-add.png"
                                        }
                                        MouseArea{
                                            anchors.fill: parent
                                            onClicked: strokeColorDialog.visible = true
                                        }
                                    }
                                }


//                                Rectangle{
//                                    width: valHeight
//                                    height: valHeight
//                                    color: rootItem.model ? rootItem.model.strokeColor : "#202020"
//                                    border.width: 2
//                                    border.color: "#c9c9c9"
//                                    radius: 5
//                                    MouseArea{
//                                        anchors.fill: parent
//                                        cursorShape: Qt.PointingHandCursor
//                                        onClicked: strokeColorDialog.visible = true

//                                    }
//                                }

//                                SpinBox {
//                                    id: strokeOpacityValue
//                                    to : 100
//                                    from : 0
//                                    Layout.fillWidth: true
//                                    Layout.minimumWidth: 80
//                                    height: valHeight
//                                    onValueChanged: {
//                                        rootItem.model.strokeColor.a= value/100
//                                    }
//                                }
//                                Binding{
//                                    target: strokeOpacityValue
//                                    property: "value"
//                                    value: rootItem.model ? rootItem.model.strokeColor.a * 100 : 0
//                                    delayed: true
//                                }

                                ColorDialog {
                                    visible: false
                                    id:  strokeColorDialog
                                    title: "Please choose a color"
                                    onAccepted: {
                                        var currentStrokeColor = strokeColorDialog.selectedColor
                                        currentStrokeColor.a = strokeOpacityValue.value / 100
                                        rootItem.model.strokeColor = currentStrokeColor
                                    }
                                }
                            }
                            StepSpinBox {
                                id: strokeWidthValue
                                editable: true
                                stepComboboxVisible: false
                                Layout.fillWidth: true
                                Layout.alignment: Qt.AlignCenter
                                Layout.topMargin: 2 / Style.monitorRatio
                                Layout.bottomMargin: 2 / Style.monitorRatio
                                Layout.rightMargin: 2 / Style.monitorRatio
                                Layout.leftMargin: 2 / Style.monitorRatio
                                height: 25 / Style.monitorRatio
                                from: 0

                                onValueChanged: {
                                    if (rootItem.model)
                                        rootItem.model.strokeWidth = value
                                }
                            }
                            Binding{
                                target: strokeWidthValue
                                property: "value"
                                value: rootItem.model ? rootItem.model.strokeWidth : 0
                                delayed: true
                            }
                        }
                    }
                }

                ////////////---------------------Location--------------------/////////////
                RowLayout{
                    Text{
                        text: "Location"
                        color: Style.foregroundColor
                        font.pointSize: 10 / Style.monitorRatio
                        Layout.alignment: Qt.AlignTop
                        Layout.preferredWidth: lblWidth
                        visible:rootItem.model ? rootItem.model.locationStatus : false
                    }

                    GroupBox{
                        id: locationSec
                        visible:rootItem.model ? rootItem.model.locationStatus : false
                        Layout.fillWidth: true
                        padding: 0

                        background: Rectangle{
                            color: foregroundColor
                            radius: 10 / Style.monitorRatio
                            border.color: "transparent"
                        }

                        ColumnLayout{
                          anchors.fill: parent
                            GridLayout{
                                columnSpacing: 1
                                rowSpacing: 1
                                columns:2

                                Text {
                                    Layout.preferredWidth: 20 / Style.monitorRatio
                                    text: "X "
                                    padding: 5 / Style.monitorRatio
                                    font.pointSize: 10 / Style.monitorRatio
                                    color: Style.foregroundColor
                                }

                                StepSpinBox {
                                    id: xLocationValue
                                    editable: true
                                    Layout.fillWidth: true
                                    Layout.minimumWidth: 100 / Style.monitorRatio
                                    Layout.rightMargin: 2 / Style.monitorRatio
                                    Layout.topMargin: 2 / Style.monitorRatio
                                    height: valHeight
                                    decimals: 4
                                    from: -2000
                                    to: 2000
                                    onValueChanged: {
                                        rootItem.model.location.x = value
                                    }
                                }

                                Binding{
                                    target: xLocationValue
                                    property: "value"
                                    value: rootItem.model ? rootItem.model.location.x : 0
                                    delayed: true
                                }

                                Text {
                                    Layout.preferredWidth: 20 / Style.monitorRatio
                                    text: "Y "
                                    padding: 5 / Style.monitorRatio
                                    font.pointSize: 10 / Style.monitorRatio
                                    color: Style.foregroundColor
                                }
                                StepSpinBox {
                                    id: yLocationValue
                                    editable: true
                                    Layout.fillWidth: true
                                    Layout.minimumWidth: 100 / Style.monitorRatio
                                    Layout.rightMargin: 2 / Style.monitorRatio
                                    height: valHeight
                                    decimals: 4
                                    from: -2000
                                    to: 2000
                                    onValueChanged: {
                                        rootItem.model.location.y = value
                                    }
                                }

                                Binding{
                                    target: yLocationValue
                                    property: "value"
                                    value: rootItem.model ? rootItem.model.location.y : 0
                                    delayed: true
                                }

                                Text {
                                    Layout.preferredWidth: 20 / Style.monitorRatio
                                    text: "Z "
                                    padding: 5 / Style.monitorRatio
                                    font.pointSize: 10
                                    color: Style.foregroundColor
                                }
                                StepSpinBox {

                                    id: zLocationValue
                                    editable: true
                                    Layout.fillWidth: true
                                    Layout.minimumWidth: 100 / Style.monitorRatio
                                    Layout.rightMargin: 2 / Style.monitorRatio
                                    height: valHeight / Style.monitorRatio
                                    decimals: 4
                                    from: -2000
                                    to: 2000
                                    onValueChanged: {
                                        rootItem.model.location.z = value
                                    }
                                }

                                Binding{
                                    target: zLocationValue
                                    property: "value"
                                    value: rootItem.model ? rootItem.model.location.z : 0
                                    delayed: true
                                }
                            }

                            CheckBox {
                                id: relative
                                text: "Relative"
                                font.pointSize: 10 / Style.monitorRatio
                                checked: false

                                onCheckStateChanged: if(checked === true){
                                                         rootItem.model.locationRelative =true
                                                     }
                                                     else{
                                                         rootItem.model.locationRelative = false
                                                     }

                                indicator: Rectangle {
                                    implicitWidth: 15 / Style.monitorRatio
                                    implicitHeight: 15 / Style.monitorRatio
                                    x: relative.leftPadding  / Style.monitorRatio
                                    y: (parent.height / 2 - height / 2) / Style.monitorRatio
                                    radius: 3 / Style.monitorRatio
                                    border.color: relative.down ? "black" : "#313131"

                                    Rectangle {
                                        width: 10 / Style.monitorRatio
                                        height: 10 / Style.monitorRatio
                                        x: 3 / Style.monitorRatio
                                        y: 3 / Style.monitorRatio
                                        radius: 2 / Style.monitorRatio
                                        color: relative.down ? "black" : "dark green"
                                        visible: relative.checked
                                    }
                                }
                                contentItem: Text {
                                    text: relative.text
                                    font: relative.font
                                    opacity: enabled ? 1.0 : 0.3
                                    color: relative.down ? "black" : Style.foregroundColor
                                    verticalAlignment: Text.AlignVCenter
                                    leftPadding: relative.indicator.width + relative.spacing
                                }
                            }
                        }


                    }
                }

                //                }
                ///////////-----------------------center-------------------//////////////

                RowLayout{
                    Text{
                        text: "Center"
                        color: Style.foregroundColor
                        font.pointSize: 10 / Style.monitorRatio
                        Layout.alignment: Qt.AlignTop
                        Layout.preferredWidth: lblWidth / Style.monitorRatio
                        visible:rootItem.model ? rootItem.model.centerStatus : false
                    }
                    GroupBox{
                        id: centerSec
                        visible:rootItem.model ? rootItem.model.centerStatus : false
                        padding: 0
                        Layout.fillWidth: true

                        background: Rectangle{
                            color: foregroundColor
                            radius: 10 / Style.monitorRatio
                            border.color: "transparent"
                        }
                        GridLayout{
                            anchors.fill: parent
                            columnSpacing: 1
                            rowSpacing: 1
                            columns:2

                            Text {
                                Layout.preferredWidth: 20 / Style.monitorRatio
                                text: "X "
                                padding: 5 / Style.monitorRatio
                                font.pointSize: 10 / Style.monitorRatio
                                color: Style.foregroundColor
                            }

                            StepSpinBox {
                                id: xCenterValue
                                editable: true
                                Layout.fillWidth: true
                                Layout.minimumWidth: 100 / Style.monitorRatio
                                Layout.rightMargin: 2 / Style.monitorRatio
                                Layout.topMargin: 2 / Style.monitorRatio
                                height: valHeight / Style.monitorRatio
                                decimals: 2
                                from: -2000
                                to: 2000
                                onValueChanged: {
                                    rootItem.model.center.x = value
                                }
                            }
                            Binding{
                                target: xCenterValue
                                property: "value"
                                value: rootItem.model ? rootItem.model.center.x : 0
                                delayed: true
                            }

                            Text {
                                Layout.preferredWidth: 20 / Style.monitorRatio
                                text: "Y "
                                padding: 5 / Style.monitorRatio
                                font.pointSize: 10 / Style.monitorRatio
                                color: Style.foregroundColor
                            }

                            StepSpinBox {
                                id: yCenterValue
                                editable: true
                                Layout.fillWidth: true
                                Layout.minimumWidth: 100 / Style.monitorRatio
                                Layout.rightMargin: 2 / Style.monitorRatio
                                height: valHeight / Style.monitorRatio
                                decimals: 2
                                from: -2000
                                to: 2000
                                onValueChanged: {
                                    rootItem.model.center.y =value
                                }
                            }
                            Binding{
                                target: yCenterValue
                                property: "value"
                                value: rootItem.model ? rootItem.model.center.y : 0
                                delayed: true
                            }

                            Text {
                                Layout.preferredWidth: 20 / Style.monitorRatio
                                text: "Z "
                                padding: 5
                                font.pointSize: 10 / Style.monitorRatio
                                color: Style.foregroundColor
                            }

                            StepSpinBox {
                                id: zCenterValue
                                editable: true
                                Layout.fillWidth: true
                                Layout.minimumWidth: 100 / Style.monitorRatio
                                Layout.rightMargin: 2 / Style.monitorRatio
                                Layout.bottomMargin: 2 / Style.monitorRatio
                                height: valHeight
                                decimals: 2
                                from: -2000
                                to: 2000
                                onValueChanged: {
                                    rootItem.model.center.z =value
                                }
                            }
                            Binding{
                                target: zCenterValue
                                property: "value"
                                value: rootItem.model ? rootItem.model.center.z : 0
                                delayed: true
                            }
                        }
                    }
                }

                ////////// -------------------- Arc ----------------------//////////////
                RowLayout{
                    Text{
                        text: "Arc"
                        color: Style.foregroundColor
                        font.pointSize: 10 / Style.monitorRatio
                        Layout.alignment: Qt.AlignTop
                        Layout.preferredWidth: lblWidth / Style.monitorRatio
                        visible:rootItem.model ? rootItem.model.arcStatus : false
                    }
                    GroupBox{
                        id: arcSec
                        visible:rootItem.model ? rootItem.model.arcStatus : false
                        padding: 0
                        Layout.fillWidth: true
                        background: Rectangle{
                            color: foregroundColor
                            border.color: "transparent"
                            radius:10
                        }

                        GridLayout{
                            anchors.fill: parent
                            columnSpacing: 1
                            rowSpacing: 1
                            columns:2

                            Text{
                                Layout.preferredWidth: lblWidth / Style.monitorRatio
                                text: "Start "
                                padding: 5 / Style.monitorRatio
                                font.pointSize: 10 / Style.monitorRatio
                                color: Style.foregroundColor
                            }
                            StepSpinBox {
                                id: startArcValue
                                editable: true
                                stepComboboxVisible: false
                                Layout.fillWidth: true
                                Layout.minimumWidth: 100 / Style.monitorRatio
                                Layout.rightMargin: 2 / Style.monitorRatio
                                Layout.topMargin: 2 / Style.monitorRatio
                                height: valHeight
                                value: 0
                                from : 0
                                to : 360
                                onValueChanged: {
                                    if (rootItem.model)
                                        rootItem.model.arc.x = value
                                }
                            }
                            Binding{
                                target: startArcValue
                                property: "value"
                                value: rootItem.model ? rootItem.model.arc.x : 0
                                delayed: true
                            }
                            Text {
                                Layout.preferredWidth: lblWidth / Style.monitorRatio
                                text: "End "
                                padding: 5 / Style.monitorRatio
                                font.pointSize: 10 / Style.monitorRatio
                                color: Style.foregroundColor
                            }
                            StepSpinBox {
                                id: endArcValue
                                editable: true
                                stepComboboxVisible: false
                                Layout.fillWidth: true
                                Layout.minimumWidth: 100 / Style.monitorRatio
                                Layout.rightMargin: 2 / Style.monitorRatio
                                //Layout.topMargin: 2
                                Layout.bottomMargin: 2 / Style.monitorRatio
                                height: valHeight
                                value: 360
                                from: 0
                                to: 360
                                onValueChanged: {
                                    if (rootItem.model){
                                        rootItem.model.arc.y = value
                                    }
                                }
                            }
                            Binding{
                                target: endArcValue
                                property: "value"
                                value: rootItem.model ? rootItem.model.arc.y : 0
                                delayed: true
                            }
                        }
                    }
                }

                ////////------------------------- radius------------------//////////////
                Item{
                    Layout.fillWidth: true
                    Layout.preferredHeight: 25 / Style.monitorRatio
                    visible:rootItem.model ? rootItem.model.radiusStatus : false

                    RowLayout{
                        id:radiusSec
                        anchors.fill: parent
                        Text {
                            Layout.preferredWidth: lblWidth
                            text: "Radius"
                            font.pointSize: 10 / Style.monitorRatio
                            color: Style.foregroundColor
                        }
                        Rectangle{
                            color: foregroundColor
                            radius: 10 / Style.monitorRatio
                            border.color: "transparent"
                            Layout.fillWidth: true
                            Layout.preferredHeight: valHeight / Style.monitorRatio

                            StepSpinBox {
                                id: radiusValue
                                editable: true
                                anchors.leftMargin: 2 / Style.monitorRatio
                                anchors.rightMargin: 2 / Style.monitorRatio
                                anchors.centerIn: parent
                                width: (parent.width - 5) / Style.monitorRatio
                                height: 28 / Style.monitorRatio
                                decimals: 2
                                from : 0
                                to: 2000
                                onValueChanged: {
                                    rootItem.model.radius = value
                                }
                            }
                            Binding{
                                target: radiusValue
                                property: "value"
                                value: rootItem.model ? rootItem.model.radius :0
                                delayed: true
                            }
                        }
                    }
                }


                ////////------------------------- radiusMinor------------------//////////////

                RowLayout{
                    id:radiusMinorSec
                    visible:rootItem.model ? rootItem.model.radiusMinorStatus : false
                    Layout.fillWidth: true
                    Text {
                        Layout.preferredWidth: swtchWidth / Style.monitorRatio
                        text: "Radius Minor "
                        font.pointSize: 10 / Style.monitorRatio
                        color: Style.foregroundColor
                    }
                    StepSpinBox {
                        id: radiusMinorValue
                        editable: true
                        Layout.fillWidth: true
                        Layout.minimumWidth: 100 / Style.monitorRatio
                        height: valHeight / Style.monitorRatio
                        decimals: 2
                        from : 0
                        to: 2000
                        onValueChanged: {
                            rootItem.model.radiusMinor = value
                        }
                    }
                    Binding{
                        target: radiusMinorValue
                        property: "value"
                        value: rootItem.model ? rootItem.model.radiusMinor :0
                        delayed: true
                    }
                }

                ////////------------------------- radiusMajor------------------//////////////

                RowLayout{
                    id:radiusMajorSec
                    visible:rootItem.model ? rootItem.model.radiusMajorStatus : false
                    Layout.fillWidth: true
                    Text {
                        Layout.preferredWidth: swtchWidth / Style.monitorRatio
                        text: "Radius Major "
                        font.pointSize: 10 / Style.monitorRatio
                        color: Style.foregroundColor
                    }
                    StepSpinBox {
                        id: radiusMajorValue
                        editable: true
                        Layout.fillWidth: true
                        Layout.minimumWidth: 100 / Style.monitorRatio
                        height: valHeight / Style.monitorRatio
                        decimals: 2
                        from : 0
                        to: 2000
                        onValueChanged: {
                            rootItem.model.radiusMajor = value
                        }
                    }
                    Binding{
                        target: radiusMajorValue
                        property: "value"
                        value: rootItem.model ? rootItem.model.radiusMajor :0
                        delayed: true
                    }
                }
                ////////------------------------- height------------------//////////////

                Item{
                    Layout.fillWidth: true
                    Layout.preferredHeight: 25 / Style.monitorRatio
                    visible:rootItem.model ? rootItem.model.heightStatus : false
                    RowLayout{
                        id:heightSec
                        anchors.fill: parent
                        Text {
                            Layout.preferredWidth: lblWidth / Style.monitorRatio
                            text: "Height "
                            font.pointSize: 10 / Style.monitorRatio
                            color: Style.foregroundColor
                            Layout.alignment: Qt.AlignVCenter
                        }
                        Rectangle{
                            color: foregroundColor
                            radius: 10
                            border.color: "transparent"
                            Layout.fillWidth: true
                            Layout.alignment: Qt.AlignVCenter
                            Layout.preferredHeight: valHeight / Style.monitorRatio
                            StepSpinBox {
                                id:heightValue
                                editable: true
                                anchors.leftMargin: 2 / Style.monitorRatio
                                anchors.rightMargin: 2 / Style.monitorRatio
                                anchors.centerIn: parent
                                width: (parent.width - 5) / Style.monitorRatio
                                height: 30 / Style.monitorRatio
                                decimals: 2
                                from : 0
                                to: 2000
                                onValueChanged: {
                                    rootItem.model.heighT= value
                                }
                            }
                            Binding{
                                target: heightValue
                                property: "value"
                                value: rootItem.model? rootItem.model.heighT: 0
                                delayed: true
                            }
                        }
                    }
                }

                ////////------------------------- lenght ------------------//////////////

                Item{
                    Layout.fillWidth: true
                    Layout.preferredHeight: 25 / Style.monitorRatio
                    visible: rootItem.model ? rootItem.model.lenghtStatus : false
                        RowLayout{
                            id:lenghtSec
                            anchors.fill: parent
                          //  visible: rootItem.model ? rootItem.model.lenghtStatus : false
                            Text {
                                Layout.preferredWidth: lblWidth
                                text:  "Lenght "
                                font.pointSize: 10 / Style.monitorRatio
                                color: Style.foregroundColor
                            }

                            Rectangle{
                               // color: lenghtSec.visible ? foregroundColor : "transparent"
                                color: foregroundColor
                                radius: 10
                                border.color: "transparent"
                                Layout.fillWidth: true
                                Layout.preferredHeight: valHeight

                                StepSpinBox {
                                    id: lengthValue
                                    editable: true
                                    anchors.leftMargin: 2 / Style.monitorRatio
                                    anchors.rightMargin: 2 / Style.monitorRatio
                                    anchors.centerIn: parent
                                    width: (parent.width - 5) / Style.monitorRatio
                                    height: 30 / Style.monitorRatio
                                    decimals: 2
                                    from : 0
                                    to: 2000
                                    onValueChanged: {
                                        rootItem.model.lenghT = value
                                    }
                                }
                                Binding{
                                    target: lengthValue
                                    property: "value"
                                    value: rootItem.model ? rootItem.model.lenghT : 0
                                    delayed: true
                                }
                            }
                        }

                }

                ////////------------------------- width ------------------//////////////
                Item{
                    Layout.fillWidth: true
                    Layout.preferredHeight: 25 / Style.monitorRatio
                    visible:rootItem.model ? rootItem.model.widthStatus : false
                        RowLayout{
                            id:widthSec
                            anchors.fill: parent
                            Text {
                                Layout.preferredWidth: lblWidth
                                text: "Width "
                                font.pointSize: 10 / Style.monitorRatio
                                color: Style.foregroundColor
                            }
                            Rectangle{
                                color: foregroundColor
                                radius: 10 / Style.monitorRatio
                                border.color: "transparent"
                                Layout.fillWidth: true
                                Layout.preferredHeight: 25 / Style.monitorRatio

                                StepSpinBox {
                                    id:widthValue
                                    editable: true
                                    anchors.leftMargin: 2 / Style.monitorRatio
                                    anchors.rightMargin: 2 / Style.monitorRatio
                                    anchors.centerIn: parent
                                    width: (parent.width - 5) / Style.monitorRatio
                                    height: 28 / Style.monitorRatio
                                    decimals: 2
                                    from : 0
                                    to: 2000
                                    onValueChanged: {
                                        rootItem.model.widtH = value
                                    }
                                }
                                Binding{
                                    target: widthValue
                                    property: "value"
                                    value: rootItem.model ? rootItem.model.widtH : 0
                                    delayed: true
                                }
                            }
                        }
                }

                // --------------------------------- clamp -----------------------////////////

                RowLayout{
                    id:clampSec
                    Layout.fillWidth: true
                    visible:rootItem.model ? rootItem.model.clampStatus : false
                    Text {
                        Layout.preferredWidth: lblWidth / Style.monitorRatio
                        text: "Clamp "
                        font.pointSize: 10 / Style.monitorRatio
                        color: Style.foregroundColor
                    }

                    Rectangle{
                        color: foregroundColor
                        radius: 10 / Style.monitorRatio
                        border.color: "transparent"
                        Layout.fillWidth: true
                        Layout.preferredHeight: valHeight / Style.monitorRatio

                        ComboBox {
                            id: clampCombo
                            model: ["None", "Terrain", "Relative","Absolute"]
                            width: (parent.width - 5) / Style.monitorRatio
                            anchors.centerIn: parent
                            height: 25 / Style.monitorRatio

                            onCurrentIndexChanged: {
                                if(rootItem.model)
                                    rootItem.model.clamp = currentIndex
                            }

                            delegate: ItemDelegate {
                                width: clampCombo.width
                                contentItem: Text {
                                    text: clampCombo.textRole
                                          ? (Array.isArray(clampCombo.model) ? modelData[clampCombo.textRole] : model[clampCombo.textRole])
                                          : modelData
                                    color: "#5f5f5f"
                                    font: clampCombo.font
                                    elide: Text.ElideRight
                                    verticalAlignment: Text.AlignVCenter
                                }
                                highlighted: clampCombo.highlightedIndex === index
                            }

                            indicator: Canvas {
                                id: canvas
                                x: (clampCombo.width - width - clampCombo.rightPadding) / Style.monitorRatio
                                y: (clampCombo.topPadding + (clampCombo.availableHeight - height) / 2) / Style.monitorRatio
                                width: 12 / Style.monitorRatio
                                height: 8 / Style.monitorRatio
                                contextType: "2d"

                                Connections {
                                    target: clampCombo
                                    function onPressedChanged() { canvas.requestPaint(); }
                                }
                            }

                            contentItem: Text {
                                leftPadding: 5 / Style.monitorRatio
                                rightPadding: clampCombo.indicator.width + clampCombo.spacing

                                text: clampCombo.displayText
                                font: clampCombo.font
                                color: clampCombo.pressed ? "#5f5f5f" : "#404040"
                                verticalAlignment: Text.AlignVCenter
                                elide: Text.ElideRight
                            }

                            background: Rectangle {
                                implicitWidth: 100 / Style.monitorRatio
                                implicitHeight: 22 / Style.monitorRatio
                                border.color: clampCombo.pressed ? "#5f5f5f" : "#404040"
                                border.width: clampCombo.visualFocus ? 2 : 1
                                radius: 5
                            }

                            popup: Popup {
                                y: (clampCombo.height - 1) / Style.monitorRatio
                                width: clampCombo.width
                                implicitHeight: contentItem.implicitHeight
                                padding: 1 / Style.monitorRatio

                                contentItem: ListView {
                                    clip: true
                                    implicitHeight: contentHeight / Style.monitorRatio
                                    model: clampCombo.popup.visible ? clampCombo.delegateModel : null
                                    currentIndex: clampCombo.highlightedIndex

                                    ScrollIndicator.vertical: ScrollIndicator { }
                                }

                                background: Rectangle {
                                    border.color: "#404040"
                                    radius: 5 / Style.monitorRatio
                                }
                            }
                        }
                    }
                }
                ////////-------------------------/tesselation/------------------//////////////

                RowLayout{
                    id:tesselationSec
                    visible:rootItem.model ? rootItem.model.tesselationStatus : false
                    Layout.fillWidth: true
                    Text {
                        Layout.preferredWidth: swtchWidth / Style.monitorRatio
                        text: "Tesselation "
                        font.pointSize: 10 / Style.monitorRatio
                        color: Style.foregroundColor
                    }
                    StepSpinBox {
                        id: tesselationValue
                        stepComboboxVisible: false
                        editable: true
                        from : 0
                        to: 2000
                        Layout.fillWidth: true
                        height: valHeight / Style.monitorRatio
                        onValueChanged: {
                            rootItem.model.tesselation = value
                        }
                    }
                    Binding{
                        target: tesselationValue
                        property: "value"
                        value: rootItem.model ? rootItem.model.tesselation: 0
                        delayed: true
                    }
                }
                ////------------------------ Show lenght -------------------- ///////////////

                RowLayout{
                    id:showLenghtSec
                    visible:rootItem.model ? rootItem.model.showLenStatus : false
                    Layout.fillWidth: true

                    Text {
                        Layout.preferredWidth: swtchWidth
                        text: "Show Length "
                        font.pointSize: 10 / Style.monitorRatio
                        color: Style.foregroundColor
                    }
                    Switch {
                        id: showLenghtValue
                        padding: 0
                        width: 100
                        height: valHeight / Style.monitorRatio
                        checked: false
                        onToggled: function() {
                            rootItem.model.showLen = showLenghtValue.checked
                        }
                    }
                    Binding{
                        target: showLenghtValue
                        property: "checked"
                        value: rootItem.model ? rootItem.model.showLen: 0
                        delayed: true
                    }
                }
                ////------------------------Show Bearing -------------------- ///////////////

                RowLayout{
                    id:bearingSec
                    visible:rootItem.model ? rootItem.model.bearingStatus : false
                    Layout.fillWidth: true
                    Text {
                        Layout.preferredWidth: swtchWidth / Style.monitorRatio
                        text: "Show Bearing "
                        font.pointSize: 10 / Style.monitorRatio
                        color: Style.foregroundColor
                    }
                    Switch {
                        id: bearingValue
                        padding: 0 / Style.monitorRatio
                        width: 100 / Style.monitorRatio
                        height: valHeight / Style.monitorRatio
                        checked: false
                        onToggled: function() {
                            rootItem.model.showBearing = bearingValue.checked
                        }
                    }
                    Binding{
                        target: bearingValue
                        property: "checked"
                        value: rootItem.model ? rootItem.model.showBearing: 0
                        delayed: true
                    }
                }

                ////------------------------  showAltitude -------------------- ///////////////

                RowLayout{
                    id:showAltitudeSec
                    visible:rootItem.model ? rootItem.model.altitudeStatus : false
                    Layout.fillWidth: true
                    Text {
                        Layout.preferredWidth: swtchWidth
                        text: "Show Altitude "
                        font.pointSize: 10 / Style.monitorRatio
                        color: Style.foregroundColor
                    }
                    Switch {
                        id: showAltitudeValue
                        padding: 0
                        width: 100 / Style.monitorRatio
                        height: valHeight / Style.monitorRatio
                        checked: false
                        onToggled: function() {
                            rootItem.model.showAltitude = showAltitudeValue.checked
                        }
                    }
                    Binding{
                        target: showAltitudeValue
                        property: "checked"
                        value: rootItem.model ? rootItem.model.showAltitude: 0
                        delayed: true
                    }
                }



                ////------------------------ Show Slope -------------------- ///////////////

                RowLayout{
                    id:slopeSec
                    visible:rootItem.model ? rootItem.model.showSlopStatus : false
                    Layout.fillWidth: true
                    Text {
                        Layout.preferredWidth: swtchWidth / Style.monitorRatio
                        text: "Show Slope "
                        font.pointSize: 10 / Style.monitorRatio
                        color: Style.foregroundColor
                    }
                    Switch {
                        id: slopeValue
                        padding: 0
                        width: 100 / Style.monitorRatio
                        height: valHeight / Style.monitorRatio
                        checked: false
                        onToggled: function() {
                            rootItem.model.showSlop  = slopeValue.checked
                        }
                    }
                    Binding{
                        target: slopeValue
                        property: "checked"
                        value: rootItem.model ? rootItem.model.showSlop: 0
                        delayed: true
                    }
                }
                ////------------------------ Points  -------------------- ///////////////

                GroupBox{
                    id: pointSec
                    visible: rootItem.model ? rootItem.model.pointsStatus : false
                    padding: 0
                    Layout.fillWidth: true

                    background: Rectangle{
                        color: "transparent"
                        border.color: "transparent"
                    }

                     ColumnLayout{
                         anchors.fill: parent
                          RowLayout{
                             Text {
                                 Layout.preferredWidth: swtchWidth / Style.monitorRatio
                                 text: "Points "
                                 font.pointSize: 10 / Style.monitorRatio
                                 color: Style.foregroundColor
                             }
                             Switch {
                                 id:pointVisible
                                 padding: 0
                                 width: 100
                                 height: valHeight
                                 checked: false
                                 onToggled: function() {
                                     rootItem.model.pointsVisible  = pointVisible.checked
                                 }
                             }
                             Binding{
                                 target: pointVisible
                                 property: "checked"
                                 value: rootItem.model ? rootItem.model.pointsVisible: 0
                                 delayed: true
                             }
                         }
                         RowLayout{
                             Text{
                                 text: "Point Color"
                                 color: Style.foregroundColor
                                 font.pointSize: 10 / Style.monitorRatio
                                 Layout.alignment: Qt.AlignTop
                                 Layout.preferredWidth: swtchWidth / Style.monitorRatio
                                 visible:rootItem.model ? rootItem.model.strokeStatus : false
                             }
                             GroupBox{

                                 padding: 0
                                 Layout.fillWidth: true
                                 Layout.margins: 0

                                 background: Rectangle{
                                     color: foregroundColor
                                     radius: 10 / Style.monitorRatio
                                     border.color: "transparent"
                                 }

                                 ColumnLayout{
                                     anchors.fill: parent

                                     RowLayout{
                                         Layout.fillWidth: true
                                         Layout.fillHeight: true
                                         Layout.topMargin: 2 / Style.monitorRatio

                                         RowLayout{
                                             spacing:3
                                             Layout.leftMargin: 2 / Style.monitorRatio
                                             Repeater{
                                                 model: ["red","orange","yellow","green","blue","white"]
                                                 Rectangle{
                                                     implicitWidth: 15 / Style.monitorRatio
                                                     implicitHeight: 15 / Style.monitorRatio
                                                     radius: width
                                                     color: modelData
                                                     MouseArea{
                                                         anchors.fill: parent
                                                         onClicked: {

                                                         }
                                                     }
                                                 }
                                             }

                                             Rectangle{
                                                 implicitWidth: 15 / Style.monitorRatio
                                                 implicitHeight: 15 / Style.monitorRatio
                                                 radius: width
                                                 border.width:1 / Style.monitorRatio
                                                 border.color:Style.foregroundColor
                                                 IconImage{
                                                     Layout.alignment: Qt.AlignCenter
                                                     width: 15 / Style.monitorRatio
                                                     height: 15 / Style.monitorRatio
                                                     source: "qrc:/Resources/location-add.png"
                                                 }
                                                 MouseArea{
                                                     anchors.fill: parent
                                                     onClicked: pointColorDialog.visible = true
                                                 }
                                             }
                                         }

                                         ColorDialog {
                                             visible: false
                                             id:  pointColorDialog
                                             title: "Please choose a color"
                                             onAccepted: {
                                                 var currentColor = pointColorDialog.selectedColor
                                                 currentColor.a = pointOpacityValue.value / 100
                                                 rootItem.model.pointsColor = currentColor
                                             }
                                         }
                                     }
                                     StepSpinBox {
                                         id: pointOpacityValue
                                         Layout.fillWidth: true
                                         editable: true
                                         stepComboboxVisible: false
                                         height: valHeight
                                         from : 0
                                         to : 100
                                         onValueChanged: {
                                             rootItem.model.pointsColor.a = value/100
                                         }
                                     }
                                     Binding{
                                         target: pointOpacityValue
                                         property: "value"
                                         value: rootItem.model ? rootItem.model.pointsColor.a * 100 : 0
                                         delayed: true
                                     }
                                 }
                             }
                         }

                         RowLayout{
                             Text {
                                 text: "Point Width "
                                 font.pointSize: 10 / Style.monitorRatio
                                 Layout.preferredWidth: swtchWidth
                                 color: Style.foregroundColor
                             }
                             StepSpinBox {
                                 id: pointWidthValue
                                 editable: true
                                 stepComboboxVisible: false
                                 enabled: pointVisible.checked
                                 Layout.fillWidth: true
                                 height: valHeight / Style.monitorRatio
                                 from: 0

                                 onValueChanged: {
                                     rootItem.model.pointsWidth = value
                                 }

                             }
                             Binding{
                                 target: pointWidthValue
                                 property: "value"
                                 value: rootItem.model ? rootItem.model.pointsWidth: 0
                                 delayed: true
                             }
                         }
                             RowLayout{
                                 Text {
                                     Layout.preferredWidth: swtchWidth
                                     text: "Point Smooth "
                                     font.pointSize: 10 / Style.monitorRatio
                                     color: Style.foregroundColor
                                 }
                                 Switch {
                                     id:pointSmoothValue
                                     enabled: pointVisible.checked
                                     padding: 0
                                     width: 100 / Style.monitorRatio
                                     height: valHeight
                                     checked: false
                                     onToggled: function() {
                                         rootItem.model.pointsSmooth  = pointSmoothValue.checked
                                     }
                                 }
                                 Binding{
                                     target: pointSmoothValue
                                     property: "checked"
                                     value: rootItem.model ? rootItem.model.pointsSmooth: 0
                                     delayed: true
                                 }
                             }
                     }

                }
            }
        }
    }
}


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
    property int swtchWidth: 100
    property int lblWidth: 85
    property int valHeight: 30
    property int previousIndex:0
    property int nextIndex:0
    property int strokePreviousIndex:0
    property int strokeNextIndex:0
    property int pointColorPreviousIndex: 0
    property int pointColorNextIndex: 0
    //////////////////////////Main Content////////////////////////
    Rectangle {
        id: item
        visible: true
        width: parent.width *0.9
        height: rootItem.height
        anchors.fill: parent
        color: "transparent"
        anchors.margins: 6 / Style.monitorRatio
        radius: 10 / Style.monitorRatio

        ColorBoxInput{
            id:colorBox
            z:mainLayout.z + 10
            x:nameSec.x
            y:nameSec.y
            width: frame.width
            visible: false
            onColorChosen: {
              colorSelectCircle.color = selectedColor
              rootItem.model.fillColor = selectedColor
              addIconImage.visible = false
              propertyCheckIcon.visible = true
              colorModel.setProperty(previousIndex,"checkIconVisible",false)
            }
        }

        ColorBoxInput{
            id:colorBoxStroke
            z:mainLayout.z + 10
            x:nameSec.x
            y:nameSec.y
            width: frame.width
            visible: false
            onColorChosen: {
                strokeColorCircle.color = selectedColor
                rootItem.model.strokeColor = selectedColor
                strokeColorAddIcon.visible = false
                strokeCheckIcon.visible = true
                strokeColorModel.setProperty(strokePreviousIndex,"checkIconVisible",false)
            }
        }

        ColorBoxInput{
            id:colorBoxPointColor
            z:mainLayout.z + 10
            x:nameSec.x
            y:nameSec.y
            width: frame.width
            visible: false
            Layout.fillWidth: true
            Layout.fillHeight: true
            onColorChosen: {
                pointColorCircle.color = selectedColor
                rootItem.model.pointsColor = selectedColor
                pointColorAddIcon.visible = false
                pointColorCheckIcon.visible = true
                pointColorModel.setProperty(pointColorPreviousIndex,"checkIconVisible",false)
                console.log(rootItem.model.pointsColor)
            }
        }

        ScrollView {
            id: frame
            clip: true
            contentWidth: parent.width
            anchors.fill: parent
            /////////////////////// components Grid ////////////////////////////
            ColumnLayout{
                id: mainLayout
                z:0
                anchors.fill: parent
                spacing: 2 / Style.monitorRatio
                ////////////////////////////////// Name ////////////////////////////////////////////////
                RowLayout{
                    id:nameSec
                    Layout.fillWidth: true
                    spacing: 0
                    Text {
                        Layout.preferredWidth: lblWidth  / Style.monitorRatio
                        text: "Name"
                        font.pixelSize: 17 / Style.monitorRatio
                        color: Style.foregroundColor
                    }
                    TextField {
                        id: namevalue
                        Layout.preferredWidth: 200 / Style.monitorRatio
                        Layout.minimumWidth: 100 / Style.monitorRatio
                        Layout.fillWidth: true
                        Layout.rightMargin: 15 / Style.monitorRatio
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

                RowLayout{
                    id:fillcolorSec
                    Layout.fillWidth: true
                    visible: rootItem.model ? rootItem.model.fillColorStatus : false
                    spacing:0
                    Text {
                        Layout.preferredWidth: lblWidth / Style.monitorRatio
                        text: "Color"
                        font.pixelSize: 17 / Style.monitorRatio
                        color: Style.foregroundColor
                    }
                    RowLayout{
                        Layout.rightMargin: 15 / Style.monitorRatio
                        Layout.fillWidth: true
                        spacing:3 / Style.monitorRatio
                        ListModel{
                            id:colorModel
                            ListElement{checkIconVisible:false; propertyColorSelect: "#EF2929"}
                            ListElement{checkIconVisible:false; propertyColorSelect: "#FCAF3E"}
                            ListElement{checkIconVisible:false; propertyColorSelect: "#FCE94F"}
                            ListElement{checkIconVisible:false; propertyColorSelect: "#8AE234"}
                            ListElement{checkIconVisible:false; propertyColorSelect: "#729FCF"}
                            ListElement{checkIconVisible:false; propertyColorSelect: "#FFFFFF"}
                        }

                        Repeater{
                            id:colorModelRepeater
                            model: colorModel
                            Button{
                                required property color propertyColorSelect
                                required property bool checkIconVisible
                                required property int index

                                checkable: true
                                implicitWidth: 26 / Style.monitorRatio
                                implicitHeight: 26 / Style.monitorRatio
                                background: Rectangle{
                                    radius: width
                                    color:propertyColorSelect
                                }
                                Image{
                                    anchors.fill: parent
                                    source: "qrc:/Resources/add-place-color-select.png"
                                    visible: checkIconVisible
                                }
                                onClicked:{
                                    addIconImage.visible = true
                                    propertyCheckIcon.visible = false
                                    colorSelectCircle.color = Style.backgroundColor

                                    colorModel.setProperty(previousIndex,"checkIconVisible",false)
                                    nextIndex = index
                                    if(previousIndex !== nextIndex){
                                        colorModelRepeater.itemAt(previousIndex).checked = false
                                    }
                                    colorModel.setProperty(nextIndex,"checkIconVisible",checked)
                                    if(checked){
                                        previousIndex = index
                                        rootItem.model.fillColor = propertyColorSelect
                                    }
                                    else{
                                        rootItem.model.fillColor = "#FFFFFF"
                                    }
                                }
                            }
                        }

                        Rectangle{
                            id:colorSelectCircle
                            implicitWidth: 26 / Style.monitorRatio
                            implicitHeight: 26 / Style.monitorRatio
                            radius: width
                            border.width:1 / Style.monitorRatio
                            border.color:Style.foregroundColor
                            IconImage{
                                id:addIconImage
                                anchors.centerIn: parent
                                width: 20 / Style.monitorRatio
                                height: 20 / Style.monitorRatio
                                source: "qrc:/Resources/location-add.png"
                            }
                            IconImage{
                                id:propertyCheckIcon
                                anchors.centerIn: parent
                                width: 20 / Style.monitorRatio
                                height: 20 / Style.monitorRatio
                                source: "qrc:/Resources/add-place-color-select.png"
                                visible: false
                            }

                            MouseArea{
                                anchors.fill: parent
                                onClicked: colorBox.visible = true
                            }
                        }
                    }
                }
                RowLayout{
                    spacing: 0
                    Text{
                        text: "Stroke"
                        color: Style.foregroundColor
                        font.pixelSize: 17 / Style.monitorRatio
                        Layout.alignment: Qt.AlignTop
                        Layout.preferredWidth: lblWidth / Style.monitorRatio
                        visible:rootItem.model ? rootItem.model.strokeStatus : false
                    }
                    GroupBox{
                        id: strokeSec
                        visible:rootItem.model ? rootItem.model.strokeStatus : false
                        Layout.rightMargin: 15 / Style.monitorRatio
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
                                    spacing: 3 / Style.monitorRatio
                                    Layout.leftMargin: 7 / Style.monitorRatio
                                    Layout.topMargin: 5 / Style.monitorRatio
                                    ListModel{
                                        id:strokeColorModel
                                        ListElement{checkIconVisible:false; propertyColorSelect: "#EF2929"}
                                        ListElement{checkIconVisible:false; propertyColorSelect: "#FCAF3E"}
                                        ListElement{checkIconVisible:false; propertyColorSelect: "#FCE94F"}
                                        ListElement{checkIconVisible:false; propertyColorSelect: "#8AE234"}
                                        ListElement{checkIconVisible:false; propertyColorSelect: "#729FCF"}
                                        ListElement{checkIconVisible:false; propertyColorSelect: "#FFFFFF"}
                                    }
                                    Repeater{
                                        id:strokeColorModelRepeater
                                        model: strokeColorModel
                                        Button{
                                            required property color propertyColorSelect
                                            required property bool checkIconVisible
                                            required property int index
                                            checkable: true
                                            implicitWidth: 26 / Style.monitorRatio
                                            implicitHeight: 26 / Style.monitorRatio
                                            background:Rectangle{
                                                radius: width
                                                color: propertyColorSelect
                                            }
                                            Image{
                                                anchors.centerIn: parent
                                                anchors.fill: parent
                                                source: "qrc:/Resources/add-place-color-select.png"
                                                visible: checkIconVisible
                                            }
                                          onClicked: {
                                              strokeColorAddIcon.visible = true
                                              strokeCheckIcon.visible = false
                                              strokeColorCircle.color = Style.backgroundColor

                                              strokeColorModel.setProperty(strokePreviousIndex,"checkIconVisible",false)
                                              strokeNextIndex = index
                                              if(strokePreviousIndex !== strokeNextIndex){
                                                  strokeColorModelRepeater.itemAt(strokePreviousIndex).checked = false
                                              }
                                              strokeColorModel.setProperty(strokeNextIndex,"checkIconVisible",checked)
                                              if(checked){
                                                  rootItem.model.strokeColor = propertyColorSelect
                                                  strokePreviousIndex = index
                                              }
                                              else{
                                                  rootItem.model.strokeColor = "#FFFFFF"
                                              }
                                          }
                                        }
                                    }

                                    Rectangle{
                                        id:strokeColorCircle
                                        implicitWidth: 26 / Style.monitorRatio
                                        implicitHeight: 26 / Style.monitorRatio
                                        radius: width
                                        border.width:1 / Style.monitorRatio
                                        border.color:Style.foregroundColor
                                        IconImage{
                                            id:strokeColorAddIcon
                                            anchors.centerIn: parent
                                            width: 20 / Style.monitorRatio
                                            height: 20 / Style.monitorRatio
                                            source: "qrc:/Resources/location-add.png"
                                        }
                                        IconImage{
                                            id:strokeCheckIcon
                                            anchors.fill: parent
                                            source: "qrc:/Resources/add-place-color-select.png"
                                            visible: false
                                        }
                                        MouseArea{
                                            anchors.fill: parent
                                            onClicked: colorBoxStroke.visible = true
                                        }
                                    }
                                }

                            FloatSpinbox {
                                id: strokeWidthValue
                                editable: true
                                Layout.fillWidth: true
                                Layout.alignment: Qt.AlignCenter
                                Layout.topMargin: 10 / Style.monitorRatio
                                Layout.bottomMargin: 5 / Style.monitorRatio
                                Layout.rightMargin: 5 / Style.monitorRatio
                                Layout.leftMargin: 5 / Style.monitorRatio
                                height: 20 / Style.monitorRatio
                                from: 0
                                to:20000000
                                stepSize: 1
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
                    spacing: 0
                    Text{
                        text: "Location"
                        color: Style.foregroundColor
                        font.pixelSize: 17 / Style.monitorRatio
                        Layout.alignment: Qt.AlignTop
                        Layout.preferredWidth: (lblWidth / Style.monitorRatio)
                        visible:rootItem.model ? rootItem.model.locationStatus : false
                    }

                    GroupBox{
                        id: locationSec
                        visible:rootItem.model ? rootItem.model.locationStatus : false
                        Layout.fillWidth: true
                        Layout.preferredWidth: 200 / Style.monitorRatio
                        Layout.rightMargin: 15 / Style.monitorRatio
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
                                    Layout.topMargin: 5 / Style.monitorRatio
                                    font.pointSize: 10 / Style.monitorRatio
                                    color: Style.foregroundColor
                                }

                                StepSpinBox {
                                    id: xLocationValue
                                    editable: true
                                    Layout.fillWidth: true
                                    Layout.minimumWidth: 100 / Style.monitorRatio
                                    Layout.rightMargin: 5 / Style.monitorRatio
                                    Layout.topMargin: 5 / Style.monitorRatio
                                    height: valHeight / Style.monitorRatio
                                    decimals: 4
                                    from: -180
                                    to: 180
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
                                    Layout.topMargin: 5 / Style.monitorRatio
                                    font.pointSize: 10 / Style.monitorRatio
                                    color: Style.foregroundColor
                                }
                                StepSpinBox {
                                    id: yLocationValue
                                    editable: true
                                    Layout.fillWidth: true
                                    Layout.minimumWidth: 100 / Style.monitorRatio
                                    Layout.topMargin: 5 / Style.monitorRatio
                                    Layout.rightMargin: 5 / Style.monitorRatio
                                    height: valHeight / Style.monitorRatio
                                    decimals: 4
                                    from: -180
                                    to: 180
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
                                    Layout.topMargin: 5 / Style.monitorRatio
                                    font.pointSize: 10
                                    color: Style.foregroundColor
                                }
                                StepSpinBox {

                                    id: zLocationValue
                                    editable: true
                                    Layout.fillWidth: true
                                    Layout.minimumWidth: 100 / Style.monitorRatio
                                    Layout.topMargin: 5 / Style.monitorRatio
                                    Layout.rightMargin: 5 / Style.monitorRatio
                                    height: valHeight / Style.monitorRatio
                                    decimals: 4
                                    from: -180
                                    to: 180
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
                                    implicitWidth: 11 / Style.monitorRatio
                                    implicitHeight: 11 / Style.monitorRatio
                                    x: relative.leftPadding  / Style.monitorRatio
                                    y: (parent.height / 2 - height / 2) / Style.monitorRatio
                                    radius: (height/2) / Style.monitorRatio
                                    border.color: relative.down ? "black" : "#313131"

                                    Rectangle {
                                        width: 11 / Style.monitorRatio
                                        height: 11 / Style.monitorRatio
                                        radius: (height/2) / Style.monitorRatio
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

                ///////////-----------------------center-------------------//////////////

                RowLayout{
                    spacing: 0
                    Text{
                        text: "Center"
                        color: Style.foregroundColor
                        font.pixelSize: 17 / Style.monitorRatio
                        Layout.alignment: Qt.AlignTop
                        Layout.preferredWidth: lblWidth / Style.monitorRatio
                        visible:rootItem.model ? rootItem.model.centerStatus : false
                    }
                    GroupBox{
                        id: centerSec
                        visible:rootItem.model ? rootItem.model.centerStatus : false
                        padding: 0
                        Layout.fillWidth: true
                        Layout.rightMargin: 15 / Style.monitorRatio
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
                                Layout.topMargin: 5 / Style.monitorRatio
                                color: Style.foregroundColor
                            }

                            StepSpinBox {
                                id: xCenterValue
                                editable: true
                                Layout.fillWidth: true
                                Layout.minimumWidth: 100 / Style.monitorRatio
                                Layout.rightMargin: 5 / Style.monitorRatio
                                Layout.topMargin: 5 / Style.monitorRatio
                                height: valHeight / Style.monitorRatio
                                decimals: 2
                                from: -180
                                to: 180
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
                                Layout.topMargin: 5 / Style.monitorRatio
                                color: Style.foregroundColor
                            }

                            StepSpinBox {
                                id: yCenterValue
                                editable: true
                                Layout.fillWidth: true
                                Layout.minimumWidth: 100 / Style.monitorRatio
                                Layout.topMargin: 5 / Style.monitorRatio
                                Layout.rightMargin: 5 / Style.monitorRatio
                                height: valHeight / Style.monitorRatio
                                decimals: 2
                                from: -180
                                to: 180
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
                                Layout.topMargin: 5 / Style.monitorRatio
                                color: Style.foregroundColor
                            }

                            StepSpinBox {
                                id: zCenterValue
                                editable: true
                                Layout.fillWidth: true
                                Layout.minimumWidth: 100 / Style.monitorRatio
                                Layout.rightMargin: 5 / Style.monitorRatio
                                Layout.topMargin: 5 / Style.monitorRatio
                                Layout.bottomMargin: 5 / Style.monitorRatio
                                height: valHeight / Style.monitorRatio
                                decimals: 2
                                from: -180
                                to: 180
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
                    spacing: 0
                    Text{
                        text: "Arc"
                        color: Style.foregroundColor
                        font.pixelSize: 17 / Style.monitorRatio
                        Layout.alignment: Qt.AlignTop
                        Layout.preferredWidth: lblWidth / Style.monitorRatio
                        visible:rootItem.model ? rootItem.model.arcStatus : false
                    }
                    GroupBox{
                        id: arcSec
                        visible:rootItem.model ? rootItem.model.arcStatus : false
                        padding: 0
                        Layout.fillWidth: true
                        Layout.rightMargin: 15 / Style.monitorRatio
                        background: Rectangle{
                            color: foregroundColor
                            border.color: "transparent"
                            radius:10 / Style.monitorRatio
                        }

                        GridLayout{
                            anchors.fill: parent
                            columnSpacing: 1 / Style.monitorRatio
                            rowSpacing: 1 / Style.monitorRatio
                            columns:2

                            Text{
                                Layout.preferredWidth: lblWidth / Style.monitorRatio
                                text: "Start "
                                padding: 5 / Style.monitorRatio
                                font.pixelSize: 17 / Style.monitorRatio
                                color: Style.foregroundColor
                            }
                            FloatSpinbox {
                                id: startArcValue
                                editable: true
                                Layout.fillWidth: true
                                Layout.minimumWidth: 100 / Style.monitorRatio
                                Layout.rightMargin: 5 / Style.monitorRatio
                                Layout.topMargin: 2 / Style.monitorRatio
                                value: 0
                                stepSize: 1
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
                                font.pixelSize: 17 / Style.monitorRatio
                                color: Style.foregroundColor
                            }
                            FloatSpinbox {
                                id: endArcValue
                                editable: true
                                Layout.fillWidth: true
                                Layout.minimumWidth: 100 / Style.monitorRatio
                                Layout.rightMargin: 5 / Style.monitorRatio
                                Layout.bottomMargin: 2 / Style.monitorRatio
                                value: 360
                                stepSize: 1
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


                    RowLayout{
                        id:radiusSec
                        visible:rootItem.model ? rootItem.model.radiusStatus : false
                        spacing: 0
                        Text {
                            Layout.preferredWidth: lblWidth / Style.monitorRatio
                            text: "Radius"
                            font.pixelSize: 17 / Style.monitorRatio
                            color: Style.foregroundColor
                        }
                        Rectangle{
                            color: foregroundColor
                            radius: 10 / Style.monitorRatio
                            border.color: "transparent"
                            Layout.fillWidth: true
                            Layout.preferredHeight: valHeight / Style.monitorRatio
                            Layout.rightMargin: 15 / Style.monitorRatio

                            StepSpinBox {
                                id: radiusValue
                                editable: true
                                anchors.leftMargin: 5 / Style.monitorRatio
                                anchors.rightMargin: 5 / Style.monitorRatio
                                anchors.left: parent.left
                                anchors.right: parent.right
                                anchors.verticalCenter: parent.verticalCenter
                                height: 30 / Style.monitorRatio
                                decimals: 2
                                from : 0
                                to: 20000000
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


                ////////------------------------- radiusMinor------------------//////////////

                RowLayout{
                    id:radiusMinorSec
                    visible:rootItem.model ? rootItem.model.radiusMinorStatus : false
                    spacing: 0
                    Text {
                        Layout.preferredWidth: lblWidth / Style.monitorRatio
                        text: "Radius Minor "
                        font.pixelSize: 14/ Style.monitorRatio
                        color: Style.foregroundColor
                    }

                    Rectangle{
                        color: foregroundColor
                        radius: 10 / Style.monitorRatio
                        border.color: "transparent"
                        Layout.fillWidth: true
                        Layout.preferredHeight: valHeight / Style.monitorRatio
                        Layout.rightMargin: 15 / Style.monitorRatio

                        StepSpinBox {
                            id: radiusMinorValue
                            editable: true
                            anchors.leftMargin: 5 / Style.monitorRatio
                            anchors.rightMargin: 5 / Style.monitorRatio
                            anchors.left: parent.left
                            anchors.right: parent.right
                            anchors.verticalCenter: parent.verticalCenter
                            height: 20 / Style.monitorRatio
                            decimals: 2
                            from : 0
                            to: 20000000
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
                }

                ////////------------------------- radiusMajor------------------//////////////

                RowLayout{
                    id:radiusMajorSec
                    visible:rootItem.model ? rootItem.model.radiusMajorStatus : false
                    spacing: 0
                    Text {
                        Layout.preferredWidth: lblWidth / Style.monitorRatio
                        text: "Radius Major "
                        font.pixelSize: 14 / Style.monitorRatio
                        color: Style.foregroundColor
                    }
                    Rectangle{
                        color: foregroundColor
                        radius: 10 / Style.monitorRatio
                        border.color: "transparent"
                        Layout.fillWidth: true
                        Layout.preferredHeight: valHeight / Style.monitorRatio
                        Layout.rightMargin: 15 / Style.monitorRatio

                        StepSpinBox {
                            id: radiusMajorValue
                            editable: true
                            height: 20 / Style.monitorRatio
                            anchors.leftMargin: 5 / Style.monitorRatio
                            anchors.rightMargin: 5 / Style.monitorRatio
                            anchors.left: parent.left
                            anchors.right: parent.right
                            anchors.verticalCenter: parent.verticalCenter
                            decimals: 2
                            from : 0
                            to: 20000000
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

                }
                ////////------------------------- height------------------//////////////

                    RowLayout{
                        id:heightSec
                        visible:rootItem.model ? rootItem.model.heightStatus : false
                        spacing: 0
                        Text {
                            Layout.preferredWidth: lblWidth / Style.monitorRatio
                            text: "Height "
                            font.pixelSize: 17 / Style.monitorRatio
                            color: Style.foregroundColor
                            Layout.alignment: Qt.AlignVCenter
                        }
                        Rectangle{
                            color: foregroundColor
                            radius: 10 / Style.monitorRatio
                            border.color: "transparent"
                            Layout.rightMargin: 15 / Style.monitorRatio
                            Layout.fillWidth: true
                            Layout.alignment: Qt.AlignVCenter
                            Layout.preferredHeight: valHeight / Style.monitorRatio
                            Layout.preferredWidth: 200 / Style.monitorRatio
                            StepSpinBox {
                                id:heightValue
                                editable: true
                                anchors.leftMargin: 5 / Style.monitorRatio
                                anchors.rightMargin: 5 / Style.monitorRatio
                                anchors.left: parent.left
                                anchors.right: parent.right
                                height: 30 / Style.monitorRatio
                                decimals: 2
                                from : 0
                                to: 20000000
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

                ////////------------------------- lenght ------------------//////////////

                        RowLayout{
                            id:lenghtSec
                            spacing: 0
                            visible: rootItem.model ? rootItem.model.lenghtStatus : false
                            Text {
                                Layout.preferredWidth: lblWidth / Style.monitorRatio
                                text:  "Length "
                                font.pixelSize: 17 / Style.monitorRatio
                                color: Style.foregroundColor
                            }

                            Rectangle{
                                color: foregroundColor
                                radius: 10
                                border.color: "transparent"
                                Layout.fillWidth: true
                                Layout.preferredHeight: valHeight / Style.monitorRatio
                                Layout.rightMargin: 15 / Style.monitorRatio

                                StepSpinBox {
                                    id: lengthValue
                                    editable: true
                                    anchors.leftMargin: 5 / Style.monitorRatio
                                    anchors.rightMargin: 5 / Style.monitorRatio
                                    anchors.left: parent.left
                                    anchors.right: parent.right
                                    height: 30 / Style.monitorRatio
                                    decimals: 2
                                    from : 0
                                    to: 20000000
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


                ////////------------------------- width ------------------//////////////

                        RowLayout{
                            id:widthSec
                            visible:rootItem.model ? rootItem.model.widthStatus : false
                            spacing: 0
                            Text {
                                Layout.preferredWidth: lblWidth / Style.monitorRatio
                                text: "Width "
                                font.pixelSize: 17 / Style.monitorRatio
                                color: Style.foregroundColor
                            }
                            Rectangle{
                                color: foregroundColor
                                radius: 10 / Style.monitorRatio
                                border.color: "transparent"
                                Layout.fillWidth: true
                                Layout.preferredHeight: valHeight / Style.monitorRatio
                                Layout.rightMargin: 15 / Style.monitorRatio
                                StepSpinBox {
                                    id:widthValue
                                    editable: true
                                    anchors.leftMargin: 5 / Style.monitorRatio
                                    anchors.rightMargin: 5 / Style.monitorRatio
                                    anchors.left: parent.left
                                    anchors.right: parent.right
                                    anchors.verticalCenter: parent.verticalCenter
                                    height: 30 / Style.monitorRatio
                                    decimals: 2
                                    from : 0
                                    to: 20000000
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

                // --------------------------------- clamp -----------------------////////////

                RowLayout{
                    id:clampSec
                    spacing: 0
                    visible:rootItem.model ? rootItem.model.clampStatus : false
                    Text {
                        Layout.preferredWidth: lblWidth / Style.monitorRatio
                        text: "Clamp "
                        font.pixelSize: 17 / Style.monitorRatio
                        color: Style.foregroundColor
                    }

                    Rectangle{
                        color: foregroundColor
                        radius: 10 / Style.monitorRatio
                        border.color: "transparent"
                        Layout.fillWidth: true
                        Layout.rightMargin: 15 / Style.monitorRatio
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
                    spacing: 0
                    Text {
                        Layout.preferredWidth: lblWidth / Style.monitorRatio
                        text: "Tesselation "
                        font.pixelSize: 17 / Style.monitorRatio
                        color: Style.foregroundColor
                    }

                    Rectangle{
                        color: foregroundColor
                        radius: 10 / Style.monitorRatio
                        border.color: "transparent"
                        Layout.fillWidth: true
                        Layout.preferredHeight: valHeight / Style.monitorRatio
                        Layout.rightMargin: 15 / Style.monitorRatio

                        FloatSpinbox {
                            id: tesselationValue
                            editable: true
                            anchors.left: parent.left
                            anchors.right: parent.right
                            anchors.leftMargin: 5 / Style.monitorRatio
                            anchors.rightMargin: 5 / Style.monitorRatio
                            anchors.verticalCenter: parent.verticalCenter
                            stepSize: 1
                            from : 0
                            to: 20000000
                            height: 20 / Style.monitorRatio
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
                }
                ////------------------------ Show lenght -------------------- ///////////////

                RowLayout{
                    id:showLenghtSec
                    visible:rootItem.model ? rootItem.model.showLenStatus : false
                    spacing: 0

                    Text {
                        Layout.preferredWidth: swtchWidth / Style.monitorRatio
                        text: "Show Length "
                        font.pixelSize: 17 / Style.monitorRatio
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
                    spacing: 0
                    Text {
                        Layout.preferredWidth: swtchWidth / Style.monitorRatio
                        text: "Show Bearing "
                        font.pixelSize: 17 / Style.monitorRatio
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
                    spacing: 0
                    Text {
                        Layout.preferredWidth: swtchWidth / Style.monitorRatio
                        text: "Show Altitude "
                        font.pixelSize: 17 / Style.monitorRatio
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
                    spacing: 0
                    Text {
                        Layout.preferredWidth: swtchWidth / Style.monitorRatio
                        text: "Show Slope "
                        font.pixelSize: 17 / Style.monitorRatio
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
                              spacing: 0
                             Text {
                                 Layout.preferredWidth: swtchWidth / Style.monitorRatio
                                 text: "Points "
                                 font.pixelSize: 17 / Style.monitorRatio
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
                             spacing: 0
                             Text{
                                 text: "Point Color"
                                 color: Style.foregroundColor
                                 font.pixelSize: 17 / Style.monitorRatio
                                 Layout.alignment: Qt.AlignTop
                                 Layout.preferredWidth: lblWidth / Style.monitorRatio
                                 visible:rootItem.model ? rootItem.model.strokeStatus : false
                             }
                             GroupBox{
                                 padding: 0
                                 Layout.fillWidth: true
                                 Layout.margins: 0
                                 Layout.rightMargin: 15 / Style.monitorRatio
                                 enabled: pointVisible.checked
                                 background: Rectangle{
                                     color: foregroundColor
                                     radius: 10 / Style.monitorRatio
                                     border.color: "transparent"
                                 }

                                 ColumnLayout{
                                     anchors.fill: parent

                                         RowLayout{
                                             spacing:3 / Style.monitorRatio
                                             Layout.leftMargin: 7 / Style.monitorRatio
                                             Layout.topMargin: 5 / Style.monitorRatio

                                             ListModel{
                                                 id:pointColorModel
                                                 ListElement{checkIconVisible:false; propertyColorSelect: "#EF2929"}
                                                 ListElement{checkIconVisible:false; propertyColorSelect: "#FCAF3E"}
                                                 ListElement{checkIconVisible:false; propertyColorSelect: "#FCE94F"}
                                                 ListElement{checkIconVisible:false; propertyColorSelect: "#8AE234"}
                                                 ListElement{checkIconVisible:false; propertyColorSelect: "#729FCF"}
                                                 ListElement{checkIconVisible:false; propertyColorSelect: "#FFFFFF"}
                                             }
                                             Repeater{
                                                 id:pointColorRepeater
                                                 model: pointColorModel
                                                 Button{
                                                     required property color propertyColorSelect
                                                     required property bool checkIconVisible
                                                     required property int index
                                                     checkable: true
                                                     implicitWidth: 26 / Style.monitorRatio
                                                     implicitHeight: 26 / Style.monitorRatio
                                                     background:Rectangle{
                                                         radius: width
                                                         color: propertyColorSelect
                                                     }
                                                     Image{
                                                         anchors.fill: parent
                                                         source: "qrc:/Resources/add-place-color-select.png"
                                                         visible: checkIconVisible
                                                     }

                                                    onClicked: {
                                                        pointColorAddIcon.visible = true
                                                        pointColorCheckIcon.visible = false
                                                        pointColorCircle.color = Style.backgroundColor
                                                        pointColorModel.setProperty(pointColorPreviousIndex,"checkIconVisible",false)
                                                        pointColorNextIndex = index
                                                        if(pointColorPreviousIndex !== pointColorNextIndex){
                                                            pointColorRepeater.itemAt(pointColorPreviousIndex).checked = false
                                                        }
                                                        pointColorModel.setProperty(pointColorNextIndex,"checkIconVisible",checked)
                                                        if(checked){
                                                            pointColorPreviousIndex = index
                                                            rootItem.model.pointsColor = propertyColorSelect
                                                        }
                                                        else{
                                                            rootItem.model.pointsColor = "#FFFFFF"
                                                        }
                                                    }
                                                 }
                                             }

                                             Rectangle{
                                                 id:pointColorCircle
                                                 implicitWidth: 26 / Style.monitorRatio
                                                 implicitHeight: 26 / Style.monitorRatio
                                                 radius: width
                                                 border.width:1 / Style.monitorRatio
                                                 border.color:Style.foregroundColor
                                                 IconImage{
                                                     id:pointColorAddIcon
                                                     anchors.centerIn: parent
                                                     width: 20 / Style.monitorRatio
                                                     height: 20 / Style.monitorRatio
                                                     source: "qrc:/Resources/location-add.png"
                                                 }
                                                 IconImage{
                                                     id:pointColorCheckIcon
                                                     anchors.fill: parent
                                                     source: "qrc:/Resources/add-place-color-select.png"
                                                     visible: false
                                                 }
                                                 MouseArea{
                                                     anchors.fill: parent
                                                     onClicked: colorBoxPointColor.visible = true
                                                 }
                                             }
                                         }
                                     FloatSpinbox {
                                         id: pointOpacityValue
                                         Layout.fillWidth: true
                                         Layout.preferredHeight: 20 / Style.monitorRatio
                                         Layout.bottomMargin: 5 / Style.monitorRatio
                                         Layout.topMargin: 10 / Style.monitorRatio
                                         Layout.rightMargin: 5 / Style.monitorRatio
                                         Layout.leftMargin: 5 / Style.monitorRatio
                                         editable: true
                                         stepSize: 1
                                         height: 20 / Style.monitorRatio
                                         from : 0
                                         to : 100
                                         onValueChanged: {
                                             rootItem.model.pointsColor.a = value/100
                                             console.log(rootItem.model.pointsColor.a)
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
                             spacing: 0
                             Text {
                                 text: "Point Width "
                                 font.pixelSize: 17 / Style.monitorRatio
                                 Layout.preferredWidth: lblWidth / Style.monitorRatio
                                 color: Style.foregroundColor
                             }
                             Rectangle{
                                 color: foregroundColor
                                 radius: 10 / Style.monitorRatio
                                 border.color: "transparent"
                                 Layout.rightMargin: 15 / Style.monitorRatio
                                 Layout.fillWidth: true
                                 Layout.alignment: Qt.AlignVCenter
                                 Layout.preferredHeight: valHeight / Style.monitorRatio
                                 Layout.preferredWidth: 200 / Style.monitorRatio

                                 FloatSpinbox {
                                     id: pointWidthValue
                                     editable: true
                                     enabled: pointVisible.checked
                                     anchors.leftMargin: 5 / Style.monitorRatio
                                     anchors.rightMargin: 5 / Style.monitorRatio
                                     anchors.left: parent.left
                                     anchors.right: parent.right
                                     anchors.verticalCenter: parent.verticalCenter
                                     height: 20 / Style.monitorRatio
                                     stepSize: 1
                                     from: 0
                                     to:20000000
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

                         }
                             RowLayout{
                                 spacing: 0
                                 Text {
                                     Layout.preferredWidth: swtchWidth / Style.monitorRatio
                                     text: "Point Smooth "
                                     font.pixelSize: 17 / Style.monitorRatio
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


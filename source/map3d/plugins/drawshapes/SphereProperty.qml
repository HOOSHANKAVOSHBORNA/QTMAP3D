import QtQuick 2.13
import QtQuick.Window 2.12
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.2
import Crystal 1.0

Item {
    id: rootItem
    implicitHeight: parent.height


        function show() {
            dialog.visible = true
            dialog.anchors.centerIn = parent
        }

        function hide() {
            dialog.close();
        }


        property SphereProperties sphereProperties



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
                ScrollBar.vertical.policy: ScrollBar.AlwaysOn
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

                            ///////////////////////////////////Color Property//////////////////////////////////
                            Rectangle{
                                Layout.fillWidth: true
                                color: "#313131"
                                height: 35
                                border.color: "#5f5f5f"
                                border.width: 1


                                Rectangle{
                                    height: 20
                                    width: 20
                                    color: sphereProperties.color
                                    border.width: 2
                                    border.color: "#c9c9c9"
                                    x: 5
                                    radius: 5
                                    anchors.verticalCenter: parent.verticalCenter

                                    MouseArea{
                                        anchors.fill: parent
                                        cursorShape: Qt.PointingHandCursor
                                        onClicked: colorDialog.visible = true

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
                                        id: colorField
                                        text: sphereProperties.color
                                        anchors.centerIn: parent
                                        font.pointSize: 10

                                    }
                                }
                                ColorDialog {
                                    visible: false
                                    id: colorDialog
                                    title: "Please choose a color"
                                    onAccepted: {
                                        //                                        dialog.myColor = colorDialog.color
                                        sphereProperties.color = colorDialog.color
                                    }

                                }
                            }
                            Rectangle{
                                Layout.fillWidth: true
                                color: "#313131"
                                height: 35
                                border.color: "#5f5f5f"
                                border.width: 1

                                Text {
                                    id: colorSphere
                                    text: qsTr("Color:")
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
                                color: "#313131"
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

                                            TextInput {
                                                id: mlocationX
                                                text: sphereProperties.location.x
                                                anchors.fill: parent
                                                font.pointSize: 10
                                                selectByMouse: true
                                                selectionColor: "dark green"
                                                validator: DoubleValidator {
                                                    decimals: 5;
                                                    notation: DoubleValidator.StandardNotation
                                                    locale: "insert x"
                                                }
                                                onTextChanged: {
                                                    sphereProperties.location.x = text
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
                                                text: sphereProperties.location.y
                                                anchors.fill: parent
                                                font.pointSize: 10
                                                selectByMouse: true
                                                selectionColor: "dark green"
                                                validator: DoubleValidator {
                                                    decimals: 5;
                                                    notation: DoubleValidator.StandardNotation
                                                    locale: "insert y"
                                                }
                                                onTextChanged: {
                                                    sphereProperties.location.y = text
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
                                                text: sphereProperties.location.z
                                                anchors.fill: parent
                                                font.pointSize: 10
                                                selectByMouse: true
                                                selectionColor: "dark green"
                                                validator: DoubleValidator {
                                                    decimals: 5;
                                                    notation: DoubleValidator.StandardNotation
                                                    locale: "insert z"
                                                }
                                                onTextChanged: {
                                                    sphereProperties.location.z = text
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                            Rectangle{
                                id: locationTitle
                                Layout.fillWidth: true
                                color: "#313131"
                                height: 80
                                border.color: "#5f5f5f"
                                border.width: 1

                                Text {
                                    id: locationSphere
                                    text: qsTr("Location:")
                                    font.pointSize: 10
                                    color: "white"
                                    y: 10
                                    x:7
                                }
                                CheckBox {
                                    id: relative
                                    text: qsTr("Relative")
                                    checked: sphereProperties.relative
                                    anchors.bottom: locationTitle.bottom
                                    onCheckStateChanged: if(checked === true){
                                                             sphereProperties.relative = true
                                                         }
                                                         else{
                                                             sphereProperties.relative = false
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
                            ///////////////////////////////////Center Propertiess///////////////////////////////
                            Rectangle{
                                Layout.fillWidth: true
                                color: "#313131"
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

                                            TextInput {
                                                id: mcenterX
                                                text: sphereProperties.center.x
                                                anchors.fill: parent
                                                font.pointSize: 10
                                                selectByMouse: true
                                                selectionColor: "dark green"
                                                validator: DoubleValidator {
                                                    decimals: 8;
                                                    notation: DoubleValidator.StandardNotation
                                                    locale: "insert x"
                                                }
                                                onTextChanged: {
                                                    sphereProperties.center.x = text
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
                                                id: mcenterY
                                                text: sphereProperties.center.y
                                                anchors.fill: parent
                                                font.pointSize: 10
                                                selectByMouse: true
                                                selectionColor: "dark green"
                                                validator: DoubleValidator {
                                                    decimals: 8;
                                                    notation: DoubleValidator.StandardNotation
                                                    locale: "insert y"
                                                }
                                                onTextChanged: {
                                                    sphereProperties.center.y = text
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
                                                id: mcenterZ
                                                text: sphereProperties.center.z
                                                anchors.fill: parent
                                                font.pointSize: 10
                                                selectByMouse: true
                                                selectionColor: "dark green"
                                                validator: DoubleValidator {
                                                    decimals: 8;
                                                    notation: DoubleValidator.StandardNotation
                                                    locale: "insert z"
                                                }
                                                onTextChanged: {
                                                    sphereProperties.center.z = text
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                            Rectangle{
                                Layout.fillWidth: true
                                color: "#313131"
                                height: 80
                                border.color: "#5f5f5f"
                                border.width: 1

                                Text {
                                    id: centerSphere
                                    text: qsTr("Center:")
                                    font.pointSize: 10
                                    color: "white"
                                    anchors.verticalCenter:  parent.verticalCenter
                                    x:7
                                }
                            }
                            ///////////////////////////////////Radius///////////////////////////////////////////
                            Rectangle{
                                Layout.fillWidth: true
                                color: "#313131"
                                height: 30
                                border.color: "#5f5f5f"
                                border.width: 1


                                Rectangle{
                                    height: 20
                                    width: 75
                                    color: "#c9c9c9"
                                    clip:  true
                                    radius: 5
                                    anchors.verticalCenter:  parent.verticalCenter
                                    x: 28

                                    TextInput {
                                        id: radiusValue
                                        text: sphereProperties.radius
                                        anchors.fill: parent
                                        font.pointSize: 10
                                        selectByMouse: true
                                        selectionColor: "dark green"
                                        validator: DoubleValidator {
                                            decimals: 8;
                                            notation: DoubleValidator.StandardNotation
                                            locale: "insert radius"
                                        }
                                        onTextChanged: {
                                            sphereProperties.radius = text
                                        }
                                    }
                                }
                            }
                            Rectangle{
                                Layout.fillWidth: true
                                color: "#313131"
                                height: 30
                                border.color: "#5f5f5f"
                                border.width: 1

                                Text {
                                    id: radiusSphere
                                    text: qsTr("Radius:")
                                    font.pointSize: 10
                                    color: "white"
                                    anchors.verticalCenter:  parent.verticalCenter
                                    x:7
                                }
                            }
                            ///////////////////////////////////Transparency/////////////////////////////////////

                            Rectangle{
                                id: transContainer
                                Layout.fillWidth: true
                                color: "#313131"
                                height: 30
                                border.color: "#5f5f5f"
                                border.width: 1

                                SpinBox {
                                    id: transValue
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
                                        z: 2
                                        text: transValue.textFromValue(transValue.value, transValue.locale)
                                        font: transValue.font
                                        color: "#313131"
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
                                                sphereProperties.transparency = transValue.value
                                            }
                                        }
                                    }
                                    up.indicator: Rectangle {
                                        x: transValue.mirrored ? 0 : parent.width - width
                                        height: parent.height
                                        implicitWidth: 20
                                        implicitHeight: 20
                                        color: transValue.up.pressed ? "#5f5f5f" : "#313131"
                                        border.color: enabled ? "#313131" : "#5f5f5f"

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
                                        color: transValue.down.pressed ? "#5f5f5f" : "#313131"
                                        border.color: enabled ? "#313131" : "#5f5f5f"

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
                                Layout.fillWidth: true
                                color: "#313131"
                                height: 30
                                border.color: "#5f5f5f"
                                border.width: 1

                                Text {
                                    id: transSphere
                                    text: qsTr("Transparency:")
                                    font.pointSize: 10
                                    color: "white"
                                    anchors.verticalCenter:  parent.verticalCenter
                                    x:7
                                }
                            }
                            ///////////////////////////////////Type Icons ///////////////////////////////////////
                            Rectangle{
                                Layout.fillWidth: true
                                color: "#313131"
                                height: 30
                                border.color: "#5f5f5f"
                                border.width: 1

                                Row{
                                    spacing: 5
                                    anchors.centerIn: parent
                                    Rectangle{
                                        width: 20
                                        height: 20
                                        color: "#5f5f5f"
                                        radius: 4

                                        Image {
                                            id: globe
                                            source: "qrc:/resources/globe.png"
                                            sourceSize: "20 x 20"
                                            anchors.centerIn: parent
                                        }
                                        MouseArea{
                                            anchors.fill: parent
                                            cursorShape: Qt.PointingHandCursor
                                            onClicked: {
                                                sphereProperties.shape = 0
                                            }
                                        }
                                    }
                                    Rectangle{
                                        width: 20
                                        height: 20
                                        color: "#5f5f5f"
                                        radius: 4

                                        Image {
                                            id: tophalf
                                            source: "qrc:/resources/topHalf.png"
                                            sourceSize: "20 x 20"
                                            anchors.centerIn: parent
                                        }
                                        MouseArea{
                                            anchors.fill: parent
                                            cursorShape: Qt.PointingHandCursor
                                            onClicked: {
                                                sphereProperties.shape = 1
                                            }
                                        }
                                    }
                                    Rectangle{
                                        width: 20
                                        height: 20
                                        color: "#5f5f5f"
                                        radius: 4

                                        Image {
                                            id: bothalf
                                            source: "qrc:/resources/botHalf.png"
                                            sourceSize: "20 x 20"
                                            anchors.centerIn: parent
                                        }
                                        MouseArea{
                                            anchors.fill: parent
                                            cursorShape: Qt.PointingHandCursor
                                            onClicked: {
                                                sphereProperties.shape = 2
                                            }
                                        }
                                    }
                                }
                            }
                            Rectangle{
                                Layout.fillWidth: true
                                color: "#313131"
                                height: 30
                                border.color: "#5f5f5f"
                                border.width: 1

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

//import QtQuick 2.13
//import QtQuick.Window 2.12
//import QtQuick.Controls 2.13
//import QtQuick.Layouts 1.12
//import QtQuick.Dialogs 1.2
//import Crystal 1.0

//Item {
//    id: rootItem
//    function show() {
//        dialog.open()
//    }

//    function hide() {
//        dialog.close();
//    }


//    property SphereProperties sphereProperties

//    Dialog {
//        id: dialog
//        width: 250
//        height: 300
//        title: qsTr("Sphere Properties")





//        //////////////////////////Main Content////////////////////////
//        contentItem:  Rectangle {
//            id: item
//            visible: true
//            width: 240
//            height: 300
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
//                    contentHeight: 300
//                    width: 240
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

//                            ///////////////////////////////////Color Property//////////////////////////////////
//                            Rectangle{
//                                Layout.fillWidth: true
//                                color: "#313131"
//                                height: 35
//                                border.color: "#5f5f5f"
//                                border.width: 1


//                                Rectangle{
//                                    height: 20
//                                    width: 20
//                                    color: sphereProperties.color
//                                    border.width: 2
//                                    border.color: "#c9c9c9"
//                                    x: 5
//                                    radius: 5
//                                    anchors.verticalCenter: parent.verticalCenter

//                                    MouseArea{
//                                        anchors.fill: parent
//                                        cursorShape: Qt.PointingHandCursor
//                                        onClicked: colorDialog.visible = true

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
//                                        id: colorField
//                                        text: sphereProperties.color
//                                        anchors.centerIn: parent
//                                        font.pointSize: 10

//                                    }
//                                }
//                                ColorDialog {
//                                    visible: false
//                                    id: colorDialog
//                                    title: "Please choose a color"
//                                    onAccepted: {
////                                        dialog.myColor = colorDialog.color
//                                        sphereProperties.color = colorDialog.color
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
//                                    id: colorSphere
//                                    text: qsTr("Color:")
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

//                                            TextInput {
//                                                id: mlocationX
//                                                text: sphereProperties.location.x
//                                                anchors.fill: parent
//                                                font.pointSize: 10
//                                                selectByMouse: true
//                                                selectionColor: "dark green"
//                                                validator: DoubleValidator {
//                                                    decimals: 5;
//                                                    notation: DoubleValidator.StandardNotation
//                                                    locale: "insert x"
//                                                }
//                                                onTextChanged: {
//                                                    sphereProperties.location.x = text
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
//                                                text: sphereProperties.location.y
//                                                anchors.fill: parent
//                                                font.pointSize: 10
//                                                selectByMouse: true
//                                                selectionColor: "dark green"
//                                                validator: DoubleValidator {
//                                                    decimals: 5;
//                                                    notation: DoubleValidator.StandardNotation
//                                                    locale: "insert y"
//                                                }
//                                                onTextChanged: {
//                                                    sphereProperties.location.y = text
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
//                                                text: sphereProperties.location.z
//                                                anchors.fill: parent
//                                                font.pointSize: 10
//                                                selectByMouse: true
//                                                selectionColor: "dark green"
//                                                validator: DoubleValidator {
//                                                    decimals: 5;
//                                                    notation: DoubleValidator.StandardNotation
//                                                    locale: "insert z"
//                                                }
//                                                onTextChanged: {
//                                                    sphereProperties.location.z = text
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
//                                    id: locationSphere
//                                    text: qsTr("Location:")
//                                    font.pointSize: 10
//                                    color: "white"
//                                    y: 10
//                                    x:7
//                                }
//                                CheckBox {
//                                    id: relative
//                                    text: qsTr("Relative")
//                                    checked: sphereProperties.relative
//                                    anchors.bottom: locationTitle.bottom
//                                    onCheckStateChanged: if(checked === true){
//                                                             sphereProperties.relative = true
//                                                         }
//                                                         else{
//                                                             sphereProperties.relative = false
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
//                            ///////////////////////////////////Center Propertiess///////////////////////////////
//                            Rectangle{
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

//                                            TextInput {
//                                                id: mcenterX
//                                                text: sphereProperties.center.x
//                                                anchors.fill: parent
//                                                font.pointSize: 10
//                                                selectByMouse: true
//                                                selectionColor: "dark green"
//                                                validator: DoubleValidator {
//                                                    decimals: 8;
//                                                    notation: DoubleValidator.StandardNotation
//                                                    locale: "insert x"
//                                                }
//                                                onTextChanged: {
//                                                    sphereProperties.center.x = text
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
//                                                id: mcenterY
//                                                text: sphereProperties.center.y
//                                                anchors.fill: parent
//                                                font.pointSize: 10
//                                                selectByMouse: true
//                                                selectionColor: "dark green"
//                                                validator: DoubleValidator {
//                                                    decimals: 8;
//                                                    notation: DoubleValidator.StandardNotation
//                                                    locale: "insert y"
//                                                }
//                                                onTextChanged: {
//                                                    sphereProperties.center.y = text
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
//                                                id: mcenterZ
//                                                text: sphereProperties.center.z
//                                                anchors.fill: parent
//                                                font.pointSize: 10
//                                                selectByMouse: true
//                                                selectionColor: "dark green"
//                                                validator: DoubleValidator {
//                                                    decimals: 8;
//                                                    notation: DoubleValidator.StandardNotation
//                                                    locale: "insert z"
//                                                }
//                                                onTextChanged: {
//                                                    sphereProperties.center.z = text
//                                                }
//                                            }
//                                        }
//                                    }
//                                }
//                            }
//                            Rectangle{
//                                Layout.fillWidth: true
//                                color: "#313131"
//                                height: 80
//                                border.color: "#5f5f5f"
//                                border.width: 1

//                                Text {
//                                    id: centerSphere
//                                    text: qsTr("Center:")
//                                    font.pointSize: 10
//                                    color: "white"
//                                    anchors.verticalCenter:  parent.verticalCenter
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
//                                        text: sphereProperties.radius
//                                        anchors.fill: parent
//                                        font.pointSize: 10
//                                        selectByMouse: true
//                                        selectionColor: "dark green"
//                                        validator: DoubleValidator {
//                                            decimals: 8;
//                                            notation: DoubleValidator.StandardNotation
//                                            locale: "insert radius"
//                                        }
//                                        onTextChanged: {
//                                            sphereProperties.radius = text
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
//                                    id: radiusSphere
//                                    text: qsTr("Radius:")
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
//                                           sphereProperties.transparency = transValue.value
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
//                                    id: transSphere
//                                    text: qsTr("Transparency:")
//                                    font.pointSize: 10
//                                    color: "white"
//                                    anchors.verticalCenter:  parent.verticalCenter
//                                    x:7
//                                }
//                            }
//                            ///////////////////////////////////Type Icons ///////////////////////////////////////
//                            Rectangle{
//                                Layout.fillWidth: true
//                                color: "#313131"
//                                height: 30
//                                border.color: "#5f5f5f"
//                                border.width: 1

//                                Row{
//                                    spacing: 5
//                                    anchors.centerIn: parent
//                                    Rectangle{
//                                        width: 20
//                                        height: 20
//                                        color: "#5f5f5f"
//                                        radius: 4

//                                        Image {
//                                            id: globe
//                                            source: "qrc:/resources/globe.png"
//                                            sourceSize: "20 x 20"
//                                            anchors.centerIn: parent
//                                        }
//                                        MouseArea{
//                                            anchors.fill: parent
//                                            cursorShape: Qt.PointingHandCursor
//                                            onClicked: {
//                                                sphereProperties.shape = 0
//                                            }
//                                        }
//                                    }
//                                    Rectangle{
//                                        width: 20
//                                        height: 20
//                                        color: "#5f5f5f"
//                                        radius: 4

//                                        Image {
//                                            id: tophalf
//                                            source: "qrc:/resources/topHalf.png"
//                                            sourceSize: "20 x 20"
//                                            anchors.centerIn: parent
//                                        }
//                                        MouseArea{
//                                            anchors.fill: parent
//                                            cursorShape: Qt.PointingHandCursor
//                                            onClicked: {
//                                                sphereProperties.shape = 1
//                                            }
//                                        }
//                                    }
//                                    Rectangle{
//                                        width: 20
//                                        height: 20
//                                        color: "#5f5f5f"
//                                        radius: 4

//                                        Image {
//                                            id: bothalf
//                                            source: "qrc:/resources/botHalf.png"
//                                            sourceSize: "20 x 20"
//                                            anchors.centerIn: parent
//                                        }
//                                        MouseArea{
//                                            anchors.fill: parent
//                                            cursorShape: Qt.PointingHandCursor
//                                            onClicked: {
//                                                sphereProperties.shape = 2
//                                            }
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
//                                    id: shapeSphere
//                                    text: qsTr("Shape:")
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

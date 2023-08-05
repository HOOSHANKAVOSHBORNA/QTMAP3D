import QtQuick
import QtQuick.Controls
import "style"


TextField {
    id : txtid
    width : 40
    implicitHeight: 40
//    property string A: ["0","1"]


    property  color colorDefault: Style.backgroundColor
    property  color colorOnFocus: Style.secondaryColor
    property  color colorMouseOver: Style.textHoverColor
    property  color colorBorderOnFocus: Style.hoverColor

    placeholderText:  qsTr("Search By")
    color: "#ffffff"
    font.family: "Segoe UI"
    font.pointSize: 10
    clip: true
    leftPadding: 42
    rightPadding: 35

    QtObject{
        id :internal
        property var dynamicColor: if(txtid.focus){
                                       txtid.focus? colorOnFocus : colorDefault
                                   }else if(!txtid.focus && txtid.width > 40){
                                       txtid.focus? colorOnFocus : colorDefault

                                   }else{
                                        txtid.hovered? colorMouseOver : colorDefault
                                   }
    }

    background: Rectangle {
        id :txtid2
        radius: 8
        color: internal.dynamicColor
        opacity: 0.7
        border.width :2

        Image {
            id: txtid3
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
//            anchors.bottom: parent.bottom
            source: "qrc:/Resources/sear.svg"

            ///images/sear.svg
            fillMode: Image.PreserveAspectFit
            anchors.leftMargin: 5
            sourceSize.width: 25
            sourceSize.height: 10
//            opacity: 0.5
        }
        Image {

            id: iconClearText
            visible: false
            anchors.verticalCenter: parent.verticalCenter
            anchors.right:  parent.right
//            anchors.bottom: parent.bottom
            source: "qrc:/Resources/back.svg"
            fillMode: Image.PreserveAspectFit
            anchors.rightMargin: 10
            sourceSize.width: 25
            sourceSize.height: 10
//            opacity: 0.5

        }
        MouseArea{
            cursorShape: Qt.PointingHandCursor
            anchors.fill: iconClearText
            hoverEnabled: true

            onEntered: iconClearText.opacity=0.75
            onExited: iconClearText.opacity =0.5
            onClicked: {
                txtid.text=""
//                comboBox.state = comboBox.state==="dropDown"?"":"dropDown"
            }

            visible: iconClearText.visible




            }
        MouseArea{
//            anchors.fill: txtid
            onClicked: {
                txtid.state = txtid.state==="dropDown"?"":"dropDown"
                hoverEnabled : true


            }
        }
        }

    selectByMouse: true
    selectedTextColor: "#FFFFFF"
    selectionColor: "#ff007f"
    placeholderTextColor: "#81848c"

    onFocusChanged: {
        if(txtid.focus){

            textonFocus.running =true
//            textwithFocus.running =true
            iconClearText.visible = true
            txtid2.border.color = colorBorderOnFocus
        }else {

            textlostFocus.running=true
//            textbitFocus.running =true
            iconClearText.visible=false
            txtid.text = ""
            txtid2.border.color = colorDefault}
        }

//function onFchange(){ if(txtid.focus){
//        textonFocus.running =true
//        textwithFocus.running =true
//        iconClearText.visible = true
//        txtid2.border.color = colorBorderOnFocus
//    }else {

//            textlostFocus.running=true
//            textbitFocus.running =true
//            iconClearText.visible=false
//            txtid.text = ""
//        txtid2.border.color = colorDefault}
//    };

    PropertyAnimation {
        id : textonFocus
        target: txtid
        properties: "width"
        to:300
        duration: 500
        easing.type: Easing.OutQuint
    }
    PropertyAnimation {
        id : textlostFocus
        target: txtid
        properties: "width"
        to:40
        duration: 500
        easing.type: Easing.OutQuint
    }
//    PropertyAnimation {
//        id : textwithFocus
//        target: dropDown
//        properties: "height"
//        from:0
//        to:150
//        duration: 1000
//        easing.type: Easing.OutQuint
//    }
//    PropertyAnimation {
//        id : textbitFocus
//        target: dropDown
//        properties: "height"
//        from : 150
//        to:0
//        duration: 1000
//        easing.type: Easing.OutQuint
//    }

}

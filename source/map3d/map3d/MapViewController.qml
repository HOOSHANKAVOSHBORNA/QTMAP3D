
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects
import "style"



Item {
    id:rootItem
    width: mainRowLayout.implicitWidth
    height: mainRowLayout.implicitHeight

    property string modeMap: "geocentric"
    property real positionFactor: 1.0

    property bool showRecMov: true
    property real myDuration: 300.0

    property bool zoomInButtonPressed: positive.pressed
    property bool zoomOutButtonPressed: negative.pressed

    property variant moveXY  : moveControlCamera.requstXY
    property variant rotateXY: rotationControlCamera.requstXY



    signal btnHomeClicked()
    signal btnProjectionClicked()

    RowLayout {
        id: mainRowLayout
        anchors.fill: parent
        spacing: 5

        ///////// ------------ Move and Rotation control Area
        /*

        */

        ColumnLayout{
            id: control
            spacing: 5
            clip: true
            ControlCamera{
                id: rotationControlCamera
                opacity:0
                width: Style.uiSecondContainerSize
                height: Style.uiSecondContainerSize
                x: positionFactor * Style.uiSecondContainerSize
                centerIcon: "qrc:/Resources/eye.png"

            }

            ControlCamera{
                id: moveControlCamera
                opacity: 0
                width: Style.uiSecondContainerSize
                height: Style.uiSecondContainerSize
                x: positionFactor * Style.uiSecondContainerSize
            }




        }
        //------------------ Navigation
        /*
          a column layout which contains every Element
          in our desired navigartion bar
        */

        ColumnLayout {
            id:navigationLayout
            spacing: 4
            Item {

                Layout.preferredWidth : Style.uiContainerSize
                Layout.preferredHeight: Style.uiContainerSize *2 + 2

                Rectangle {
                    id: zoomBtnContainer
                    width: Style.uiContainerSize
                    height:  Style.uiContainerSize *2 + 2
                    anchors.fill: parent
                    color: Style.uiWhite
                    radius: 40
                    ColumnLayout{
                        anchors.fill: parent
                        Button {
                            id: positive
                            Layout.leftMargin: 1
                            icon.source : "qrc:/Resources/add.png"
                            icon.width : Style.uiBtnIconSize
                            icon.height :Style.uiBtnIconSize
                            icon.color : hovered ? (pressed ? Style.uiBlue: Style.uiHover) :
                                                   (pressed ? Style.uiHover : Style.uiBlue);
                            background:Rectangle {
                                color:"transparent"
                            }
                        }
                        Rectangle{
                            Layout.leftMargin:6
                            visible: true
                            width: (parent.width/1.4)/1.2
                            color:"black"
                            opacity: 0.3
                            height: 2/1.3
                        }

                        Button {
                            id: negative
                            Layout.leftMargin:1
                            Layout.topMargin: -5
                            text: qsTr("Button")
                            display: AbstractButton.IconOnly
                            icon.source : "qrc:/Resources/minus.png"
                            icon.width : Style.uiBtnIconSize
                            icon.height : Style.uiBtnIconSize
                            icon.color : hovered ? (pressed ? Style.uiBlue: Style.uiHover) :
                                                   (pressed ? Style.uiHover : Style.uiBlue);
                            background:Rectangle {
                                color:"transparent"
                            }
                        }

                    }
                }
                MultiEffect {
                    source: zoomBtnContainer
                    enabled: true
                    anchors.fill: zoomBtnContainer
                    shadowColor: "black"
                    shadowEnabled: true
                    shadowBlur: 1
                    shadowHorizontalOffset: 3.5
                    shadowVerticalOffset:2.5
                    shadowOpacity:1
                    paddingRect: Qt.rect(0,0,20,20)
                    shadowScale: 0.98
                }
            }
            Item {
                Layout.preferredWidth: Style.uiContainerSize
                Layout.preferredHeight: Style.uiContainerSize
                Button {
                    id: directionBtn
                    rotation:0 /*-90 - 180*(positionFactor)*/
                    hoverEnabled: true
                    width: Style.uiContainerSize
                    height: Style.uiContainerSize
                    display: AbstractButton.IconOnly
                    icon.source : "qrc:/Resources/direction.png"
                    icon.width : Style.uiBtnIconSize
                    icon.height : Style.uiBtnIconSize
                    icon.color : hovered ? (pressed ? Style.uiBlue: Style.uiHover) :
                                           (pressed ? Style.uiHover : Style.uiBlue);

                    background: Rectangle{
                        color:Style.uiWhite
                        radius:20
                    }
                    onClicked: {

                        rotationAnim.start()
                        moveAnim.start()

                        if(rootItem.positionFactor){

                            showSlider.stop()
                            hideSlider.start()


                        } else {

                            hideSlider.stop()
                            showSlider.start()
                        }
                    }
                }
                MultiEffect {
                    source: directionBtn
                    enabled: true
                    rotation: directionBtn.rotation
                    anchors.fill: directionBtn
                    shadowColor: "black"
                    shadowEnabled: true
                    shadowBlur: 1
                    shadowHorizontalOffset: 3.5
                    shadowVerticalOffset:2.5
                    shadowOpacity:1
                    paddingRect: Qt.rect(0,0,20,20)
                    shadowScale: 0.98
                }
            }
            Item {
                Layout.preferredWidth:Style.uiContainerSize
                Layout.preferredHeight: Style.uiContainerSize
                Button {
                    id: home
                    width: Style.uiContainerSize
                    height: Style.uiContainerSize
                    hoverEnabled: true
                    display: AbstractButton.IconOnly
                    icon.source : "qrc:/Resources/home.png"
                    icon.width :Style.uiBtnIconSize
                    icon.height : Style.uiBtnIconSize
                    icon.color : hovered ? (pressed ? Style.uiBlue: Style.uiHover) :
                                           (pressed ? Style.uiHover : Style.uiBlue);
                    background: Rectangle{
                        color:Style.uiWhite
                        radius:20
                    }
                    onClicked: btnHomeClicked()
                }
                MultiEffect {
                    source: home
                    enabled: true
                    rotation: home.rotation
                    anchors.fill: home
                    shadowColor: "black"
                    shadowEnabled: true
                    shadowBlur: 1
                    shadowHorizontalOffset: 3.5
                    shadowVerticalOffset:2.5
                    shadowOpacity:1
                    paddingRect: Qt.rect(0,0,20,20)
                    shadowScale: 0.98
                }
            }

            Item {
                Layout.preferredWidth:40
                Layout.preferredHeight: 40

                Button {
                    id: project
                    display: AbstractButton.IconOnly
                    width:Style.uiContainerSize
                    height:Style.uiContainerSize
                    icon.source : modeMap === "projection" ? "qrc:///Resources/threeD.png": "qrc:/Resources/twoD.png"
                    icon.width :Style.uiBtnIconSize
                    icon.height : Style.uiBtnIconSize
                    icon.color : hovered ? (pressed ? Style.uiBlue: Style.uiHover) :
                                           (pressed ? Style.uiHover : Style.uiBlue);
                    background: Rectangle{
                        color: Style.uiWhite
                        radius:20
                    }
                    smooth: true

                    onClicked:{
                        if (modeMap==="projection")
                            modeMap = "geocentric"
                        else
                            modeMap = "projection"
                        btnProjectionClicked()
                    }

                }
                MultiEffect {
                    source: project
                    enabled: true
                    anchors.fill: project
                    shadowColor: "black"
                    shadowEnabled: true
                    shadowBlur: 1
                    shadowHorizontalOffset: 3.5
                    shadowVerticalOffset:2.5
                    shadowOpacity:1
                    paddingRect: Qt.rect(0,0,20,20)
                    shadowScale: 0.98
                }
            }

        }
    }


    PropertyAnimation {
        id: showSlider
        target: rootItem
        property: "positionFactor"
        from: rootItem.positionFactor
        to: 1.0
        duration: myDuration * Math.abs(1.0 - positionFactor)

        easing.type: Easing.OutQuint
    }
    PropertyAnimation {
        id: hideSlider
        target: rootItem
        property: "positionFactor"
        from: rootItem.positionFactor
        to: 0.0
        duration: myDuration * Math.abs(positionFactor)

        easing.type: Easing.InQuint
    }


    PropertyAnimation {
        id: rotationAnim
        target: rotationControlCamera
        property: "opacity"
        from: rotationControlCamera.opacity
        to: 1.0
        duration:  myDuration * Math.abs(positionFactor)

        easing.type: Easing.OutQuint
    }
    PropertyAnimation {
        id:  moveAnim
        target: moveControlCamera
        property: "opacity"
        from :moveControlCamera.opacity
        to: 1.0
        duration: myDuration * Math.abs(positionFactor)

        easing.type: Easing.InQuint
    }
}

/*##^##
Designer {
    D{i:4;anchors_x:8;anchors_y:64}D{i:6;anchors_y:18}D{i:5;anchors_y:18}D{i:7;anchors_y:"-8"}
D{i:9;anchors_width:24;anchors_x:9}D{i:11;anchors_width:24;anchors_y:"-8"}D{i:1;anchors_height:200;anchors_width:200;anchors_x:8;anchors_y:0}
}
##^##*/

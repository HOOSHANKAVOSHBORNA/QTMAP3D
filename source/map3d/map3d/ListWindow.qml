import QtQuick 2.13
import QtQuick.Layouts 1.13
import QtQuick.Controls 2.13
import Crystal 1.0
import QtQuick.Window 2.13
import QtGraphicalEffects 1.0
CListWindow {


    property var buttonsModel : ListModel {

    }
    property var selectColor: "gray"
    property var unselectColor: "#00587A"
    property int grad: 0
    visible: false

    id: root
    title: "List Window"
    color: "#252525"
    minimumWidth: 1100
    minimumHeight: 600
//    flags: Qt.FramelessWindowHint
//    MouseArea{
//        anchors.fill : parent
//        property variant clickPos: "1,1"
//        onPressed: {
//            clickPos = Qt.point(mouse.x ,mouse.y)
//        }
//        onPositionChanged: {
//            var delta = Qt.point(mouse.x - clickPos.x , mouse.y - clickPos.y)
//            root.x += delta.x
//            root.y += delta.y
//        }
//        onDoubleClicked: {
//            root.showFullScreen();
//        }
//    }
    BorderImage {
        id: borderImage
        anchors.fill: parent
//        source: "qrc:/Resources/mainFrame.png"
        z:1

        Rectangle{
            color: "transparent"
            id:back
            z:0
            clip: true
            anchors{
                top: parent.top
                bottom: parent.bottom
                left: parent.left
                right: parent.right
                topMargin: 15
                bottomMargin: 10
                rightMargin: 10
                leftMargin: 10

            }
            ColumnLayout {
                clip: true
                anchors.fill: back
                id:column
                spacing: 8
//                Rectangle{
//                    Layout.fillWidth: true
//                    height: 30
//                    color: "transparent"
//                    clip: true
//                    Image {
//                        id: imageclose

//                        width: 24
//                        height: 24
//                        anchors.rightMargin: 15

//                        anchors.verticalCenter: parent.verticalCenter
//                        anchors.right: parent.right
//                        fillMode: Image.PreserveAspectFit
//                        source: "qrc:/Resources/close1.png"
//                        states: ["mouseIn" , "mouseOut"]
//                        state: "mouseOut"
//                        transitions: [
//                            Transition {
//                                from: "*"
//                                to: "mouseIn"
//                                NumberAnimation {
//                                    target: imageclose
//                                    property: "scale"
//                                    from: 0.85
//                                    to:1
//                                    duration: 400
//                                    easing.type: Easing.OutBounce
//                                }
//                            }
//                        ]
//                        MouseArea{
//                            anchors.fill :parent
//                            onClicked: close()
//                            hoverEnabled: true
//                            onContainsMouseChanged: imageclose.state = containsMouse ? "mouseIn" : " mouseOut"

//                        }
//                    }


//                    Image {
//                        id: imageminimize
//                        width: 24
//                        height: 24
//                        anchors.verticalCenterOffset: 1
//                        anchors.verticalCenter: parent.verticalCenter
//                        anchors.right: imageclose.left
//                        anchors.rightMargin: 3
//                        fillMode: Image.PreserveAspectFit
//                        source: "qrc:/Resources/minimize.png"
//                        states: ["mouseIn" , "mouseOut"]
//                        state: "mouseOut"
//                        transitions: [
//                            Transition {
//                                from: "*"
//                                to: "mouseIn"

//                                NumberAnimation {
//                                    target: imageminimize
//                                    property: "scale"
//                                    from: 0.85
//                                    to:1
//                                    duration: 400
//                                    easing.type: Easing.OutBounce
//                                }
//                            }
//                        ]
//                        MouseArea{
//                            hoverEnabled: true
//                            anchors.fill : parent
//                            onClicked: showMinimized()
//                            onContainsMouseChanged: imageminimize.state = containsMouse ? "mouseIn" : " mouseOut"
//                        }
//                    }
//                    Image {
//                        id: imageCollapse
//                        width: 24
//                        height: 24
//                        anchors.verticalCenterOffset: 1
//                        anchors.verticalCenter: parent.verticalCenter
//                        anchors.right: imageminimize.left
//                        anchors.rightMargin: 3
//                        fillMode: Image.PreserveAspectFit
//                        source: "qrc:/Resources/collapse2.png"
//                        states: ["mouseIn" , "mouseOut"]
//                        state: "mouseOut"
//                        transitions: [
//                            Transition {
//                                from: "*"
//                                to: "mouseIn"

//                                NumberAnimation {
//                                    target: imageCollapse
//                                    property: "scale"
//                                    from: 0.85
//                                    to:1
//                                    duration: 400
//                                    easing.type: Easing.OutBounce
//                                }
//                            }
//                        ]
//                        MouseArea{
//                            hoverEnabled: true
//                            anchors.fill : parent
//                            onClicked: root.showMaximized()
//                            onContainsMouseChanged: imageCollapse.state = containsMouse ? "mouseIn" : " mouseOut"
//                        }
//                    }
//                }
                Row {
                    id: buttons
                    Layout.alignment: Qt.AlignCenter
                    //height: 30
                    Layout.minimumHeight: 40
                    Layout.preferredHeight: 40
                    Layout.fillWidth: true
                    width: parent.width
                    Layout.leftMargin: 30
                    Layout.rightMargin: 10
                    spacing: 2
                    Repeater {
                        id: rep
                        model: buttonsModel
                        delegate: Button {
//                            text: buttonText
                            anchors.leftMargin: 0
//                            contentItem :Text {
//                                id: name
//                                text: parent.text
//                                color: "#FFFFFF"
//                                anchors.verticalCenter: parent.verticalCenter
//                                horizontalAlignment: Text.AlignHCenter
//                                verticalAlignment: Text.AlignVCenter
//                                elide: Text.ElideRight
//                            }
//                            background: Rectangle {
//                                color: selectColor
//                                radius: 1
//                            }
                            background:  Rectangle {

                                color: "black"
                                border.color: index == grad ? "#D0C000" : parent.hovered ? "yellow" : "gray"
                                border.width: 2
                                radius: 8
                            }

                            height: 35
                            Text {
                                id: name
                                text: buttonText
                                color: (index == grad) ? "#D0C000" :  parent.hovered ? "yellow" : "#FFFFFF"
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.centerIn: parent
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                                elide: Text.ElideRight
                                font.bold: true
                                font.pointSize: 12
                            }
                            hoverEnabled: true

                            width: buttons.width / 6
                            onClicked: showTab(index)
                        }
                    }
                }


                Rectangle {
                    color: "#5f72bd"
                    Layout.leftMargin: 30
                    Layout.rightMargin: 8
                    gradient: Gradient {
                        orientation: Gradient.Horizontal
                        GradientStop {
                            position: 0
                            color: "#00587A"
                        }

                        GradientStop {
                            position: 0.5
                            color: "yellow"
                        }
                        GradientStop {
                            position: 1
                            color: "#00587A"
                        }
                    }
                    Layout.minimumHeight: 1
                    border.width: 1
                    border.color: "#4568dc"
                    Layout.maximumHeight: 1
                    Layout.fillWidth: true
                    layer.enabled: true
                }
                StackLayout {
                    id: stacklayout
                    Layout.fillWidth: true
                    Layout.fillHeight: true



                }
            }
        }
    }
    function addTab(titleString, item){
        buttonsModel.append({"buttonText": titleString})
        stacklayout.data.push(item)
        showTab(0)
    }

    function showTab(indx){
        stacklayout.currentIndex = indx
        for (var i = 0; i < stacklayout.count; i++){
//            rep.itemAt(i).background.color = unselectColor
            rep.itemAt(i).height = 40

        }

//        rep.itemAt(indx).background.color = selectColor
        rep.itemAt(indx).height = 42
        root.tabChanged(indx);
        grad = indx

    }

}

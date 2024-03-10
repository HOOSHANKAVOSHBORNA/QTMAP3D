import QtQuick
import QtQuick.Dialogs
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Shapes
import QtQuick.Particles
// import Crystal 1.0

Window {

    property Snipp snipp

    id: rootPopUp
    title: "SnapShot"
    width: 400
    height: 220
    color: "transparent"
    flags: {
        Qt.FramelessWindowHint
//        Qt.WindowStaysOnTopHint
    }


    //    SnipToolClass{
    //        id: snipObject
    //    }

    Image {
        source: "qrc:/resources/shot.png"
        anchors.horizontalCenter: parent.horizontalCenter
        y: 186
        sourceSize: ("30 x 30")
        MouseArea{
            anchors.fill: parent
            onClicked: {
                timmer.running = true
                rootPopUp.snipp.takeSnapShot(400,220);
                //                fDialog.open();

            }
        }
    }

    FileDialog{
        id:fDialog
        fileMode: FileDialog.SaveFile
        //        visible: false
        onAccepted: {
            rootPopUp.snipp.saveImageFile(fDialog.currentFile.toString().slice(7));
            rootPopUp.close();
        }
    }

    Image {
        source: "qrc:/resources/close.png"
        anchors.horizontalCenter: parent.horizontalCenter
        y: 5
        sourceSize: ("20 x 20")
        MouseArea{
            anchors.fill: parent
        }
    }



    Rectangle{
        id:scene
        color: "white"
        opacity: 0.2
        anchors.fill: parent
        radius: 10
    }


    Image {
        source: "qrc:/resources/group.png"
        anchors.fill: parent
    }




    /////////////////////////////////////////////////////////



    Image {
        id: snipImage
        //        source: "image://sniptoolprovider/SnipToolImage"
        anchors.fill: parent
        visible: false
        opacity: 1
        Rectangle{
            anchors.fill: parent
            color: "black"
            opacity: 0.2
            border.color: "black"
            border.width: 5
            radius: 10
        }
        Rectangle{
            anchors.fill: parent
            color: "transparent"
            border.color: "black"
            border.width: 5
            radius: 10
        }

        RowLayout{
            anchors.bottom: parent.bottom
            spacing: 10
            anchors.horizontalCenter: parent.horizontalCenter
            Button{
                text: "save"
                onClicked:{
                    fDialog.open();
                }
            }
            Button{
                text: "delete"
                onClicked: {
                    rootPopUp.close()
                }
            }
        }
    }

    Timer{
        id: timmer
        interval: 500
        running: false
        repeat: false
        onTriggered: {
            snipImage.visible = true
            snipImage.source = "image://sniptoolprovider/SnipToolImage"
        }
    }



}





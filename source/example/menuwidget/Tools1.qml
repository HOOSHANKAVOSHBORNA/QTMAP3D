import QtQuick 2.0
import QtQuick.Layouts 1.12


    Grid{
        id: grid
        anchors.margins: 10

        anchors.fill: parent

        columns: 4
        spacing: 3
        Rectangle {
            color: "#55000000"
            border.color: "black"
            border.width: 1
            radius: 5
            width: grid.width/6-space
            height: grid.width/6-space
            Image {
                id: penciel
                source: "qrc:/res/icons8-height-96.png"
                width: 20
                height: 20
                anchors.centerIn: parent
            }
            MouseArea{
                anchors.fill: parent
                hoverEnabled: true
                onClicked: {console.log("3");}
                onEntered: {anim_pen.running=true;}
                onExited: {anim_pen.running=false;}
            }
            RotationAnimation{
                id:anim_pen
                targets: penciel
                from :-60
                to:30
                duration: 200
                running: false
                loops: Animation.Infinite


            }

        }
        Rectangle {
            color: "#55000000"
            border.color: "black"
            border.width: 1
            radius: 5
            width: grid.width/6-space
            height: grid.width/6-space
            Image {
                id: location
                source: "qrc:/res/icons8-ruler-96.png"
                width: 20
                height: 20
                anchors.centerIn: parent
            }
            MouseArea{
                anchors.fill: parent
                hoverEnabled: true
                onClicked: {console.log("3");}
                onEntered: {anim_location.running=true;}
                onExited: {anim_location1.running=true;anim_location.running=false;}
            }
            RotationAnimation{
                id:anim_location
                targets: location
                from :-60
                to:30
                duration: 200
                running: false
                loops: Animation.Infinite
            }
            RotationAnimation{
                id:anim_location1
                targets: location
                to:0
                duration: 200
                running: false
            }


        }
        Rectangle {
            color: "#55000000"
            border.color: "black"
            border.width: 1
            radius: 5
            width: grid.width/6-space
            height: grid.width/6-space
            Image {
                id: polygan
                source: "qrc:/res/icons8-sewing-tape-measure-96.png"
                width: 20
                height: 20
                anchors.centerIn: parent
            }
            MouseArea{
                anchors.fill: parent
                hoverEnabled: true
                onClicked: {console.log("3");}
                onEntered: {anim_polygan.running=true;}
                onExited: {anim_polygan1.running=true;anim_polygan.running=false;}
            }
            RotationAnimation{
                id:anim_polygan
                targets: polygan
                from :-60
                to:30
                duration: 200
                running: false
                loops: Animation.Infinite
            }
            RotationAnimation{
                id:anim_polygan1
                targets: polygan
                to:0
                duration: 200
                running: false
            }

        }
        Rectangle {
            color: "#55000000"
            border.color: "black"
            border.width: 1
            radius: 5
            width: grid.width/6-space
            height: grid.width/6-space
            Image {
                id: polygan_suf
                source: "qrc:/res/icons8-orthogonal-view-96.png"
                width: 20
                height: 20
                anchors.centerIn: parent
            }
            MouseArea{
                anchors.fill: parent
                hoverEnabled: true
                onClicked: {console.log("3");}
                onEntered: {anim_polygan_suf.running=true;}
                onExited: {anim_polygan_suf1.running=true;anim_polygan_suf.running=false;}
            }
            RotationAnimation{
                id:anim_polygan_suf
                targets: polygan_suf
                from :-60
                to:30
                duration: 200
                running: false
                loops: Animation.Infinite
            }
            RotationAnimation{
                id:anim_polygan_suf1
                targets: polygan_suf
                to:0
                duration: 200
                running: false
            }

        }
        Rectangle {
            color: "#55000000"
            border.color: "black"
            border.width: 1
            radius: 5
            width: grid.width/6-space
            height: grid.width/6-space
            Image {
                id: template
                source: "qrc:/res/icons8-puzzle-96.png"
                width: 20
                height: 20
                anchors.centerIn: parent
            }
            MouseArea{
                anchors.fill: parent
                hoverEnabled: true
                onClicked: {console.log("3");}
                onEntered: {anim_tempalte.running=true;}
                onExited: {anim_template1.running=true;anim_tempalte.running=false;}
            }
            RotationAnimation{
                id:anim_tempalte
                targets: template
                from :-60
                to:30
                duration: 200
                running: false
                loops: Animation.Infinite
            }
            RotationAnimation{
                id:anim_template1
                targets: template
                to:0
                duration: 200
                running: false
            }

        }



}

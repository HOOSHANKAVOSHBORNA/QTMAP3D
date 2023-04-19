import QtQuick 2.12

Item {
    id: rootIte
    visible: true
    width: 50
    height: 50
    readonly property int  _widthimage : 32
    readonly property int  _heightimage:  32
    readonly property color _colorRec: "#404040"


    property real headingAngle

    signal compassDoubleClicked();

    Rectangle {



        id: compass
        anchors.fill: parent
        color: _colorRec
        smooth: true
        radius: 100
        opacity: 0.8

        MouseArea {
            anchors.fill: parent
            onDoubleClicked: function() {
                rootIte.compassDoubleClicked();
            }
        }

        Image {
            rotation: rootIte.headingAngle
            id: image
            width: _widthimage
            height: _widthimage
            anchors.centerIn: compass
            source: "qrc:/Resources/compass.png"
            opacity: 1            
        }


    }
}

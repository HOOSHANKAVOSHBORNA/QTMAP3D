import QtQuick 2.13
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.13
import QtQuick.Layouts 1.12

Item{

    readonly property int heightHeader: 50
    readonly property string    _fontFamily : "Srouce Sans Pro"
    readonly property int       _fontPointSize : 11
    readonly property int       _widgetmargin : 3

    property string nameTarget:"Airplan"
    property url imageUrl: "qrc:/Resources/airplan.jpg"


    Rectangle{
        anchors.fill : parent
        color: _colorRec
        opacity: 0.8
        radius: _radius
    }

    Text {
        id: header
        text: nameTarget
        verticalAlignment: Text.AlignVCenter
        font.pointSize: _fontPointSize * 1.5
        font.family: _fontFamily
        width: parent.width
        height: heightHeader
        padding:20
        color: _colorHover
        font.bold: true
    }
    Rectangle{
        id : backgrand
        anchors{
            top: header.bottom
            bottom: parent.bottom
        }
        color: "#FFFFFF"
        width: parent.width
    }
     Column{
         anchors{
             top: header.bottom
             bottom: parent.bottom
         }
         width: parent.width
         spacing: 5
         Image {
             id: img
             source: imageUrl
             height: parent.height / 4
             width: parent.width
         }
         InfoJson{
             height: parent.height / 6
             width: parent.width - _widgetmargin
         }

         InfoMapTarget{
             height: parent.height / 6
             width: parent.width - _widgetmargin
         }
         ListTarget{
             height: parent.height / 6
             width: parent.width - _widgetmargin
         }
         ListTarget{
             height: parent.height / 6
             width: parent.width - _widgetmargin
         }
     }

}




import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../style"

Button {
    property alias  iconColor:       iconImage.color
            property alias  iconSize: iconImage.sourceSize
    property alias  iconImageSource: iconImage.source
    property alias  backgroundColor:     backgroundBtn.color
padding : 0
    background: Rectangle {
        id: backgroundBtn
        radius: width / 2
    }
    contentItem:IconImage {
        id: iconImage
        color:  hovered ?  Style.hoverColor: Style.foregroundColor
        sourceSize: Qt.size(Math.floor(26/Style.monitorRatio),Math.floor(26/Style.monitorRatio))
        anchors.centerIn: parent
    }



}




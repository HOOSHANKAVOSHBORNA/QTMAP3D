
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../style"

Button {
    property alias  iconColor:       iconImage.color
            property alias  iconSize: iconImage.sourceSize
    property alias  iconImageSource: iconImage.source
    property alias  backgroundColor:     backgroundBtn.color

    background: Rectangle {
        id: backgroundBtn
        radius: width / 2
    }
    contentItem:IconImage {
        id: iconImage
        sourceSize: Qt.size(26/Style.monitorRatio,26/Style.monitorRatio)
        anchors.centerIn: parent
    }



}




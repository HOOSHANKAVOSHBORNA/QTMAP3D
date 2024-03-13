import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects
import "style"
import "Components"

Item{
    id: rootItem
    property var model
    property var filterManager
    property bool flag: true
    property real maxheight : 420 / Style.monitorRatio
    property int iconSize: 26/Style.monitorRatio
    readonly property color fg: Qt.rgba(Style.foregroundColor.r, Style.foregroundColor.g, Style.foregroundColor.b, 0.50)
    property ListModel history: ListModel {
        id: histModel
    }

    onWidthChanged: {
        if(width == 350 /Style.monitorRatio ){
            heightMotion.running = true
            filterPresentation.z = 0
        }
        if (width == 0 /Style.monitorRatio ){
            heightMotionClose.running = true
            filterPresentation.z = 0
        }
    }
    width: 40 /Style.monitorRatio
    height:  40/Style.monitorRatio

    Rectangle{
        id:mainRec
        width: parent.width

        height: 40 / Style.monitorRatio
        color:Style.backgroundColor
        radius:width > 40 / Style.monitorRatio ? 20 / Style.monitorRatio : width/2
        z:1
        IconImage{
            id:searchIcon
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            anchors.leftMargin: 10 /Style.monitorRatio
            source: "qrc:/Resources/search.png"
            width: iconSize
            height: iconSize
            visible: !flag
        }
        TextField {
            visible: !flag
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: searchIcon.right
            anchors.right: openCloseIcons.left
            verticalAlignment: Text.AlignVCenter
            font.family: Style.fontFamily
            font.pixelSize: Style.regularFontSize
            color: Style.foregroundColor
            background: Rectangle{
                color: "transparent"
                radius: height/2
            }
            onTextChanged: function() {
                rootItem.model.setFilterString(text)
            }
            placeholderText: "Search ..."
            placeholderTextColor: Style.disableColor
        }
        Rectangle{
            id:openCloseIcons
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            height: 40/Style.monitorRatio
            width:  40/Style.monitorRatio
            radius:width/2
            color:Style.backgroundColor
            IconImage{
                anchors.centerIn: parent
                source: "qrc:/Resources/index.png"
                width: iconSize
                height: iconSize
                visible: !flag
            }
            IconButton {
                anchors.centerIn: parent
                iconImageSource: "qrc:/Resources/search.png"
                width: 40 / Style.monitorRatio
                height: 40 / Style.monitorRatio
                padding: 0
                visible: flag
            }
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    if (!(rootItem.width > 40 / Style.monitorRatio && rootItem.width < 350/ Style.monitorRatio )){
                        if(flag){
                            openMotion.running =  true
                            filterPresentation.z = 1
                        }
                        else{
                            heightMotionClose.running = true
                            closeMotion.running = true
                        }
                        if (filterRect.height !=0 && flag)
                        {
                            filterRect.height  =  0
                            flag = false

                        }

                        if( rootItem.width == (40)/Style.monitorRatio)
                            flag = false


                        if( rootItem.width == (350)/Style.monitorRatio){
                            flag = true
                        }
                    }

                }
            }
        }
    }
    MultiEffect {
        source: mainRec
        enabled: true
        anchors.fill: mainRec
        shadowColor: "black"
        shadowEnabled: true
        shadowBlur: 1
        shadowHorizontalOffset: 1.5
        shadowVerticalOffset:.5
        shadowOpacity:.7
        shadowScale: 0.7

    }
    Rectangle{
        id:filterPresentation
        anchors.fill: mainRec
        color:Style.backgroundColor
        visible: filterRect.height < 300 / Style.monitorRatio
        z:0
        radius: 20
        Label {
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            anchors.leftMargin: 10
            visible: parent.width > 300 / Style.monitorRatio
            text: "Filter"
            font.pixelSize:Style.titleFontSize
            font.family: Style.fontFamily
            color: Style.foregroundColor
        }

    }

    PropertyAnimation {
        id : openMotion
        target: rootItem
        properties: "width"
        to:(350)/Style.monitorRatio
        duration: 500
        easing.type: Easing.OutQuint
    }
    PropertyAnimation {
        id : closeMotion
        target: rootItem
        properties: "width"
        to:40 / Style.monitorRatio
        duration: 150
        easing.type: Easing.OutQuint
    }

    Rectangle{
        id:filterRect
        anchors.top:  parent.top
        anchors.topMargin: 9 / Style.monitorRatio
        color: Qt.rgba(Style.backgroundColor.r,Style.backgroundColor.g,Style.backgroundColor.b,0.9)
        width:350 / Style.monitorRatio
        height:0
        radius: 20 / Style.monitorRatio
        z:-1
        visible: !flag


        Filter {
            id: dropDown
            anchors.top: filterRect.top
            anchors.topMargin: 25 / Style.monitorRatio
            anchors.left: parent.left
            filterManager: rootItem.filterManager
            visible: parent.height > 300 / Style.monitorRatio

        }
    }

    ScrollView{
        z:1
        id:nodeShowListScroller
        anchors.top: filterRect.top
        anchors.topMargin: dropDown.innerHeight + mainRec.height
        anchors.left: parent.left
        anchors.leftMargin: 15/Style.monitorRatio
        anchors.right: parent.right
        width: filterRect.width
        height: 100/Style.monitorRatio
        visible: filterRect.height > 300 / Style.monitorRatio
        ListView{
            id:listView
            clip: true
            anchors.leftMargin: 10/Style.monitorRatio
            model: rootItem.model
            delegate:
                Button{
                id:delegateBtn
                width: listView.width
                height: 20
                hoverEnabled: true
                visible: !flag

                contentItem:Item {
                    anchors.fill: parent

                    Text {
                        id:lvText
                        anchors.left: seperator.right
                        anchors.leftMargin: 10 / Style.monitorRatio
                        text:model.display
                        font.family: Style.fontFamily
                        font.pixelSize: Style.regularFontSize
                        color: delegateBtn.hovered ? Style.hoverColor : Style.foregroundColor
                        elide: Text.ElideRight
                    }
                    Text {
                        id:id
                        text: model.id_
                        font.family: Style.fontFamily
                        font.pixelSize: Style.regularFontSize
                        color: delegateBtn.hovered ? Style.hoverColor : Style.foregroundColor

                        elide: Text.ElideRight
                    }
                    Text {
                        id:seperator
                        anchors.left: id.right
                        text:" :"
                        font.family: Style.fontFamily
                        font.pixelSize:Style.regularFontSize
                        color: delegateBtn.hovered ? Style.hoverColor : Style.foregroundColor

                        elide: Text.ElideRight
                    }
                }
                onClicked:{

                    rootItem.model.onNodeClicked(index)
                }
                background: Rectangle
                {
                    color:"transparent"
                    radius:height/2
                }
            }
        }
    }
    PropertyAnimation {
        id : heightMotion
        target: filterRect
        properties: "height"
        to:rootItem.maxheight
        duration: 550
        easing.type: Easing.OutQuint
    }
    PropertyAnimation {
        id : heightMotionClose
        target: filterRect
        properties: "height"
        to:0 / Style.monitorRatio
        duration: 150
        easing.type: Easing.OutQuint
    }
}



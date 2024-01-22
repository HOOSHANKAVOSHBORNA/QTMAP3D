import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects
import "style"

Item{
    id: rootItem

    property var model
    property var filterManager
    property bool flag: true
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
        radius:width > 40/ Style.monitorRatio ? 20/ Style.monitorRatio :width/2
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
            font.pixelSize: 16/Style.monitorRatio
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
            IconImage{
                anchors.centerIn: parent
                source: "qrc:/Resources/search.png"
                width: iconSize
                height: iconSize
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
            font.pixelSize:20/Style.monitorRatio
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
        color: Style.backgroundColor
        width:350 / Style.monitorRatio
        height:0 /*340 / Style.monitorRatio*/
        radius: 20 / Style.monitorRatio
        z:-1
        visible: !flag
        Filter {
            id: dropDown
            anchors.top: filterRect.top
            anchors.left: parent.left
            filterManager: rootItem.filterManager
            visible: parent.height > 300
        }
    }
    ScrollView{
        z:10
        anchors.bottom: filterRect.bottom
        anchors.bottomMargin: 5
        anchors.left: parent.left
        anchors.leftMargin: 15/Style.monitorRatio
        width: filterRect.width
        height: 110/Style.monitorRatio
        visible: filterRect.height > 300
        ListView{
            id:listView


            clip: true
            model: /*rootItem.model*/ [ "Aircraft","Rectangel","Circle","Aircraft","Tank","Car"]
            delegate:
                Button
            {
                id:delegateBtn
                width: listView.width
                height: 20
                hoverEnabled: true
                visible: !flag
                required property string modelData
                contentItem:Item {
                    anchors.fill: parent

                    Text {
                        id:id
                        anchors.left: lvText.right
                        anchors.leftMargin: 15 / Style.monitorRatio
                        text: /*model.id_*/"5"
                        font.family: Style.fontFamily
                        font.pixelSize: 15 / Style.monitorRatio
                        color: delegateBtn.hovered ? Style.hoverColor : Style.foregroundColor

                        elide: Text.ElideRight
                    }
                    Text {
                        id:lvText
                        text: /*model.display*/ modelData
                        font.family: Style.fontFamily
                        font.pixelSize: 15 / Style.monitorRatio
                        color: delegateBtn.hovered ? Style.hoverColor : Style.foregroundColor

                        elide: Text.ElideRight
                    }
                }
                onClicked:{
                    print("onNodeCliked")
                    //                    rootItem.model.onNodeClicked(index)
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
        to:380 / Style.monitorRatio
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
    //    }
}



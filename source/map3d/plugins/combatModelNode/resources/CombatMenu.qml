import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

//import "style"
Item {
    id: root
    anchors.fill: parent
    property var assignmentListModel
    property var operatorListModel

    property bool addCheck: false
    property bool removeCheck: false
    property bool selectall: true

    readonly property color foregroundColor: "#DEE3E6"
    readonly property color fg75: Qt.rgba(foregroundColor.r, foregroundColor.g,
                                          foregroundColor.b, 0.75)
    readonly property color fg50: Qt.rgba(foregroundColor.r, foregroundColor.g,
                                          foregroundColor.b, 0.50)
    readonly property color fg02: Qt.rgba(foregroundColor.r, foregroundColor.g,
                                          foregroundColor.b, 0.02)
    readonly property color backgroundColor: "#003569"
    readonly property color bg20: Qt.rgba(backgroundColor.r, backgroundColor.g,
                                          backgroundColor.b, 0.20)
    readonly property color bg60: Qt.rgba(backgroundColor.r, backgroundColor.g,
                                          backgroundColor.b, 0.8)


    RowLayout {
        id: mainRow
        //        anchors.centerIn: parent
        anchors.left: parent.left
        anchors.leftMargin: 110
        Rectangle {
            id: nodeInfoHolder
            width: 300 / 1.3 /*Style.monitorRatio*/
            height: 75 / 1.3 /*Style.monitorRatio*/
            radius: 20
            color: "#DEE3E6"

            RowLayout {
                anchors.left: parent.left
                Rectangle {
                    id:downController
                    Layout.preferredHeight: 75 / 1.3 /*Style.monitorRatio*/
                    Layout.preferredWidth: 55 / 1.3 /*Style.monitorRatio*/
                    radius: 20
                    color: fg02
                    IconImage {
                        id:downIcon
                        source: "qrc:/Resources/down.png"
                        height: 25 / 1.3 /*Style.monitorRatio*/
                        width: 25 / 1.3 /*Style.monitorRatio*/
                        anchors.centerIn: parent
                    }
                    MouseArea{
                        anchors.fill: parent
                        hoverEnabled: true
                        onEntered: downController.color = bg20
                        onExited: downController.color = fg02
                        onClicked:{ downIcon.rotation +=180
                            openMotion.running = true
                        }
                    }
                }
                IconImage {
                    source: operatorListModel ? operatorListModel.operatorIcon : ""
                    Layout.preferredHeight: 55 / 1.3 /*Style.monitorRatio*/
                    Layout.preferredWidth: 55 / 1.3 /*Style.monitorRatio*/
                    Layout.leftMargin: 10 / 1.3 /*Style.monitorRatio*/
                    color: backgroundColor/*operatorListModel ? operatorListModel.operatorColor : "black"*/
                }

                ColumnLayout {
                    Layout.leftMargin: 10 / 1.3 /*Style.monitorRatio*/
                    RowLayout {
                        id: bulletInfo
                        visible: operatorListModel ? operatorListModel.operatorIsAttacker : false
                        IconImage {
                            source: "qrc:/Resources/bullet.png"
                            Layout.preferredHeight: 22 / 1.3 /*Style.monitorRatio*/
                            Layout.preferredWidth: 22 / 1.3 /*Style.monitorRatio*/
                        }
                        Text {
                            font.pixelSize: 17 / 1.3 /*Style.monitorRatio*/
                            font.family: "Roboto"
                            color: backgroundColor
                            text: operatorListModel ? 30 : ""
                        }
                    }
                    Text {
                        id: txt
                        font.pixelSize: 17 / 1.3 /*Style.monitorRatio*/
                        font.family: "Roboto"
                        color: backgroundColor
                        text: operatorListModel ? operatorListModel.operatorName : ""
                        Layout.maximumWidth: 85 / 1.3 /*Style.monitorRatio*/
                        Layout.preferredWidth: 85 / 1.3 /*Style.monitorRatio*/
                        elide: Text.ElideRight
                        ToolTip {
                            id: control
                            property string toolTipText: txt.text
                            text: toolTipText
                            y: -20
                            visible: toolTipText ? ma.containsMouse : false
                            font.pixelSize: 17 / 1.3 /*Style.monitorRatio*/
                            font.family: "Roboto"
                            contentItem: Text {
                                text: control.text
                                font: control.font
                                color: "gold"
                            }
                            background: Rectangle {
                                radius: 10
                                border.color: root.backgroundColor
                                border.width: 3
                                color: root.bg60
                            }
                        }

                        MouseArea {
                            id: ma
                            anchors.fill: parent
                            hoverEnabled: true
                        }

                        onTextChanged: {
                            root.addCheck = false
                            root.removeCheck = false
                            assignmentListModel.onRemoveButtonChecked(false)
                            assignmentListModel.onAddButtonChecked(false)
                        }
                    }
                }
                Rectangle {
                    id: attackholder
//                    Layout.leftMargin: -15
                    width: 75 / 1.3 /*Style.monitorRatio*/
                    height: 75 / 1.3 /*Style.monitorRatio*/
                    color: "transparent"
                    //                    border.width: 2
                    //                    border.color: "gold"
                    Rectangle{
                        id:attack
                        anchors.top: parent.top
                        anchors.topMargin: 2
                        anchors.left: parent.left
                        width: attackMA.containsMouse ? parent.width + 5 : parent.width
                        height: attackMA.containsMouse ? parent.height / 3 :  parent.height / 3 - 3
                        color: "#119cbf"
                        radius: 15

                        Text {
                            id: name
                            anchors.centerIn: parent
                            text: qsTr("Assign")
                            font.family: "Roboto"
                            font.pixelSize: attackMA.containsMouse ? 15 / 1.3 : 13/1.3
                            //                            font.weight: Font.Bold
                            color: "white"
                        }
                        MouseArea {
                            id:attackMA
                            anchors.fill: parent
                            onClicked: assignmentListModel.onAttackButtonClicked()
                            hoverEnabled: true

                        }
                    }
                    Rectangle{
                        anchors.centerIn: parent
                        width: selectMA.containsMouse ? parent.width + 5 : parent.width
                        height: selectMA.containsMouse ? parent.height / 3 :  parent.height / 3 - 3
                        color:"#0b42b0"
                        radius: 15
                        Text {
                            id: nameSelect
                            anchors.centerIn: parent
                            text: qsTr("Select All")
                            font.family: "Roboto"
                            font.pixelSize: selectMA.containsMouse ? 15 / 1.3 : 13/1.3
                            //                            font.weight: Font.Bold
                            color: "white"
                        }
                        MouseArea {
                            id:selectMA
                            anchors.fill: parent
                            onClicked: {
                                assignmentListModel.selectAll(selectall)

                                for (var i = 0; i < modelDataContainer.count; ++i) {
                                    var item = modelDataContainer.itemAt(i);
                                    if (item !== null && selectall) {
                                        item.color =  bg20
                                    }
                                    else
                                        item.color =  "transparent"

                                }
                                selectall =! selectall
                            }
                            hoverEnabled: true

                        }
                    }
                    Rectangle{
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 1
                        width: closeMA.containsMouse ? parent.width + 5 : parent.width
                        height: closeMA.containsMouse ? parent.height / 3 :  parent.height / 3 - 3
                        color:"#2fde78"
                        radius: 15
                        Text {
                            id: nameClose
                            anchors.centerIn: parent
                            text: qsTr("Cancel")
                            font.family: "Roboto"
                            font.pixelSize: closeMA.containsMouse ? 15 / 1.3 : 13/1.3
                            color: "white"
                        }
                        MouseArea {
                            id:closeMA
                            anchors.fill: parent
                            onClicked: {assignmentListModel.onCancelButtonClicked()}
                            hoverEnabled: true

                        }
                    }
                }
            }
        }

        Flickable {
            id: flickable
            width: rowLay.width > 425 ?425 : rowLay.width  ; height: rowLay.height
            Layout.leftMargin: 8
            Layout.bottomMargin: nodeInfoHolder.height - 5 / 1.3
            contentWidth: rowLay.width; contentHeight:  rowLay.height
            flickableDirection : Flickable.HorizontalFlick
            boundsMovement: Flickable.StopAtBounds
            clip: true
            boundsBehavior: Flickable.DragOverBounds

            RowLayout{
                id: rowLay
                Layout.alignment: Qt.AlignVCenter
                //                Layout.leftMargin:
                Layout.maximumWidth: 425
                spacing: 0
                clip: true
                onWidthChanged: {
                    if(width > 420){
                        rightMotionOpen.running = true
                        leftMotionOpen.running = true
                        marginMotionOp.running = true
                    }
                    else{
                        rightMotionClose.running = true
                        leftMotionClose.running = true
                        marginMotionCl.running = true
                    }
                }

                Repeater {
                    id: modelDataContainer
                    model: root.assignmentListModel
                    delegate: Rectangle {
                        id: node
                        width: 131 / 1.3 /*Style.monitorRatio*/
                        height: 65 / 1.3 /*Style.monitorRatio*/
                        color: objectSelection ? bg20 : "transparent"
                        z: -2
                        radius: 7
                        Layout.leftMargin: 5 / 1.3

                        Rectangle {
                            anchors.top: parent.top
                            anchors.topMargin: 5
                            anchors.horizontalCenter: parent.horizontalCenter
                            height: parent.height / 2
                            width: parent.width
                            color: "transparent"

                            IconImage {
                                id: myIcon
                                anchors.centerIn: parent
                                source: objectIcon
                                width: 35 / 1.3 /*Style.monitorRatio*/
                                height: 35 / 1.3 /*Style.monitorRatio*/
                                color: backgroundColor/*objectColor*/
                            }
                        }
                        Rectangle {
                            anchors.bottom: parent.bottom
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.bottomMargin: -3
                            height: parent.height / 2
                            width: parent.width
                            color: "transparent"

                            Image {
                                id: repeaterImg
                                source: "qrc:/Resources/information.png"
                                width: 24 / 1.3
                                height: 24 / 1.3
                                anchors.left: parent.left
                                anchors.leftMargin: 30 / 1.3
                                anchors.verticalCenter: parent.verticalCenter
                                antialiasing: true
                            }
                            Text {
                                width:75 / 1.3
                                elide: Text.ElideRight
                                anchors.left: repeaterImg.right
                                anchors.leftMargin: 5
                                text: objectID
                                font.pixelSize: 17 / 1.3 /*Style.monitorRatio*/
                                color: root.backgroundColor
                                anchors.verticalCenter: parent.verticalCenter
                            }
                        }
                        MouseArea {
                            anchors.fill: parent
                            hoverEnabled: true
                            onClicked: {
                                assignmentListModel.onMenuItemSelect(
                                            assignmentListModel.index(index,0).row)
                            }
                            onEntered: {
                                node.color = bg20
                                assignmentListModel.onItemHovered(
                                            assignmentListModel.index(index,0).row,true)
                            }
                            onExited: {
                                if (!objectSelection === true) {
                                    node.color = "transparent"
                                }
                                assignmentListModel.onItemHovered(
                                            assignmentListModel.index(index,0).row,false)
                            }
                        }
                    }
                }
            }
        }
        //        }
        //        }
    }
    /////   nodeholderbackground
    Rectangle {

        id: nodesBackground
        anchors.left: mainRow.left
        anchors.verticalCenter: mainRow.verticalCenter
        anchors.leftMargin: nodeInfoHolder.width - attackholder.width / 2
        radius: 20
        color: fg75

        width: (rowLay.childrenRect.width  > 420 ? 524 : (rowLay.childrenRect.width  +  90))
        //                / 1.3) ? 645 / 1.3  : */rowLay.childrenRect.width  +  80
        height: 75 / 1.3 /*Style.monitorRatio*/
        z: -2
        Rectangle{
            id:right
            anchors.right: parent.right
            anchors.rightMargin: 0
            height: 75 / 1.3 /*Style.monitorRatio*/
            width: 55 / 1.3 /*Style.monitorRatio*/
            color: Qt.rgba(foregroundColor.r,foregroundColor.g,foregroundColor.b,0.01)
            radius: 20
            //            border.width: 1
            //            border.color: "white"
            IconImage {
                id:rightIcon
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                anchors.leftMargin: -5
                source: "qrc:/Resources/down"
                height: 30 / 1.3 /*Style.monitorRatio*/
                width: 30 / 1.3 /*Style.monitorRatio*/

                rotation: 90
                MouseArea{
                    anchors.fill: parent
                    hoverEnabled: true
//                    propagateComposedEvents: true
                    onClicked:{
                        flickable.contentX -= 131 / 1.3

                    }
                    onEntered: rightIcon.width = rightIcon.width * 1.3
                    onExited: rightIcon.width = 30 / 1.3
                }
            }
        }
        Rectangle{

            id:left
            height: 75 / 1.3 /*Style.monitorRatio*/
            width: /*55 / 1.3 /*Style.monitorRatio*/ 0
            color: Qt.rgba(foregroundColor.r,foregroundColor.g,foregroundColor.b,0.01)
            radius: 20

            IconImage {
                id:leftIcon
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                anchors.rightMargin: -5
                source: "qrc:/Resources/down"
                height: 30 / 1.3 /*Style.monitorRatio*/
                width: 30 / 1.3 /*Style.monitorRatio*/

                rotation: -90
                MouseArea{
                    anchors.fill: parent
                    hoverEnabled: true
                    propagateComposedEvents: true
                    onClicked:{
                                  print(flickable.contentX)
                                  flickable.contentX += 131 / 1.3

                              }
                    onEntered: leftIcon.width = leftIcon.width * 1.3
                    onExited: leftIcon.width = 30 / 1.3
                }
            }


        }

        Rectangle{
            height: 75 / 1.3 /*Style.monitorRatio*/
            width: 55 / 1.3 /*Style.monitorRatio*/
            radius: 20
            color:/*Qt.rgba(nodeInfoHolder.r, nodeInfoHolder.g,
                          nodeInfoHolder.b, .03)*/"#DEE3E6"
            anchors.right: parent.right


            Rectangle {
                id:addbtn
                anchors.top:parent.top
                anchors.topMargin: 8/1.3
                anchors.horizontalCenter: parent.horizontalCenter
                radius: parent.radius
                width: 39 / 1.3 /*Style.monitorRatio*/
                height: 29 / 1.3  /*Style.monitorRatio*/
                color: addCheck ? root.bg20 : "transparent"
                IconImage {
                    anchors.top: parent.top
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    source: "qrc:/Resources/add"
                    height: 25 / 1.3 /*Style.monitorRatio*/
                    width: 25 / 1.3 /*Style.monitorRatio*/


                    MouseArea {
                        anchors.fill: parent
//                        hoverEnabled:true
                        onEntered: addbtn.color = root.bg20
                        onExited: {if(!addCheck)
                                addbtn.color = "transparent"
                                print(addCheck)
                        }
                        z:3
                        onClicked: {
                            print(nodesBackground.width )
                            addCheck = !addCheck
                            removeCheck = false
                            assignmentListModel.onAddButtonChecked(addCheck)
                            if(addCheck){
                                deletebtn.color = "transparent"
                            }

                        }
                    }
                }
            }
            Rectangle {
                id:deletebtn
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 8/1.3
                anchors.horizontalCenter: parent.horizontalCenter
                radius: parent.radius
                width: 39 / 1.3 /*Style.monitorRatio*/
                height: 29 / 1.3  /*Style.monitorRatio*/

                color: removeCheck ? root.bg20 : "transparent"
                IconImage {
                    anchors.bottom: parent.bottom
                    anchors.horizontalCenter: parent.horizontalCenter
                    source: "qrc:/Resources/location-delete"
                    height: 25 / 1.3 /*Style.monitorRatio*/
                    width: 25 / 1.3 /*Style.monitorRatio*/

                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled:true
                        onEntered: deletebtn.color = root.bg20
                        propagateComposedEvents: true
                        onExited: {if(!removeCheck)
                                deletebtn.color = "transparent"
                        print(removeCheck)}
                        onClicked:(mouse)=> {
                            removeCheck = !removeCheck
                            addCheck = false
                            assignmentListModel.onRemoveButtonChecked(removeCheck)
                            if(removeCheck){
                                addbtn.color = "transparent"
                                      }

                        }
                    }
                }
            }

        }
        PropertyAnimation {
            id: rightMotionOpen
            target: right
            properties: "anchors.rightMargin"
            to: 13
            from: 0
            duration: 200
            easing.type: Easing.OutQuint
        }
        PropertyAnimation {
            id: rightMotionClose
            target: right
            properties: "anchors.rightMargin"
            to: 0
            duration: 200
            easing.type: Easing.OutQuint
        }

        PropertyAnimation {
            id: leftMotionOpen
            target: left
            properties: "width"
            to: 55 / 1.3
            from: 0
            duration: 200
            easing.type: Easing.OutQuint
        }
        PropertyAnimation {
            id: leftMotionClose
            target: left
            properties: "width"
            to: 0
            duration: 200
            easing.type: Easing.OutQuint
        }

    }

    Rectangle {
        id:closebtn
        width: 45 / 1.3 /*Style.monitorRatio*/
        height: 45 / 1.3 /*Style.monitorRatio*/
        color: fg50
        radius: width / 2
        anchors.left: nodesBackground.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.leftMargin: 10 / 1.3
        IconImage {
            anchors.centerIn: parent
            source: "qrc:/Resources/close-icon"
            height: 35 / 1.3 /*Style.monitorRatio*/
            width: 35 / 1.3 /*Style.monitorRatio*/
            color: "white"
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                assignmentListModel.onCloseMenuClicked()
                root.addCheck = false
                root.removeCheck = false
                assignmentListModel.onRemoveButtonChecked(false)
                assignmentListModel.onAddButtonChecked(false)
            }
        }
    }
    //////////

    Rectangle{
        width: bottomLayer .width   < 215 / 1.3  ? bottomLayer .width  : 215 / 1.3
        height: 150 / 1.3
        color: "#DEE3E6"
        anchors.left: mainRow.left
        anchors.top: mainRow.top
        z:-2
        radius: 20

    }

    ////////// bottom layer

    Rectangle {
        id:bottomLayer
        color: "#DEE3E6"
        radius: 20
        width: (downIcon.rotation / 180) % 2 !== 0 ?  (operatorLayout.width + battleLocationIcons.width + 10 < 455 ?
                                                           operatorLayout.width + battleLocationIcons.width + 10
                                                         : 455) : 0
        height: 70 / 1.3
        anchors.left: mainRow.left
        anchors.top: mainRow.bottom
        anchors.topMargin: 5
        clip: true
        Rectangle {
            id:battleLocationIcons
            property bool flag: operatorListModel ? operatorListModel.operatorIsAttacker : 0
            anchors.top: parent.top
            anchors.left: parent.left
            height: 70 / 1.3
            width : 55 / 1.3
            color :Qt.rgba(nodeInfoHolder.r, nodeInfoHolder.g,
                           nodeInfoHolder.b, .02)
            radius: 20
            Rectangle{
                width: 39 / 1.3 /*Style.monitorRatio*/
                height: 29 / 1.3  /*Style.monitorRatio*/
                anchors.top: parent.top
                anchors.topMargin: 7 /1.3
                anchors.horizontalCenter: parent.horizontalCenter
                radius: 12
                color: mouseArea.containsMouse ? bg20 : (battleLocationIcons.flag? bg20 :"transparent")
                IconImage {
                    anchors.centerIn: parent
                    source: "qrc:/Resources/attacker.png"
                    width: 25 / 1.3 /*Style.monitorRatio*/
                    height: 25 / 1.3 /*Style.monitorRatio*/
                    //                color: operatorColor
                }
                MouseArea{
                    id:mouseArea
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: {battleLocationIcons.flag= !battleLocationIcons.flag
                                operatorListModel.operatorToggle(true)
                    }
                }
            }

            Rectangle{
                width: 39 / 1.3 /*Style.monitorRatio*/
                height: 29 / 1.3  /*Style.monitorRatio*/
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 7 / 1.3
                anchors.horizontalCenter: parent.horizontalCenter
                radius: parent.radius
                color: mouseA.containsMouse ? bg20 : (battleLocationIcons.flag ? "transparent" : bg20)

                IconImage {
                    anchors.centerIn: parent
                    source: "qrc:/Resources/target.png"
                    width: 25 / 1.3 /*Style.monitorRatio*/
                    height: 25 / 1.3 /*Style.monitorRatio*/
                    //                color: operatorColor
                    color: backgroundColor
                }
                MouseArea{
                    id:mouseA
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: {battleLocationIcons.flag= !battleLocationIcons.flag
                    operatorListModel.operatorToggle(false)
                }

                }
            }
        }
        ScrollView{
            anchors.left: battleLocationIcons.right
            width: 405

            Layout.topMargin: 5 / 1.3
            ScrollBar.horizontal.interactive: true
            clip: true
            RowLayout {
                id: operatorLayout
                Layout.alignment: Qt.AlignVCenter
                //            anchors.left: battleLocationIcons.right
                Layout.maximumWidth: 450
                Layout.topMargin: 5 / 1.3
                spacing: 0
                clip: true
                Repeater {
                    id: operatorModelContainer
                    model: root.operatorListModel

                    delegate: Rectangle {
                        id: objects
                        width: 81 / 1.3 /*Style.monitorRatio*/
                        height: 60 / 1.3 /*Style.monitorRatio*/
                        color: operatorSelect ? bg20 : "transparent"
                        z: -2
                        radius: 7
                        Layout.topMargin: 5 / 1.3
                        Layout.leftMargin: 5 / 1.3

                        Rectangle {
                            anchors.top: parent.top
                            anchors.topMargin: 5
                            anchors.horizontalCenter: parent.horizontalCenter
                            height: parent.height / 2
                            width: parent.width
                            color: "transparent"

                            IconImage {
                                id: opIco
                                anchors.centerIn: parent
                                source: operatorIcon
                                width: 35 / 1.3 /*Style.monitorRatio*/
                                height: 35 / 1.3 /*Style.monitorRatio*/
                                color: backgroundColor /*operatorColor*/
                            }
                        }
                        RowLayout {
                            anchors.bottom: parent.bottom
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.bottomMargin: -3
                            height: parent.height / 2
                            width: parent.width
                            //                        color: "transparent"

                            Text {
                                Layout.maximumWidth: 81 / 1.3
                                Layout.alignment: Qt.AlignHCenter
                                text: operatorName
                                font.pixelSize: 17 / 1.3 /*Style.monitorRatio*/
                                color:backgroundColor /*operatorSelect ? "red" : operatorColor*/
                                //                            anchors.verticalCenter: parent.verticalCenter
                                elide: Text.ElideRight
                            }
                        }
                        MouseArea {
                            anchors.fill: parent
                            hoverEnabled: true
                            onClicked: {
                                operatorListModel.select(operatorListModel.index(index, 0).row)
                            }
                            onEntered: {
                                if(!operatorSelect){
                                    objects.color = bg20
                                }
                            }
                            onExited: {
                                if(!operatorSelect){
                                    objects.color = "transparent"
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    PropertyAnimation {
        id: openMotion
        target: bottomLayer
        properties: "width"
        to: bottomLayer.width
        from: 0
        duration: 200
        easing.type: Easing.OutQuint
    }

    PropertyAnimation {
        id: marginMotionOp
        target: attackholder
        properties: "Layout.leftMargin"
        to: -15
        duration: 200
        easing.type: Easing.OutQuint
    }
    PropertyAnimation {
        id: marginMotionCl
        target: attackholder
        properties: "Layout.leftMargin"
        to:0
        duration: 200
        easing.type: Easing.OutQuint
    }
}

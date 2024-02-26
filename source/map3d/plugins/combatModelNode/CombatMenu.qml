import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import "style"

Item {
    id: root
    anchors.fill: parent
    property var assignmentListModel
    property var operatorListModel

    property bool addCheck: false
    property bool removeCheck: false
    property bool selectall: true

    property bool opIsAttacker: false

    Connections {
        target: operatorListModel ? operatorListModel : null
        function onOperatorListChanged(attackerType){
            root.opIsAttacker = attackerType
        }
    }


    readonly property color foregroundColor: "#DEE3E6"
    readonly property color fg75: Qt.rgba(foregroundColor.r, foregroundColor.g,foregroundColor.b, 0.75)
    readonly property color fg50: Qt.rgba(foregroundColor.r, foregroundColor.g,foregroundColor.b, 0.50)
    readonly property color fg02: Qt.rgba(foregroundColor.r, foregroundColor.g,foregroundColor.b, 0.02)
    readonly property color backgroundColor: "#003569"
    readonly property color bg20: Qt.rgba(backgroundColor.r, backgroundColor.g,backgroundColor.b, 0.20)
    readonly property color bg60: Qt.rgba(backgroundColor.r, backgroundColor.g,backgroundColor.b, 0.8)

    RowLayout {
        anchors.centerIn: parent
        id: mainRow
        z:1
        Rectangle {
            id: nodeInfoHolder
            width: 300 / Style.monitorRatio
            height: 75 / Style.monitorRatio
            radius: 20 / Style.monitorRatio
            color: foregroundColor
            RowLayout {
                anchors.left: parent.left
                Rectangle {
                    id:downController
                    Layout.preferredHeight: 75 / Style.monitorRatio
                    Layout.preferredWidth: 55 / Style.monitorRatio
                    radius: 20 / Style.monitorRatio
                    color: fg02
                    IconImage {
                        id:downIcon
                        source: "qrc:/Resources/down.png"
                        height: 25 / Style.monitorRatio
                        width: 25 / Style.monitorRatio
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
                    Layout.preferredHeight: 55 / Style.monitorRatio
                    Layout.preferredWidth: 55 / Style.monitorRatio
                    color: backgroundColor/*operatorListModel ? operatorListModel.operatorColor : "black"*/
                }

                ColumnLayout {
                    Layout.leftMargin: 10 / Style.monitorRatio
                    RowLayout {
                        id: bulletInfo
                        visible: opIsAttacker
                        IconImage {
                            source: "qrc:/Resources/bullet.png"
                            Layout.preferredHeight: 22 / Style.monitorRatio
                            Layout.preferredWidth: 22 / Style.monitorRatio
                        }
                        Text {
                            font.pixelSize: 17 / Style.monitorRatio
                            font.family: "Roboto"
                            color: backgroundColor
                            text: operatorListModel ? 30 : ""
                        }
                    }
                    Text {
                        id: txt
                        font.pixelSize: 17 / Style.monitorRatio
                        font.family: "Roboto"
                        color: backgroundColor
                        text: operatorListModel ? operatorListModel.operatorName : ""
                        Layout.maximumWidth: 85 / Style.monitorRatio
                        Layout.preferredWidth: 85 / Style.monitorRatio
                        elide: Text.ElideRight
                        ToolTip {
                            id: control
                            property string toolTipText: txt.text
                            text: toolTipText
                            y: -20
                            visible: toolTipText ? ma.containsMouse : false
                            font.pixelSize: 17 / Style.monitorRatio
                            font.family: "Roboto"
                            contentItem: Text {
                                text: control.text
                                font: control.font
                                color: "gold"
                            }
                            background: Rectangle {
                                radius: 10 / Style.monitorRatio
                                border.color: root.backgroundColor
                                border.width: 3 / Style.monitorRatio
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
                            addbtn.color = "transparent"
                            deletebtn.color = "transparent"
                        }
                    }
                }
                Rectangle {
                    id: attackholder
                    Layout.leftMargin: -10 / Style.monitorRatio
                    width: 77 / Style.monitorRatio
                    height: 75 / Style.monitorRatio
                    color: "transparent"

                    Rectangle{
                        id:attack
                        property color red: "red"


                        anchors.top: parent.top
                        anchors.topMargin: 6 / Style.monitorRatio
                        anchors.left: parent.left
                        width: attackMA.containsMouse ? 82 / Style.monitorRatio : 77 / Style.monitorRatio
                        height: attackMA.containsMouse ?  32 / Style.monitorRatio : 27 / Style.monitorRatio
                        gradient: Gradient {
                            orientation: Gradient.Horizontal
                            GradientStop { position: 0.0; color: attack.red }
                            GradientStop { position: 1.0; color: Qt.rgba(attack.red.r,attack.red.g,attack.red.b,0.5)}
                        }
//                        color: Qt.rgba(red.r,red.g,red.b,0.8)
                        radius: 15 / Style.monitorRatio

                        Text {
                            id: name
                            anchors.centerIn: parent
                            text: qsTr("Assign")
                            font.family: "Roboto"
                            font.pixelSize: attackMA.containsMouse ? 15 / Style.monitorRatio : 13/Style.monitorRatio
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
                        id:cancel
                        property color blue65: Qt.rgba(backgroundColor.r,backgroundColor.g,backgroundColor.b,.35)
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 7 / Style.monitorRatio
                        width: closeMA.containsMouse ?  82 / Style.monitorRatio : 77 / Style.monitorRatio
                        height: closeMA.containsMouse ? 32 / Style.monitorRatio : 27 / Style.monitorRatio
                        gradient: Gradient {
                            orientation: Gradient.Horizontal
                            GradientStop { position: 0.0; color: root.backgroundColor }
                            GradientStop { position: 1.0; color: cancel.blue65}
                        }
                        radius: 15 / Style.monitorRatio
                        Text {
                            id: nameClose
                            anchors.centerIn: parent
                            text: qsTr("Cancel")
                            font.family: "Roboto"
                            font.pixelSize: closeMA.containsMouse ? 15 / Style.monitorRatio : 13 / Style.monitorRatio
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

        Rectangle{
            width:55 / Style.monitorRatio
            height: 75 / Style.monitorRatio
            color: fg75
            radius: 20 / Style.monitorRatio
            ColumnLayout{
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter

                IconImage {
                    property bool flag: false
                    id: tick
                    sourceSize:flag ? "15x15" : "11x11"
                    source: flag ? "qrc:/Resources/tick7" : "qrc:/Resources/tick6"
                    width: 15 / Style.monitorRatio
                    height: 15 / Style.monitorRatio
                    color: backgroundColor/*objectColor*/
                    Layout.alignment:  Qt.AlignHCenter
                    MouseArea{
                        anchors.fill : parent
                        onClicked:{ tick.flag = !tick.flag

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

                    }
                }
                Label{
                    text: "All"
                    font.pixelSize: 15 / Style.monitorRatio
                    Layout.alignment: Qt.AlignHCenter
                    color: backgroundColor
                }
            }
        }
        IconImage {
            id:rightIcon
            source: "qrc:/Resources/down"
            Layout.maximumWidth:20 / Style.monitorRatio
            Layout.preferredHeight:  20 / Style.monitorRatio

            rotation: 90
            MouseArea{
                anchors.fill: parent
                hoverEnabled: true
                onClicked:{
                    flickable.contentX -= 131 / Style.monitorRatio


                }
                onEntered: rightIcon.width = rightIcon.width * Style.monitorRatio
                onExited: rightIcon.width =20 / Style.monitorRatio
            }
        }



                Flickable {
                    id: flickable
                    Layout.minimumWidth: 270/ Style.monitorRatio
                    height: rowLay.childrenRect.height
                    Layout.bottomMargin: nodeInfoHolder.height - 5 / Style.monitorRatio
                    contentWidth: rowLay.width; contentHeight:  rowLay.height
                    flickableDirection : Flickable.HorizontalFlick
                    boundsMovement: Flickable.StopAtBounds
                    clip: true
                    boundsBehavior: Flickable.DragOverBounds

                    RowLayout{
                        id: rowLay
                        Layout.alignment: Qt.AlignVCenter
                        //                Layout.leftMargin:
                        Layout.maximumWidth: 425 / Style.monitorRatio
                        spacing: 0
                        clip: true

                        Repeater {
                            id: modelDataContainer
                            model: root.assignmentListModel
                            delegate: Rectangle {
                                id: node
                                width: 131 / Style.monitorRatio
                                height: 65 / Style.monitorRatio
                                color: objectSelection ? bg20 : "transparent"
                                z: -2
                                radius: 7
                                Layout.leftMargin: 5 / Style.monitorRatio

                                Rectangle {
                                    anchors.top: parent.top
                                    anchors.topMargin: 5
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    height: parent.height / 2
                                    width: parent.width
                                    color: "transparent"

                                    IconImage {
                                        id: myIcon
                                        anchors.centerIn:  parent
                                        source: objectIcon
                                        width: 35 / Style.monitorRatio
                                        height: 35 / Style.monitorRatio
                                        color: backgroundColor/*objectColor*/
                                    }
                                    IconImage {
                                        id: bulletIcon
                                        visible: !opIsAttacker
                                        anchors.left: myIcon.right
                                        source: "qrc:/Resources/bullet.png"
                                        width: 35 / Style.monitorRatio
                                        height: 35 / Style.monitorRatio
                                        color: backgroundColor/*objectColor*/
                                    }
                                }
                                Rectangle {
                                    anchors.bottom: parent.bottom
                                    anchors.left: parent.left
                                    anchors.leftMargin: -10/ Style.monitorRatio
                                    anchors.bottomMargin: -3
                                    height: parent.height / 2
                                    width: parent.width
                                    color: "transparent"

                                    Image {
                                        id: repeaterImg
                                        source: "qrc:/Resources/information.png"
                                        width: 24 / Style.monitorRatio
                                        height: 24 / Style.monitorRatio
                                        anchors.left: parent.left
                                        anchors.leftMargin: 30 / Style.monitorRatio
                                        anchors.verticalCenter: parent.verticalCenter
                                        antialiasing: true
                                    }
                                    Text {
                                        width:70 / Style.monitorRatio
                                        elide: Text.ElideRight
                                        anchors.left: repeaterImg.right
                                        anchors.leftMargin: 5
                                        text: objectID
                                        font.pixelSize: 17 / Style.monitorRatio
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


        IconImage {
            id:leftIcon
            source: "qrc:/Resources/down"
            Layout.maximumWidth:20 / Style.monitorRatio
            Layout.preferredHeight:  20 / Style.monitorRatio
            rotation: -90
            MouseArea{
                anchors.fill: parent
                hoverEnabled: true
                onClicked:{
                    flickable.contentX += 131 / Style.monitorRatio

                }
                onEntered: leftIcon.width = leftIcon.width * Style.monitorRatio
                onExited: leftIcon.width = 20 / Style.monitorRatio
            }
        }

        Rectangle{
            height: 75 / Style.monitorRatio
            width: 55 / Style.monitorRatio
            radius: 20 / Style.monitorRatio
            color:fg50
            Rectangle {
                id:addbtn
                anchors.top:parent.top
                anchors.topMargin: 8/Style.monitorRatio
                anchors.horizontalCenter: parent.horizontalCenter
                radius: parent.radius
                width: 39 / Style.monitorRatio
                height: 29 / Style.monitorRatio
                color: addCheck ? root.bg20 : "transparent"
                IconImage {
                    anchors.top: parent.top
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    source: "qrc:/Resources/add"
                    height: 25 / Style.monitorRatio
                    width: 25 / Style.monitorRatio


                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled:true
                        onEntered: addbtn.color = root.bg20
                        onExited: {if(!addCheck)
                                addbtn.color = "transparent"
                        }
                        z:3
                        onClicked: {
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
                anchors.bottomMargin: 8/Style.monitorRatio
                anchors.horizontalCenter: parent.horizontalCenter
                radius: parent.radius
                width: 39 / Style.monitorRatio
                height: 29 / Style.monitorRatio

                color: removeCheck ? root.bg20 : "transparent"
                IconImage {
                    anchors.bottom: parent.bottom
                    anchors.horizontalCenter: parent.horizontalCenter
                    source: "qrc:/Resources/location-delete"
                    height: 25 / Style.monitorRatio
                    width: 25 / Style.monitorRatio

                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled:true
                        onEntered: deletebtn.color = root.bg20
                        propagateComposedEvents: true
                        onExited: {if(!removeCheck)
                                deletebtn.color = "transparent"
                        }
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

    }
    Rectangle{
        anchors.fill: mainRow
        anchors.bottomMargin: 1 / Style.monitorRatio
        color: fg50
        z:0
        radius: 20 / Style.monitorRatio
    }

    Rectangle {
        id:closebtn
        width: 45 / Style.monitorRatio
        height: 45 / Style.monitorRatio
        color: fg50
        radius: width / 2
        anchors.left: mainRow.right
        anchors.verticalCenter: mainRow.verticalCenter
        IconImage {
            anchors.centerIn: parent
            source: "qrc:/Resources/close-icon"
            height: 35 / Style.monitorRatio
            width: 35 / Style.monitorRatio
            color: "white"
        }
        MouseArea {

            anchors.fill: parent
            onClicked: {
                assignmentListModel.onCloseMenuClicked()
                root.addCheck = false
                addbtn.color="transparent"
                deletebtn.color="transparent"
                root.removeCheck = false
                assignmentListModel.onRemoveButtonChecked(false)
                assignmentListModel.onAddButtonChecked(false)
                tick.flag = false
            }
        }
    }

    Rectangle{
        width: bottomLayer .width   < 215 / Style.monitorRatio  ? bottomLayer .width  : 215 / Style.monitorRatio
        height: 150 / Style.monitorRatio
        color: "#DEE3E6"
        anchors.left: mainRow.left
        anchors.top: mainRow.top
        z:-2
        radius: 20 / Style.monitorRatio

    }

    ////////// bottom layer

    Rectangle {
        id:bottomLayer
        color: "#DEE3E6"
        radius: 20 / Style.monitorRatio
        width: (downIcon.rotation / 180) % 2 !== 0 ?  (operatorLayout.width + battleLocationIcons.width + 10 < 455 ?
                                                           operatorLayout.width + battleLocationIcons.width + 10
                                                         : 455) : 0
        height: 70 / Style.monitorRatio
        anchors.left: mainRow.left
        anchors.top: mainRow.bottom
        anchors.topMargin: 5
        clip: true
        Rectangle {
            id:battleLocationIcons

            // property bool flag: operatorListModel ? operatorListModel.operaAttacker : 0
            anchors.top: parent.top
            anchors.left: parent.left
            height: 70 / Style.monitorRatio
            width : 55 / Style.monitorRatio
            color :Qt.rgba(nodeInfoHolder.r, nodeInfoHolder.g,
                           nodeInfoHolder.b, .02)
            radius: 20 / Style.monitorRatio
            Rectangle{
                width: 39 / Style.monitorRatio
                height: 29 / Style.monitorRatio
                anchors.top: parent.top
                anchors.topMargin: 7 /Style.monitorRatio
                anchors.horizontalCenter: parent.horizontalCenter
                radius: 12 / Style.monitorRatio
                color: mouseArea.containsMouse ? bg20 : (opIsAttacker ? bg20 :"transparent")
                IconImage {
                    anchors.centerIn: parent
                    source: "qrc:/Resources/attacker.png"
                    width: 25 / Style.monitorRatio
                    height: 25 / Style.monitorRatio
                    //                color: operatorColor
                }
                MouseArea{
                    id:mouseArea
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: {
                        // battleLocationIcons.flag= !battleLocationIcons.flag
                        operatorListModel.operatorToggle(true)
                        root.addCheck = false
                        root.removeCheck = false
                        assignmentListModel.onRemoveButtonChecked(false)
                        assignmentListModel.onAddButtonChecked(false)
                        addbtn.color = "transparent"
                        deletebtn.color = "transparent"
                        operatorListModel.select(operatorListModel.index(0 , 0).row)
                    }
                }
            }

            Rectangle{
                width: 39 / Style.monitorRatio
                height: 29 / Style.monitorRatio
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 7 / Style.monitorRatio
                anchors.horizontalCenter: parent.horizontalCenter
                radius: parent.radius
                color: mouseA.containsMouse ? bg20 : (opIsAttacker ? "transparent" : bg20)

                IconImage {
                    anchors.centerIn: parent
                    source: "qrc:/Resources/target.png"
                    width: 25 / Style.monitorRatio
                    height: 25 / Style.monitorRatio
                    //                color: operatorColor
                    color: backgroundColor
                }
                MouseArea{
                    id:mouseA
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: {
                        // battleLocationIcons.flag = !battleLocationIcons.flag
                        operatorListModel.operatorToggle(false)
                        root.addCheck = false
                        root.removeCheck = false
                        assignmentListModel.onRemoveButtonChecked(false)
                        assignmentListModel.onAddButtonChecked(false)
                        addbtn.color = "transparent"
                        deletebtn.color = "transparent"
                        operatorListModel.select(operatorListModel.index(0, 0).row)
                    }

                }
            }
        }
        ScrollView{
            anchors.left: battleLocationIcons.right
            width: 405

            Layout.topMargin: 5 / Style.monitorRatio
            ScrollBar.horizontal.interactive: true
            clip: true
            RowLayout {
                id: operatorLayout
                Layout.alignment: Qt.AlignVCenter
                //              anchors.left: battleLocationIcons.right
                Layout.maximumWidth: 450
                Layout.topMargin: 5 / Style.monitorRatio
                spacing: 0
                clip: true
                Repeater {
                    id: operatorModelContainer
                    model: root.operatorListModel

                    delegate: Rectangle {
                        id: objects
                        width: 81 / Style.monitorRatio
                        height: 60 / Style.monitorRatio
                        color: operatorSelect ? bg20 : "transparent"
                        z: -2
                        radius: 7 / Style.monitorRatio
                        Layout.topMargin: 5 / Style.monitorRatio
                        Layout.leftMargin: 5 / Style.monitorRatio

                        Rectangle {
                            anchors.top: parent.top
                            anchors.topMargin: 5 / Style.monitorRatio
                            anchors.horizontalCenter: parent.horizontalCenter
                            height: parent.height / 2
                            width: parent.width
                            color: "transparent"

                            IconImage {
                                id: opIco
                                anchors.centerIn: parent
                                source: operatorIcon
                                width: 35 / Style.monitorRatio
                                height: 35 / Style.monitorRatio
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
                                Layout.maximumWidth: 81 / Style.monitorRatio
                                Layout.alignment: Qt.AlignHCenter
                                text: operatorName
                                font.pixelSize: 17 / Style.monitorRatio
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

}

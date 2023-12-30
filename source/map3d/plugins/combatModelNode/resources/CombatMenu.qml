import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
//import "style"


Item {
    id:root
    anchors.fill: parent
    property var assignmentListModel
    property var operatorListModel

    property bool addCheck: false
    property bool removeCheck: false

    readonly property color foregroundColor: "#DEE3E6"
    readonly property color fg75: Qt.rgba(foregroundColor.r, foregroundColor.g, foregroundColor.b, 0.75)
    readonly property color fg50: Qt.rgba(foregroundColor.r, foregroundColor.g, foregroundColor.b, 0.50)

    readonly property color backgroundColor: "#003569"
    readonly property color bg20: Qt.rgba(backgroundColor.r, backgroundColor.g, backgroundColor.b, 0.20)

    RowLayout{
        id:mainRow
        anchors.centerIn: parent
        Rectangle {
            id:nodeInfoHolder
            width: 209 / 1.3/*Style.monitorRatio*/
            height: 75 / 1.3/*Style.monitorRatio*/
            radius: 10
            color: "#DEE3E6"

            RowLayout{
                anchors.centerIn: parent
                IconImage {
                   source:operatorListModel?  operatorListModel.operatorIcon : ""
                    Layout.preferredHeight: 55/1.3/*Style.monitorRatio*/
                    Layout.preferredWidth: 55/1.3/*Style.monitorRatio*/
                    Layout.leftMargin: 37.5/1.3/*Style.monitorRatio*/
                    color: operatorListModel?  operatorListModel.operatorColor : "black"
                }

                ColumnLayout{
                    RowLayout{
                        id : bulletInfo
                       visible: operatorListModel ? operatorListModel.operatorIsAttacker : false
                        IconImage {
                            source: "qrc:/Resources/bullet.png"
                            Layout.preferredHeight: 22/1.3/*Style.monitorRatio*/
                            Layout.preferredWidth: 22/1.3/*Style.monitorRatio*/
                        }
                        Text {
                            font.pixelSize: 17/1.3/*Style.monitorRatio*/
                            font.family: "Roboto"
                            color: backgroundColor
                           text: operatorListModel?  30 : ""
                        }
                    }
                    Text {
                        font.pixelSize: 17/1.3/*Style.monitorRatio*/
                        font.family: "Roboto"
                        color: backgroundColor
                       text: operatorListModel?  operatorListModel.operatorName : ""
                       onTextChanged: {
                           root.addCheck = false
                           root.removeCheck = false
                           assignmentListModel.onRemoveButtonChecked(false)
                           assignmentListModel.onAddButtonChecked(false)
                       }
                    }
                }
                Rectangle{
                    id:attackholder
                    width: 75 / 1.3/*Style.monitorRatio*/
                    height: 75 / 1.3/*Style.monitorRatio*/
                    radius: width / 2
                    Layout.leftMargin: 37.5/1.3/*Style.monitorRatio*/

                    gradient: Gradient {
                        GradientStop { position: 0.0; color: "#537597"  }
                        GradientStop { position: 1.0; color: "#003569" }
                    }

                    Rectangle{
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter

                        width: 65 / 1.3/*Style.monitorRatio*/
                        height: 65 / 1.3/*Style.monitorRatio*/
                        radius: width / 2
                        border{
                            width: 3
                            color: "#c8d0d5"
                        }
                        gradient: Gradient {
                            GradientStop { position: 0.0; color: "#DEE3E6" }
                            GradientStop { position: 1.0; color: "#DEE3E6" }
                        }
                        Rectangle{
                            property var model: assignmentListModel
                            readonly property color myRed: "#FF0000"
                            readonly property color myRed75: Qt.rgba(myRed.r,myRed.g,myRed.b, 0.75)
                            anchors.fill: parent
                            gradient: Gradient {
                                orientation: Gradient.Horizontal
                                GradientStop { position: 0.0; color: "#FF7C7C"  }
                                GradientStop { position: 1.0; color: "#FF0000" }
                            }
                            radius:parent.radius
                            border.width: 3
                            border.color: myRed75

                        }
                    }
                    Text {
                        id: name
                        anchors.centerIn: parent
                        text: qsTr("ATTACK")
                        font.family: "Roboto"
                        font.pixelSize: 13 / 1.3
                        font.weight: Font.Bold
                        color:"white"
                    }
                    MouseArea{
                        anchors.fill: parent
                        onClicked: assignmentListModel.onAttackButtonClicked()
                    }
                }
            }
        }

        ScrollView{
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.maximumWidth: 400
            Layout.topMargin: 5/1.3
            Layout.leftMargin: 15

            clip: true
            RowLayout{
                id:rowLay
                Layout.alignment: Qt.AlignVCenter
                Layout.maximumWidth: 400
                spacing: 0
                clip: true
                Repeater{
                    id:modelDataContainer
                    model: root.assignmentListModel

                    delegate: Rectangle{
                        id:node
                        width: 131 /  1.3/*Style.monitorRatio*/
                        height: 65 /1.3 /*Style.monitorRatio*/
                        color: objectSelection ? bg20 : "transparent"
                        z:-2
                        radius: 7
                        Layout.leftMargin: 5 / 1.3
                        Rectangle{
                            anchors.top: parent.top
                            anchors.topMargin: 5
                            anchors.horizontalCenter: parent.horizontalCenter
                            height: parent.height/2
                            width: parent.width
                            color:"transparent"


                            IconImage {
                                id:myIcon
                                anchors.centerIn: parent
                                source: objectIcon
                                width: 35 /  1.3/*Style.monitorRatio*/
                                height: 35 /1.3 /*Style.monitorRatio*/
                                color: objectColor

                            }
                        }
                        Rectangle{
                            anchors.bottom: parent.bottom
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.bottomMargin: -3
                            height: parent.height/2
                            width: parent.width
                            color:"transparent"

                            Image {
                                id:repeaterImg
                                source: "qrc:/Resources/information.png"
                                width: 24 / 1.3
                                height: 24 / 1.3
                                anchors.left: parent.left
                                anchors.leftMargin: 30/1.3
                                anchors.verticalCenter:  parent.verticalCenter
                                antialiasing: true
                            }
                            Text {
                                anchors.left: repeaterImg.right
                                anchors.leftMargin: 5
                                text: objectID
                                font.pixelSize: 17 / 1.3/*Style.monitorRatio*/
                                color: objectStateColor
                                anchors.verticalCenter: parent.verticalCenter

                            }
                        }
                        MouseArea{
                            anchors.fill: parent
                            hoverEnabled: true
                            onClicked: {
                                assignmentListModel.onMenuItemSelect(assignmentListModel.index(index,0).row)
                            }
                            onEntered: {
                                node.color = bg20
                                assignmentListModel.onItemHovered(assignmentListModel.index(index,0).row,true)
                            }
                            onExited: {
                                if (!objectSelection === true){
                                    node.color = "transparent"
                                }
                                assignmentListModel.onItemHovered(assignmentListModel.index(index,0).row,false)
                            }
                        }
                    }
                }
            }
        }
    }

    Rectangle{
        id:nodesBackground
        anchors.left: mainRow.left
        anchors.verticalCenter: mainRow.verticalCenter
        anchors.leftMargin: nodeInfoHolder.width -  attackholder.width / 2
        radius: 10
        color: fg75
        width: (rowLay.childrenRect.width + 30 + 65 / 1.3 >= 645/1.3) ?  645/1.3 +50 : rowLay.childrenRect.width + 30 + 75 +50
        height: 65 / 1.3 /*Style.monitorRatio*/
        z:-2
        Rectangle{
            anchors.right: parent.right
            width: 65 / 1.3 /*Style.monitorRatio*/
            height: 65 / 1.3  /*Style.monitorRatio*/
            color:"transparent"
            IconImage {
                anchors.top: parent.top
                source: "qrc:/Resources/add"
                height: 50/2/*Style.monitorRatio*/
                width: 50/2/*Style.monitorRatio*/
                color: addCheck ? "orange" : "#003569"
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        addCheck = !addCheck
                        removeCheck = false
                        assignmentListModel.onAddButtonChecked(addCheck)

                    }
                }
            }
        }
        Rectangle{
            anchors.right: parent.right
            width: 65 / 1.3 /*Style.monitorRatio*/
            height: 65 / 1.3  /*Style.monitorRatio*/
            color:"transparent"
            IconImage {
                anchors.bottom:  parent.bottom
                source: "qrc:/Resources/location-delete"
                height: 50/2/*Style.monitorRatio*/
                width: 50/2/*Style.monitorRatio*/
                color: removeCheck ? "orange" : "#003569"
                MouseArea{
                    anchors.fill: parent
                    onClicked:{
                        removeCheck = !removeCheck
                        addCheck = false
                        assignmentListModel.onRemoveButtonChecked(removeCheck)

                    }
                }
            }
        }
    }

    Rectangle{
        width: 45 / 1.3/*Style.monitorRatio*/
        height: 45 / 1.3/*Style.monitorRatio*/
        color: fg50
        radius: width / 2
        anchors.left: nodesBackground.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.leftMargin: 10 / 1.3
        IconImage {
            anchors.centerIn: parent
            source: "qrc:/Resources/close-icon"
            height: 35/1.3/*Style.monitorRatio*/
            width: 35/1.3/*Style.monitorRatio*/
            color: "white"
        }
        MouseArea{
            anchors.fill: parent
            onClicked: {assignmentListModel.onCloseMenuClicked();
                root.addCheck = false
                root.removeCheck = false
                assignmentListModel.onRemoveButtonChecked(false)
                assignmentListModel.onAddButtonChecked(false)
            }
        }
    }

    Rectangle{
        color: "#DEE3E6"
        radius: 10
        width: 600
        height: 60
        anchors.left: mainRow.left
        anchors.top : mainRow.bottom
        clip: true
        RowLayout{
            id:operatorLayout
            Layout.alignment: Qt.AlignVCenter
            Layout.maximumWidth: 600
            spacing: 0
            clip: true
            Repeater{
                id:operatorModelContainer
                model: root.operatorListModel

                delegate: Rectangle{
                    id:objects
                    width: 131 /  1.3/*Style.monitorRatio*/
                    height: 65 /1.3 /*Style.monitorRatio*/
                    color: operatorSelect ? bg20 : "transparent"
                    z:-2
                    radius: 7
                    Layout.leftMargin: 5 / 1.3
                    Rectangle{
                        anchors.top: parent.top
                        anchors.topMargin: 5
                        anchors.horizontalCenter: parent.horizontalCenter
                        height: parent.height/2
                        width: parent.width
                        color:"transparent"


                        IconImage {
                            id:opIcon
                            anchors.centerIn: parent
                            source: operatorIcon
                            width: 35 /  1.3/*Style.monitorRatio*/
                            height: 35 /1.3 /*Style.monitorRatio*/
                            color: operatorColor

                        }
                    }
                    Rectangle{
                        anchors.bottom: parent.bottom
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.bottomMargin: -3
                        height: parent.height/2
                        width: parent.width
                        color:"transparent"

                        Text {
                            anchors.leftMargin: 5
                            text: operatorName
                            font.pixelSize: 17 / 1.3/*Style.monitorRatio*/
                            color: operatorSelect ? "red" : operatorColor
                            anchors.verticalCenter: parent.verticalCenter

                        }
                    }
                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            operatorListModel.select(operatorListModel.index(index,0).row)

                        }
                    }
                }
            }
        }
    }
}


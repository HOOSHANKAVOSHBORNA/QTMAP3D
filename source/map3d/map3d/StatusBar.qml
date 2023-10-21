import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "style"

Rectangle {
    id: root
    color: Style.backgroundColor

    readonly property color lightBlue: Qt.rgba(Style.foregroundColor.r, Style.foregroundColor.g, Style.foregroundColor.b, 0.80)
    property bool heightVisiblity : false
    property real longitude : 0.0
    property real latitude : 0.0
    property real altitude : 0.0

    property real coordinate1 : 0.0
    property real coordinate2 : 0.0
    property real coordinate3 : 0.0

    property real heightFactor : 0.0

    property double massageFontSize: 16 / Style.monitorRatio
    property var model
    property var sourceModel
    property string modeMap: "geocentric"
    property var timer: 0
    property var message: ""

    property var fe: ["f", modeMap == "geocentric" ? "f" : "E", modeMap == "geocentric" ? "f" : "E"]

    function showMessage(messages, timerr){
        time.running = false
        timer = timerr
        message = messages
        time.running = true
    }

    function stopp(){
        if(timer != -1)
            message = "Ready"
    }


    // --------------  ---      ---
    Item {
        id: messegeLogoItem
        implicitWidth: Math.max(root.width-(theRowLayout.width), 270)
        height: parent.height
        visible: true

        Button {
            id: messageLogo
            //            topPadding: 0
            anchors.verticalCenter: parent.verticalCenter
            icon{
                source : "qrc:/Resources/message.png"
                height: 20/Style.monitorRatio
                width: 20/Style.monitorRatio
                color: hovered ? (pressed ? Style.foregroundColor: Style.hoverColor) :
                                 (pressed ? Style.hoverColor : Style.foregroundColor);

            }
            background:Rectangle {
                color:"transparent"
            }
            onClicked: {
                if(root.heightFactor == 0){
                    showRect.start()
                    root.heightVisiblity = true
                    heading.visible = true}
                else{

                    hideRect.start()
                    heading.visible = false
                    root.heightVisiblity = false}
            }

        }
        TextField{
            id:messageTextField
            wrapMode: Text.WrapAnywhere
            readOnly: true
            selectByMouse: false
            anchors.left: messageLogo.right
            anchors.verticalCenter: parent.verticalCenter
            implicitWidth:messegeLogoItem.width - messageTextField.x
            placeholderText: implicitWidth? root.model.data(root.model.index(0, 0), Qt.UserRole + 100) : ""
            color: Style.backgroundColor
            font.family: Style.fontFamily
            font.pointSize: Style.fontPointSize
            selectedTextColor: Style.backgroundColor
            selectionColor: Style.selectColor
            placeholderTextColor: Style.foregroundColor
            background: Rectangle{
                color: "transparent"
            }
        }

        Connections {
            target: root.sourceModel
            function onRowsInserted() {
                messageTextField.placeholderText = root.sourceModel.data(root.model.index(0, 0), Qt.UserRole + 100)
            }
            function onRowsRemoved() {
                messageTextField.placeholderText = root.sourceModel.data(root.model.index(0, 0), Qt.UserRole + 100) ?? 0
            }
        }
         MouseArea{
             anchors.fill: messageTextField
             onClicked: {
                 if(root.heightFactor == 0){
                     showRect.start()
                     root.heightVisiblity = true
                     heading.visible = true}
                 else{

                     hideRect.start()
                     heading.visible = false
                     root.heightVisiblity = false}
             }
         }
    }




    // Altitude ------------------------------------
    RowLayout{
        id:theRowLayout
        anchors.left: messegeLogoItem.right

        height: 22/Style.monitorRatio
        layoutDirection: Qt.RightToLeft
        clip:true
        spacing: 6/Style.monitorRatio
        Label {
            id: altitudevalue
            Layout.minimumWidth: 28/Style.monitorRatio + 80/Style.monitorRatio
            Layout.preferredHeight: 20/Style.monitorRatio
            background: Rectangle {
                color: "transparent"
            }

            Text {
                color: Style.foregroundColor
                font.pixelSize: 14/Style.monitorRatio
                text: root.model.range
                font.family: Style.fontFamily
            }
        }
        Label {
            id: altitudeText
            Layout.minimumWidth: 52/Style.monitorRatio
            Layout.preferredHeight: 20/Style.monitorRatio
            background: Rectangle {
                color: "transparent"
            }

            Text {
                color: Style.foregroundColor
                font.pixelSize: 14/Style.monitorRatio
                text: "Altitude: "
                font.family: Style.fontFamily
            }
        }

        Rectangle{
            id:seperator1
            Layout.alignment: Qt.AlignCenter
            width: 2/Style.monitorRatio
            height: 16/Style.monitorRatio
            color:Style.disableColor
            radius:10
        }

        // Space  ----------------------------------
        Rectangle{
            Layout.leftMargin: -5 *Style.monitorRatio
            width:50/Style.monitorRatio
            height: 6/Style.monitorRatio
            color: "transparent"
            Rectangle{
                width: 50/Style.monitorRatio
                height: 2/Style.monitorRatio
                color:Style.foregroundColor
                anchors.bottom: parent.bottom
            }
            Rectangle{
                width: 2/Style.monitorRatio
                height: 6/Style.monitorRatio
                color:Style.foregroundColor
                anchors.bottom: parent.bottom
                anchors.left: parent.left
            }
            Rectangle{
                width: 2/Style.monitorRatio
                height: 6/Style.monitorRatio
                color:Style.foregroundColor
                anchors.bottom: parent.bottom
                anchors.right: parent.right
            }

        }

        Label {
            id: coordinateText
            Layout.minimumWidth: 78/Style.monitorRatio
            Layout.preferredHeight: 20/Style.monitorRatio
            background: Rectangle {
                color: "transparent"
            }

            Text {
                color: Style.foregroundColor
                font.pixelSize: 14/Style.monitorRatio
                text: "1000 KM "
                font.family: Style.fontFamily
            }
        }
        Rectangle{
            id:seprator2
            Layout.alignment: Qt.AlignCenter
            Layout.leftMargin: 37/Style.monitorRatio
            width: 2/Style.monitorRatio
            height: 16/Style.monitorRatio
            color:Style.disableColor
            radius:10
        }

        // Long, Lat   Cordinate  -------------------------------
        Repeater {
            model: [coordinate3, coordinate2, coordinate1]
            Label {
                Layout.preferredWidth: 55/Style.monitorRatio
                Layout.preferredHeight: 20/Style.monitorRatio
                background: Rectangle {
                    color:"transparent"
                }

                Text {

                    Layout.alignment: Qt.AlignVCenter
                    color: lightBlue
                    text: Number(modelData).toLocaleString(Qt.locale(), root.fe[index], 3)
                    font.family: Style.fontFamily
                    font.pixelSize: 14/Style.monitorRatio


                }
            }
        }

        ComboBox {
            id: control
            model: ["Long, Lat", "Cordinate"]
            Layout.alignment: Qt.AlignCenter
            delegate: ItemDelegate {
                id:itemDelegate
                width: control.width
                background:Rectangle{
                    color:Style.backgroundColor
                    radius:5
                }

                contentItem: Text {
                    text: control.textRole
                          ? (Array.isArray(control.model) ? modelData[control.textRole] : model[control.textRole])
                          : modelData
                    color: Style.foregroundColor
                    font.family: Style.fontFamily
                    font.pixelSize: 14/Style.monitorRatio
                }
                highlighted: control.highlightedIndex === index
            }

            indicator: Canvas {
                id: canvas
                x: control.width - width - control.rightPadding
                y: control.topPadding + (control.availableHeight - height) / 2
                width: 12
                height: 8
                contextType: "2d"

                Connections {
                    target: control
                    function onPressedChanged() { canvas.requestPaint(); }
                }

                onPaint: {
                    context.reset();
                    context.moveTo(0, 0);
                    context.lineTo(width, 0);
                    context.lineTo(width / 2, height);
                    context.closePath();
                    context.fillStyle = control.pressed ? Style.backgroundColor : Style.foregroundColor;
                    context.fill();
                }
            }

            contentItem: Text {
                leftPadding: 0
                rightPadding: control.indicator.width + control.spacing

                text: control.displayText
                font.family: Style.fontFamily
                font.pixelSize: 14/Style.monitorRatio
                color:Style.foregroundColor
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }

            background: Rectangle {
                implicitWidth: 102/Style.monitorRatio
                implicitHeight:20/Style.monitorRatio
                color: Style.backgroundColor
                radius: 2
            }

            popup: Popup {
                y: control.height - 20
                width: control.width
                implicitHeight: contentItem.implicitHeight +2
                padding: 1

                contentItem: ListView {
                    clip: true
                    implicitHeight: contentHeight
                    model: control.popup.visible ? control.delegateModel : null
                    currentIndex: control.highlightedIndex

                    ScrollIndicator.vertical: ScrollIndicator { }
                }

                background: Rectangle {
                    color:"transparent"
                    radius: 2
                }
            }
        }

        Rectangle{
            id:sepratorRectangle
            Layout.alignment: Qt.AlignVCenter
            width: 2/Style.monitorRatio
            height: 16/Style.monitorRatio

            color:Style.disableColor
            radius:10
        }
        // -  messageContainer    -- -- -----------------

    }
    Rectangle{
        id:messageContainer
        anchors.bottom:root.top
        anchors.bottomMargin: 5/Style.monitorRatio
        implicitHeight: 0

        x: 3
        width: 670 / Style.monitorRatio
        height: 288/Style.monitorRatio * heightFactor
        color:Qt.rgba(Style.backgroundColor.r, Style.backgroundColor.g, Style.backgroundColor.b, 0.80)
        radius:10
        Item {
            id: heading

            visible:false
            width: 630 / Style.monitorRatio
            height:60/Style.monitorRatio
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            Label {
                id:massageHeadingLabel
                anchors.verticalCenter: parent.verticalCenter
                width: 87/Style.monitorRatio
                height: 28/Style.monitorRatio
                background: Rectangle {
                    color:"transparent"

                }
                Text {

                    Layout.alignment: Qt.AlignVCenter
                    color: Style.foregroundColor
                    text: "Messages"
                    font.family: Style.fontFamily
                    font.weight: Font.Medium
                    font.pixelSize: 24/Style.monitorRatio

                }
            }
            Button {
                id:garbage
                anchors.right:  searchBox.left
                anchors.verticalCenter: parent.verticalCenter
                antialiasing: true
                background: Rectangle{
                    radius:height/2
                    opacity: 1
                    color:"transparent"
                }

                icon{
                    source: "qrc:/Resources/garbage.png"
                    width: 24/Style.monitorRatio
                    height: 24/Style.monitorRatio
                }

                onClicked: root.model.removeMessage()

            }
            Button {
                id:closeBtn
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                background: Rectangle{
                    radius:height/2
                    color:"transparent"
                }

                icon{
                    source: "qrc:/Resources/index.png"
                    width: 26/Style.monitorRatio
                    height: 26/Style.monitorRatio
                }
                onClicked: {
                    hideRect.start()
                    heading.visible = false
                    root.heightVisiblity = false
                }
            }
            Rectangle{
                id:searchBox
                width: 190/Style.monitorRatio
                height: 28/Style.monitorRatio
                anchors.right: closeBtn.left
                anchors.verticalCenter: parent.verticalCenter
                anchors.rightMargin: 2
                radius: 10
                opacity: 0.85
                color: Qt.rgba(Style.foregroundColor.r, Style.foregroundColor.g, Style.foregroundColor.b, 0.2)

                Button {
                    id:searchicon
                    anchors.left:  parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.leftMargin: 2
                    background: Rectangle{
                        radius:height/2
                        color:"transparent"
                    }

                    icon{
                        source: "qrc:/Resources/search.png"
                        width: 24/Style.monitorRatio
                        height: 24/Style.monitorRatio
                    }

                }
                TextField {
                    id : searchText
                    width:140/Style.monitorRatio
                    anchors.left: searchicon.right
                    anchors.bottom: parent.bottom
                    placeholderText: implicitWidth? qsTr("Search ..." ) : ""
                    color: Style.foregroundColor
                    font.family: Style.fontFamily
                    font.pointSize: Style.fontPointSize
                    selectByMouse: true
                    selectedTextColor: Style.foregroundColor
                    selectionColor: Style.selectColor
                    placeholderTextColor: Style.disableColor
                    background: Rectangle{
                        color: "transparent"
                        radius: height/2
                    }
                    onTextChanged: {
                        root.model.setFilterString(text)
                    }
                }
            }
        }

        Rectangle{
            id : boldSepratorLine
            width:628/Style.monitorRatio
            height: 2/Style.monitorRatio
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: heading.bottom
            color: Style.foregroundColor

        }

        Item{
            id : topofDelegate
            width:628/Style.monitorRatio
            height: 45/Style.monitorRatio
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: boldSepratorLine.bottom
            ButtonGroup {
                id: childGroup
                exclusive: false
                checkState: subjectCheckBox.checkState
            }
            CheckBox {
                id: subjectCheckBox
                topPadding: 0
                rightPadding: 0
                leftPadding: 0
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                checkState: childGroup.checkState
                background: Rectangle{
                    height: 20/Style.monitorRatio
                    width:20/Style.monitorRatio
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    color:"transparent"
                }

                nextCheckState: function() {

                    if (checkState === Qt.Checked){
                        root.model.selectAllMessages(false)
                        return Qt.Unchecked
                    }else{
                        root.model.selectAllMessages(true)
                        return Qt.Checked
                    }
                }


                indicator: Rectangle {
                    implicitWidth: 20/Style.monitorRatio
                    implicitHeight: 20/Style.monitorRatio
                    x: subjectCheckBox.leftPadding
                    y: parent.height / 2 - height / 2
                    radius: 4
                    color: "transparent"
                    border.color: Style.disableColor
                    border.width: 2/Style.monitorRatio

                    Rectangle {
                        anchors.centerIn: parent
                        implicitWidth: 17/Style.monitorRatio
                        implicitHeight: 17/Style.monitorRatio
                        x: 4/Style.monitorRatio
                        y: 4/Style.monitorRatio
                        radius: 4
                        color: Style.foregroundColor
                        visible: subjectCheckBox.checked
                    }
                }

            }

            TextField {
                id : subject
                width:430/Style.monitorRatio
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: subjectCheckBox.right
                text:"Subject"
                color: Style.foregroundColor
                font.family:Style.fontFamily
                font.pixelSize:  16/ Style.monitorRatio
                selectedTextColor: Style.foregroundColor
                selectionColor: Style.selectColor
                placeholderTextColor: Style.disableColor
                readOnly: true
                selectByMouse: false

                background: Rectangle{
                    color: "transparent"
                    radius: height/2
                }
            }

            Text {
                id: dateTime
                width:76/Style.monitorRatio
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: subject.right
                anchors.leftMargin: 10/Style.monitorRatio
                text:"Date/Time"
                color: Style.foregroundColor
                font.family:Style.fontFamily
                font.pixelSize: root.massageFontSize
            }

            Image {
                width: 19/Style.monitorRatio; height: 19/Style.monitorRatio
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: dateTime.right
                anchors.leftMargin: 10 /Style.monitorRatio
                fillMode: Image.PreserveAspectFit
                source: "qrc:/Resources/down.png"
            }

            Rectangle{
                id : thinSepratorLine
                width:628/Style.monitorRatio
                height: 2/Style.monitorRatio
                anchors.bottom:  parent.bottom
                color: Qt.rgba(Style.foregroundColor.r,Style.foregroundColor.g,Style.foregroundColor.b,0.2)

            }
            ScrollView{
                anchors.top: thinSepratorLine.bottom
                anchors.left: parent.left
                height: messageContainer.height - heading.height - topofDelegate.height
                width: parent.width



                ListView{
                    id:listView
                    anchors.fill: parent
                    property list<int> selectedIndexes: []
                    visible: true
                    clip: true
                    model: root.model
                    delegate:
                        Item {
                        Rectangle{
                            radius: height/2
                            height: 8
                            width: 8
                            color: "green"
                            x:30/Style.monitorRatio + delegateSubject.implicitWidth
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.rightMargin: 10
                            visible: model.isnewMessage
                        }
                        width: listView.width
                        height: 45/Style.monitorRatio
                        MouseArea{
                            anchors.fill: parent
                            hoverEnabled: true
                            onEntered: root.model.isNewMessage(root.model.index(index, 0))
                        }
                        CheckBox {
                            id:delegateCheckBox
                            topPadding: 0
                            rightPadding: 0
                            leftPadding: 0
                            anchors.left: parent.left
                            anchors.verticalCenter: parent.verticalCenter
                            ButtonGroup.group: childGroup
                            checkState: model.textChecked ? Qt.Checked: Qt.Unchecked

                            background: Rectangle{
                                height: 20/Style.monitorRatio
                                width:20/Style.monitorRatio
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.left: parent.left
                                color:"transparent"
                            }

                            nextCheckState: function() {
                                if (checkState === Qt.Checked){
                                    root.model.toggleCheck(root.model.index(index, 0), false)
                                    return Qt.Unchecked
                                }
                                else{
                                    root.model.toggleCheck(root.model.index(index, 0), true)
                                    return Qt.Checked
                                }
                            }

                            indicator: Rectangle {
                                implicitWidth: 20/Style.monitorRatio
                                implicitHeight: 20/Style.monitorRatio
                                x: delegateCheckBox.leftPadding
                                y: parent.height / 2 - height / 2
                                radius: 4
                                color: "transparent"
                                border.color: Style.disableColor
                                border.width: 2/Style.monitorRatio

                                Rectangle {
                                    anchors.centerIn: parent
                                    implicitWidth: 17/Style.monitorRatio
                                    implicitHeight: 17/Style.monitorRatio
                                    x: 4/Style.monitorRatio
                                    y: 4/Style.monitorRatio
                                    radius: 4
                                    color: Style.foregroundColor
                                    visible: delegateCheckBox.checked
                                }
                            }
                        }
                        TextField {
                            id : delegateSubject
                            width:430/Style.monitorRatio
                            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: delegateCheckBox.right
                            text: messageText
                            color: model.isnewMessage?Style.foregroundColor:Qt.rgba(Style.foregroundColor.r, Style.foregroundColor.g, Style.foregroundColor.b, 0.75)
                            font.family:Style.fontFamily
                            font.pixelSize:  16/ Style.monitorRatio
                            selectedTextColor: Style.foregroundColor
                            selectionColor: Style.selectColor
                            placeholderTextColor: Style.disableColor
                            readOnly: true
                            selectByMouse: false

                            background: Rectangle{
                                color: "transparent"
                                radius: height/2
                            }
                        }

                        Text {
                            id: delegatedateTime
                            width:96/Style.monitorRatio
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: delegateSubject.right
                            anchors.leftMargin: 10/Style.monitorRatio
                            text: dateText
                            color: model.isnewMessage?Style.foregroundColor:Qt.rgba(Style.foregroundColor.r, Style.foregroundColor.g, Style.foregroundColor.b, 0.75)
                            font.family:Style.fontFamily
                            font.pixelSize: root.massageFontSize
                        }

                        Text {
                            id: delegatehourTime
                            width:62/Style.monitorRatio
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: delegatedateTime.right
                            text: timeText
                            color: model.isnewMessage?Style.foregroundColor:Qt.rgba(Style.foregroundColor.r, Style.foregroundColor.g, Style.foregroundColor.b, 0.75)
                            font.family:Style.fontFamily
                            font.pixelSize: root.massageFontSize
                            font.weight: Font.Light
                        }
                    }
                }
            }
        }
    }

    Rectangle{
        id:margint
        anchors.right:  parent.left
        width: 80/Style.monitorRatio
        height: 22/Style.monitorRatio
        color: Style.backgroundColor
    }
    PropertyAnimation {
        id: showRect
        target: root
        properties: "heightFactor"
        from: 0
        to: 1
        duration: 300
    }

    PropertyAnimation {
        id: hideRect
        target: root
        properties: "heightFactor"
        from: 1
        to: 0
        duration: 300
    }
}





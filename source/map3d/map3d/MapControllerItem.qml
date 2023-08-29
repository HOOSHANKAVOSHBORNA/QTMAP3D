import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects
import Crystal 1.0


MapController {
    id: map
    zoomInButtonPressed:      navigationWidget.zoomInButtonPressed
    zoomOutButtonPressed:     navigationWidget.zoomOutButtonPressed
//    upButtonPressed:          navigationWidget.upButtonPressed
//    downButtonPressed:        navigationWidget.downButtonPressed
//    leftButtonPressed:        navigationWidget.leftButtonPressed
//    rightButtonPressed:       navigationWidget.rightButtonPressed
//    rotateUpButtonPressed:    navigationWidget.rotateUpButtonPressed
//    rotateDownButtonPressed:  navigationWidget.rotateDownButtonPressed
//    rotateLeftButtonPressed:  navigationWidget.rotateLeftButtonPressed
//    rotateRightButtonPressed: navigationWidget.rotateRightButtonPressed
    anchors.fill: parent
    objectName: "MainMap"
    property real widgetsPositionFactor: 1.0
    property bool widgetsVisible: true
    readonly property int       _iconSize   : 24
    readonly property int       _margin     : 15
    readonly property int       _radius     : 10
    readonly property color     _colorRec   : "#404040"
    readonly property color     _colorHover : "#01AED6"
    readonly property color     _colorPresed : "#003569"
    readonly property color     _colorIcon  : "#FFFFFF"
    readonly property color     _colorButton: "#55FFFFFF"
    readonly property string    _fontFamily : "Srouce Sans Pro"
    readonly property int       _fontPointSize : 11
    readonly property color     itemColor: "#404040"
    readonly property real      widgetsMargins: 10
    clip: true
    DropArea {
        id: dragTarget

        property string colorKey

        width: 64
        height: 64
        keys: [ colorKey ]

//        Rectangle {
//            id: dropRectangle

//            anchors.fill: parent
//            color: dragMouseArea.drag.active ? (dragTarget.containsDrag ? "grey" : dragTarget.colorKey) : "transparent"
//        }
    }
    //    MouseArea {
    //        id: mouseArea

    //        width: 64
    //        height: 64
    //        anchors.centerIn: parent

    //        drag.target: tile

    //        onReleased: parent = tile.Drag.target !== null ? tile.Drag.target : rootItem

    //        Rectangle {
    //            id: tile

    //            width: 64
    //            height: 64
    //            anchors {
    //                verticalCenter: parent.verticalCenter
    //                horizontalCenter: parent.horizontalCenter
    //            }

    //            color: "#505050"

    //            Drag.keys: [ rootItem.colorKey ]
    //            Drag.active: mouseArea.drag.active
    //            Drag.hotSpot.x: 32
    //            Drag.hotSpot.y: 32
    //            states: State {
    //                when: mouseArea.drag.active
    //                AnchorChanges {
    //                    target: tile
    //                    anchors {
    //                        verticalCenter: undefined
    //                        horizontalCenter: undefined
    //                    }
    //                }
    //            }
    //        }
    //    }

    onClicked: function() {
        toggleWidgetsVisible();
    }

    function toggleWidgetsVisible() {
        if (map.widgetsVisible === true) {
            widgetsShowAnimation.stop();
            widgetsHideAnimation.start();
            map.widgetsVisible = false;
        } else {

            widgetsHideAnimation.stop();
            widgetsShowAnimation.start();
            map.widgetsVisible = true;
        }
    }

    PropertyAnimation {
        id: widgetsShowAnimation
        target: map
        property: "widgetsPositionFactor"
        from: map.widgetsPositionFactor
        to: 1.0
        duration: 200 * Math.abs(1.0 - map.widgetsPositionFactor)

        easing.type: Easing.OutQuint
    }
    PropertyAnimation {
        id: widgetsHideAnimation
        target: map
        property: "widgetsPositionFactor"
        from: map.widgetsPositionFactor
        to: 0.0
        duration: 200 * Math.abs(map.widgetsPositionFactor)

        easing.type: Easing.InQuint
    }
    //
    Label {
        id: fpsLabel
        text: map.fps.toLocaleString(Qt.locale(), 'f', 2)
        color: 'springgreen'
        style: Text.Outline;
        styleColor: "black"
        font.pointSize: 20
        font.weight: Font.Bold
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.topMargin: 20
        anchors.leftMargin: 20
    }
    SearchBar {
        anchors.top: parent.top
        anchors.topMargin: 20
        anchors.rightMargin: 20
        anchors.right: parent.right
        model: map.searchNodeProxyModel()
    }
    //        id:mapItem
    Rectangle {
        color: "transparent"
        id:compass
        anchors.left: parent.left
        anchors.leftMargin: widgetsMargins
        anchors.bottomMargin: widgetsMargins
        height: compassWidget.height+10
        width: compassWidget.width
        y: parent.height  - widgetsPositionFactor * (compassWidget.height + (widgetsMargins) + statusBar.height)
//        Drag.active: dragMouseArea.drag.active
        Drag.hotSpot.x: 32
        Drag.hotSpot.y: 32
//        states: State {
//            when: dragMouseArea.drag.active
//            AnchorChanges {
//                target: compass
//                anchors {
//                    verticalCenter: undefined
//                    horizontalCenter: undefined
//                }
//            }
//        }
//        MouseArea {
//            id: dragMouseArea

//            width: 64
//            height: 64
//            anchors.fill: dragImg

//            drag.target: compass

//            onReleased: parent = compass.Drag.target !== null ? compass.Drag.target : rootItem
//        }
//        Image {
//            id: dragImg
//            source: "qrc:/Resources/drag.png"
//            anchors.top: parent.top
//            anchors.left: parent.left
//            width: 14
//            height: 14
//        }

        Compass{
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            id:compassWidget
            headingAngle: map.headingAngle
            //            anchors.left: parent.left
            //            anchors.leftMargin: widgetsMargins
            //            anchors.bottomMargin: widgetsMargins
            //            y: parent.height  - widgetsPositionFactor * (height + (widgetsMargins) + statusBar.height)

            onCompassDoubleClicked: function() {
                map.setHeadingToNorth();
            }
        }
    }


    MapViewController{
        id : navigationWidget
        //anchors.right: parent.right
        anchors.rightMargin: widgetsMargins
        //            y:25 + parent.height  - (wnd.widgetsPositionFactor * (height + ((widgetsMargins)/2+3)+25))
        //y: parent.height  - widgetsPositionFactor * (height + (widgetsMargins/2+3) + statusBar.height)
        // slot button
        y: parent.height/2
        x:parent.width - widgetsPositionFactor * (width + widgetsMargins)
        onBtnHomeClicked: function() {
            map.home();
        }
        onBtnProjectionClicked: function() {
            map.changeMode();
        }
    }

    ///////////////////------------------------------------

//    Item {
//        id : navigationWidget
//        //anchors.right: parent.right
//        anchors.rightMargin: widgetsMargins
//        //            y:25 + parent.height  - (wnd.widgetsPositionFactor * (height + ((widgetsMargins)/2+3)+25))
//        //y: parent.height  - widgetsPositionFactor * (height + (widgetsMargins/2+3) + statusBar.height)
//        // slot button
//        y: parent.height/2
//        x:parent.width - widgetsPositionFactor * (width + widgetsMargins)
//        onBtnHomeClicked: function() {
//            map.home();
//        }
//        onBtnProjectionClicked: function() {
//            map.changeMode();
//        }
//        width: mainRowLayout.implicitWidth
//        height: mainRowLayout.implicitHeight

//        property string modeMap: "geocentric"
//        property real positionFactor: 1.0
//        property bool showRecMov: true
//        property real myDuration: 300.0

//        property bool zoomInButtonPressed: positive.pressed
//        property bool zoomOutButtonPressed: negative.pressed

//        property bool upButtonPressed:    moveControlCamera.upButtonPressed
//        property bool downButtonPressed:  moveControlCamera.downButtonPressed
//        property bool leftButtonPressed:  moveControlCamera.leftButtonPressed
//        property bool rightButtonPressed: moveControlCamera.rightButtonPressed

//        property bool rotateUpButtonPressed:   rotationControlCamera.upButtonPressed
//        property bool rotateDownButtonPressed: rotationControlCamera.downButtonPressed
//        property bool rotateLeftButtonPressed: rotationControlCamera.leftButtonPressed
//        property bool rotateRightButtonPressed:rotationControlCamera.rightButtonPressed


//        signal btnHomeClicked()
//        signal btnProjectionClicked()

//        RowLayout {
//            id: mainRowLayout
//            spacing: 5
//            Rectangle {
//                id: control
//                color: "transparent"
//                Layout.fillWidth: true
//                Layout.fillHeight: true
//                Layout.minimumWidth: 65
//                Layout.minimumHeight: 150
//                anchors.rightMargin: 5
//                clip: true
//                ControlCamera{
//                    id: rotationControlCamera
//                    width: 65
//                    height: 65
//                    anchors.top: parent.top
//                    x: positionFactor * 65
//                    anchors.topMargin: 17
//                    centerIcon: "qrc:/Resources/eye.png"
//                    //color: "transparent"
//                    //buttonIcon: "qrc:/Resources/rotate.png"
//                }

//                ControlCamera{
//                    id: moveControlCamera
//                    width: 65
//                    height: 65
//                    x: positionFactor * 65
//                    anchors.top: rotationControlCamera.bottom
//                    anchors.topMargin: 17
//                    //color: "transparent"
//                }
//            }


//            GridLayout {
//                id:grL
//                columns:  1
//                rows: 8
//                rowSpacing: 3
//                columnSpacing: 0

//                Layout.preferredWidth: implicitWidth
//                Layout.preferredHeight: implicitHeight

//                    Rectangle {
//                        id: zoomBtnContainer
//                        width: 32
//                        height: 76
//                        color: "white"
//                        radius: 20

//                        ColumnLayout{
//                            anchors.fill: parent

//                            Button {
//                                id: positive

//                                icon.source : "qrc:/Resources/add.png"
//                                icon.width : _iconSize-2
//                                icon.height : _iconSize-2
//                                icon.color : hovered ? (pressed ? _colorPresed: _colorHover) :
//                                                       (pressed ? _colorHover : _colorPresed);
//                                background:Rectangle {
//                                    color:"transparent"
//                                }
//                            }
//                            Rectangle{
//                                Layout.leftMargin: 5
//                                visible: true
//                                width: parent.width/1.4
//                                color:"black"
//                                opacity: 0.3
//                                height: 2
//                            }


//                            Button {
//                                id: negative
//                                text: qsTr("Button")
//                                display: AbstractButton.IconOnly
//                                icon.source : "qrc:/Resources/minus.png"
//                                icon.width : _iconSize
//                                icon.height : _iconSize
//                                icon.color : hovered ? (pressed ? _colorPresed: _colorHover) :
//                                                       (pressed ? _colorHover : _colorPresed);
//                                background:Rectangle {
//                                    color:"transparent"
//                                }
//                            }

//                        }
//                    }




//                    Rectangle {
//                        id: sliderMenuBtn
//                        width: 32
//                        height:32
//                        color: "white"
//                        radius:20


//                        Image {
//                            id: direction
//                            source: "./Resources/direction.png"
//                            sourceSize: ("24 x 24")
//                            anchors.centerIn: parent
//                            MouseArea{
//                                anchors.fill: parent
//                                hoverEnabled: true
//                                onEntered:
//                                    chevronRecolor.colorizationColor = _colorHover
//                                onExited:
//                                    chevronRecolor.colorizationColor = _colorPresed
//                            }

//                        }
//                        MultiEffect{
//                            id:chevronRecolor
//                            source: direction
//                            anchors.fill: direction
//                            colorization: 1
//                            colorizationColor: "#FFFFFF"
//                        }
//                        states: [
//                            State {
//                                name: "rotated"
//                                PropertyChanges { target: direction; rotation: 180; }
//                                PropertyChanges { target: chevronRecolor; rotation: 180; }
//                            },
//                            State {
//                                name: "default"
//                                PropertyChanges { target: direction; rotation: 0 }
//                                PropertyChanges { target: chevronRecolor; rotation: 0 }
//                            }

//                        ]
//                        MouseArea{
//                            anchors.fill: parent
//                            onClicked: function() {
//                                sliderMenuBtn.state = navigationWidget.showRecMov ? "rotated" : "default"
//                                if(navigationWidget.showRecMov){
//                                    showSlider.stop()
//                                    hideSlider.start()
//                                    navigationWidget.showRecMov = false
//                                } else {
//                                    hideSlider.stop()
//                                    showSlider.start()
//                                    navigationWidget.showRecMov = true
//                                }
//                            }

//                        }

//                        transitions: Transition {
//                            RotationAnimation { duration: navigationWidget.myDuration; direction: RotationAnimation.Counterclockwise }
//                        }



//                    }




//                    Button {
//                        id: home
//                        hoverEnabled: true
//                        display: AbstractButton.IconOnly
//                        icon.source : "qrc:/Resources/home.png"
//                        icon.width : _iconSize
//                        icon.height : _iconSize
//                        icon.color : hovered ? (pressed ? _colorPresed: _colorHover) :
//                                               (pressed ? _colorHover : _colorPresed);
//                        background: Rectangle{

//                            color:"white"
//                            radius:20

//                        }

//                        onClicked: btnHomeClicked()
//                    }



//                    Button {
//                        id: project
//                        display: AbstractButton.IconOnly
//                        icon.source : modeMap === "projection" ? "qrc:///Resources/threeD.png": "qrc:/Resources/twoD.png"
//                        icon.width : _iconSize
//                        icon.height : _iconSize
//                        icon.color : hovered ? (pressed ? _colorPresed: _colorHover) :
//                                               (pressed ? _colorHover : _colorPresed);
//                        background: Rectangle{

//                            color: "white"
//                            radius:20

//                        }
//                        smooth: true

//                        onClicked:{
//                            if (modeMap==="projection")
//                                modeMap = "geocentric"
//                            else
//                                modeMap = "projection"
//                            btnProjectionClicked()
//                        }

//                    }


//                    MultiEffect {
//                        source: zoomBtnContainer
//                        enabled: true

//                        anchors.fill: zoomBtnContainer
//                        shadowColor: "black"
//                        shadowEnabled: true
//                        shadowBlur: 1
//                        shadowHorizontalOffset: 3.5
//                        shadowVerticalOffset:2.5
//                        shadowOpacity:1
//                        paddingRect: Qt.rect(0,0,20,20)
//                        shadowScale: 0.98

//                    }

//                    MultiEffect {
//                        source: sliderMenuBtn
//                        enabled: true

//                        anchors.fill: sliderMenuBtn
//                        shadowColor: "black"
//                        shadowEnabled: true
//                        shadowBlur: 1
//                        shadowHorizontalOffset: 3.5
//                        shadowVerticalOffset:2.5
//                        shadowOpacity:1
//                        paddingRect: Qt.rect(0,0,20,20)
//                        shadowScale: 0.98

//                    }
//                    MultiEffect {
//                        source: home
//                        enabled: true

//                        anchors.fill: home
//                        shadowColor: "black"
//                        shadowEnabled: true
//                        shadowBlur: 1
//                        shadowHorizontalOffset: 3.5
//                        shadowVerticalOffset:2.5
//                        shadowOpacity:1
//                        paddingRect: Qt.rect(0,0,20,20)
//                        shadowScale: 0.98

//                    }
//                    MultiEffect {
//                        source: project
//                        enabled: true

//                        anchors.fill: project
//                        shadowColor: "black"
//                        shadowEnabled: true
//                        shadowBlur: 1
//                        shadowHorizontalOffset: 3.5
//                        shadowVerticalOffset:2.5
//                        shadowOpacity:1
//                        paddingRect: Qt.rect(0,0,20,20)
//                        shadowScale: 0.98


//                    }






//            }

//        }




//        PropertyAnimation {
//            id: showSlider
//            target: rootItem
//            property: "positionFactor"
//            from: rootItem.positionFactor
//            to: 1.0
//            duration: myDuration * Math.abs(1.0 - positionFactor)

//            easing.type: Easing.OutQuint
//        }
//        PropertyAnimation {
//            id: hideSlider
//            target: rootItem
//            property: "positionFactor"
//            from: rootItem.positionFactor
//            to: 0.0
//            duration: myDuration * Math.abs(positionFactor)

//            easing.type: Easing.InQuint
//        }

//    }



//    ////////////////////---------------------------

    StatusBar {
        id: statusBar
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 0
        anchors.rightMargin: 0
        width: parent.width
        height: childrenRect.height
        latitude: map.mapMouseGeoLocation.x
        longitude: map.mapMouseGeoLocation.y
        altitude: map.mapMouseGeoLocation.z
        coordinate1: map.mapMouseLocation.x
        coordinate2: map.mapMouseLocation.y
        coordinate3: map.mapMouseLocation.z
        message: "Ready"
        timer: -1

    }

    //    }
}


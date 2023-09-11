pragma Singleton
import QtQuick 2.0

QtObject {
    id: l
    readonly property int       padding:        20
    readonly property int       marginSize:     15
    readonly property int       _iconSize:      24
    readonly property int       containeSize:   40
    readonly property color     tiltleColor:    "white"
    readonly property int       titleFontSize:  24
//    readonly property string    fontFamily:     "Srouce Sans Pro"
//    readonly property int       fontPointSize: 11
    readonly property real      widgetsMargins: 10
    readonly property int       borderwidth:    1
    readonly property int       radius:         4

    readonly property color     selectionColor: "#8c98ae"
//    readonly property color     hoverColor:     "#0dc2df"
    readonly property color     textHoverColor: "#9b9ca0"
//    readonly property color     backgroundColor:"#24242b"
    readonly property color     textColor:      "white"
    readonly property color     borderColor:    "#c9c9c9"
    readonly property color     primaryColor: /*"#213f72"*/ "#253D5B"
    readonly property color     secondaryColor: "#363739"

//    readonly property color     selectionColor: "#86BBD8"
//    readonly property color     hoverColor:     "#0dc2df"
//    readonly property color     textHoverColor: "#F26419"
//    readonly property color     backgroundColor:"#24242b"
//    readonly property color     textColor:      "white"
//    readonly property color     borderColor:    "#F6AE2D"
//    readonly property color     primaryColor:   "#2F4858"
//    readonly property color     secondaryColor: "#33658A"


    readonly property color     _darkestGray:   "#252935"
    readonly property color     _darkGray:      "#384250"
    readonly property color     _mainGray:      "#6C737F"
    readonly property color     _darkestBlue:   "#164C63"
    readonly property color     _darkBlue:      "#0E7090"
    readonly property color     _mainBlue:      "#06AED4"
    readonly property color     _mainYellow:    "#F79009"
    readonly property color     _persianGreen:  "#15B79E"

    //--------  UI Desired Color Added   ---------//

    readonly property color     uiWhite:        "#DEE3E6"
    readonly property color     uiHover:        "#01AED6"
    readonly property color     uiBlue :        "#003569"


    //-------   UI Desired Size Added    ---------//
    readonly property double monitorRatio: 1.
    readonly property string fontFamily:"Roboto"
    readonly property double fontPointSize: 11/monitorRatio

    readonly property int containerSize      : 40/1.3
    readonly property int uiSecondContainerSize: 65
    readonly property int uiBtnIconSize        : 26 / 1.3

    //--them white--------------------------------------------------------------------
    readonly property color     backgroundColor: "#DEE3E6"
    readonly property color     foregroundColor: "#003569"
    readonly property color     disableColor: Qt.rgba(foregroundColor.r, foregroundColor.g, foregroundColor.b, 0.5)
    readonly property color     hoverColor:      "#01AED6"
    readonly property color     selectColor :    "#B6C0CA"
    //--them black--------------------------------------------------------------------
//    readonly property color     backgroundColor:        "#DEE3E6"
//    readonly property color     forgroundColor:        "#DEE3E6"
//    readonly property color     hoverColor:        "#01AED6"
//    readonly property color     selectColor :        "#003569"

}



//"#2F4858" -- "#33658A" -- "#86BBD8" -- "#F6AE2D" -- "#F26419"

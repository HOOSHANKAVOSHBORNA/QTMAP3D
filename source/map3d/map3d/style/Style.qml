﻿pragma Singleton
import QtQuick 2.0

QtObject {
    id: l
    readonly property int       padding:        20
    readonly property int       marginSize:     15
    readonly property int       _iconSize:      24
    readonly property color     tiltleColor:    "white"
    readonly property int       titleFontSize:  24
    readonly property string    fontFamily:     "Srouce Sans Pro"
    readonly property int       _fontPointSize: 11
    readonly property real      widgetsMargins: 10
    readonly property int       borderwidth:    1
    readonly property int       radius:         4

    readonly property color     selectionColor: "#8c98ae"
    readonly property color     hoverColor:     "#0dc2df"
    readonly property color     textHoverColor: "#9b9ca0"
    readonly property color     backgroundColor:"#24242b"
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
}



//"#2F4858" -- "#33658A" -- "#86BBD8" -- "#F6AE2D" -- "#F26419"
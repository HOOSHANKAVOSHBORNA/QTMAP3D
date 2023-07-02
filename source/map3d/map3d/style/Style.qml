pragma Singleton
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

    readonly property color     selectionColor: "#8c98ae"
    readonly property color     hoverColor:     "#0dc2df"
    readonly property color     backgroundColor: "#24242b"
    readonly property color     textColor:      "white"
    readonly property color     borderColor:    "#c9c9c9"
    readonly property int       borderwidth:    2
//    readonly property int       radiusSize:     ""
    readonly property int       radius:         4
    readonly property color     primaryColor: "#213f72"
    readonly property color     secondaryColor: "#363739"
}

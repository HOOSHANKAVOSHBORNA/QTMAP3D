pragma Singleton

import QtQuick 2.0

QtObject {
    id: l
    readonly property int marginSize: 15
    readonly property real widgetsMargins: 10
    readonly property int radius: 4


    readonly property color _persianGreen: "#15B79E"

    //--------  UI Desired Color Added   ---------//
    readonly property color uiWhite: "#DEE3E6"
    readonly property color uiHover: "#01AED6"
    readonly property color uiBlue: "#003569"

    //-------   UI Desired Size Added    ---------//
    readonly property double monitorRatio: 1920 / Screen.width
    readonly property string fontFamily:"Roboto"
    readonly property double fontPointSize: 11/monitorRatio

    //------    UI Font Size             --------//
    readonly property double titleFontSize: 20 / monitorRatio
    readonly property double regularFontSize: 17 / monitorRatio
    readonly property double smallFontSize: 15 / monitorRatio



    //--them white--------------------------------------------------------------------
    readonly property color backgroundColor: "#DEE3E6"
    readonly property color foregroundColor: "#003569"
    readonly property color disableColor: Qt.rgba(foregroundColor.r,
                                                  foregroundColor.g,
                                                  foregroundColor.b, 0.5)
    readonly property color hoverColor: "#01AED6"
    readonly property color selectColor: "#B6C0CA"

    readonly property color topGradient: "#90DEE3E6"
    readonly property color bottomGradient: "#20DEE3E6"
    //--them black--------------------------------------------------------------------
    //    readonly property color     backgroundColor:        "#DEE3E6"
    //    readonly property color     forgroundColor:        "#DEE3E6"
    //    readonly property color     hoverColor:        "#01AED6"
    //    readonly property color     selectColor :        "#003569"
}
//"#2F4858" -- "#33658A" -- "#86BBD8" -- "#F6AE2D" -- "#F26419"


import QtQuick
import QtQuick.Controls
import 'style'

SplitView {

    id: splitView

    property bool resizeTrigger

    handle: Rectangle {
        id: handleDelegate
        implicitWidth: 3
        implicitHeight: 3
        color: SplitHandle.pressed ? Style._darkBlue : Style._darkestBlue

        SplitHandle.onPressedChanged: function() {
            if (handleDelegate.SplitHandle.pressed === false) {
                splitView.resizeTrigger = !splitView.resizeTrigger;
            }
        }

//        containmentMask: Item {
//            x: (handleDelegate.width - width) / 2
//            width: 12
//            height: splitView.height
//        }
    }
}

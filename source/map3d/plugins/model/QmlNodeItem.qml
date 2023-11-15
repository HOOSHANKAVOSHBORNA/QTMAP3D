import QtQuick
import Crystal
import QtQuick.Controls

QmlNode {
    property double farRange: 50000
    property double scaledNodeRadius: nodeRadius > 50000 ? 13 : 30
    width: 0
    height: 0
    property var cppModel : ListModel{}

    CircularMenu {
        visible: nodeRadius < 100000
        outerRadius: scaledNodeRadius + 30
        innerRadius: scaledNodeRadius
        listModel: cppModel
        onClickedIndex: (index) => {
                            console.log(index)
                        }
    }
}

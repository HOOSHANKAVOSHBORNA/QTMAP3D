import QtQuick
import Crystal
import QtQuick.Controls

QmlNode {
    property double farRange: 50000
    property double scaledNodeRadius: nodeRadius
//    property double scaledNodeRadius: 40

    width: 0
    height: 0
    property var cppModel : ListModel{}

    CircularMenu {
        id: circularMenu
        outerRadius: scaledNodeRadius + 25
        innerRadius: scaledNodeRadius
        listModel: cppModel
        onClicked: (index) => {
                        cppModel.onItemClicked(cppModel.index(index, 0))
                    }
        onToggled: (index, checked) =>{
                       cppModel.onItemChecked(cppModel.index(index, 0), checked)
                   }
    }
}

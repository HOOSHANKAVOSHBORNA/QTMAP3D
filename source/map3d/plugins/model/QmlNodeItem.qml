import QtQuick
import Crystal
import QtQuick.Controls

QmlNode {
    property double outerScale: 1.5
    property double maximumRadius: 60
    property double minimumRadius: 30
    property double nodeInnerRadius: nodeRadius < maximumRadius ? (nodeRadius < minimumRadius ? minimumRadius: nodeRadius) : maximumRadius
//    property double nodeInnerRadius: 40

    width: 0
    height: 0
    property var cppModel : ListModel{}

    CircularMenu {
        id: circularMenu
        outerRadius: nodeInnerRadius * outerScale
        innerRadius: nodeInnerRadius
        listModel: cppModel
        onClicked: (index) => {
                        cppModel.onItemClicked(cppModel.index(index, 0))
                    }
        onToggled: (index, checked) =>{
                       cppModel.onItemChecked(cppModel.index(index, 0), checked)
                   }
    }
}

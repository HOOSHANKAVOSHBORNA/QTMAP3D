import QtQuick
import Crystal
import QtQuick.Controls

QmlNode {
    property double farRange: 50000
    property double scaledNodeRadius: nodeRadius /*> 50000 ? 15 : 30*/
    width: 0
    height: 0
    property var cppModel : ListModel{}

    CircularMenu {
        id: circularMenu
        //        visible: nodeRadius < 80000
        outerRadius: scaledNodeRadius + 30
        innerRadius: scaledNodeRadius
        listModel: cppModel
        onClicked: (index) => {
                            //                            console.log(cppModel.index)
                        cppModel.onItemClicked(cppModel.index(index, 0))

                    }
        onToggled: (index, checked) =>{
                       cppModel.onItemChecked(cppModel.index(index, 0), checked)
                   }
    }
}

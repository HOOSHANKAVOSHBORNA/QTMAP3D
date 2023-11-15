import QtQuick
import Crystal

QmlNode {
    property double farRange: 50000
    property double scaledNodeRadius: nodeRadius > 50000 ? 10 : 30
    width: 0
    height: 0

    CircularMenu {
//        visible: nodeRadius < 50000
        outerRadius: scaledNodeRadius + 40
        innerRadius: scaledNodeRadius
        listModel: ListModel {
            ListElement {
                name: 'Info'
                color: 'green'
                iconSource: 'qrc:/Resources/info.png'
            }
            ListElement {
                name: 'Bookmark'
                color: 'red'
                iconSource: 'qrc:/Resources/bookmark.png'
            }
            ListElement {
                name: 'Attack'
                color: 'blue'
                iconSource: 'qrc:/Resources/radar.png'
            }
            ListElement {
                name: 'Target'
                color: 'purple'
                iconSource: 'qrc:/Resources/hand.png'
            }
        }
        onClickedIndex: (index) => {
                            console.log(index)
                        }
    }
}

import QtQuick
import Crystal

QmlNode {
    width: 0
    height: 0

    CircularMenu {
        outerRadius: 70
        innerRadius: 30
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

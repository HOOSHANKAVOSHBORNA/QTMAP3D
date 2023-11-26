import QtQuick 2.13
import QtQuick.Layouts 1.13
import QtQuick.Controls 2.13
import Crystal 1.0
import QtQuick.Window 2.13
//import QtGraphicalEffects 1.0
CListWindow {
    width: 1000
    height: 600
    color: 'lightblue'

    ColumnLayout {
        anchors.fill: parent

        TabBar {
            id: listTabbar
            Layout.fillWidth: true

            Repeater {
                id: rep
                model: tabbarModel

                TabButton {
                    required property var model
                    width: 50
                    text: model.title
                }

                onItemAdded: (index, modelItem) => {
                    stackLayout.data.push(modelItem.model.item)
                }
            }
        }

        StackLayout {
            id: stackLayout
            Layout.fillWidth: true
            Layout.fillHeight: true
            currentIndex: listTabbar.currentIndex
        }
    }



    // --------------------- just for test
    RowLayout {
        height: 50
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 10

        TextField {
            id: addNewTitle
            Layout.fillHeight: true
            Layout.fillWidth: true
            text: 'newTitle'
        }

        Button {
            Layout.fillHeight: true
            Layout.fillWidth: true
            text: 'add with null'

            onClicked: {
                appendTest(addNewTitle.text)
            }
        }
    }
    // just for test


}

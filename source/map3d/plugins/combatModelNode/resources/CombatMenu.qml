import QtQuick
import QtQuick.Layouts



Item {
    id:root
    anchors.fill: parent

    property var combatModel


    Rectangle{
        anchors.fill: parent
        radius: 10
        opacity: 0.7
        color: "blue"

        RowLayout{
            id:mainObject
            anchors.right: parent.right
            Image {
                id: mainObjectIcon
                source: combatModel.iconUrl
MouseArea{
anchors.fill: parent
onClicked: {
    console.log(combatModel.rowCount())
}
}
            }
            Text {
                id: mainObjectTitle
                text: combatModel.title
            }

        }
        Rectangle{
            anchors.right: mainObject.left
            width: parent.width - mainObject.width
            height: parent.height *0.8
            anchors.verticalCenter: parent.verticalCenter

            ListView{
                id:se
                clip: true
                model: root.combatModel
                anchors.fill: parent
                delegate: Item {
                    RowLayout{
                        anchors.fill: parent
                        anchors.right: parent.right
                        Image {
                            source: objectIcon
                            sourceSize: ("50x50")
                        }
                        Text {
                            text: objectID
                            font.pixelSize: 20
                        }
                    }
                }
            }
        }
    }
}




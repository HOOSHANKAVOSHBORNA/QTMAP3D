import QtQuick 2.13
import QtQuick.Layouts 1.13
import QtQuick.Controls 2.13
import Crystal 1.0

CListWindow {


    property var buttonsModel : ListModel {

    }
    visible: true
    width: 400
    height: 300
    title: "List Window"
    color: "#404040"
    Row {
        id: buttons
        height: 30
        width: parent.width
        spacing: 3
        Repeater {
            id: rep
            model: buttonsModel
            Button {
                anchors.topMargin: 10
                text: buttonText
                anchors.leftMargin: 0
                background: Rectangle {
                    radius: 4
                    color: "#808080"
                }

                height: 30
                width: buttons.width / 3
                onClicked: showTab(index)
            }
        }
    }

    StackLayout {
        id: stacklayout
        y: 50

    }

    function addTab(titleString, item){
        buttonsModel.append({"buttonText": titleString})
        stacklayout.data.push(item)
    }

    function showTab(indx){
        stacklayout.currentIndex = indx
        for (var i = 0; i < stacklayout.count; i++){
            rep.itemAt(i).background.color = "#808080"
        }

        rep.itemAt(indx).background.color = "#606060"

    }

}

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
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 0
        spacing: 0
        Row {
            id: buttons
            //height: 30
            Layout.minimumHeight: 30
            Layout.preferredHeight: 30
            Layout.fillWidth: true
            width: parent.width
            spacing: 3
            Repeater {
                id: rep
                model: buttonsModel
                Button {
                    text: buttonText
                    anchors.leftMargin: 0
                    background: Rectangle {
                        radius: 4
                        color: "#808080"
                    }

                    anchors.top: parent.top
                    anchors.bottom: parent.bottom

                    width: buttons.width / 3
                    onClicked: showTab(index)
                }
            }
        }

        StackLayout {
            id: stacklayout
            Layout.fillWidth: true
            Layout.fillHeight: true


        }
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

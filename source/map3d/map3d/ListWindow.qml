import QtQuick 2.13
import QtQuick.Layouts 1.13
import QtQuick.Controls 2.13
import Crystal 1.0

CListWindow {


    property var buttonsModel : ListModel {

    }
    property var selectColor: "#808080"
    property var unselectColor: "#606060"
    visible: true
    x: 800
    y: 600
    width: 800
    height: 600
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
                        color: selectColor
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
        showTab(stacklayout.count - 1)
    }

    function showTab(indx){
        stacklayout.currentIndex = indx
        for (var i = 0; i < stacklayout.count; i++){
            rep.itemAt(i).background.color = unselectColor
        }

        rep.itemAt(indx).background.color = selectColor

    }

}

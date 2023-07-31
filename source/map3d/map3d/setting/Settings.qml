import QtQuick 2.13
import Qt.labs.platform 1.1
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.13
Item {
    Rectangle {
        id: rectangle
        color: "#202020"
        anchors.fill: parent
        radius: 10
        opacity: 0.7
    }
    id: rootItem
    ScrollView {
        id: scroller
        anchors.rightMargin: 5
        anchors.leftMargin: 5
        anchors.bottomMargin: 5
        anchors.topMargin: 5
        anchors.fill: parent
        contentHeight: columnLayout.implicitHeight
        //ScrollBar.vertical.interactive: true
        ScrollBar.vertical.interactive : false
        clip : true
        Item {
            id: tmp
            anchors.fill: parent
        }
        Column {
            id :columnLayout
            anchors.fill: parent
            spacing: 2
            TabBar {
                id: bar
                width: parent.width
                background: Rectangle{
                    color: "transparent"
                }

                Repeater {
                    model: ["Monitor", "tab1","tab2", "tab3"]
                    TabButton {
                        id :btn
                        text: modelData
                        width: Math.max(90, bar.width / 3.1)
                        property int roundEdge: 0
                        contentItem: Text {
                            text: btn.text
                            font: btn.font
                            color: "#252525"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            elide: Text.ElideRight
                        }
                        background: Item {
                            Rectangle{
                                id:roundRec
                                color: btn.checked ? "yelow" : "#FFFFFF"
                                width: 100
                                height: 30
                                radius: roundEdge == 0 ? 0 :10
                            }
                            Rectangle{
                                id:leftRec
                                color: btn.checked ? "yelow" : "#FFFFFF"
                                visible: roundEdge == 2
                                width: roundRec.width - roundRec.radius
                                height: roundRec.height
                                anchors.bottom: roundRec.bottom
                                anchors.left: roundRec.left
                            }
                            Rectangle{
                                id:rightRec
                                color: btn.checked ? "yelow" : "#FFFFFF"
                                visible: roundEdge == 1
                                width: roundRec.width - roundRec.radius
                                height: roundRec.height
                                anchors.bottom: roundRec.bottom
                                anchors.right: roundRec.right
                            }
                        }
                    }
                }
                onCountChanged: function(){
                    for(let i=0 ;i<bar.count;i++){
                        if (i===0)
                            bar.itemAt(i).roundEdge=1
                        else if(i === bar.count -1)
                            bar.itemAt(i).roundEdge=2
                        else
                            bar.itemAt(i).roundEdge=0
                    }
                }
                Component.onCompleted: bar.removeItem(bar.itemAt(1))
            }

            StackLayout {
                width: parent.width
                currentIndex: bar.currentIndex
                DelegateItem{
                    id: monitor
                    width: 200
                    height: 300
                }

                Item {
                    id: discoverTab
                }
                Item {
                    id: activityTab
                }
            }
            Rectangle {
                id: rectangle1
                color: "#ffffff"
               height :2
               width :parent.width

            }

        }
    }

}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}D{i:14;anchors_width:630}
}
##^##*/

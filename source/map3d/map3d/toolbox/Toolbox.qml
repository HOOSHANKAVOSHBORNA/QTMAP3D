
import QtQuick 2.13
import Qt.labs.platform 1.1
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.13

Item {
    id : rootItem
    readonly property int margin: 5
    signal itemClicked(string itemname, string category_name);
    signal changeCheckable(string itemname, string category_name, bool check)

    property var listModel
        ScrollView {
            id: scroller
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
                Repeater {
                    id: repeater
                    model: rootItem.listModel
                    delegate: DelegateItem{
                        Layout.fillWidth: true
                        width: tmp.width
                        height: 30
                        onItemClicked: function(itemname, category_name) {
                            rootItem.itemClicked(itemname, category_name)
                        }
                        onChangeCheckable: {
                            rootItem.changeCheckable(itemname, category_name, check)
                        }
                    }
                }
            }
        }
}


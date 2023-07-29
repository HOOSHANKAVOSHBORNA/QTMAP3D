import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

ColumnLayout{

    id: rootItem
    property ListModel model:ListModel{}

    function addTabItem(item, name){

    }

    //--tab-----------------------------------------------

    clip: true
    TabBar {
        //        property int tabVisibleCount: 0
        id: tabBar
        //        Layout.fillWidth: true

        Repeater {
            id: repeater
            model: rootItem.model

            TabButton {
                id:tb
                width: visible ? implicitWidth + 10 :0
                text: name ?? "unknown"
                onDoubleClicked: {
                    var docItem = stackLayout.data[index]
                    docItem.state = "undocked"

                    tb.visible = Qt.binding(function() { return docItem.state === "docked" })

                    for(var i = 1; i<rootItem.model.count; i++){
                        var mindex = (i + index) % rootItem.model.count
                        if(repeater.itemAt(mindex).visible){
                            tabBar.currentIndex = mindex
                            break
                        }
                    }
                }
                onVisibleChanged: {
                    tabBar.currentIndex = visible ? index: tabBar.currentIndex
                }
            }
            onItemAdded: (index, it)=>{
                             var item = rootItem.model.get(index).item
                             var name = rootItem.model.get(index).name
                             var docItemCom = Qt.createComponent("DockWindow.qml");
                             if (docItemCom.status == Component.Ready) {
                                 var docItem = docItemCom.createObject(tabBar, {});
                                 docItem.containerItem.push(item)
                                 docItem.name = name
                                 stackLayout.data.push(docItem)
                             }
                             else
                                 print("error load docItem")
                         }
            onItemRemoved: (index, item)=>{
                               var docItem = stackLayout.data[index]
                               docItem.destroy()
                           }
        }
    }


    StackLayout {
        id: stackLayout
        Layout.fillHeight: true
        //        Layout.fillWidth: true
        Layout.preferredWidth: childrenRect.width > 0 ? 200 : childrenRect.width
        currentIndex: tabBar.currentIndex
    }
    //------------------------------------------
}

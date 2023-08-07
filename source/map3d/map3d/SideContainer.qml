import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

ColumnLayout{

    id: rootItem
    property ListModel model:ListModel{}
    property int visibleCount : 0
//    Layout.preferredWidth: implicitWidth
    function setCurrentIndex(index){
        tabBar.currentIndex = index
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

            function changeVisibleCount(st) {
                rootItem.visibleCount += st === "docked" ? 1 : -1
            }
            onItemAdded: (index, it)=>{
                             var item = rootItem.model.get(index).item
                             var name = rootItem.model.get(index).name
                             var docItemCom = Qt.createComponent("DockWindow.qml");
                             if (docItemCom.status === Component.Ready) {
                                 var docItem = docItemCom.createObject(tabBar, {});
                                 docItem.containerItem.push(item)
                                 docItem.name = name
                                 stackLayout.data.push(docItem)
                                 tabBar.currentIndex = index
                                 repeater.itemAt(index).visible = Qt.binding(function() { return docItem.state === "docked" })
                                 rootItem.visibleCount += 1

                                 docItem.stateChanged.connect(changeVisibleCount)
                             }
                             else
                             print("error load docItem")
                         }
            onItemRemoved: (index, item)=>{
                               var docItem = stackLayout.data[index]
                               rootItem.visibleCount -= 1
                               docItem.destroy()
                           }
        }
    }


    StackLayout {
        id: stackLayout
        Layout.fillHeight: true
        //        Layout.fillWidth: true
        Layout.preferredWidth: childrenRect.width
        currentIndex: tabBar.currentIndex
    }
    //------------------------------------------
}

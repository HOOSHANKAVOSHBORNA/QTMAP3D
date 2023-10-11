import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import "style"

ColumnLayout{

    id: rootItem
    property ListModel model:ListModel{}
    property int visibleCount : 0
    function setCurrentIndex(index){
        tabBar.currentIndex = index
    }

    //--tab-----------------------------------------------

    clip: true
    TabBar {
        id: tabBar
contentWidth: rootItem.model.count ?parent.width - 40 /Style.monitorRatio: 0
Layout.leftMargin: rootItem.model.count ? 18 / Style.monitorRatio : 0

                clip: true
        Repeater {
            id: repeater
            model: rootItem.model

            TabButton {
                id:tb

                width:{
                    if (rootItem.model.count === 1){

                        return implicitWidth
                    }else{
                        if(tabBar.currentIndex === index){
                            return implicitWidth
                        }
                        else return (tabBar.width - implicitWidth*2) / rootItem.model.count
                    }
                    }





//                    if (rootItem.model.count){
//                        if(tabBar.currentIndex === index){
//                            return txt.implicitWidth
//                        }
////                        else return (parent.width - tabBar.implicitWidth) / (rootItem.model.count - 1)
//                    }
//                    else return 0
//                }




//                    (tabBar.currentIndex === index) ? implicitWidth + 10 :50
                contentItem: Text {
                    id:txt
                    text: name ?? "unknown"
                    font: Style.fontFamily
                    opacity: enabled ? 1.0 : 0.3
                    color: tabBar.currentIndex === index ? Style.foregroundColor : Style.disableColor
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }

                background:Rectangle{
                        color:"transparent"
                    }


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
                                 docItem.width = stackLayout.count > 0 ? stackLayout.childrenRect.width : 300
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
        currentIndex: tabBar.currentIndex
    }
    //------------------------------------------
}

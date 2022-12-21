
import QtQuick 2.0

Item {
    id: root
    property var nowItem: null
    height: parent.height
    function showItem(item) {
        if (nowItem)
            hideItem()
        item.parent = root
        nowItem = item

    }

    function updatePosition(x, y) {
        root.x = x
        root.y = y
    }

    function hideItem(){
        nowItem.parent = null
        nowItem.destroy()
    }
}

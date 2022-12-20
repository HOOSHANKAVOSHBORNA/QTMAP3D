
import QtQuick 2.0

Item {
    id: root
    property var nowItem: null

    function showItem(item) {
        if (nowItem)
            removeItem()
        item.parent = root.parent
        nowItem = item
    }

    function updatePosition(x, y) {
        nowItem.x = x
        nowItem.y = y
    }

    function hideItem(){
        nowItem.parent = null
        nowItem.destroy()
    }
}

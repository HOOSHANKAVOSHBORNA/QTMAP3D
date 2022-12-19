
import QtQuick 2.0

Item {
    id: root
    function showItem(item, previousContextmenuItem) {
        if (previousContextmenuItem)
            removeItem(previousContextmenuItem)
        item.parent = root.parent
    }

    function updatePosition(item, x, y) {
        item.x = x
        item.y = y
    }

    function hideItem(item){
        item.parent = null
        item.destroy()
    }
}

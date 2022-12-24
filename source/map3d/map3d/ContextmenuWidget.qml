
import QtQuick 2.0

Item {
    id: root
    property var nowItem: null
    property var _height: 110
    clip: true
    width: 200
    height: _height
    function showItem(item) {
        if (nowItem)
            hideItem()
        item.parent = root
        nowItem = item

    }

    function updatePosition(x, y) {
        if (x + 200 > root.parent.width) {
            x = x - 200
        }
        if (y + _height > root.parent.height)
            y = y - _height
        root.x = x
        root.y = y
    }

    function hideItem(){
        nowItem.parent = null
        nowItem.destroy()
    }
}

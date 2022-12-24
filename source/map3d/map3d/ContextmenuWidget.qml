
import QtQuick 2.0

Item {
    id: root
    property var nowItem: null
    property var _height: 200
    property var _width: 200
    clip: true
    width: _width
    height: _height
    function showItem(item) {
        if (nowItem)
            hideItem()
        item.parent = root
        nowItem = item

    }

    function updatePosition(x, y) {
        if (nowItem)
            _height = nowItem.model.rowCount() * 25 > 200 ? 200 : nowItem.model.rowCount()*25
        else
            _height = 200

        if (x + _width > root.parent.width) {
            x = x - _width
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

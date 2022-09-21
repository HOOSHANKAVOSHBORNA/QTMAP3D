import QtQuick 2.0
import QtQuick.Layouts 1.12

Item {
    Connections {
        target: NamePlugin
        onItemAdded: {
            var component = Qt.createComponent("ItemRec.qml");

                if (NamePlugin.getItemCategory() === "Analyze"){
                    var object = component.createObject(column);
                    object.name =String(NamePlugin.getItemName());
                    object.icon = String(NamePlugin.getItemIcon())
                    object.category = "Analyze"


                }
        }
    }

    property string title: "teste"
    height:column.implicitHeight +6
    width: column.implicitWidth

    ColumnLayout{
        id:column
        anchors.top: parent.top
        anchors.topMargin: 3
        width: laout_back.width



    }
}

import QtQuick 2.0
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.13
import QtQuick.Layouts 1.12
Item {
    id:rootItem
    signal goToLocation(real latitude , real longitude, real range)

    property var listSaveLocation: ListModel {

    }
    function addListSaveLocation( _namelocation,  _longitude, _latitude){
        const listSaveLocationCount = listSaveLocation.count;
        var category_found = false;
        var category_index = -1;
        for (var i = 0; i < listSaveLocationCount; i++) {
            if (listSaveLocation.get(i).Name === _namelocation) {
                category_found = true;
                category_index = i;
                break;
            }
        }
        if (category_found !== true) {
            listSaveLocation.append({"Name"       : _namelocation,
                                     "Longitude" : _longitude,
                                     "Latitude"   : _latitude})
        }
    }

    GoPointWidget{
        id:gPointWidget
        anchors.left: parent.left
        anchors.leftMargin: 0

        onGoToLocation: function(lat, lon, range){
            rootItem.goToLocation(lat, lon, range);
        }
    }
    ListPointWidget{

        width: parent.width / 2
        height: gPointWidget.height + sPointWidget.height  - 50
        anchors.left: sPointWidget.right
        anchors.leftMargin: 5
        anchors.top: parent.top
        anchors.topMargin: 30
        onLocationClicked: {
            console.log(name,longitude,latitude)
        }
    }

    SavePointWidget{
        id:sPointWidget
        anchors.top: gPointWidget.bottom
        anchors.topMargin: 5

        onSavePointClicked: {
            addListSaveLocation(name,longitude,latitude);
        }
    }

}


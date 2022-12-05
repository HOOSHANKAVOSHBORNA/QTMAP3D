import QtQuick 2.0
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.13
import QtQuick.Layouts 1.12
Item {
    id:rootItem
    signal goToLocation(real latitude , real longitude, real range)
    signal goToView(string name, real longitude, real latitude,
                    real range, real pitch);
    property real longitude
    property real latitude
    property real pitch
    property real range
    property real head
    property var listSaveLocation: ListModel {

    }
    function addListSaveLocation( _namelocation, _longitude, _latitude,
                                 _range, _pitch, _head){
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
            listSaveLocation.append({"_Name"       : _namelocation,
                                     "_Longitude"  : _longitude,
                                     "_Latitude"   : _latitude,
                                     "_Range"      : _range,
                                     "_Pitch"      : _pitch,
                                     "_Head"       : _head
                                        })
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
        onGoToView:function(name, lon, lat, range,pitch, head) {
            console.log(name, lon, lat, range, pitch, head);
            rootItem.goToView(name, lon, lat, range, pitch, head);
        }
    }

    SavePointWidget{
        id:sPointWidget
        anchors.top: gPointWidget.bottom
        anchors.topMargin: 5

        onSaveLocation:function(name, lon, lat, range, pitch, head) {
            addListSaveLocation(name, lon, lat, range, pitch, head);
        }

        longitude: rootItem.longitude
        latitude: rootItem.latitude
        pitch: rootItem.pitch
        range: rootItem.range
        head : rootItem.head


    }

}


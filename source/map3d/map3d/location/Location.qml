import QtQuick 2.0
import QtQuick.Controls 2.12
//import QtGraphicalEffects 1.13
import QtQuick.Layouts 1.12
Item {
    id:rootItem
    signal goToLocation(real latitude , real longitude, real range)
    signal goToView(real latitude, real longitude,
                    real range, real pitch, real heading);
    property real longitude
    property real latitude
    property real pitch
    property real range
    property real head
    property var listSaveLocation: ListModel {

    }
    function addListSaveLocation( _namelocation, _latitude, _longitude,
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
                                     "_Latitude"   : _latitude,
                                     "_Longitude"  : _longitude,
                                     "_Range"      : _range,
                                     "_Pitch"      : _pitch,
                                     "_Head"       : _head
                                        })
        }
    }




//    Item {
//            anchors.left: listpoint.right
//            id: sidePush
//            visible: true


//                Image {
//                    id: minimize
//                    source: "/Resources/sideTab.png"
//                    sourceSize: ("25 x 100")
//                    y:8
//                    MouseArea{
//                        anchors.fill: parent
////                        onClicked: if(root.x !== -620){
////                                       minix.running = true
////                                       mintxt.running = true
////                                       minarrow.running = true
////                                   } else{
////                                       maxix.running = true
////                                       maxtxt.running = true
////                                       maxarrow.running = true
////                                   }
//                    }
//                    Text {
//                        id: sideInfoTxt
//                        anchors.centerIn: parent
//                        text: "root.titleText"
//                        color:"white"
//                        font.pointSize: 8
//                        visible: false
//                        rotation: -90
//                    }

//                    Image {
//                        id: arrow
//                        source: "/Resources/sidearrow.png"
//                        sourceSize: ("25 x 25")
//                        anchors.centerIn: parent

//                    }
//                }
////            PropertyAnimation {id:maxmove ; target: minimize ; property: "x" ; from :-300 ; to :0; duration: 200 ; easing.type: Easing.OutQuint ;running: false}
////            PropertyAnimation {id:minix ;target:root ; property:"x";  to: -620 ; duration: 200 ; running: false}
////            PropertyAnimation {id:maxix ;target:root ; property:"x";  to: -300 ; duration: 200 ; running: false}
////            PropertyAnimation {id:mintxt; target: sideInfoTxt ; property: "visible" ;  to: true  ; duration: 200 ; running: false}
////            PropertyAnimation {id:maxtxt; target: sideInfoTxt ; property: "visible" ;  to: false ; duration: 200 ; running: false}
////            PropertyAnimation {id:minarrow; target: arrow ; property: "opacity" ; to:0 ; duration: 200 ; running: false}
////            PropertyAnimation {id:maxarrow; target: arrow ; property: "opacity" ; to:1 ; duration: 200 ; running: false}

//        }



    GoPointWidget{
        id:gPointWidget
        anchors.left: parent.left
        anchors.leftMargin: 0

        onGoToLocation: function(lat, lon, range){
            rootItem.goToLocation(lat, lon, range);
        }
    }
    ListPointWidget{
        id: listpoint
        width: parent.width / 2
        height: gPointWidget.height + sPointWidget.height  - 50
        anchors.left: sPointWidget.right
        anchors.leftMargin: 5
        anchors.top: parent.top
        anchors.topMargin: 30

        onGoToView:function(name, lat,  lon, range,pitch, head) {
            rootItem.goToView(lat, lon, range, pitch, head);
        }
    }

    SavePointWidget{
        id:sPointWidget
        anchors.top: gPointWidget.bottom
        anchors.topMargin: 5

        onSaveLocation:function(name, lat, lon, range, pitch, head) {
            addListSaveLocation(name, lat, lon, range, pitch, head);
        }

        longitude: rootItem.longitude
        latitude: rootItem.latitude
        pitch: rootItem.pitch
        range: rootItem.range
        head : rootItem.head


    }

}


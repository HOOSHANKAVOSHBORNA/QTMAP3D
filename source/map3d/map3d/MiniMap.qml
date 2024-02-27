import QtQuick
// import QtQuick.Effects
import QtQuick3D
// import Qt3D.Core
// import Qt3D.Extras
// import Qt3D.Render


Item {
    width: 110
    height: 110

    property alias rotate: miniMap3D.eulerRotation.y
    property alias recSize: rec.width

    smooth: true


    View3D {
        smooth: true
        antialiasing: true
        renderMode: View3D.Inline
        anchors.fill: parent
        importScene: Node {
            id: node3D
            DirectionalLight {
                brightness: 1.0
                eulerRotation.x: cam.eulerRotation.x -10
            }
            rotation: Quaternion.fromAxisAndAngle(Qt.vector3d(1, 0, 0), mapItem.mapRotation.y)
            Model {
                opacity: 0.7
                id: miniMap3D
                source: "#Sphere"
                materials: [
                    PrincipledMaterial{
                        baseColorMap: Texture{
                            source: "qrc:/Resources/world_simple.png"
                            scaleU: 1.0
                            scaleV: 1.0
                        }
                    }
                ]
            }

        }
        camera: OrthographicCamera {
            id: cam
            z: 150
        }
        Rectangle{
            id:rec
            width: 5
            height: width
            color: "transparent"
            border.color: "white"
            border.width: 2
            radius: width /8
            anchors.centerIn: parent
            z:5

        }
    }
}

import QtQuick 2.12
import QtQuick.Effects
import QtQuick3D

Rectangle {
    width: 50
    height: 50

    property real headingAngle

    signal compassDoubleClicked();

    color: "#404040"
    smooth: true
    radius: 100
    opacity: 0.8

    View3D {
        anchors.fill: parent
        importScene: DirectionalLight {
            ambientColor: Qt.rgba(0.5, 0.5, 0.5, 1.0)
            brightness: 1.0
            eulerRotation.x: -25
        }

        Node {
            scale: Qt.vector3d(20, 20, 20)

            eulerRotation.x: -90
            eulerRotation.y: 0
            eulerRotation.z: 0

            PropertyAnimation on eulerRotation.x {
                //            running: animationPlay
                loops: Animation.Infinite
                duration: 5000
                to: 0
                from: -360
            }

            PropertyAnimation on eulerRotation.y {
                //            running: animationPlay
                loops: Animation.Infinite
                duration: 5000
                to: 0
                from: -360
            }

            Model {
                id: plane
                source: "qrc:/Resources/compass-surface.mesh"
                materials: PrincipledMaterial {
                    baseColor: "red"
                    roughness: 1
                    indexOfRefraction: 1
                }
            }

            Model {
                id: plane_001
                source: "qrc:/Resources/compass-premeter.mesh"
                materials: PrincipledMaterial {
                    baseColor: "green"
                    roughness: 1
                    indexOfRefraction: 1
                }
            }
        }

        camera: PerspectiveCamera { z: 100 }
    }
}

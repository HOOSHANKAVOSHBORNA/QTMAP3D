import QtQuick 2.12
import QtQuick.Effects
import QtQuick3D

Rectangle {
    width: 50
    height: 50

    property alias headingAngle: compassObject.eulerRotation.z
    property alias pitchAngle: compassObject.eulerRotation.x
//    property alias offsetAngle: compassObject.eulerRotation.z


    signal compassDirectionChanged()
    signal compassClicked()

    color: "transparent"
    smooth: true
    radius: 100
    opacity: 0.8

    MouseArea {
        anchors.fill: parent

        onClicked: compassClicked()
    }

    View3D {
        anchors.fill: parent
        importScene: DirectionalLight {
            ambientColor: Qt.rgba(0.5, 0.5, 0.5, 1.0)
            brightness: 1.0
            eulerRotation.x: -25
        }

//        Node {
//            id: compassObject
//            scale: Qt.vector3d(20, 20, 20)

//            eulerRotation.x: -90
//            eulerRotation.y: 0
//            eulerRotation.z: 0

//            onEulerRotationChanged: compassDirectionChanged()

////            PropertyAnimation on eulerRotation.x {
////                running: animationPlay
////                loops: Animation.Infinite
////                duration: 5000
////                to: 0
////                from: -360
////            }

////            PropertyAnimation on eulerRotation.y {
////                running: animationPlay
////                loops: Animation.Infinite
////                duration: 5000
////                to: 0
////                from: -360
////            }

//            Model {
//                id: plane
//                source: "qrc:/Resources/compass-surface.mesh"
//                materials: PrincipledMaterial {
//                    baseColor: "#dee3e6"
//                    roughness: 1
//                    indexOfRefraction: 1
//                }
//            }

//            Model {
//                id: plane_001
//                source: "qrc:/Resources/compass-premeter.mesh"
//                materials: PrincipledMaterial {
//                    baseColor: "#e6c0ca"
//                    roughness: 1
//                    indexOfRefraction: 1
//                }
//            }
//        }
        Node {
            id: compassObject

            // Resources
            PrincipledMaterial {
                id: material_001_material
                baseColor: "#ffeeeeee"
                alphaMode: PrincipledMaterial.Mask
                indexOfRefraction: 5
            }

            // Nodes:
            Node {
                id: scene
//                rotation: Qt.quaternion(0.707107, -0.707107, 0, 0)
                Model {
                    id: plane
                    scale: Qt.vector3d(1.5, 1.3, 1.5)
                    source: "qrc:/Resources/plane.mesh"
                    materials: [
//                        material_001_material,
                        material_001_material
                    ]
                }
            }

            // Animations:
        }

        camera: PerspectiveCamera { z: 100 }
    }
}

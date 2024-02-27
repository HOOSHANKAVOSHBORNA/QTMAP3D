import QtQuick 2.12
import QtQuick.Effects
import QtQuick3D
import "style"

Item {
    width: 50
    height: 50

    property alias headingAngle: compassObject.eulerRotation.z
    property alias pitchAngle: compassObject.eulerRotation.x
    property color color: Style.backgroundColor

    smooth: true
    View3D {
        renderMode: View3D.Inline
        anchors.fill: parent
        importScene: Node {
            DirectionalLight {
                ambientColor: Qt.rgba(0.5, 0.5, 0.5, 1.0)
                brightness: 1.0
                eulerRotation.x: -25
            }

            Node {
                id: compassObject

                // Resources
                PrincipledMaterial {
                    id: material
                    baseColor: color
                    alphaMode: PrincipledMaterial.Mask
                    indexOfRefraction: 5
                }

                Node {
                    id: scene
                    Model {
                        id: plane
                        scale: Qt.vector3d(1.5, 1.3, 1.5)
                        source: "qrc:/Resources/plane.mesh"
                        materials: [
                            material
                        ]
                    }

                }

            }
        }
        camera: PerspectiveCamera { z: 100 }
    }
}

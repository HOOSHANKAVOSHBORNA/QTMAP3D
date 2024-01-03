import QtQuick
import Crystal

CSplash {
    id: splash
    // flags: Qt.SplashScreen
    // // the transparent color lets background behind the image edges show through
    // color: "transparent"
    // modality: Qt.ApplicationModal // in case another application window is showing
    // title: "Splash Window" // for the taskbar/dock, task switcher etc.
    // visible: true

    // // here we use the Screen attached property to center the splash window
    x: (Screen.width - splashImage.width) / 2
    y: (Screen.height - splashImage.height) / 2
    width: splashImage.width
    height: splashImage.height

    property int timeoutInterval: 4000
    // // signal timeout

    AnimatedImage { id: splashImage; source: "qrc:/Resources/splash.gif"; speed: 0.4    }

    // Rectangle {
    //     property int frames: splashImage.frameCount

    //     width: 4; height: 8
    //     x: (splashImage.width - width) * splashImage.currentFrame / frames
    //     y: splashImage.height
    //     color: "red"
    // }

    // TapHandler {
    //     onTapped: splash.timeout()
    // }

    Timer {
        interval: splash.timeoutInterval; running: true; repeat: false
        onTriggered: {
            splash.visible = false
            splash.timeout()
        }
    }
}

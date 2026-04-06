import QtQuick
import QtQuick.Window

Window {
    width: 32 * 24
    height: 32 * 24
    visible: true
    title: "PathFinder"

    Grid {
        columns: 32

        Repeater {
            model: gameController.cells

            Rectangle {
                width: 24
                height: 24
                border.color: "gray"
                border.width: 1
                color: {
                    if (modelData === 0) return "white"   // ground
                    if (modelData === 1) return "black"   // elevated
                    if (modelData === 2) return "blue"    // path
                    if (modelData === 3) return "green"   // start
                    if (modelData === 4) return "red"     // target
                    return "white"
                }
            }
        }
    }
}
import QtQuick
import QtQuick.Window
import QtQuick.Layouts
import QtQuick.Dialogs
import QtQuick.Controls

Window {
    visible: true
    title: "PathFinder"
    width: Math.max(1024, gameController.gridColumns * 24 + 200)
    height: Math.max(768, gameController.gridRows * 24)
    property real cellSize: gameController.gridColumns > 0
        ? Math.min(width / gameController.gridColumns, height / gameController.gridRows)
        : 24


    FileDialog {
        id: fileDialog
        nameFilters: ["JSON files (*.json)"]
        onAccepted: gameController.loadMap(fileDialog.selectedFile)
    }

    Timer {
        id: runTimer
        interval: 100
        repeat: true
        onTriggered: gameController.nextStep()
    }

    RowLayout {
        anchors.fill: parent
        spacing: 0

        // Main view
        Grid {
            columns: gameController.gridColumns
            Layout.preferredWidth: gameController.gridColumns * cellSize
            Layout.preferredHeight: gameController.gridRows * cellSize

            Repeater {
                model: gameController.cells

                Rectangle {
                    width: cellSize
                    height: cellSize
                    border.color: "gray"
                    border.width: 1
                    color: {
                        if (modelData === 0) return "white"
                        if (modelData === 1) return "black"
                        if (modelData === 2) return "blue"
                        if (modelData === 3) return "green"
                        if (modelData === 4) return "red"
                        if (modelData === 6) return "orange"
                        return "white"
                    }
                }
            }
        }

        // Right Bar
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#2b2b2b"  // dark sidebar

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 16
                spacing: 12

                Button {
                    text: "Load Map"
                    Layout.fillWidth: true
                    implicitHeight: 40
                    onClicked: fileDialog.open()
                }

                Button {
                    text: "Next Step"
                    Layout.fillWidth: true
                    implicitHeight: 40
                    onClicked: gameController.nextStep()
                }

                Button {
                    text: "Run"
                    Layout.fillWidth: true
                    implicitHeight: 40
                    onClicked: runTimer.start()
                }

                Button {
                    text: "Stop"
                    Layout.fillWidth: true
                    implicitHeight: 40
                    onClicked: runTimer.stop()
                }

            }
        }
    }
}

import QtQuick
import QtQuick.Window
import QtQuick.Layouts
import QtQuick.Dialogs
import QtQuick.Controls

Window {
    visible: true
    title: "PathFinder"

    // Initial window size — grows to fit the grid if it is larger than 1200x1200
    width: Math.max(1200, gameController.gridColumns * 32 + 200)
    height: Math.max(1200, gameController.gridRows * 32)
    property int sidebarWidth: 200

    // cellSize is computed dynamically so the grid always fits within the window minus the sidebar
    property real cellSize: gameController.gridColumns > 0
        ? Math.min((width - sidebarWidth) / gameController.gridColumns, height / gameController.gridRows)
        : 24

    property bool goingForward: true

    // FileDialog is opened by the Load Map button and passes the selected file path to the backend
    FileDialog {
        id: fileDialog
        nameFilters: ["JSON files (*.json)"]
        onAccepted: {
            runTimer.stop()
            gameController.loadMap(fileDialog.selectedFile)
        }
    }

    // Timer drives Run mode — fires nextStep() every 100ms until stopped
    Timer {
        id: runTimer
        interval: 100
        repeat: true
        onTriggered: goingForward ? gameController.nextStep() : gameController.previousStep()
    }

    RowLayout {
        anchors.fill: parent
        spacing: 0

        // Main view container — fills remaining space and centers the grid
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#1a1a1a"

            // Grid is centered within the container and sized exactly to the number of cells
            Grid {
                columns: gameController.gridColumns
                width: gameController.gridColumns * cellSize
                height: gameController.gridRows * cellSize
                anchors.centerIn: parent
                clip: true

                Repeater {
                    model: gameController.cells

                    Rectangle {
                        width: cellSize
                        height: cellSize
                        border.color: "gray"
                        border.width: 1
                        color: {
                            if (modelData === 0) return "white"
                            if (modelData === 1) return "#2d0a4e"
                            if (modelData === 2) return "#aaaaff"
                            return "white"
                        }

                        Image {
                            anchors.fill: parent
                            fillMode: Image.PreserveAspectFit
                            visible: modelData === 3
                            source: "qrc:/images/gojo_chibi.jpeg"
                        }

                        Image{
                            anchors.fill: parent
                            fillMode: Image.PreserveAspectCrop
                            visible: modelData === 4
                            source: "qrc:/images/sukuna_chibi.jpeg"
                        }
                    }
                }
            }

        }

        // Right Bar
        Rectangle {
            Layout.preferredWidth: sidebarWidth
            Layout.fillHeight: true
            color: "#2b2b2b"

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 16
                spacing: 12

                component StyledButton: Button {
                    Layout.fillWidth: true
                    implicitHeight: 40

                    contentItem: Text {
                        text: parent.text
                        color: "white"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }

                    background: Rectangle {
                        color: parent.pressed ? "#555555" : "#3c3c3c"
                        radius: 4
                    }
                }

                StyledButton {
                    text: "Load Map"
                    onClicked: { runTimer.stop(); fileDialog.open() }
                }

                StyledButton {
                    text: "Next Step"
                    onClicked: gameController.nextStep()
                }

                StyledButton {
                    text: "Previous Step"
                    onClicked: gameController.previousStep()
                }

                StyledButton {
                    text: "Run Forward"
                    onClicked: { goingForward = true; runTimer.start() }
                }

                StyledButton {
                    text: "Run Back"
                    onClicked: { goingForward = false; runTimer.start() }
                }

                StyledButton {
                    text: "Stop"
                    onClicked: runTimer.stop()
                }

                // Spacer
                Item { Layout.fillHeight: true }

                // Legend title
                Text {
                    text: "Legend"
                    color: "white"
                    font.bold: true
                    font.pixelSize: 13
                }

                // Legend entries
                component LegendRow: RowLayout {
                    property string label: ""
                    property color swatchColor: "white"
                    property string imageSource: ""
                    spacing: 8
                    Layout.fillWidth: true

                    Rectangle {
                        width: 24; height: 24
                        color: swatchColor
                        border.color: "gray"
                        border.width: 1

                        Image {
                            anchors.fill: parent
                            fillMode: Image.PreserveAspectFit
                            source: imageSource
                            visible: imageSource !== ""
                        }
                    }

                    Text {
                        text: label
                        color: "#cccccc"
                        font.pixelSize: 12
                        Layout.fillWidth: true
                        wrapMode: Text.WordWrap
                    }
                }

                LegendRow { label: "Open terrain"; swatchColor: "white" }
                LegendRow { label: "Elevated terrain"; swatchColor: "#2d0a4e" }
                LegendRow { label: "Gojo (unit)"; swatchColor: "white"; imageSource: "qrc:/images/gojo_chibi.jpeg" }
                LegendRow { label: "Sukuna (target)"; swatchColor: "white"; imageSource: "qrc:/images/sukuna_chibi.jpeg" }

                Item { implicitHeight: 8 }
            }
        }
    }
}

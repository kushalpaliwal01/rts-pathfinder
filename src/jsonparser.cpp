#include "jsonparser.h"
#include "iconset.h"
#include <QFile>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <cmath>
#include <QDebug>

JsonParser::JsonParser() {}

ParseResult JsonParser::parseJson(const std::string& filePath) {
    QFile file(QString::fromStdString(filePath));

    // If file failed to open return an empty grid
    if (!file.open(QIODevice::ReadOnly)){
        qWarning() << "Failed to open file: " << file.errorString();
        return {};
    }

    QByteArray data = file.readAll();
    if (data.isEmpty()){
        qWarning() << "File has no data";
        return {};
    }

    QJsonDocument document = QJsonDocument::fromJson(data);
    if (document.isNull()){
        qWarning() << "Json parsing failed";
        return {};
    }

    QJsonObject root = document.object();

    // Extracting the data field with the mapping of the tiles from the layers field of the json
    QJsonArray layers = root["layers"].toArray();
    if (layers.isEmpty()) {
        qWarning() << "Invalid JSON: no layers found";
        return {};
    }

    QJsonObject layersObj = layers[0].toObject();
    if (layersObj["name"] != "world" && layers.size() > 1){
        layersObj = layers[1].toObject();
    }

    QJsonArray gridData = layersObj["data"].toArray();
    if (gridData.isEmpty()) {
        qWarning () << "Invalid JSON: layer has no data";
        return {};
    }

    // Extracting the image and tile height and width from the tilesets field of the json to determine the grid size
    QJsonArray tileSets = root["tilesets"].toArray();
    if (tileSets.isEmpty()) {
        qWarning() << "Invalid JSON: no tilesets found";
        return {};
    }

    QJsonObject firstIndexVal = tileSets[0].toObject();

    QJsonObject canvasVals = root["canvas"].toObject();
    if (canvasVals.isEmpty()) {
        qWarning() << "Invalid JSON: no canvas found";
        return {};
    }

    int rows = canvasVals["height"].toInt() / firstIndexVal["tileheight"].toInt();
    int columns = canvasVals["width"].toInt() / firstIndexVal["tilewidth"].toInt();


    ParseResult result;
    std::vector<std::vector<int>> grid(rows, std::vector<int>(columns, 0));

    int dataIndex = 0;
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < columns; j++){
            int value = (int)std::floor(gridData[dataIndex].toDouble());
            grid[i][j] = value;
            if (value == IconSet::TARGET) {
                result.targetPositions.push_back({i, j});
            }
            else if (value == IconSet::START) {
                result.startPositions.push_back({i, j});
            }
            dataIndex++;
        }
    }

    result.grid = grid;

    return result;

}
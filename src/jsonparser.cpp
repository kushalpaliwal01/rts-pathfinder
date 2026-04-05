#include "jsonparser.h"
#include "woodlandtileset.h"
#include <QFile>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <cmath>

JsonParser::JsonParser() {}

ParseResult JsonParser::parseJson(std::string filePath) {
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
    QJsonObject layersObj = layers[0].toObject();
    QJsonArray gridData = layersObj["data"].toArray();

    // Extracting the image and tile height and width from the tilesets field of the json to determine the grid size
    QJsonArray tileSets = root["tilesets"].toArray();
    QJsonObject firstIndexVal = tileSets[0].toObject();

    QJsonObject canvasVals = root["canvas"].toObject();

    int rows = canvasVals["height"].toInt() / firstIndexVal["tileheight"].toInt();
    int columns = canvasVals["width"].toInt() / firstIndexVal["tilewidth"].toInt();
    std::string name = firstIndexVal["name"].toString().toStdString();

    std::unique_ptr<TileSet> tileSet = createTileSet(name);


    ParseResult result;
    std::vector<std::vector<int>> grid(rows, std::vector<int>(columns, 0));

    int dataIndex = 0;
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < columns; j++){
            int value = (int)std::floor(gridData[dataIndex].toDouble());
            grid[i][j] = value;
            if (value == tileSet->getTargetValue()) {
                result.targetPosition = {i, j};
            }
            else if (value == tileSet->getStartValue()) {
                result.startPosition = {i, j};
            }
            dataIndex++;
        }
    }

    result.grid = grid;

    return result;

}

std::unique_ptr<TileSet> JsonParser::createTileSet(const std::string& name) {
    if (name.find("woodland") != std::string::npos) {
        return std::make_unique<WoodLandTileSet>();
    }

    return std::make_unique<WoodLandTileSet>(); // default fallback for now
}
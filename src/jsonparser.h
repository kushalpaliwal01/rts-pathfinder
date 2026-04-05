#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <string>
#include <vector>
#include <memory>
#include "tileset.h"
#include "types.h"

class JsonParser
{

public:
    JsonParser();
    ParseResult parseJson(std::string filePath);

private:
    std::unique_ptr<TileSet> createTileSet(const std::string& name);

};

#endif // JSONPARSER_H

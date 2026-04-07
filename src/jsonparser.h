#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <string>
#include <vector>
#include <memory>
#include "types.h"

class JsonParser
{

public:
    JsonParser();

    /**
     * Parses the json file compatible with RiskyLab format and returns a ParseResult
     * which contains the list of starting and target positions and the grid
     * Returns an empty ParseResult if the file is invalid
    **/
    ParseResult parseJson(const std::string& filePath);
};

#endif // JSONPARSER_H

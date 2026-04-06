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
    ParseResult parseJson(std::string filePath);


};

#endif // JSONPARSER_H

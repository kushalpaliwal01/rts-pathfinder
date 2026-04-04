#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <QObject>

class JsonParser : public QObject
{
    Q_OBJECT
public:
    explicit JsonParser(QObject *parent = nullptr);
};

#endif // JSONPARSER_H

#ifndef PARSER_H
#define PARSER_H
#include <tag.h>
#include <category.h>
#include <note.h>


class Parser
{
public:
    Parser();
    QVector <Tag> parseTags(QString json);
    void parseCategories(QString json);
    QVector<Note *> parseUserNotes(QString json);
};

#endif // PARSER_H

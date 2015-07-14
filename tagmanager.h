#ifndef TAGMANAGER_H
#define TAGMANAGER_H
#include <tag.h>
#include <QVector>


class TagManager
{
    QVector <Tag> tagsAvailable;
    TagManager();
    static TagManager* instance;
public:
    static TagManager* getInst();
    QVector<Tag> getTags();
    void insertTag(Tag);
    void clear();

};

#endif // TAGMANAGER_H

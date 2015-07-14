#include "tagmanager.h"

TagManager* TagManager::instance = 0;

TagManager::TagManager()
{

}

TagManager *TagManager::getInst()
{
    if (instance == 0)
        instance = new TagManager();
    return instance;
}

QVector<Tag> TagManager::getTags()
{
    return tagsAvailable;
}

void TagManager::insertTag(Tag t)
{
    tagsAvailable.push_back(t);
}

void TagManager::clear()
{
    tagsAvailable.clear();
}


#include "tag.h"
#include <QDebug>


QString Tag::getId() const
{
    return id;
}

void Tag::setId(const QString &value)
{
    id = value;
}

QString Tag::getText() const
{
    return text;
}

void Tag::setText(const QString &value)
{
    text = value;
}

bool Tag::getOwns() const
{
    return owns;
}

void Tag::setOwns(bool value)
{
    owns = value;
}

Tag::Tag()
{
}


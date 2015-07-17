#include "note.h"

/*
 * commentary lolhsshshdsfdh
 */


QString Note::getTheme() const
{
    return theme;
}

void Note::setTheme(const QString &value)
{
    theme = value;
}

QVector<QString> Note::getLabels() const
{
    return Labels;
}

void Note::setLabels(const QVector<QString> &value)
{
    Labels = value;
}

QVector<QString> Note::getCategories() const
{
    return Categories;
}

void Note::setCategories(const QVector<QString> &value)
{
    Categories = value;
}

QString Note::getText() const
{
    return text;
}

void Note::setText(const QString &value)
{
    text = value;
}

QString Note::getId() const
{
    return id;
}

void Note::setId(const QString &value)
{
    id = value;
}

QString Note::getColor() const
{
    return color;
}

void Note::setColor(const QString &value)
{
    color = value;
}

QString Note::getOwner() const
{
    return owner;
}

void Note::setOwner(const QString &value)
{
    owner = value;
}
Note::Note()
{

}


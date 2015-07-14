#include "parser.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include <QDebug>
#include <tagmanager.h>

Parser::Parser()
{

}

QVector<Tag> Parser::parseTags(QString json)
{
    QVector <Tag> res;
    QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());
    QJsonObject ob = doc.object();
    QJsonArray tags = ob["Tags"].toArray();
    TagManager::getInst()->clear();
    foreach(QJsonValue tag, tags)
    {
        Tag t;
        QJsonObject a = tag.toObject();
        t.setId(a["id"].toString());
        t.setText(a["name"].toString());
        t.setOwns(a["own"].toBool());
        res.push_back(t);
        TagManager::getInst()->insertTag(t);
    }
    return res;
}

void Parser::parseCategories(QString json)
{
    QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());

    QJsonArray arr = doc.object().value("Categories").toArray();
    foreach (QJsonValue val, arr)
    {
        QJsonObject ob = val.toObject();
        if (ob["parent"].toString() == "-1")
        {
            Category* c = Category::getStart()->add(ob["id"].toString());
            c->setName(ob["name"].toString());
        }
        else
        {
            Category* c = Category::getStart()->find(ob["parent"].toString())->add(ob["id"].toString());
            c->setName(ob["name"].toString());
        }
    }
}

QVector<Note*> Parser::parseUserNotes(QString json)
{
    QVector <Note*> res;
    QJsonArray arr = QJsonDocument::fromJson(json.toUtf8()).object().value("notes").toArray();
    foreach(QJsonValue val, arr)
    {
        QJsonObject o = val.toObject();

        Note *n = new Note();

        n->setTheme(o["name"].toString());
        n->setText(o["text"].toString());
        n->setColor(o["color"].toString());
        n->setId(o["id"].toString());

        QJsonArray cats = o["categories"].toArray();
        QVector <QString> scats;
        foreach (QJsonValue val, cats)
        {
            scats.push_back(val.toObject().value("name").toString());
        }
        n->setCategories(scats);

        QVector <QString> labels;
        QJsonArray labs = o["labels"].toArray();
        foreach(QJsonValue val, labs)
        {
            labels.push_back(val.toObject().value("name").toString());
        }
        n->setLabels(labels);

        if (o.contains("owner"))
            n->setOwner(o["owner"].toObject().value("name").toString());
        else
            n->setOwner("");

        res.push_back(n);
    }
    return res;
}


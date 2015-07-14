#ifndef CATEGORY_H
#define CATEGORY_H
#include <QString>
#include <QVector>
#include <QCheckBox>


class Category
{
    Category* parent;
    QString id;
    QString name;
    QVector <Category*> child;
    static Category* root;
public:
    Category(Category* parent);
    ~Category();
    QString getName() const;
    void setName(const QString &value);
    Category* find(QString name);
    bool checkName(QString name);
    Category* getParent() const;
    Category* add(QString name);
    static Category* getStart();
    static void clear();
    QString getId() const;
    void setId(const QString &value);
    QVector <Category*> getChildren();
};

#endif // CATEGORY_H

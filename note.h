#ifndef NOTE_H
#define NOTE_H
#include <QString>
#include <QVector>


class Note
{
    QString id;
    QString color;
    QString theme;
    QVector <QString> Labels;
    QVector <QString> Categories;
    QString text;
    QString owner;

public:
    Note();
    QString getTheme() const;
    void setTheme(const QString &value);
    QVector<QString> getLabels() const;
    void setLabels(const QVector<QString> &value);
    QVector<QString> getCategories() const;
    void setCategories(const QVector<QString> &value);
    QString getText() const;
    void setText(const QString &value);
    QString getId() const;
    void setId(const QString &value);
    QString getColor() const;
    void setColor(const QString &value);
    QString getOwner() const;
    void setOwner(const QString &value);
};

#endif // NOTE_H

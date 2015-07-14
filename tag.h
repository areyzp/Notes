#ifndef TAG_H
#define TAG_H
#include <QString>
#include <QVector>

class Tag
{
    QString id;
    QString text;
    bool owns;
public:
    Tag();
    QString getId() const;
    void setId(const QString &value);
    QString getText() const;
    void setText(const QString &value);
    bool getOwns() const;
    void setOwns(bool value);
};

#endif // TAG_H

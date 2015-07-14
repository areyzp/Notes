#ifndef CATEGORYCHECKBOX_H
#define CATEGORYCHECKBOX_H

#include <QWidget>
#include <QCheckBox>
#include <category.h>
#include <QVector>

class CategoryCheckBox : public QCheckBox
{
    Q_OBJECT
public:
    explicit CategoryCheckBox(QWidget *parent = 0);

    Category *getC() const;
    void setC(Category *value);

    CategoryCheckBox *getParent() const;
    void setParent(CategoryCheckBox *value);

    QVector <CategoryCheckBox*> getChildren();
    void addChild(CategoryCheckBox* ch);

signals:

public slots:

private slots:
    void turned(int i);

private:
    Category* c;
    QVector <CategoryCheckBox*> children;
    CategoryCheckBox* parent;
};

#endif // CATEGORYCHECKBOX_H

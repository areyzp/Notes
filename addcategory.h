#ifndef ADDCATEGORY_H
#define ADDCATEGORY_H

#include <QWidget>
#include <QPushButton>
#include <category.h>

class AddCategory : public QPushButton
{
    Q_OBJECT
    Category* parentc;

public:
    explicit AddCategory(QWidget *parent = 0);
    Category* getParentc();
    void setParentc(Category* c);


signals:
    void newCat(Category*);

private slots:
    void onClick();

public slots:
};

#endif // ADDCATEGORY_H

#include "addcategory.h"

AddCategory::AddCategory(QWidget *parent) : QPushButton(parent)
{
    //this->setGeometry(this->geometry().x(), this->geometry().y(), 32, 12);
    //this->setText("Новая категория");
    connect(this, SIGNAL(released()), this, SLOT(onClick()));
}

Category *AddCategory::getParentc()
{
    return parentc;
}

void AddCategory::setParentc(Category *c)
{
    parentc = c;
}

AddCategory::onClick()
{
    emit newCat(parentc);
}


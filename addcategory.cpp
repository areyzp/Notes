#include "addcategory.h"

AddCategory::AddCategory(QWidget *parent) : QPushButton(parent)
{
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

void AddCategory::onClick()
{
    emit newCat(parentc);
}


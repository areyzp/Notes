#include "categorycheckbox.h"

CategoryCheckBox::CategoryCheckBox(QWidget *parent) : QCheckBox(parent)
{
    connect(this, SIGNAL(stateChanged(int)), this, SLOT(turned(int)));
}
Category *CategoryCheckBox::getC() const
{
    return c;
}

void CategoryCheckBox::setC(Category *value)
{
    c = value;
}
CategoryCheckBox *CategoryCheckBox::getParent() const
{
    return parent;
}

void CategoryCheckBox::setParent(CategoryCheckBox *value)
{
    parent = value;
    if (parent!=NULL)
        parent->addChild(this);
}

QVector<CategoryCheckBox *> CategoryCheckBox::getChildren()
{
    return children;
}

void CategoryCheckBox::addChild(CategoryCheckBox *ch)
{
    children.push_back(ch);
}

void CategoryCheckBox::turned(int i)
{
    if (this->isChecked())
    {
        if (parent!=NULL)
            parent->setChecked(true);
    }
    else
    {
        foreach(CategoryCheckBox* ch, children)
        {
            ch->setChecked(false);
        }
    }
}




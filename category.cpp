#include "category.h"

Category* Category::root = 0;

QString Category::getName() const
{
    return name;
}

void Category::setName(const QString &value)
{
    name = value;
}

Category *Category::find(QString name)
{
    if (this->id == name)
    {
        return this;
    }
    else
    {
        foreach(Category* c, child)
        {
            Category *res = c->find(name);
            if (res != NULL)
                return res;
        }
        return NULL;
    }
}

bool Category::checkName(QString name)
{
    if (this->name == name)
    {
        return true;
    }
    else
    {
        foreach(Category* c, child)
        {
            bool res = c->checkName(name);
            if (res)
                return res;
        }
        return false;
    }
}

Category *Category::getParent() const
{
    return parent;
}

Category *Category::add(QString name)
{
    if (getStart()->find(name) == NULL)
    {
        Category* c = new Category(this);
        c->setId(name);
        this->child.push_back(c);
        return c;
    }
}

Category *Category::getStart()
{
    if (root==0)
    {
        root = new Category(0);
        Category::root->setId("-1");
        Category::root->setName("");
    }
    return root;
}

void Category::clear()
{
    if (root!=NULL)
        delete root;
    root=NULL;
}


QString Category::getId() const
{
    return id;
}

void Category::setId(const QString &value)
{
    id = value;
}

QVector<Category *> Category::getChildren()
{
    return child;
}
Category::Category(Category* parent)
{
    this->parent = parent;
}

Category::~Category()
{
    foreach(Category* c, child)
    {
        delete c;
    }
}


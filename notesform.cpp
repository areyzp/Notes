#include "notesform.h"
#include "ui_notesform.h"
#include <QFile>
#include <QTcpSocket>
#include <QMessageBox>
#include <parser.h>
#include <QJsonDocument>
#include <notewidget.h>
#include <QTimer>
#include <addcategory.h>
#include <QInputDialog>
#include <QLineEdit>
#include <mainwindow.h>
#include <QLabel>
#include <noteview.h>
#include <noteedit.h>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

NotesForm::NotesForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NotesForm)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon("../ClientPro/icon.png"));
    this->setFixedSize(353, 568);

    QFile f("../ClientPro/address");
    QString s;
    if (f.open(QIODevice::ReadOnly))
    {
        s = QString(f.readAll());
    }
    ip = s.split(":").first();
    port = s.split(":").last();

    defaultTags.clear();
    userTags.clear();
    userNoteList.clear();
    sharedNoteList.clear();
    Category::clear();

    QList <QWidget*> l1 = ui->scrollAreaWidgetContents_2->findChildren<QWidget*>();
    foreach(QWidget* w, l1)
        w->deleteLater();

    QList <QWidget*> l2 = ui->scrollAreaWidgetContents->findChildren<QWidget*>();
    foreach(QWidget* w, l2)
        w->deleteLater();

    QList <QWidget*> l3 = ui->myNotesArea->findChildren<QWidget*>();
    foreach(QWidget* w, l3)
        w->deleteLater();

    QList <QWidget*> l4 = ui->sharedNoteArea->findChildren<QWidget*>();
    foreach(QWidget* w, l4)
        w->deleteLater();

    QFile file("../ClientPro/token");
    if (file.open(QIODevice::ReadOnly))
    {
        QString s = QString(file.readAll());
        token=s;
        file.close();
    }
    message = "GET_DATA||"+token;
    client = new QTcpSocket();
    connect(client, SIGNAL(connected()), this, SLOT(send()));
    client->connectToHost(ip, port.toInt());
}

NotesForm::~NotesForm()
{
    delete ui;
}

void NotesForm::endSession()
{
    QMessageBox msg;
    msg.setText("Токен устарел.");
    msg.exec();
    logout();
}

void NotesForm::readData(QString tags, QString categories, QString userNotes, QString friendNotes)
{
    Parser p;
    qDebug() << "Parsing tags";
    userTags.clear();
    defaultTags.clear();
    QVector <Tag> t = p.parseTags(tags);
    foreach(Tag tag, t)
    {
        if (tag.getOwns())
        {
            userTags.push_back(tag);
        }
        else
        {
            defaultTags.push_back(tag);
        }
    }
    qDebug() << "Parsing categories";
    Category::clear();
    p.parseCategories(categories);
    qDebug() << "Parsing user notes";
    userNoteList = p.parseUserNotes(userNotes);
    qDebug() << "Parsing shared notes";
    sharedNoteList = p.parseUserNotes(friendNotes);
    temp=0;
    CategoryCheckBox* empty = new CategoryCheckBox(this);
    empty->setC(Category::getStart());
    empty->setParent(NULL);
    drawTags();
    QList <QWidget*> l1 = ui->scrollAreaWidgetContents_2->findChildren<QWidget*>();
    foreach(QWidget* w, l1)
        w->deleteLater();
    qDebug() << "Drawing categories";
    catBoxes.clear();
    drawCategories(Category::getStart(), empty, 0);
    drawMyNotes();
    drawSharedNotes();
}

void NotesForm::drawTags()
{
    tagsBoxes.clear();
    QList <QWidget*> l2 = ui->scrollAreaWidgetContents->findChildren<QWidget*>();
    foreach(QWidget* w, l2)
        w->deleteLater();
    qDebug() << "Drawing tags";
    QLabel *defl = new QLabel(ui->scrollAreaWidgetContents);
    defl->setText("Стандартные ярлыки:");
    defl->setGeometry(40,12,250, defl->geometry().height());
    defl->show();
    int rows = defaultTags.size()/2;
    for (int i=0; i<rows; i++)
    {
        QCheckBox *cb = new QCheckBox(ui->scrollAreaWidgetContents);
        tagsBoxes.push_back(cb);
        cb->setText(defaultTags[i].getText());
        cb->setChecked(true);
        cb->setGeometry(50,35+25*i,cb->geometry().width(), cb->geometry().height());
        cb->show();
    }
    for (int i=rows; i<defaultTags.size(); i++)
    {
        QCheckBox *cb = new QCheckBox(ui->scrollAreaWidgetContents);
        tagsBoxes.push_back(cb);
        cb->setText(defaultTags[i].getText());
        cb->setChecked(true);
        cb->setGeometry(180,35+25*(i-rows),cb->geometry().width(), cb->geometry().height());
        cb->show();
    }
    int tab = 35+25*rows+10;

    QLabel *usrl = new QLabel(ui->scrollAreaWidgetContents);
    usrl->setText("Пользовательские ярлыки:");
    usrl->setGeometry(40,tab+12,250, usrl->geometry().height());
    usrl->show();
    rows = userTags.size()/2;
    for (int i=0; i<rows; i++)
    {
        QCheckBox *cb = new QCheckBox(ui->scrollAreaWidgetContents);
        tagsBoxes.push_back(cb);
        cb->setText(userTags[i].getText());
        cb->setChecked(true);
        cb->setGeometry(50,tab+35+25*i,cb->geometry().width(), cb->geometry().height());
        cb->show();
    }
    for (int i=rows; i<userTags.size(); i++)
    {
        QCheckBox *cb = new QCheckBox(ui->scrollAreaWidgetContents);
        tagsBoxes.push_back(cb);
        cb->setText(userTags[i].getText());
        cb->setChecked(true);
        cb->setGeometry(180,tab+35+25*(i-rows),cb->geometry().width(), cb->geometry().height());
        cb->show();
    }

    QPushButton *add = new QPushButton(ui->scrollAreaWidgetContents);
    add->setText("Добавить ярлык");
    add->setGeometry(10, tab+rows*25+75, 130, 35);
    add->show();
    connect(add, SIGNAL(pressed()), this, SLOT(createTag()));

    QPushButton *remove = new QPushButton(ui->scrollAreaWidgetContents);
    remove->setText("Удалить ярлык");
    remove->setGeometry(160, tab+rows*25+75, 130, 35);
    remove->show();
    connect(remove, SIGNAL(pressed()), this, SLOT(removeTag()));

    ui->scrollAreaWidgetContents->setGeometry(0,0,ui->scrollAreaWidgetContents->geometry().width(), remove->geometry().y()+remove->geometry().height()+30);
}

void NotesForm::drawCategories(Category *c, CategoryCheckBox* parent, int depth)
{
    foreach(Category *ch, c->getChildren())
    {
        CategoryCheckBox* chbox = new CategoryCheckBox(ui->scrollAreaWidgetContents_2);
        chbox->setC(ch);
        chbox->setParent(parent);
        chbox->setText(ch->getName());
        chbox->setGeometry(30+35*depth, 10+30*temp++, 24+int(chbox->text().length()*5.7), chbox->geometry().height());
        chbox->show();
        chbox->setChecked(true);
        catBoxes.push_back(chbox);

        AddCategory* del = new AddCategory(ui->scrollAreaWidgetContents_2);
        del->setGeometry(30+35*depth+chbox->geometry().width()+5, 14+30*(temp-1), 20, 20);
        del->setParentc(ch);
        del->setText("x");
        del->show();
        connect(del,SIGNAL(newCat(Category*)), this, SLOT(removeCat(Category*)));

        drawCategories(ch,chbox,depth+1);
    }
    AddCategory* a = new AddCategory(ui->scrollAreaWidgetContents_2);
    a->setGeometry(30+35*depth, 10+30*temp++, 105, 20);
    a->setParentc(c);
    a->setText("Новая категория");
    a->show();
    connect(a, SIGNAL(newCat(Category*)), this, SLOT(createCat(Category*)));
    ui->scrollAreaWidgetContents_2->setGeometry(ui->scrollAreaWidgetContents_2->x(), ui->scrollAreaWidgetContents_2->y(), ui->scrollAreaWidgetContents_2->geometry().width(), 30+30*temp);
}

void NotesForm::drawMyNotes()
{
    qDebug() << "Drawing user notes";
    int drawn = 0;

    bool checkTags = ui->checkTags->checkState()==2;
    bool checkCats = ui->checkCats->checkState()==2;
    QVector <QString> tags;
    if (checkTags)
    {
        foreach(QCheckBox* b, tagsBoxes)
        {
            if (b->checkState()==2)
                tags.push_back(b->text());
        }
    }
    QVector <QString> cats;
    if (checkCats)
    {
        foreach(CategoryCheckBox* b, catBoxes)
        {
            if (b->checkState()==2)
            {
                cats.push_back(b->getC()->getName());
            }
        }
    }
    QList <QWidget*> l3 = ui->myNotesArea->findChildren<QWidget*>();
    foreach(QWidget* w, l3)
        w->deleteLater();
    int i = 0;
    foreach(Note* n, userNoteList)
    {
        bool ok = true;
        if (checkTags)
        {
            ok = false;
            foreach(QString s,n->getLabels())
            {
                if (tags.contains(s))
                    ok = true;
            }
        }
        if (checkCats && ok)
        {
            ok = false;
            foreach(QString s, n->getCategories())
            {
                if (cats.contains(s))
                    ok = true;
            }
        }
        if (ok)
        {
            drawn++;
            NoteWidget* w = new NoteWidget(ui->myNotesArea);
            w->setUp(n);
            w->setGeometry(0, 0+122*i++, w->geometry().width(),w->geometry().height());
            w->show();
            connect(w, SIGNAL(share(Note*)), this, SLOT(share(Note*)));
            connect(w, SIGNAL(clicked(Note*)), this, SLOT(showNote(Note*)));
        }
    }
    ui->myNotesArea->setGeometry(0,0,ui->myNotesArea->geometry().width(), drawn*122+18);
}

void NotesForm::drawSharedNotes()
{
    int drawn=0;
    bool checkTags = ui->checkTags->checkState()==2;
    bool checkCats = ui->checkCats->checkState()==2;
    QVector <QString> tags;
    if (checkTags)
    {
        foreach(QCheckBox* b, tagsBoxes)
        {
            if (b->checkState()==2)
                tags.push_back(b->text());
        }
    }
    QVector <QString> cats;
    if (checkCats)
    {
        foreach(CategoryCheckBox* b, catBoxes)
        {
            if (b->checkState()==2)
            {
                cats.push_back(b->getC()->getName());
            }
        }
    }

    QList <QWidget*> l3 = ui->sharedNoteArea->findChildren<QWidget*>();
    foreach(QWidget* w, l3)
        w->deleteLater();
    qDebug() << "Drawing shared notes";
    int i=0;
    foreach(Note* n, sharedNoteList)
    {
        bool ok = true;
        if (checkTags)
        {
            ok = false;
            foreach(QString s,n->getLabels())
            {
                if (tags.contains(s))
                    ok = true;
            }
        }
        if (checkCats && ok)
        {
            ok = false;
            foreach(QString s, n->getCategories())
            {
                if (cats.contains(s))
                    ok = true;
            }
        }
        if (ok)
        {
            drawn++;
            NoteWidget* w = new NoteWidget(ui->sharedNoteArea);
            w->setUp(n);
            w->setGeometry(0, 0+122*i++, w->geometry().width(),w->geometry().height());
            w->show();
            connect(w,SIGNAL(clicked(Note*)), this, SLOT(showNote(Note*)));
        }
    }
    ui->sharedNoteArea->setGeometry(0,0,ui->sharedNoteArea->geometry().width(), drawn*122+18);
}

void NotesForm::readAnswer()
{
    QByteArray data;
    QString ssize = client->peek(8);

    if (client->bytesAvailable()<ssize.toInt()+8)
    {
        return;
    }

    data = client->readAll();

    QString s = data;
    s = s.right(s.size()-8);
    QStringList l = s.split("||");
    if (l[0] == "TOKEN_FAIL")
    {
        endSession();
    }
    else if (l[0] == "DATA_OK")
    {
        readData(l[1], l[2], l[3], l[4]);
    }
    else if (l[0] == "NEW_CAT_ERROR")
    {
        QMessageBox msg;
        msg.setText("Ошибка создания новой категории.");
        msg.exec();
        return;
    }
    else if (l[0] == "NEW_CAT_OK")
    {
        Category *newcat = Category::getStart()->find(l[3])->add(l[1]);
        newcat->setName(l[2]);
        QList <QWidget*> list = ui->scrollAreaWidgetContents_2->findChildren<QWidget*>();
        foreach(QWidget* w, list)
            w->deleteLater();
        temp=0;
        CategoryCheckBox* empty = new CategoryCheckBox(this);
        empty->setC(Category::getStart());
        empty->setParent(NULL);
        catBoxes.clear();
        drawCategories(Category::getStart(), empty, 0);
    }
    else if (l[0] == "DEL_CAT_OK")
    {
        Category::clear();
        Parser p;
        p.parseCategories(l[1]);
        QList <QWidget*> list = ui->scrollAreaWidgetContents_2->findChildren<QWidget*>();
        foreach(QWidget* w, list)
            w->deleteLater();
        temp=0;
        CategoryCheckBox* empty = new CategoryCheckBox(this);
        empty->setC(Category::getStart());
        empty->setParent(NULL);
        catBoxes.clear();
        drawCategories(Category::getStart(), empty, 0);
    }
    else if (l[0] == "NEW_TAG_OK" || l[0] == "DEL_TAG_OK")
    {
        defaultTags.clear();
        userTags.clear();

        QList <QWidget*> l2 = ui->scrollAreaWidgetContents->findChildren<QWidget*>();
        foreach(QWidget* w, l2)
            w->deleteLater();

        Parser p;
        QVector <Tag> t = p.parseTags(l[1]);
        foreach(Tag tag, t)
        {
            if (tag.getOwns())
            {
                userTags.push_back(tag);
            }
            else
            {
                defaultTags.push_back(tag);
            }
        }
        drawTags();
    }
    else if (l[0]=="NO_SUCH_USER")
    {
        QMessageBox msg;
        msg.setText("Пользователь не найден.");
        msg.exec();
    }
    else if (l[0]=="SHARE_NOTE_OK")
    {
        QMessageBox msg;
        msg.setText("Заметка была открыта.");
        msg.exec();
    }
    else if (l[0] == "NEW_TAG_FAIL")
    {
        QMessageBox msg;
        msg.setText("Ошибка создания ярлыка.");
        msg.exec();
    }
    else if (l[0] == "NEW_NOTE_OK" || l[0] == "UPD_NOTE_OK" || l[0] == "DEL_NOTE_OK")
    {
        Parser p;
        userNoteList = p.parseUserNotes(l[1]);
        drawMyNotes();
    }
    else
    {
        //unknown error
    }
    client->disconnectFromHost();
}

void NotesForm::createCat(Category *c)
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                         tr("Название новой категории:"), QLineEdit::Normal,
                                         "", &ok);
    if (ok)
    {
        message = "NEW_CAT||"+text+"||"+c->getId()+"||"+token;
        client->connectToHost(ip, port.toInt());
    }
}

void NotesForm::removeCat(Category *c)
{
    message = "DEL_CAT||"+c->getId()+"||"+token;
    client->connectToHost(ip, port.toInt());
}

void NotesForm::send()
{
    connect(client,SIGNAL(readyRead()),this,SLOT(readAnswer()));
    //client->write(message.toStdString().c_str(), message.size());
    QByteArray data = message.toStdString().c_str();
    const char *start = data.constData();
    int written = 0;
    while ( written < data.size() )
    {
        written += client->write(start+written);
    }
    if (message.split("||").first() == "LOG_OUT")
    {
        client->disconnectFromHost();
    }
}

void NotesForm::on_pushButton_3_clicked()
{
    logout();
}

void NotesForm::logout()
{
    QFile token("../ClientPro/token");
    if (token.open(QIODevice::WriteOnly))
    {
        token.write("",0);
        token.close();
    }
    MainWindow *m = new MainWindow();
    m->show();
    this->close();
}

void NotesForm::createTag()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Новый ярлык"),
                                         tr("Название нового ярлыка:"), QLineEdit::Normal,
                                         "", &ok);
    if (ok)
    {
        message = "NEW_TAG||"+text+"||"+token;
        client->connectToHost(ip, port.toInt());
    }
}

void NotesForm::removeTag()
{
    bool proceed=false;
    QString ID;
    while(!proceed)
    {
        bool ok;
        QString text = QInputDialog::getText(this, tr("Удалить ярлык"),
                                             tr("Введите ярлык, который вы хотите удалить:"), QLineEdit::Normal,
                                             "", &ok);
        if (!ok)
            return;
        else
        {
            foreach(Tag t, userTags)
            {
                if (t.getText()==text)
                {
                    proceed = true;
                    ID = t.getId();
                }
            }
        }
    }
    message = "DEL_TAG||"+ID+"||"+token;
    client->connectToHost(ip, port.toInt());
}

void NotesForm::share(Note *n)
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Поделиться заметкой"),
                                         tr("Введите имя пользователя, которому вы хотите открыть заметку:"), QLineEdit::Normal,
                                         "", &ok);
    if (ok)
    {
        message = "SHARE_NOTE||"+n->getId()+"||"+text+"||"+token;
        client->connectToHost(ip, port.toInt());
    }
}

void NotesForm::showNote(Note *n)
{
    /*QVector <Tag> tags;
    tags = defaultTags;
    foreach(Tag t, userTags)
    {
        tags.push_back(t);
    }*/
    NoteView *w = new NoteView();
    w->setUp(n);
    //w->setTags(tags);
    connect(w, SIGNAL(edit(QJsonDocument*)), this, SLOT(editNote(QJsonDocument*)));
    connect(w, SIGNAL(delnote(const char*)), this, SLOT(delNote(const char*)));
    w->show();
}

void NotesForm::on_pushButton_clicked()
{
    QVector <Tag> t;
    t = defaultTags;
    foreach(Tag tag, userTags)
    {
        t.push_back(tag);
    }
    NoteEdit *e = new NoteEdit();
    //e->setTags(t);
    connect(e, SIGNAL(ok(QJsonDocument*)), this, SLOT(editNote(QJsonDocument*)));
    e->show();
}

void NotesForm::editNote(QJsonDocument *doc)
{
    QJsonObject json;
    json = doc->object();
    if (json.contains("id"))
    {
        message = "UPDATE_NOTE||"+doc->toJson()+"||"+token;
        client->connectToHost(ip, port.toInt());
    }
    else
    {
        message = "NEW_NOTE||"+doc->toJson()+"||"+token;
        client->connectToHost(ip, port.toInt());
    }
}

void NotesForm::delNote(const char *c)
{
    message = "DEL_NOTE||"+QString(c)+"||"+token;
    client->connectToHost(ip, port.toInt());
}

void NotesForm::on_pushButton_2_clicked()
{
    message = "GET_DATA||"+token;
    client->connectToHost(ip, port.toInt());
}

void NotesForm::on_checkTags_stateChanged(int arg1)
{
    drawMyNotes();
    drawSharedNotes();
}

void NotesForm::on_checkCats_stateChanged(int arg1)
{
    drawMyNotes();
    drawSharedNotes();
}

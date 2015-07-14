#include "noteview.h"
#include "ui_noteview.h"
#include <note.h>
#include <category.h>
#include <tag.h>
#include <QVector>
#include <QString>
#include <QFont>
#include <noteedit.h>
#include <QDebug>

NoteView::NoteView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NoteView)
{
    ui->setupUi(this);
    this->setFixedSize(320, 448);
    this->setWindowIcon(QIcon("../ClientPro/icon.png"));
}

NoteView::~NoteView()
{
    delete ui;
}

void NoteView::setUp(Note *n)
{
    int maxsize = 650;
    this->n = n;


    if (n->getOwner()!="")
    {
        ui->del->hide();
        ui->edit->hide();
    }

    ui->label->setPixmap(QPixmap("../ClientPro/top"+n->getColor()+".png"));
    QString text;
    if (n->getOwner()!="")
    {
        text = n->getTheme()+" by "+n->getOwner();
    }
    else
    {
        text = n->getTheme();
    }
    ui->label_2->setText(text);
    this->setWindowTitle(text);

    QLabel *tags = new QLabel(ui->content);
    if (n->getLabels().size()==0)
    {
        tags->setGeometry(0,0,0,0);
    }
    else
    {
        text = "Ярлыки: ";
        foreach(QString s, n->getLabels())
        {
            text+=s+", ";
        }

        QFont f = tags->font();
        f.setPixelSize(12);
        text = text.left(text.size()-2);
        text+=".";
        tags->setFont(f);
        tags->setText(text);
        tags->setWordWrap(true);
        tags->adjustSize();
        tags->setGeometry(7,5,300,12);
    }
    tags->show();

    QLabel *categs = new QLabel(ui->content);
    if (n->getCategories().size()==0)
    {
        categs->setGeometry(7, tags->geometry().y()+tags->geometry().height(),0,0);
    }
    else
    {
        text = "Категории: ";
        foreach(QString c, n->getCategories())
        {
            text+=c+", ";
        }
        text = text.left(text.size()-2);
        text+=".";

        QFont f = categs->font();
        f.setPixelSize(12);
        categs->setFont(f);
        categs->setText(text);
        categs->setWordWrap(true);
        categs->adjustSize();
        categs->setGeometry(7,tags->geometry().y()+tags->geometry().height()+5, 300, 12);
    }
    categs->show();

    QTextEdit *textEdit = new QTextEdit(ui->content);
    textEdit->setHtml(n->getText());
    textEdit->setGeometry(7,categs->geometry().y()+categs->geometry().height()+12, 285, 299-categs->geometry().y()+categs->geometry().height());
    textEdit->setReadOnly(true);
    textEdit->show();

    ui->content->setGeometry(0,0,ui->content->geometry().width(), textEdit->geometry().x()+textEdit->geometry().height()+39);
}

void NoteView::on_edit_clicked()
{
    NoteEdit *e = new NoteEdit();
    e->setUp(n);
    connect (e, SIGNAL(ok(QJsonDocument*)), this, SLOT(passSignal(QJsonDocument*)));
    e->show();
    this->close();
}

void NoteView::passSignal(QJsonDocument *doc)
{
    emit edit(doc);
}

void NoteView::on_del_clicked()
{
    emit delnote(n->getId().toStdString().c_str());
    this->close();
}

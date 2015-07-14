#include "notewidget.h"
#include "ui_notewidget.h"
#include <QDebug>
#include <QIcon>
#include <QMouseEvent>
#include <QMessageBox>
#include <QTextDocument>

NoteWidget::NoteWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NoteWidget)
{
    ui->setupUi(this);
}

NoteWidget::~NoteWidget()
{
    delete ui;
}

void NoteWidget::setUp(Note *n)
{
    note = n;

    if (note->getOwner()!="")
    {
        ui->title->setText(note->getOwner()+": "+note->getTheme());
        ui->pushButton->hide();
    }
    else
    {
        ui->title->setText(note->getTheme());
        QIcon i("../ClientPro/share.png");
        ui->pushButton->setIcon(i);
    }

    if (ui->title->text().size()>15)
        ui->title->setText(ui->title->text().left(13)+"...");

    QTextDocument doc;
    doc.setHtml(note->getText());
    QString text = doc.toPlainText();
    if (text.length()>20)
    {
        text = text.left(15)+"...";
    }
    ui->text->setText(text);

    QString labels;
    if (note->getLabels().size() == 0)
    {
        labels = "Ярлыки: -";
    }
    else if (note->getLabels().size() == 1)
    {
        labels = "Ярлыки: "+note->getLabels().first();
    }
    else
    {
        labels = "Ярлыки: "+note->getLabels().first()+", ... (+"+QString::number(note->getLabels().size()-1)+")";
    }
    ui->labels->setText(labels);

    QString cats;
    if (note->getCategories().size()==0)
    {
        cats = "Категории: -";
    }
    else if (note->getCategories().size()==1)
    {
        cats = "Категории: "+note->getCategories().first();
    }
    else
    {
        cats = "Категории: "+note->getCategories().first()+", ... (+"+QString::number(note->getLabels().size()-1)+")";
    }
    ui->cats->setText(cats);

    QString path = note->getColor()+".png";
    QPixmap p("../ClientPro/"+path);
    ui->color->setPixmap(p);
}

Note *NoteWidget::getNote()
{
    return note;
}

void NoteWidget::on_pushButton_clicked()
{
    emit share(note);
}

void NoteWidget::mousePressEvent(QMouseEvent *)
{
    emit clicked(note);
}

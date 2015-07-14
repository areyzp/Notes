#include "noteedit.h"
#include "ui_noteedit.h"
#include <QDebug>
#include <QMessageBox>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <tagmanager.h>
#include <QTextDocument>

NoteEdit::NoteEdit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NoteEdit)
{
    ui->setupUi(this);
    this->setFixedSize(293, 438);
    this->setWindowIcon(QIcon("../ClientPro/icon.png"));
    create = true;
    connect(ui->fontComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(fontChanged(QString)));
    connect(ui->comboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(sizeChanged(QString)));
    connect(ui->boldButton, SIGNAL(pressed()), this, SLOT(bold()));
    connect(ui->italicButton, SIGNAL(pressed()), this, SLOT(italic()));
    connect(ui->underlinedButton, SIGNAL(pressed()), this, SLOT(underlined()));
    QFontDatabase db;
    foreach(int size, db.standardSizes())
        ui->comboBox->addItem(QString::number(size));
}

NoteEdit::~NoteEdit()
{
    delete ui;
}

void NoteEdit::setUp(Note *n)
{
    create = false;
    this->n = n;

    if (n->getColor()=="blue")
        ui->color->setCurrentIndex(1);
    else if (n->getColor()=="green")
        ui->color->setCurrentIndex(0);
    else if (n->getColor()=="red")
        ui->color->setCurrentIndex(2);

    ui->theme->setText(n->getTheme());

    QString text = "";
    foreach(QString s, n->getLabels())
    {
        text+=s+", ";
    }
    text = text.left(text.size()-2);
    ui->tags->setText(text);

    QString text1 = "";
    foreach(QString c, n->getCategories())
    {
        text1+=c+", ";
    }
    text1 = text1.left(text1.size()-2);
    ui->cats->setText(text1);

    ui->text->setText(n->getText());
}
/*
NoteEdit::setTags(QVector<Tag> t)
{
    tags = t;
}*/

void NoteEdit::on_pushButton_2_clicked()
{
    this->close();
}

void NoteEdit::on_pushButton_clicked()
{
    if (ui->text->toPlainText().size() > 2500)
    {
        QMessageBox msg;
        msg.setText("Слишком длинный текст");
        msg.exec();
        return;
    }

    if (ui->text->toPlainText().size() == 0)
    {
        QMessageBox msg;
        msg.setText("Введите текст заметки");
        msg.exec();
        return;
    }

    if (ui->theme->text().size() == 0)
    {
        QMessageBox msg;
        msg.setText("Введите тему");
        msg.exec();
        return;
    }

    if (create)
    {
        Note *n = new Note();
        this->n = n;
    }
    if (ui->color->currentIndex() == 0)
        n->setColor("green");
    else if (ui->color->currentIndex()==1)
        n->setColor("blue");
    else
        n->setColor("red");
    n->setTheme(ui->theme->text());
    QString text = ui->text->toHtml();
    text = text.replace("'","''");
    text = text.replace("\"","\\\"");
    n->setText(text);
    QVector <QString> res;
    QStringList t = ui->tags->text().split(", ");
    if (ui->theme->text().size() > 40)
    {
        QMessageBox msg;
        msg.setText("Cлишком длинный заголовок");
        msg.exec();
        return;
    }
    foreach(QString tag, t)
    {
        QVector <Tag> tagsv = TagManager::getInst()->getTags();
        if (tag!="")
        {
            bool found = false;
            for (int i=0; i<tagsv.size(); i++)
            {
                if (tag == tagsv[i].getText())
                    found = true;
            }
            if (!found)
            {
                QMessageBox msg;
                msg.setText("Не существует ярлыка "+tag);
                msg.exec();
                return;
            }
            res.push_back(tag);
        }
    }
    n->setLabels(res);

    res.clear();
    t = ui->cats->text().split(", ");

    foreach(QString cat, t)
    {
        if (cat!="")
        {
            if (!Category::getStart()->checkName(cat))
            {
                QMessageBox msg;
                msg.setText("Не существует категории "+cat);
                msg.exec();
                return;
            }
            res.push_back(cat);
        }
    }
    n->setCategories(res);


    QJsonObject json;
    json["theme"] = n->getTheme();
    json["text"] = n->getText();
    json["color"] = n->getColor();
    QJsonArray arrtags;
    foreach(QString t, n->getLabels())
    {
        QJsonObject a;
        a["name"] = t;
        arrtags.push_back(a);
    }
    json["tags"] = arrtags;
    QJsonArray arrcats;
    foreach(QString c, n->getCategories())
    {
        QJsonObject a;
        a["name"] = c;
        arrcats.push_back(a);
    }
    json["cats"] = arrcats;

    if (!create)
        json["id"] = n->getId();

    QJsonDocument doc(json);
    emit ok(&doc);
    this->close();
}


void NoteEdit::apply(QTextCharFormat f)
{
    QTextCursor cursor = ui->text->textCursor();
    cursor.mergeCharFormat(f);
    ui->text->mergeCurrentCharFormat(f);
}

void NoteEdit::fontChanged(QString s)
{
    QTextCharFormat f;
    f.setFontFamily(s);
    apply(f);
}

void NoteEdit::sizeChanged(QString s)
{
    QTextCharFormat f;
    f.setFontPointSize((qreal)s.toFloat());
    apply(f);
}

void NoteEdit::bold()
{
    QTextCharFormat f;
    f.setFontWeight(ui->boldButton->isChecked() ? QFont::Normal : QFont::Bold);
    apply(f);
}

void NoteEdit::italic()
{
    QTextCharFormat f;
    f.setFontItalic(!ui->italicButton->isChecked());
    apply(f);
}

void NoteEdit::underlined()
{
    QTextCharFormat f;
    f.setFontUnderline(!ui->underlinedButton->isChecked());
    apply(f);
}

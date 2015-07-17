#ifndef NOTEEDIT_H
#define NOTEEDIT_H

#include <QWidget>
#include <note.h>
#include <QVector>
#include <QString>
#include "tag.h"
#include "category.h"
#include <QTextCharFormat>
#include <QTextEdit>
#include <QPushButton>
#include <QFontComboBox>
#include <QComboBox>
#include <QFontDatabase>
#include <QDebug>
#include <QTextCursor>

namespace Ui {
class NoteEdit;
}

class NoteEdit : public QWidget
{
    Q_OBJECT
    bool create;
    Note *n;
    //QVector <Tag> tags;

public:
    explicit NoteEdit(QWidget *parent = 0);
    ~NoteEdit();
    void setUp(Note* n);
    void apply(QTextCharFormat f);
    //setTags(QVector <Tag> t);

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void fontChanged(QString);
    void sizeChanged(QString);
    void bold();
    void italic();
    void underlined();

private:
    Ui::NoteEdit *ui;

signals:
    void ok(QJsonDocument*);
};

#endif // NOTEEDIT_H

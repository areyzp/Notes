#ifndef NOTESFORM_H
#define NOTESFORM_H

#include <QWidget>
#include <QTcpSocket>
#include <tag.h>
#include <category.h>
#include <parser.h>
#include <QVector>
#include <QCheckBox>
#include <categorycheckbox.h>

namespace Ui {
class NotesForm;
}

class NotesForm : public QWidget
{
    Q_OBJECT
    QString token;
    QString message;
    QTcpSocket *client;
    QVector <Tag> userTags;
    QVector <Tag> defaultTags;
    QVector <Note*> userNoteList;
    QVector <Note*> sharedNoteList;
    QVector <QCheckBox*> tagsBoxes;
    QVector <CategoryCheckBox*> catBoxes;
    int temp;
    QString ip;
    QString port;

public:
    explicit NotesForm(QWidget *parent = 0);
    ~NotesForm();

private:
    Ui::NotesForm *ui;
    void endSession();
    void readData(QString tags, QString categories, QString userNotes, QString friendNotes);
    void drawTags();
    void drawCategories(Category* c, CategoryCheckBox *parent, int depth);
    void drawMyNotes();
    void drawSharedNotes();

private slots:
    void send();
    void readAnswer();
    void createCat(Category*);
    void removeCat(Category*);
    void on_pushButton_3_clicked();
    void logout();
    void createTag();
    void removeTag();
    void share(Note *n);
    void showNote(Note *n);
    void on_pushButton_clicked();
    void editNote(QJsonDocument*);
    void delNote(const char*);
    void on_pushButton_2_clicked();
    void on_checkTags_stateChanged(int arg1);
    void on_checkCats_stateChanged(int arg1);
};

#endif // NOTESFORM_H

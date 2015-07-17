#ifndef NOTEVIEW_H
#define NOTEVIEW_H

#include <QWidget>
#include <note.h>
#include <tag.h>

namespace Ui {
class NoteView;
}

class NoteView : public QWidget
{
    Q_OBJECT
    Note* n;

public:
    explicit NoteView(QWidget *parent = 0);
    ~NoteView();
    void setUp(Note* n);
    //void setTags(QVector<Tag> t);

private slots:
    void on_edit_clicked();
    void passSignal(QJsonDocument*);

    void on_del_clicked();

private:
    Ui::NoteView *ui;

signals:
    void edit(QJsonDocument*);
    void delnote(const char*);
};

#endif // NOTEVIEW_H

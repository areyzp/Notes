#ifndef NOTEWIDGET_H
#define NOTEWIDGET_H

#include <QWidget>
#include <note.h>

namespace Ui {
class NoteWidget;
}

class NoteWidget : public QWidget
{
    Q_OBJECT
    Note* note;

public:
    explicit NoteWidget(QWidget *parent = 0);
    ~NoteWidget();
    void setUp(Note* n);
    Note* getNote();

protected:
    void mousePressEvent(QMouseEvent*);

signals:
    share(Note*);
    clicked(Note*);

private slots:
    void on_pushButton_clicked();

private:
    Ui::NoteWidget *ui;
};

#endif // NOTEWIDGET_H

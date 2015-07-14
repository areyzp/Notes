#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTcpServer>
#include <QString>
#include <notesform.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QTcpSocket *client;
    QString token;
    QString message;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    bool checkdata();
    void checktoken();
    void proceed();

private slots:
    void send();
    void on_pushButton_clicked();
    void readAnswer();
    void on_pushButton_2_clicked();
};

#endif // MAINWINDOW_H

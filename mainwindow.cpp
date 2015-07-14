#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QFile>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon("../ClientPro/icon.png"));
    QFile serv("../ClientPro/address");
    if (serv.open(QIODevice::ReadOnly))
    {
        QString s = QString(serv.readAll());
        ui->ipLine->setText(s.split(":").first());
        ui->portLine->setText(s.split(":").last());
    }
    QFile file("../ClientPro/token");
    if (file.open(QIODevice::ReadOnly))
    {
        QString s = QString(file.readAll());
        token=s;
        file.close();
    }
    checktoken();
    this->setFixedSize(301,200);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::checkdata()
{
    if (ui->loginLine->text() == "")
    {
        QMessageBox msg;
        msg.setText("Введите логин.");
        msg.exec();
        return false;
    }
    if (ui->passwordLine->text() == "")
    {
        QMessageBox msg;
        msg.setText("Введите пароль.");
        msg.exec();
        return false;
    }
    return true;
}

void MainWindow::checktoken()
{
    message = "CHECK_TOKEN||"+token;
    QString host = ui->ipLine->text();
    QString port = ui->portLine->text();
    client = new QTcpSocket();
    client->connectToHost(host, port.toInt());
    connect(client,SIGNAL(readyRead()), this, SLOT(readAnswer()));
    client->write(message.toStdString().c_str(), message.length());
}

void MainWindow::proceed()
{
    QFile file("../ClientPro/token");
    if (file.open(QIODevice::WriteOnly))
    {
        file.write(token.toStdString().c_str(),token.length());
        file.close();
    }
    QFile serv("../ClientPro/address");
    if (serv.open(QIODevice::WriteOnly))
    {
        QString data = ui->ipLine->text() + ":" + ui->portLine->text();
        serv.write(data.toStdString().c_str(), data.length());
        serv.close();
    }
    NotesForm *n = new NotesForm();
    n->show();
    this->close();
}

void MainWindow::send()
{
    message += ui->loginLine->text() + "||" + ui->passwordLine->text();
    connect(client,SIGNAL(readyRead()),this,SLOT(readAnswer()));
    client->write(message.toStdString().c_str(), message.length());
}

void MainWindow::on_pushButton_clicked()
{
    if (checkdata())
    {
        message = "LOGIN||";
        QString host = ui->ipLine->text();
        QString port = ui->portLine->text();
        client = new QTcpSocket();
        connect(client, SIGNAL(connected()), this, SLOT(send()));
        client->connectToHost(host, port.toInt());
    }
}

void MainWindow::readAnswer()
{
    QString data = client->readAll();
    QString answer = data.split("||").first();
    if (answer=="LOG_OK")
    {
        token = data.split("||").value(1);
        checktoken();
    }
    else if (answer=="LOG_NOUSR")
    {
        QMessageBox msg;
        msg.setText("Неправильное имя пользователя!");
        msg.exec();
    }
    else if (answer=="REG_FAIL")
    {
        QMessageBox msg;
        msg.setText("Такой пользователь уже есть!");
        msg.exec();
    }
    else if (answer=="REG_OK")
    {
        QMessageBox msg;
        msg.setText("Пользователь зарегистрирован успешно!");
        msg.exec();
    }
    else if (answer=="TOKEN_OK")
    {
        proceed();
    }
    else if (answer=="TOKEN_FAIL")
    {
        //could possibly do something
    }
    else
    {
        QMessageBox msg;
        msg.setText("Неизвестная ошибка!");
        msg.exec();
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    if (checkdata())
    {
        message = "REGISTER||";
        QString host = ui->ipLine->text();
        QString port = ui->portLine->text();
        client = new QTcpSocket();
        connect(client, SIGNAL(connected()), this, SLOT(send()));
        client->connectToHost(host, port.toInt());
    }
}

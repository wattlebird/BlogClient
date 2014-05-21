#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    dlgLogin(new Login2(this))
{
    ui->setupUi(this);
    ui->actionLogout->setEnabled(false);

    connect(dlgLogin, SIGNAL(sendauth(QString,QString)), this, SLOT(getauth(QString,QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::login(){
    int rt = dlgLogin->exec();
    if(rt){
        //Need to connect to internet!
        ui->actionLogout->setEnabled(true);
        ui->actionLogin->setEnabled(false);
    }
}

void MainWindow::logout(){
    ui->actionLogout->setEnabled(false);
    ui->actionLogin->setEnabled(true);
}

void MainWindow::getauth(QString user, QString pw){
    strUser = user;
    strPw   = pw;
}

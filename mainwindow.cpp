#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QByteArray>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    netManager(new QNetworkAccessManager(this)),
    cookies(new QNetworkCookieJar(this)),
    dlgLogin(new Login2(this))
{
    ui->setupUi(this);
    ui->actionLogout->setEnabled(false);
    netManager->setCookieJar(cookies);
    ui->statusBar->showMessage(QString("Unlogged."));
    connect(dlgLogin, SIGNAL(sendauth(QString,QString)), this, SLOT(getauth(QString,QString)));
    //connect(this, SIGNAL(logging()), this, SLOT(statusToLogging()));
    //connect(this, SIGNAL(logged()), this, SLOT(statusToLogged()));
    //connect(this, SIGNAL(unlogged()), this, SLOT(statusToUnlogged()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::login(){
	ui->statusBar->showMessage(QString("Logging..."));
    int rt = dlgLogin->exec();
    if(rt){
        //Need to connect to internet!
		QByteArray postdata;
		postdata.append("username=");
		postdata.append(strUser);
		postdata.append("&password=");
		postdata.append(strPw);
		QNetworkRequest request(QUrl("http://ikewikipage.appspot.com/login"));
		reply = netManager->post(request,postdata);
		connect(reply,SIGNAL(finished()),this,SLOT(getauthreply()));
    }else{
        ui->actionLogout->setEnabled(false);
        ui->actionLogin->setEnabled(true);
		ui->statusBar->showMessage(QString("Unlogged."));
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

void MainWindow::getauthreply(){
    if(reply->error()==QNetworkReply::NoError){
        QNetworkCookie cookie;
        bool bCookieexist = false;
        QList<QNetworkCookie> cookieslist = cookies->cookiesForUrl(QUrl("http://ikewikipage.appspot.com/login"));
        foreach(cookie, cookieslist){
            if(cookie.name()=="user") bCookieexist = true;
            break;
        }
		if(bCookieexist){
			ui->statusBar->showMessage(QString("Successfully logged in."));
			ui->actionLogout->setEnabled(true);
			ui->actionLogin->setEnabled(false);
		}else{
			ui->statusBar->showMessage(QString("Unlogged."));
		}
    }else{
		ui->statusBar->showMessage(QString("Unlogged."));
	}

}

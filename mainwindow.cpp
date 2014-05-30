#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QByteArray>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    netManager(new QNetworkAccessManager(this)),
    cookies(new CookieJar(this)),
    dlgLogin(new Login2(this)),
    labelCurrentStatus(new QLabel("Unlogged."))
{
    ui->setupUi(this);
    ui->actionLogout->setEnabled(false);
    netManager->setCookieJar(cookies);
    ui->statusBar->addWidget(labelCurrentStatus);

    connect(dlgLogin, SIGNAL(sendauth(QString,QString)), this, SLOT(getauth(QString,QString)));
    connect(ui->btnPush, SIGNAL(clicked()), this, SLOT(pusharticle()));
    connect(ui->listWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(showarticle()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::login(){
    labelCurrentStatus->setText("Logging...");
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
        labelCurrentStatus->setText("Unlogged.");
	}
}

void MainWindow::logout(){
    reply = netManager->get(QNetworkRequest(QUrl("http://ikewikipage.appspot.com/logout")));
    connect(reply,SIGNAL(finished()),this,SLOT(getlogoutreply()));
}

void MainWindow::sync(){
    ui->statusBar->showMessage("syncing...");
    QUrl xmlsource(QString("http://ikewikipage.appspot.com/api?user=").append(strUser));
    reply = netManager->get(QNetworkRequest(xmlsource));
    connect(reply,SIGNAL(finished()),this,SLOT(updatelist()));
}

void MainWindow::getauth(QString user, QString pw){
    strUser = user;
    strPw   = pw;
}

void MainWindow::getauthreply(){
    if(reply->error()==QNetworkReply::NoError){
        QString username;
        if(cookies->checkUser(username) && username==strUser){
            labelCurrentStatus->setText(QString("Successfully logged in as %1.").arg(strUser));
			ui->actionLogout->setEnabled(true);
			ui->actionLogin->setEnabled(false);
		}else{
            labelCurrentStatus->setText("Unlogged.");
		}
    }else{
        labelCurrentStatus->setText("Unlogged.");
	}
    disconnect(reply,SIGNAL(finished()),this,SLOT(getauthreply()));
}

void MainWindow::getlogoutreply(){
    if(reply->error()==QNetworkReply::NoError){
        ui->actionLogout->setEnabled(false);
        ui->actionLogin->setEnabled(true);
        labelCurrentStatus->setText("Unlogged.");
    }
    disconnect(reply,SIGNAL(finished()),this,SLOT(getlogoutreply()));
}

void MainWindow::pusharticle(){
    QString title = ui->lineTitle->text();
    QString content = ui->textContent->toPlainText();

    if(title.isEmpty() || content.isEmpty()){
        QMessageBox(QMessageBox::Warning, "BlogClient", "Neither the content nor the title can be empty.",
                    QMessageBox::Ok).exec();
        if (title.isEmpty()) ui->lineTitle->setFocus();
        else if (content.isEmpty()) ui->textContent->setFocus();
        return;
    }

    QString username;
    if(!cookies->checkUser(username)){
        login();
        return;
    }else if(username!=strUser){
        QMessageBox(QMessageBox::Warning, "BlogClient", "There is a conflict between cookie and program.please debug.",
                    QMessageBox::Ok).exec();
        return;
    }

    QNetworkRequest request(QUrl(QString("http://ikewikipage.appspot.com/_edit/%1").arg(title)));
    QByteArray postdata("content=");
    postdata.append(content);
    reply = netManager->post(request, postdata);
    connect(reply, SIGNAL(finished()), this, SLOT(pusharticlereply()));
}

void MainWindow::pusharticlereply(){
    if(reply->error()==QNetworkReply::NoError){
        QMessageBox(QMessageBox::Warning, "BlogClient", "Post succeed. Please check the log.",
                    QMessageBox::Ok).exec();
    }
    disconnect(reply,SIGNAL(finished()),this,SLOT(pusharticlereply()));
}

void MainWindow::updatelist(){
    if(reply->error()==QNetworkReply::NoError){
        ui->listWidget->clear();
        articlelist.clear();
        QXmlStreamReader xml(reply);
        if(xml.readNextStartElement() && xml.name()=="articles"){
            while(xml.readNextStartElement() && xml.name()=="article"){
                Article record;
                while(xml.readNextStartElement()){
                    if(xml.name()=="author"){
                        record.author = xml.readElementText();
                    }else if(xml.name()=="date"){
                        record.date = xml.readElementText();
                    }else if(xml.name()=="title"){
                        QString t = xml.readElementText();
                        ui->listWidget->addItem(t);
                        record.title = t;
                    }else if(xml.name()=="content"){
                        record.content = xml.readElementText();
                    }
                }
                articlelist.push_back(record);
            }
        }
        ui->listWidget->setCurrentRow(0);
        ui->lineTitle->setText(articlelist.at(0).title);
        ui->textContent->setPlainText(articlelist.at(0).content);
        ui->statusBar->showMessage("Sync success!");
    }else{
        ui->statusBar->showMessage("Update failed!");
    }
    disconnect(reply,SIGNAL(finished()),this,SLOT(updatelist()));
}

void MainWindow::showarticle(){
    int index = ui->listWidget->currentRow();
    ui->lineTitle->setText(articlelist.at(index).title);
    ui->textContent->setPlainText(articlelist.at(index).content);
}

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include <QLabel>
#include <QVector>
#include <QListWidget>
#include "article.h"
#include "login2.h"
#include "cookiejar.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


signals:


private slots:
    void login();
    void logout();
    void sync();
    void getauth(QString user, QString pw);
    void getauthreply();
    void getlogoutreply();
    void pusharticle();
    void pusharticlereply();
    void updatelist();
    void showarticle();

private:


    Ui::MainWindow *ui;
    QNetworkAccessManager *netManager;
    QNetworkReply *reply;
    CookieJar *cookies;
    Login2 *dlgLogin;
    QLabel *labelCurrentStatus;


    QString strUser;
    QString strPw;
    QVector<Article> articlelist;
};

#endif // MAINWINDOW_H

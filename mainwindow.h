#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include <QLabel>
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
    void getauth(QString user, QString pw);
    void getauthreply();
    void getlogoutreply();
    void pusharticle();
    void pusharticlereply();

private:


    Ui::MainWindow *ui;
    QNetworkAccessManager *netManager;
    QNetworkReply *reply;
    CookieJar *cookies;
    Login2 *dlgLogin;
    QLabel *labelCurrentStatus;

    QString strUser;
    QString strPw;
};

#endif // MAINWINDOW_H

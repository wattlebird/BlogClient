#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include "login2.h"

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

private:


    Ui::MainWindow *ui;
    QNetworkAccessManager *netManager;
    QNetworkReply *reply;
    QNetworkCookieJar *cookies;
    Login2 *dlgLogin;

    QString strUser;
    QString strPw;
};

#endif // MAINWINDOW_H

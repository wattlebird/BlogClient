#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QNetworkAccessManager>

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


private slots:
    void login();
    void logout();
    void getauth(QString user, QString pw);

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager netManager;
    Login2* dlgLogin;

public:
    QString strUser;
    QString strPw;
};

#endif // MAINWINDOW_H

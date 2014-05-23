#ifndef COOKIEJAR_H
#define COOKIEJAR_H

#include <QNetworkCookieJar>
#include <QString>

namespace Ui {
class CookieJar;
}

class CookieJar : public QNetworkCookieJar
{
    Q_OBJECT
public:
    explicit CookieJar(QObject *parent = 0);
    bool checkUser(QString&);

signals:

public slots:

private:

};

#endif // COOKIEJAR_H

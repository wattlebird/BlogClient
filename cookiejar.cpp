#include "cookiejar.h"
#include <QNetworkCookie>

CookieJar::CookieJar(QObject *parent) :
    QNetworkCookieJar(parent)
{
}

bool CookieJar::checkUser(QString & str){
    bool bUser = false;
    QNetworkCookie cookie;
    QList<QNetworkCookie> cookieslist = this->cookiesForUrl(QUrl("http://ikewikipage.appspot.com/login"));
    foreach(cookie, cookieslist){
        if(cookie.name()=="user"){
            QList<QByteArray> list = cookie.value().split('|');
            str = list.at(0);
            bUser = true;
            break;
        }
    }
    return bUser;
}

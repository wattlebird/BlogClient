#ifndef ARTICLE_H
#define ARTICLE_H

#include <QString>
#include <QDataStream>

class Article
{
public:
    Article();

    QString author;
    QString title;
    QString date;
    QString content;
    bool modified;
};

QDataStream& operator<<(QDataStream& out, const Article& entity);
QDataStream& operator>>(QDataStream& in, Article& entity);

#endif // ARTICLE_H

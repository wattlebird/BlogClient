#ifndef ARTICLE_H
#define ARTICLE_H

#include <QString>

class Article
{
public:
    Article();

    QString author;
    QString title;
    QString date;
    QString content;
};

#endif // ARTICLE_H

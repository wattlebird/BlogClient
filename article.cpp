#include "article.h"

Article::Article(){
}

QDataStream& operator<<(QDataStream& out, const Article& entity){
    return out<<entity.author<<entity.date<<entity.title<<entity.content<<entity.modified;
}

QDataStream& operator>>(QDataStream& in, Article& entity){
    QString author, date, title, content;
    bool modified;
    in>>author>>date>>title>>content>>modified;
    entity.author = author;
    entity.date = date;
    entity.title = title;
    entity.content = content;
    entity.modified = modified;
    return in;
}

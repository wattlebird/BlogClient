import webapp2
import signup
import login
import head
import secure
import database
import xmlgenerator as xg
from google.appengine.ext import ndb

def article_key(author):
    return ndb.Key('EditHandler', author)

class MainHandler(head.BasicHandler):
    def get(self, title):
        cookie_val = self.request.cookies.get('user')
        if not cookie_val or not secure.check_secure_val(cookie_val):
            user = None
            username=None
        else:
            username = cookie_val.split('|')[0]
            q = database.Users.query(database.Users.user == username)
            user = q.get()

        q = database.Post.query(database.Post.title == title)

        if q.count()!=0:
            item = q.get();
            self.render("article.html", user=username, content=item.body, title=title)
        else:
            if user:
                self.redirect('/_edit/%s'%title)
            else:
                self.redirect('/login')#A message could be added, saying "you must login first to create the page"

class EditHandler(head.BasicHandler):

    def get(self, title):
	cookie_val = self.request.cookies.get('user')
        
        if not(cookie_val and secure.check_secure_val(cookie_val)):
            user = None
            username=None
        else:
            username = cookie_val.split('|')[0]
            q = database.Users.query(database.Users.user == username)
            user = q.get()

        if not user:
            self.redirect('/login')

        q = database.Post.query(database.Post.title == title)
        if q.count() != 0:
            e = q.get()
            self.render("edit.html", user = username, content = e.body, title = e.title)
        else:
            self.render("edit.html", user = username, content = '', title = title)

    def post(self, title):
        cookie_val = self.request.cookies.get('user')
        
        if not(cookie_val and secure.check_secure_val(cookie_val)):
            user = None
            username = None
        else:
            username = cookie_val.split('|')[0]
            q = database.Users.query(database.Users.user == username)
            user = q.get()

        if not user:
            self.redirect('/login')

        body = self.request.get('content')
        
        q = database.Post.query(database.Post.title == title)
        e = q.get()
        if e:
            e.body = body
            e.author = username
        else:
            e_key = ndb.Key(database.Post,title,parent=article_key(username))
            e = database.Post(key = e_key, title = title, body = body, author = username)
        result = e.put_async()
        result.wait()
        self.redirect('/%s'%title)

class UserPosts(head.BasicHandler):
    def get(self):
        user = self.request.get('user')
        self.response.headers['Content-Type']='text/xml'
        key_search = article_key(user)
        q = database.Post.query(ancestor=key_search)
        sheet = xg.xmlgenerator()
        for item in q:
            sheet.add(author=item.author,
                      date=item.created.strftime('%X %Y/%m/%d'),
                      title=item.title,
                      content=item.body)
        self.write(sheet.out())

app = webapp2.WSGIApplication([
    (r'/api',UserPosts),
    (r'/_edit/([A-Za-z0-9_]*)',EditHandler),
    (r'/([A-Za-z0-9_]*)', MainHandler)
    
], debug=True)

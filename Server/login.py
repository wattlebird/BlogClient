import head
import validcheck
import secure
import database
import webapp2
import urlparse


class LoginHandler(head.BasicHandler):
	def get(self):
		cookie_val = self.request.cookies.get('user')
                if cookie_val and secure.check_secure_val(cookie_val):
                        self.redirect(self.request.referer)
                else:
                        self.response.delete_cookie('user')
		self.render("login.html", prev=self.request.referer)

	def post(self):
		have_error = False
		username = self.request.get('username')
		password = self.request.get('password')
                previouspage = str(self.request.get('prev'))

                hashvalue = None
		if not validcheck.valid_username(username):
                	have_error = True
                else:
                        q = database.Users.query(database.Users.user == username)
                        if not q.get():
                                have_error = True
                        else:
                                hashvalue = q.get().hashed_password

                if not validcheck.valid_password(password) or not hashvalue:
                        have_error = True
                elif not secure.check_password(username, password, hashvalue):
                        have_error = True


                if have_error:
                        self.render('login.html', error = 'Login failed!', prev=previouspage)
                else:
                        cookie_val = secure.make_secure_val(username)
                        self.response.set_cookie('user', cookie_val)
                        subpath = urlparse.urlsplit(previouspage)
                        if subpath.path=='/signup' or subpath.path=='/login':
                                self.redirect('/')
                        else:
                                self.redirect(previouspage)


class LogoutHandler(head.BasicHandler):
        def get(self):
                self.response.delete_cookie('user')
                self.redirect(self.request.referer)

app = webapp2.WSGIApplication([
        ('/login',LoginHandler),
        ('/logout', LogoutHandler)
], debug=True)

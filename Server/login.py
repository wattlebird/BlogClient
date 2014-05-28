import head
import validcheck
import secure
import database
import webapp2

class LoginHandler(head.BasicHandler):
	def get(self):
		cookie_val = self.request.cookies.get('user')
                if cookie_val and secure.check_secure_val(cookie_val):
                        webapp2.redirect(self.request.referer)
                else:
                        self.response.delete_cookie('user')
		self.render("login-form.html")

	def post(self):
		have_error = False
		username = self.request.get('username')
		password = self.request.get('password')

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
                        self.render('login-form.html', error = 'Login failed!')
                else:
                        cookie_val = secure.make_secure_val(username)
                        self.response.set_cookie('user', cookie_val)
                        if secure.check_secure_val(cookie_page):
                                webapp2.redirect(self.request.referer)


class LogoutHandler(head.BasicHandler):
        def get(self):
                self.response.delete_cookie('user')
                webapp2.redirect(self.request.referer)

app = webapp2.WSGIApplication([
        ('/login',LoginHandler),
        ('/logout', LogoutHandler)
], debug=True)

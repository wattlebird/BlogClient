import head
import validcheck
import secure
import database
import webapp2
import urlparse

class SignupHandler(head.BasicHandler):
        def get(self):
                subpath = urlparse.urlsplit(self.request.referer)

                cookie_val = self.request.cookies.get('user')
                if cookie_val:
                        if secure.check_secure_val(cookie_val):
                                if subpath.path=='/signup':
                                        self.redirect('/')
                                else:
                                        self.redirect(self.request.referer)
                        else:
                                self.response.delete_cookie('user')

                self.render("signup.html", prev = self.request.referer)

	
        def post(self):
        	have_error = False
        	username = self.request.get('username')
                password = self.request.get('password')
                verify = self.request.get('verify')
                email = self.request.get('email')
                previouspage = str(self.request.get('prev'))

                param = dict(username = username,
                	email = email)

                if not validcheck.valid_username(username):
                	have_error = True
                	param['error_username'] = 'Invalid Username'
                else:
                	q = database.Users.query(database.Users.user == username)
                	if q.get():
                		have_error = True
                		param['error_username'] = 'User already exists'

                if not validcheck.valid_password(password):
                	have_error = True
                	param['error_password'] = 'Invalid Password'
                else:
                	if password != verify:
                                have_error = True
                                param['error_verify']= 'Passwords didn\'t match.'

                if not validcheck.valid_email(email):
                	have_error = True
                	param['error_email'] = 'Invalid Email'

                if have_error:
                	self.render("signup.html", **param)
                else:
                	if email:
                		kk = database.Users(user = username, hashed_password = secure.hashed_password(username, password),
                			email = email)	
                	else:
                		kk = database.Users(user = username, hashed_password = secure.hashed_password(username, password))
                	kk.put()
                	cookie_val = secure.make_secure_val(username)
                	self.response.set_cookie('user', cookie_val)
                        subpath = urlparse.urlsplit(previouspage)
                        if subpath.path=='/signup' or subpath.path=='/login':
                                self.redirect('/')
                        else:
                                self.redirect(previouspage)

app = webapp2.WSGIApplication([
        ('/signup',SignupHandler)
], debug=True)

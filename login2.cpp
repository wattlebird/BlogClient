#include "login2.h"
#include "ui_login2.h"

Login2::Login2(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login2)
{
    ui->setupUi(this);
	ui->password->setEchoMode(QLineEdit::Password);
	ui->btn_OK->setEnabled(false);
}

Login2::~Login2()
{
    delete ui;
}

void Login2::validinput(){
    if(!(ui->username->text().isEmpty() || ui->password->text().isEmpty()))
        ui->btn_OK->setEnabled(true);
}

void Login2::accept(){
    emit sendauth(ui->username->text(), ui->password->text());

    QDialog::accept();
}

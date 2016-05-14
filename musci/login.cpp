#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    resize(300,200);
}

Login::~Login()
{
    delete ui;
}


void Login::on_pushButton_3_clicked()
{
    exit(0);
}

void Login::on_pushButton_4_clicked()
{
   this->setWindowFlags(windowFlags()|Qt::WindowMinimizeButtonHint);
}

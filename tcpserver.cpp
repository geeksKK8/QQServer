#include "tcpserver.h"
#include "ui_tcpserver.h"

Tcpserver::Tcpserver(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Tcpserver)
{
    ui->setupUi(this);
    //port = 1234;
}

Tcpserver::~Tcpserver()
{
    delete ui;
}

void Tcpserver::on_pushButton_clicked()
{
    server  = new Server(this, ui->lineEdit->text().toInt());
    connect(server, &Server::updateserver, this, &Tcpserver::slotupdateserver);
    ui->pushButton->setEnabled(false);
}

void Tcpserver::slotupdateserver(QString msg, int length)
{
    ui->textEdit->append(msg);
}



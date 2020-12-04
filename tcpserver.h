#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QWidget>
#include "server.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Tcpserver; }
QT_END_NAMESPACE

class Tcpserver : public QWidget
{
    Q_OBJECT

public:
    Tcpserver(QWidget *parent = nullptr);
    ~Tcpserver();

private:
    Ui::Tcpserver *ui;
    int port;
    Server *server;
protected slots:
    void slotupdateserver(QString, int);//接收到server发过来的信号就更新界面的信息


private slots:
    void on_pushButton_clicked();

};
#endif // TCPSERVER_H

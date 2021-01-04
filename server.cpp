#include "server.h"
#include <QHostAddress>

Server::Server(QObject *parent, int port):QTcpServer(parent)
{
    //listen(QHostAddress::Any, port); //监听
    qDebug()<<port;
    if(!listen(QHostAddress::Any,port))
    {
        qDebug()<<"监听失败";
    }
    else
    {
        qDebug()<<"监听成功";
    }
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    qDebug()<<"call incomingconnection()";
    TcpClientSocket *tcpclientsocket = new TcpClientSocket(this);//只要有新的连接就生成一个新的通信套接字
    //将新创建的通信套接字描述符指定为参数socketdescriptor
    tcpclientsocket->setSocketDescriptor(socketDescriptor);
    qDebug()<<"descriptor:"<<socketDescriptor;
    //将这个套接字加入客户端套接字列表中
    tcpclientsocketlist.append(tcpclientsocket);



    //接收到tcpclientsocket发送过来的更新界面的信号
    connect(tcpclientsocket, &TcpClientSocket::updateserver, this, &Server::sliotupdateserver);
    connect(tcpclientsocket, &TcpClientSocket::clientdisconnected, this, &Server::slotclientdisconnect);

}

void Server::sliotupdateserver(QString msg, int length)
{
    //将这个信号发送给界面
    emit updateserver(msg, length);
    //将收到的信息发送给每个客户端,从套接字列表中找到需要接收的套接字

    if(msg[0]=="2"){
        qDebug()<<"to one";
        QString remain = msg.mid(1);
        QString ptr = remain.split(":")[0];
        for(int i = 0; i < tcpclientsocketlist.count(); i++)
        {

            QTcpSocket *item = tcpclientsocketlist.at(i);
            QString name = usernamelist.at(i);
    //        if(item->write((char*)msg.toUtf8().data(), length) != length)
    //        {
    //            continue;
    //        }
            if(name == ptr)
                item->write(msg.toUtf8().data());
        }
    }
    if(msg[0]=="0"){
        QString remain = msg.mid(1);
        QString ptr = remain.split(":")[0];
        usernamelist.append(ptr);

        for(int i=0;i<tcpclientsocketlist.count();i++)
        {
            QString name = usernamelist.at(i);
            qDebug()<<usernamelist.at(i);
            msg+="_"+name;
        }

        for(int i = 0; i < tcpclientsocketlist.count(); i++)
        {
            QTcpSocket *item = tcpclientsocketlist.at(i);
            item->write(msg.toUtf8().data());
        }
    }
    if(msg[0] == "1"){
        qDebug()<<"to all";
    for(int i = 0; i < tcpclientsocketlist.count(); i++)
    {
        QTcpSocket *item = tcpclientsocketlist.at(i);
        item->write(msg.toUtf8().data());
    }
    }

}

void Server::slotclientdisconnect(int descriptor)
{
    for(int i = 0; i < tcpclientsocketlist.count(); i++)
    {
        QTcpSocket *item = tcpclientsocketlist.at(i);
        if(item->socketDescriptor() == descriptor)
        {
            qDebug()<<"delete socket"<<i;
            tcpclientsocketlist.removeAt(i);//如果有客户端断开连接， 就将列表中的套接字删除
            usernamelist.removeAt(i);
            return;
        }
    }
    return;
}

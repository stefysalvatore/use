#include "mythread.h"
#include <QTextCodec>
#include <QSqlDatabase>

MyThread::MyThread(qintptr ID, QObject *parent) :
    QThread(parent)
{
    this->socketDescriptor = ID;
}

void MyThread::run()
{
    // thread starts here
    qDebug() << " Thread started";

    socket = new QTcpSocket();

    // set the ID
    if(!socket->setSocketDescriptor(this->socketDescriptor))
    {
        // something's wrong, we just emit a signal
        emit error(socket->error());
        return;
    }

    // connect socket and signal
    // note - Qt::DirectConnection is used because it's multithreaded
    //        This makes the slot to be invoked immediately, when the signal is emitted.

    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));

    // We'll have multiple clients, we want to know which is which
    qDebug() << socketDescriptor << " Client connected";

    // make this thread a loop,
    // thread will stay alive so that signal/slot to function properly
    // not dropped out in the middle when thread dies

    exec();
}

void MyThread::readyRead()
{
    // get the information
    QByteArray Data = socket->readAll();

    // will write on server side window
    qDebug() << socketDescriptor << " Data in: " << Data;
    QString command(Data);

    QStringList pieces = command.split("|");
    QString responseString;

    if(pieces[1]=="sa"){

        QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
        db.setHostName("148.251.87.245");
        db.setPort(5432);
        db.setDatabaseName("gestionale_ordine");
        db.setUserName("egov");
        db.setPassword("eg0v");
        bool ok = db.open();

        qDebug() << ok;
        qDebug() << "Has a valid QPSQL driver: " << (db.isValid() ? "YES" : "NO");

        responseString = "ok";

    }else{
        responseString = "ciao";
    }

    QByteArray response;
    response.append(responseString);

    socket->write(response);
}

void MyThread::disconnected()
{
    qDebug() << socketDescriptor << " Disconnected";


    socket->deleteLater();
    exit(0);
}

#include "com.h"

#include <QByteArray>

Com::Com(QObject *parent) : QObject(parent), connection(0)
{
}

void Com::connecting(const QString &host, const QString &username, const QString &passwd)
{
QSsh::SshConnectionParameters parameters;
parameters.host = host;
parameters.userName = username;
parameters.password = passwd;
parameters.timeout = 20;
parameters.authenticationType = QSsh::SshConnectionParameters::AuthenticationTypePassword;
parameters.port = 22;
connection = new QSsh::SshConnection(parameters, this);

connect(connection, SIGNAL(connected()), SLOT(connected()));
connect(connection, SIGNAL(error(QSsh::SshError)), SLOT(onConnectionError(QSsh::SshError)));

qDebug().nospace() << "COM: Connecting to host " << qPrintable(host) <<"...";

connection->connectToHost();
}

void Com::onConnectionError(QSsh::SshError)
{
qDebug() << "Com: Connection error" << connection->errorString();
}

void Com::connected()
{
qDebug() << "COM: CONNECTED!";
const QByteArray comman("ls");
remoteProc = connection->createRemoteProcess(comman);

if(remoteProc){
    connect(remoteProc.data(), SIGNAL(started()), SLOT(onChannelStarted()));
    connect(remoteProc.data(), SIGNAL(readyReadStandardOutput()), SLOT(readyReadStandardOutput()));

    remoteProc->start();
}
}

void Com::onConnectionError(QSsh::SshError)
{
qDebug() << "Com: Connection error" << connection->errorString();
}

void Com::onChannelStarted(){
qDebug() << "COM: Channel Started";
}

void Com::readyReadStandardOutput()
{
qDebug() << "OUTPUT:" << remoteProc->readAll();

}@

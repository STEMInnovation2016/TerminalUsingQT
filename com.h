#ifndef COM_H
#define COM_H

#include <QObject>

#include "sshremoteprocess.h"
#include "sshconnection.h"

class Com : public QObject
{
Q_OBJECT
public:
explicit Com(QObject *parent = 0);
void connecting(const QString &host, const QString &username, const QString &passwd);

signals:

public slots:

private slots:
private slots:
void connected();
void onConnectionError(QSsh::SshError);
void onChannelStarted();
void readyReadStandardOutput();
private:
QSsh::SshRemoteProcess::Ptr remoteProc;
QSsh::SshConnection *connection;
};

#endif // COM_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QAbstractSocket>

class QPushButton;
class QSplitter;
class QLabel;

QT_BEGIN_NAMESPACE
class QTcpSocket;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QLabel *label;
    QPushButton *leftButton;
    QPushButton *rightButton;
    QSplitter *splitter;

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void connectFLIR();
    void connectVisual();
};

class Client : public QObject
{
    Q_OBJECT
public:
    Client(QString ipAddress, int port, QObject *parent=0);

private slots:
    void readSlot();
    void displayErrorSlot(QAbstractSocket::SocketError);

private:
    QTcpSocket          *mTcpSocket;
    QString             mIpAddress;
    int                 mPort;
};

#endif // MAINWINDOW_H

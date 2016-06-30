#include "mainwindow.h"
#include <stdlib.h>

#include <QGridLayout>
#include <QPushButton>
#include <QDebug>
#include <QLabel>
#include <QSplitter>
#include <QProcess>

#include <QtNetwork>
#include <QTcpSocket>

#include <iostream>
#include <QTcpSocket>
#include <QSettings>
#include <QDateTime>

#include <QPixmap>
#include <QImage>

QLabel *imageLabel;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *centralWidget = new QWidget();
    setCentralWidget(centralWidget);

    QGridLayout *layout = new QGridLayout();
    centralWidget->setLayout(layout);

    imageLabel = new QLabel("[Image goes here]");
    layout->addWidget(imageLabel, 0, 0, Qt::AlignCenter);

    splitter = new QSplitter(Qt::Horizontal, this);
    layout->addWidget(splitter, 2, 0);
    splitter->show();

    leftButton = new QPushButton("Connect FLIR");
    splitter->addWidget(leftButton);

    rightButton = new QPushButton("Connect Visual");
    splitter->addWidget(rightButton);

    connect(leftButton, SIGNAL(clicked(bool)), this, SLOT(connectFLIR()));

    connect(rightButton, SIGNAL(clicked(bool)), this, SLOT(connectVisual()));
}

void MainWindow::connectVisual() {
    qDebug()<<"gstreamer visual";

    QProcess * process = new QProcess();
    QStringList parameters;
    parameters << "-v" << "tcpclientsrc" << "host=192.168.2.122" << "port=5000" << "!" << "gdpdepay" << "!" << "rtph264depay" << "!" << "avdec_h264" << "!" << "videoconvert" << "!" << "autovideosink" << "sync=false";
    process->start("/usr/local/bin/gst-launch-1.0", parameters);
}

void MainWindow::connectFLIR(){
    qDebug()<<"FLIR";
    Client* client = new Client("192.168.2.122", 8080);
}

//code taken from: http://stackoverflow.com/questions/11081005/streaming-images-over-qtcpsocket-on-windows-7

Client::Client(QString ipAddress, int port, QObject *parent):
    QObject(parent), mTcpSocket(0), mIpAddress(ipAddress), mPort(port)
{
    mTcpSocket = new QTcpSocket(this);
    connect(mTcpSocket, SIGNAL(readyRead()),
            this, SLOT(readSlot()));
    connect(mTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(displayErrorSlot(QAbstractSocket::SocketError)));

    std::cout << "Connecting to ip: " << mIpAddress.toStdString() << " port: " << mPort << std::endl;
    mTcpSocket->connectToHost(mIpAddress, mPort);
}

void Client::readSlot()
{
    static qint64 starttime = QDateTime::currentMSecsSinceEpoch();
    static int frames = 0;

    //know that the image is this big
    qint64 blockSize = 60*80*2; //in bytes

    if (mTcpSocket->bytesAvailable() < blockSize)
    {
        return;
    }
    frames++;

    QByteArray read = mTcpSocket->readAll();
    unsigned short *data = (unsigned short *)(read.data());

    QImage image(80, 60, QImage::Format_Grayscale8);
    int r, c;
    int minValue = 0xffff, maxValue = 0;
    for (int i = 0; i < 80*60; ++i) {
        if (data[i] > maxValue) maxValue = data[i];
        if (data[i] < minValue) minValue = data[i];
    }
    for (r = 0; r < 60; ++r) {
        uchar *row = image.scanLine(r);
        for (c = 0; c < 80; ++c)
            row[c] = (uchar) ((data[80*r+c] - minValue)*256 / (maxValue-minValue+1));
    }

    imageLabel->setPixmap(QPixmap::fromImage(image.scaled(240, 180)));
}

void Client::displayErrorSlot(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        std::cout << "The host was not found. Please check the "
                                    "host name and port settings."<< std::endl;
        break;
    case QAbstractSocket::ConnectionRefusedError:
        std::cout << "The connection was refused by the peer. "
                                    "Make sure the fortune server is running, "
                                    "and check that the host name and port "
                                    "settings are correct."<< std::endl;
        break;
    default:
        std::cout << "The following error occurred: " << mTcpSocket->errorString().toStdString() << std::endl;
        break;
    }
}

MainWindow::~MainWindow()
{

}

#include "mainwindow.h"
#include <stdlib.h>

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QDebug>
#include <QProcess>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *centralWidget = new QWidget();
    setCentralWidget(centralWidget);

    QGridLayout *layout = new QGridLayout();
    centralWidget->setLayout(layout);

    label = new QLabel("Start Streaming");
    layout->addWidget(label, 0, 0, Qt::AlignCenter);

    button = new QPushButton("Go");
    layout->addWidget(button, 1, 0, Qt::AlignCenter);

    connect(button, SIGNAL(clicked(bool)), this, SLOT(go()));
}

void MainWindow::go() {
    button->setEnabled(false);
    QProcess * process = new QProcess();
    QStringList parameters;
    parameters << "-v" << "tcpclientsrc" << "host=192.168.2.122" << "port=5000" << "!" << "gdpdepay" << "!" << "rtph264depay" << "!" << "avdec_h264" << "!" << "videoconvert" << "!" << "autovideosink" << "sync=false";
    process->start("/usr/local/bin/gst-launch-1.0", parameters);
}

MainWindow::~MainWindow()
{

}

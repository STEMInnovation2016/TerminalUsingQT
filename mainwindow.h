#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QLabel;
class QPushButton;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QLabel *label;
    QPushButton *button;

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void go();

};

#endif // MAINWINDOW_H

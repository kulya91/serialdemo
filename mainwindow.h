#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QSerialPort>
#include <qserialportinfo.h>
#include <QDebug>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void init(QString port,int baudrate);
    void updateport();
private slots:
    void on_send_clicked();
    void receive();
    void on_connect_clicked();

    void on_clear_clicked();

private:
    Ui::MainWindow *ui;
    QSerialPort *mserial;
};
#endif // MAINWINDOW_H

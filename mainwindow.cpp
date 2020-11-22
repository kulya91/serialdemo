#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mserial=new QSerialPort();
    connect(mserial, SIGNAL(readyRead()), this, SLOT(receive()));
    updateport();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete mserial;
}

void MainWindow::init(QString port,int baudrate){
    mserial->setPortName(port);
    //设置波特率
    mserial->setBaudRate(baudrate);
    //设置数据位数
    mserial->setDataBits(QSerialPort::Data8);
    //设置奇偶校验
    mserial->setParity(QSerialPort::NoParity);
    //设置停止位
    mserial->setStopBits(QSerialPort::OneStop);
    //设置流控制
    mserial->setFlowControl(QSerialPort::NoFlowControl);
}
void MainWindow::updateport(){
    ui->portbox->clear();
    //通过QSerialPortInfo查找可用串口
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        ui->portbox->addItem(info.portName());
    }
}
void MainWindow::receive(){
    QByteArray arr= mserial->readAll();
   // QString res = QString(arr);
    if(ui->hexreceive->isChecked()){
        QDataStream out(&arr,QIODevice::ReadWrite);    //将字节数组读入
        while(!out.atEnd())
        {
            qint8 outChar = 0;
            out>>outChar;   //每字节填充一次，直到结束
            //十六进制的转换
            QString str = QString("%1").arg(outChar&0xFF,2,16,QLatin1Char('0'));
            ui->receivewindows->insertPlainText(" ");//每发送两个字符后添加一个空格
            ui->receivewindows->insertPlainText(str.toUpper());//大写
            ui->receivewindows->moveCursor(QTextCursor::End);
        }
    }else{
        QString str = QString::fromLocal8Bit(arr);
        qDebug()<<str;
        ui->receivewindows->insertPlainText(str);
        ui->receivewindows->moveCursor(QTextCursor::End);
    }



}
void MainWindow::on_send_clicked()
{
    QByteArray str= ui->sendwindows->toPlainText().toLocal8Bit();
    if(ui->hexsend->isChecked()){
        mserial->write(QByteArray::fromHex(str));
    }else{
        mserial->write(str);
    }
}

void MainWindow::on_connect_clicked()
{
    static bool status=false;
    int a=ui->bits->currentText().toInt();
    init(ui->portbox->currentText(),a);
    //打开串口
    if(!status){
        if(mserial->open(QIODevice::ReadWrite)){
            ui->connect->setText("关闭串口");
            ui->portbox->setEnabled(false);
            ui->bits->setEnabled(false);
            status=true;
        }
        else{
            status=false;
        }
    }
    else{
        mserial->close();
        ui->connect->setText("连接");
        status=false;
        ui->portbox->setEnabled(true);
        ui->bits->setEnabled(true);
    }
}

void MainWindow::on_clear_clicked()
{
    ui->receivewindows->clear();
}

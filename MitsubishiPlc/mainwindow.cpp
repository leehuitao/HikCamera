#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();

    sendData();

    readData();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    serial = new QSerialPort;
    serial->setPortName("COM4");

    if(serial->open(QIODevice::ReadWrite)){
        qDebug()<<"open success COM4";
    }else{
        qDebug()<<"FK";
    }
    serial->setBaudRate(9600,QSerialPort::AllDirections);
    serial->setDataBits(QSerialPort::Data7);
    serial->setParity(QSerialPort::EvenParity);
    serial->setStopBits(QSerialPort::OneStop);
    connect(serial,&QSerialPort::readyRead,this,[=]{
        auto data = serial->readAll();
        qDebug()<<"readyread data = "<<data;
    });

}
QByteArray LRC(QString source){
    QStringList list;
    for(int i = 0 ; i < source.size() ; i+=2){
        list.append(QString("%1%2").arg(source.at(i)).arg(source.at(i+1)));
    }
    int lrc = 0;
    for(int i = 0 ; i < list.size() ; i++){
        lrc += list[i].toLatin1().toInt(nullptr,16);
    }
    qDebug()<<"lrc = "<<lrc;
    QString hex = QString::number(lrc,16).toUpper();
    QByteArray res = "02";
    res += source.toLocal8Bit().data();
    for(int i = (hex.size() - 2) ; i < hex.size() ; i++){
        res += QString(hex.at(i)).toLatin1().toHex().toUpper();
    }
    qDebug()<<__FUNCTION__<<res;
    return res;
}
//以下函数实现将16进制的字节转换为十进制的数
char ConvertHexChar(char ch)
{
    if((ch >= '0') && (ch <= '9'))
    {
      return ch-0x30;
    }
    else if((ch >= 'A') && (ch <= 'F'))
    {
      return ch-'A'+10;
    }
    else if((ch >= 'a') && (ch <= 'f'))
    {
      return ch-'a'+10;
    }
    else return (-1);
}

QByteArray QStringToHex(QString str)
{
    QByteArray senddata;
    int hexdata,lowhexdata;
    int hexdatalen = 0;
    int len = str.length();
    senddata.resize(len/2);
    char lstr,hstr;
    for(int i=0; i<len; )
    {
        hstr=str[i].toLatin1();
        if(hstr == ' ')
        {
        i++;
        continue;
        }
        i++;
        if(i >= len)
        {
          break;
        }
        lstr = str[i].toLatin1();
        hexdata = ConvertHexChar(hstr);
        lowhexdata = ConvertHexChar(lstr);
        if((hexdata == 16) || (lowhexdata == 16))
        {
           break;
        }
        else
        hexdata = hexdata*16+lowhexdata;
        i++;
        senddata[hexdatalen] = (char)hexdata;
        hexdatalen++;
    }
    senddata.resize(hexdatalen);
    return senddata;

}

void MainWindow::readData()
{
    QString res = LRC("3031303134303203"); //读取D100 寄存器的值 LRC奇偶和校验
    auto sendData = QStringToHex(res);//字符串转换为字节流
    serial->write(sendData);
    serial->waitForBytesWritten();
}

void MainWindow::sendData()
{
    QString res = LRC("313130313430324646464603"); //D100写入0xFFFF
    auto sendData = QStringToHex(res);
    serial->write(sendData);
    serial->waitForBytesWritten();
    serial->waitForReadyRead(20);
}


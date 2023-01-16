
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gvcp.h"
#include "udpclient.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    GvcpServer();
    soc = new QUdpSocket(this);
    //auto status = soc->bind(QHostAddress("172.16.10.20"),54199);
    //qDebug()<<"listen status = "<<status;
    //connect(soc,&QUdpSocket::readyRead,this,&MainWindow::readyRead);
    m_totalData = new char[4096*4096];
    memset(m_totalData,0,4096*4096);
    m_dataCache = new char[4096*4096];
    m_imgCache  = new char[3072*2048];
    addr = QHostAddress("172.16.10.19");
    port = 3956;
    char ip[20] = {"172.16.10.20"};
    udp_init(ip,54199,1464);
}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_discover_btn_clicked()
{
    interfaceDiscover();
}

void MainWindow::on_read_ctr_pri_btn_clicked()
{
    interfaceReadCtrPri();
}

void MainWindow::on_set_ctr_pri_btn_clicked()
{
    interfaceSetCtrPri();
}

void MainWindow::on_set_data_channel_ip_btn_clicked()
{
    interfaceSetDataChannelIp();
}

void MainWindow::on_read_data_channel_port_btn_clicked()
{
    interfaceSetDataChannelPort();
}

void MainWindow::on_read_stream_channel_ip_btn_clicked()
{
    interfaceSetStreamChannelIp();
}

void MainWindow::on_read_stream_channel_port_btn_clicked()
{
    interfaceSetStreamChannelPort();
}

void MainWindow::on_read_stream_channel_size_btn_clicked()
{
    interfaceSetStreamChannelSize();
}

void MainWindow::on_capture_btn_clicked()
{
    interfaceCapture();
}

void MainWindow::readyRead()
{
    //    while (soc->hasPendingDatagrams()) {
    //        currentSize = soc->pendingDatagramSize();
    //        soc->readDatagram(m_dataCache+m_dataSize,soc->pendingDatagramSize(),&addr,&port);
    //        m_dataSize += currentSize;
    //    }
}

void MainWindow::on_read_conf_btn_clicked()
{
    interfaceReadReg2();
}

void MainWindow::on_read_stream_channel_size_btn_2_clicked()
{
    interfaceCapture1();
}

void MainWindow::on_auto_btn_clicked()
{
    m_dataSize = 0;
    currentSize= 0;
    isFrist= 1;
    isLast = 0;
    memset(m_dataCache,0,4096*4096);
    interfaceSetCtrPri();
    on_set_device_mode_btn_clicked();
    interfaceSetDataChannelIp();
    interfaceSetDataChannelPort();
    interfaceSetStreamChannelIp();
    interfaceSetStreamChannelPort();
    interfaceSetStreamChannelSize();
    interfaceCapture();
    interfaceCapture1();
    interfaceReadReg2();
    interfaceReadReg2();
    interfaceReadReg2();
    interfaceReadReg2();
    interfaceCapture();
    udp_rcv(m_totalData);
}

void MainWindow::parse()
{
    m_totalData = m_totalData+44;
    int packIndex;
    for (int i = 1;i < 4298;i++) {
        packIndex = (int)((unsigned char)m_totalData[(7+(i-1)*1472)]  | (unsigned char)m_totalData[(6+(i-1)*1472)] << 8 | (unsigned char)m_totalData[(5+(i-1)*1472)] << 16);
        qDebug()<<"packIndex:"<<packIndex;
        if(packIndex != 0){
            memcpy(m_imgCache+((packIndex-1)*1464),m_totalData+(i-1)*1472+8,1464);
        }
    }
    memcpy(m_imgCache+(4298*1464),m_totalData+(4298*1472)+8,648);
    QImage img((uchar *)m_imgCache,3072,2048,QImage::Format_Grayscale8);
//    img.save("D:/1.tif");
//    img.save("D:/1.JPG");
//    img.save("D:/1.png");
    ui->label->setPixmap(QPixmap::fromImage(img).scaledToWidth(500));
    memset(m_imgCache,0,3072*2048);
}

void MainWindow::on_parse_btn_clicked()
{
    parse();
}

void MainWindow::on_set_device_mode_btn_clicked()
{
    auto mode = ui->comboBox->currentIndex();
    if(mode == 0 )
        qDebug()<<"单次触发";
    else
        qDebug()<<"连续触发";
    interfaceSetDeviceMode(mode);
}

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>
#include <QTimer>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_discover_btn_clicked();

    void on_read_ctr_pri_btn_clicked();

    void on_set_ctr_pri_btn_clicked();

    void on_set_data_channel_ip_btn_clicked();

    void on_read_data_channel_port_btn_clicked();

    void on_read_stream_channel_ip_btn_clicked();

    void on_read_stream_channel_port_btn_clicked();

    void on_read_stream_channel_size_btn_clicked();

    void on_capture_btn_clicked();

    void readyRead();

    void on_read_conf_btn_clicked();

    void on_read_stream_channel_size_btn_2_clicked();

    void on_auto_btn_clicked();

    void parse();

    void on_parse_btn_clicked();

    void on_set_device_mode_btn_clicked();

private:
    Ui::MainWindow *ui;
    QUdpSocket *soc;
    char * m_totalData;
    int     currentSize;
    char * m_imgTotalCache;
    char * m_dataCache;
    char * m_imgCache;
    int currentImgSize = 0;
    QHostAddress addr;
    quint16 port;
    int m_dataSize= 0;
    bool isFrist = 1;
    bool isLast = 0;
    QTimer m_time;
};
#endif // MAINWINDOW_H

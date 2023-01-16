#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFileDialog>
#include <QFuture>
#include <QtConcurrent>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
}

void MainWindow::init(){
    m_qrcode.loadHdvpEngine("./code.hdvp");
    m_char.loadHdvpEngine("./charlist.hdvp","./charlist.hdvp");
    m_highpass.loadHdvpEngine("./highpass.hdvp");
    m_shapModel.loadHdvpEngine("./scaledshapmodel.hdvp");
    m_xldFindPoints.loadHdvpEngine("./autofindpoints_xld.hdvp");
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_code_img_select_btn_clicked()
{
    //打开操作
    QString fileName = QFileDialog::getOpenFileName(
                this, tr("open file"));
    if(!fileName.isEmpty()){
        ui->code_img_line->setText(fileName);
    }
}

void MainWindow::on_code_detect_clicked()
{
    QString result;
    QImage resimg;
    QImage img(ui->code_img_line->text());
    m_qrcode.codeDetection(img,result,resimg);
    //320 * 240
    QPixmap pix = QPixmap::fromImage(resimg);
    if(pix.width() > 320 || pix.height() > 240){
        pix = pix.scaled(320, 240, Qt::KeepAspectRatio, Qt::SmoothTransformation);  // 按比例缩放
    }
    ui->img_lab->resize(pix.width(),pix.height());
    ui->img_lab->setPixmap(pix);
    if(!result.isEmpty()){
        ui->code_result_text->append(result);
    }
}

void MainWindow::on_char_img_select_btn_clicked()
{
    //打开操作
    QString fileName = QFileDialog::getOpenFileName(
                this, tr("open file"));
    if(!fileName.isEmpty()){
        ui->char_img_line->setText(fileName);
    }
}

void MainWindow::on_number_detect_clicked()
{
    QString result;
    QImage resimg;
    QImage img(ui->char_img_line->text());
    m_char.numberDetection(img,result,resimg);
    //320 * 240
    QPixmap pix = QPixmap::fromImage(resimg);
    if(pix.width() > 320 || pix.height() > 240){
        pix = pix.scaled(320, 240, Qt::KeepAspectRatio, Qt::SmoothTransformation);  // 按比例缩放
    }
    ui->char_img_lab->resize(pix.width(),pix.height());
    ui->char_img_lab->setPixmap(pix);
    if(!result.isEmpty()){
        ui->char_result_text->append(result);
    }
}

void MainWindow::on_english_detect_clicked()
{

}

void MainWindow::on_cod_page_btn_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_auto_find_page_btn_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_defect_analysis_page_btn_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_highpass_img_btn_clicked()
{
    //打开操作
    QString fileName = QFileDialog::getOpenFileName(
                this, tr("open file"));
    if(!fileName.isEmpty()){
        ui->highpass_img_line->setText(fileName);
    }
    ui->source_img_lab->setImage(fileName);
}

void MainWindow::on_highpass_detect_btn_clicked()
{
    auto rect = ui->source_img_lab->getRectMap();
    QString result;
    QImage resimg;
    QImage img;
    if(rect.find(1) != rect.end()){
        QImage tmp(ui->highpass_img_line->text());
        img = tmp.copy(rect[1]);
    }
    else{
        QImage tmp(ui->highpass_img_line->text());
        img = tmp;
    }
    ui->result_img_lab_2->setPixmap(QPixmap::fromImage(img.scaled(labelw, labelh,Qt::KeepAspectRatio)));
    m_highpass.highpassDetection(img,result,resimg);
    //320 * 240
    QPixmap pix = QPixmap::fromImage(resimg);
    if(pix.width() > 320 || pix.height() > 240){
        pix = pix.scaled(320, 240, Qt::KeepAspectRatio, Qt::SmoothTransformation);  // 按比例缩放
    }
    ui->result_img_lab->resize(pix.width(),pix.height());
    ui->result_img_lab->setPixmap(pix);
    if(!result.isEmpty()){
        ui->highpass_text->append(result);
    }
}

void MainWindow::on_model_img_select_btn_clicked()
{
    //打开操作
    QString fileName = QFileDialog::getOpenFileName(
                this, tr("open file"));
    if(!fileName.isEmpty()){
        ui->model_img_line->setText(fileName);
    }
}

void MainWindow::on_model_select_btn_clicked()
{
    //打开操作
    QString fileName = QFileDialog::getOpenFileName(
                this, tr("open file"));
    if(!fileName.isEmpty()){
        ui->model_img_line->setText(fileName);
    }
    ui->model_source_lab->setPixmap(QPixmap(fileName));
    //ui->model_source_lab->setPixmap(fileName);
}

void MainWindow::on_model_detect_clicked()
{
    QString result;
    QImage resimg;
    QImage img(ui->model_img_line->text());
    m_shapModel.shapModelDetection(img,result,resimg);
    //320 * 240
    QPixmap pix = QPixmap::fromImage(resimg);
    if(pix.width() > 320 || pix.height() > 240){
        pix = pix.scaled(320, 240, Qt::KeepAspectRatio, Qt::SmoothTransformation);  // 按比例缩放
    }
    ui->model_result_lab->resize(pix.width(),pix.height());
    ui->model_result_lab->setPixmap(pix);
    if(!result.isEmpty()){
        //ui->char_result_text->append(result);
    }
}

void MainWindow::on_highpass_page_btn_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(0);
}

void MainWindow::on_detect_model_page_btn_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(1);
}

void MainWindow::on_create_model_page_btn_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(2);
}

void MainWindow::on_create_model_select_btn_clicked()
{
    //打开操作
    QString fileName = QFileDialog::getOpenFileName(
                this, tr("open file"));
    if(!fileName.isEmpty()){
        ui->create_model_img_line->setText(fileName);
    }
    ui->create_model_lab->setImage(fileName);
}

void MainWindow::on_create_btm_clicked()
{
    auto rect = ui->create_model_lab->getRectMap();
    QString result;
    QImage resimg;
    QImage img;
    if(rect.find(1) != rect.end()){
        QImage tmp(ui->create_model_img_line->text());
        img = tmp.copy(rect[1]);
    }
    else{
        QImage tmp(ui->create_model_img_line->text());
        img = tmp;
    }
    m_shapModel.createShapModel(img);
}

void MainWindow::on_auto_find_points_select_btn_clicked()
{
    //打开操作
    QString fileName = QFileDialog::getOpenFileName(
                this, tr("open file"));
    if(!fileName.isEmpty()){
        ui->auto_find_points_img_line->setText(fileName);
    }
}

void MainWindow::on_auto_find_points_detect_clicked()
{
    QString result;
    QImage resimg;
    QImage img(ui->auto_find_points_img_line->text());
    m_xldFindPoints.findPointsDetection(img,result,resimg);
    //320 * 240
    QPixmap pix = QPixmap::fromImage(resimg);
    if(pix.width() > 480 || pix.height() > 480){
        pix = pix.scaled(480, 480, Qt::KeepAspectRatio, Qt::SmoothTransformation);  // 按比例缩放
    }
    ui->auto_find_points_img_lab->resize(pix.width(),pix.height());
    ui->auto_find_points_img_lab->setPixmap(pix);
    if(!result.isEmpty()){
        ui->auto_find_points_result_text->append(result);
    }
}

void MainWindow::on_auto_find_points_btn_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

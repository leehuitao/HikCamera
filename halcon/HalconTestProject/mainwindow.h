#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include "qrcode/qr_code.h"
#include "characterrecognition/character_recognition.h"
#include "contour/highpass_contour_detect.h"
#include "contour/auto_template_match.h"
#include "autofindpoints/xld_find_points.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private Q_SLOTS:
    void on_code_img_select_btn_clicked();

    void on_code_detect_clicked();

    void on_char_img_select_btn_clicked();

    void on_number_detect_clicked();

    void on_english_detect_clicked();

    void on_cod_page_btn_clicked();

    void on_auto_find_page_btn_clicked();

    void on_defect_analysis_page_btn_clicked();

    void on_highpass_img_btn_clicked();

    void on_highpass_detect_btn_clicked();

    void on_model_img_select_btn_clicked();

    void on_model_select_btn_clicked();

    void on_model_detect_clicked();

    void on_highpass_page_btn_clicked();

    void on_detect_model_page_btn_clicked();

    void on_create_model_page_btn_clicked();

    void on_create_model_select_btn_clicked();

    void on_create_btm_clicked();

    void on_auto_find_points_select_btn_clicked();

    void on_auto_find_points_detect_clicked();

    void on_auto_find_points_btn_clicked();

private:
    void init();
private:
    Ui::MainWindow *            ui;
    QRCode                      m_qrcode;
    CharacterRecognition        m_char;
    HighpassContourDetect       m_highpass;
    AutoTemplateMatch           m_shapModel;
    XldFindPoints               m_xldFindPoints;
};
#endif // MAINWINDOW_H

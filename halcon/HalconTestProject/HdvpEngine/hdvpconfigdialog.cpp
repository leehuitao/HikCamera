#include "hdvpconfigdialog.h"
#include "ui_hdvpconfigdialog.h"
#include <QFileDialog>

HdvpConfigDialog::HdvpConfigDialog(QWidget *parent, int visionCount, QStringList *files, QStringList *files_270,QString * machine) :
    QDialog(parent),
    ui(new Ui::HdvpConfigDialog)
{
    ui->setupUi(this);
    this->setWindowTitle(QString::fromLocal8Bit("Hdvp方案选择"));

    for (int i = 0; i < visionCount - files->size(); ++i)
    {
        files->append("");
    }

    m_oriFiles = files;
    m_oriFiles_270 = files_270;
    m_currentFiles = *files;
    m_currentFiles_270 = *files_270;
    m_machine = machine;
    m_edit_machine = *machine;
    if(m_edit_machine == "250"){
        ui->check_250_btn->setChecked(1);
        ui->page_250_btn->setChecked(1);
    }else{
        ui->check_270_btn->setChecked(1);
        ui->page_270_btn->setChecked(1);
    }
    for (int i = 0; i < visionCount; ++i)
    {
        ui->comboBox_vision->addItem(QString::fromLocal8Bit("工位%1").arg(i + 1));
    }
    dispFiles();
}

HdvpConfigDialog::~HdvpConfigDialog()
{
    delete ui;
}

void HdvpConfigDialog::dispFiles()
{
    ui->textEdit_files->clear();
    if(*m_machine == "250" || m_machine->isEmpty()){
        for (int i = 0; i < m_currentFiles.size(); ++i)
        {
            ui->textEdit_files->append(QString::fromLocal8Bit("工位%1:%2").arg(i + 1).arg(m_currentFiles[i]));
        }
    }else if(*m_machine == "270"){
        for (int i = 0; i < m_currentFiles_270.size(); ++i)
        {
            ui->textEdit_files->append(QString::fromLocal8Bit("工位%1:%2").arg(i + 1).arg(m_currentFiles_270[i]));
        }
    }

}

void HdvpConfigDialog::dispFiles(QString machine)
{
    ui->textEdit_files->clear();
    if(machine == "250"){
        for (int i = 0; i < m_currentFiles.size(); ++i)
        {
            ui->textEdit_files->append(QString::fromLocal8Bit("工位%1:%2").arg(i + 1).arg(m_currentFiles[i]));
        }
    }else if (machine == "270"){
        for (int i = 0; i < m_currentFiles_270.size(); ++i)
        {
            ui->textEdit_files->append(QString::fromLocal8Bit("工位%1:%2").arg(i + 1).arg(m_currentFiles_270[i]));
        }
    }
}

void HdvpConfigDialog::on_pushButton_select_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open Hdvp"), QString::fromLocal8Bit(""),
                                                tr("Hdvp Files(*.hdvp)"));

    if (path.isNull())
    {
        return;
    }
    if(m_edit_machine == "250" || m_edit_machine.isEmpty()){
        m_currentFiles[ui->comboBox_vision->currentIndex()] = path;
        m_edit_machine = "250";
    } else
        m_currentFiles_270[ui->comboBox_vision->currentIndex()] = path;
    dispFiles();
}

void HdvpConfigDialog::on_pushButton_confirm_clicked()
{
    *m_oriFiles = m_currentFiles;
    *m_oriFiles_270 = m_currentFiles_270;
    close();
}

void HdvpConfigDialog::on_pushButton_cancel_clicked()
{
    close();
}

void HdvpConfigDialog::on_page_250_btn_clicked()
{
    m_edit_machine = "250";
    dispFiles("250");
}

void HdvpConfigDialog::on_page_270_btn_clicked()
{
    m_edit_machine = "270";
    dispFiles("270");
}

void HdvpConfigDialog::on_check_250_btn_stateChanged(int arg1)
{
    if(arg1)
        *m_machine = "250";
    else
        *m_machine = "270";
}

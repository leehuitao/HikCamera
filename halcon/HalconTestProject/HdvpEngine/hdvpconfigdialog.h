#ifndef HDVPCONFIGDIALOG_H
#define HDVPCONFIGDIALOG_H

#include <QDialog>

namespace Ui {
class HdvpConfigDialog;
}

class HdvpConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HdvpConfigDialog(QWidget *parent = nullptr, int visionCount = 1, QStringList *files = 0,QStringList *files_270 = 0 , QString * machime = 0);
    ~HdvpConfigDialog();

protected:
    void dispFiles();
    void dispFiles(QString machine);
private slots:
    void on_pushButton_select_clicked();

    void on_pushButton_confirm_clicked();

    void on_pushButton_cancel_clicked();

    void on_page_250_btn_clicked();

    void on_page_270_btn_clicked();

    void on_check_250_btn_stateChanged(int arg1);

private:
    Ui::HdvpConfigDialog *ui;
    QStringList *m_oriFiles;
    QStringList *m_oriFiles_270;
    QStringList m_currentFiles;
    QStringList m_currentFiles_270;
    QString     *m_machine;
    QString     m_edit_machine;
};

#endif // HDVPCONFIGDIALOG_H

#ifndef REDRAW_LABEL_H
#define REDRAW_LABEL_H

#include <QWidget>
#include <QLabel>
namespace Ui {
class RedrawLabel;
}

class RedrawLabel : public QLabel
{
    Q_OBJECT

public:
    explicit RedrawLabel(QWidget *parent = nullptr);
    ~RedrawLabel();


    void wheelEvent(QWheelEvent *event);
};

#endif // REDRAW_LABEL_H

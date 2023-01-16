#ifndef SHOTLABEL_H
#define SHOTLABEL_H

#include <QMap>
#include <QWidget>
#include <QLabel>
#include <QMouseEvent>
#define  labelw 480
#define  labelh 320
class ShotLabel : public QLabel
{
public:
    ShotLabel(QWidget *wid = nullptr);
    void setImage(QString imgpath){
        imagrpath = imgpath;
        QImage  m = QImage(imgpath);//.scaled(300, 1200);
        img = m.scaled(labelw, labelh,Qt::KeepAspectRatio);
        sourcew = m.width();
        sourceh = m.height();
        scalew  = double(sourcew)/double(labelw);
        scaleh  = double(sourceh)/double(labelh);
        this->setMaximumSize(img.width(),img.height());
    }

    void setImage(QImage img){
        QImage  m = img;//.scaled(300, 1200);
        sourcew = m.width();
        sourceh = m.height();
        img = m.scaled(labelw, labelh,Qt::KeepAspectRatio);
        scalew  = double(sourcew)/double(labelw);
        scaleh  = double(sourceh)/double(labelh);
    }

    void setSelectNumber(int num){
        selectNumber = num;
    }
    void setStartBuild(bool build){
        startbuild = build;
    }

    void setRect(QList<QRect> r){
        rlist = r;
        update();
    }


    void setRectMap(QMap<int,QRect> r){
        rectMap = r;
        rect = QRect();
        update();
    }

    QList<QRect> getRect(){
        return rlist;
    }

    QMap<int,QRect> getRectMap(){
        return rectMap;
    }
protected:
    void mousePressEvent(QMouseEvent * event)
    {
        x = event->x();
        y = event->y();
        rectMap.clear();
        build = true;
    }//鼠标按下
    void mouseMoveEvent(QMouseEvent * e);
    void mouseReleaseEvent(QMouseEvent * e);//鼠标抬起
    void paintEvent(QPaintEvent *event);
private:
    int x;
    int y;
    int selectNumber = 1;
    QString imagrpath;
    QSize size;
    bool startbuild=true;
    bool build=false;
    QRect rect;
    QMap<int,QRect> rectMap;
    QList<QRect> rlist;
    QImage img;
    int     sourcew;
    int     sourceh;
    double  scalew;
    double  scaleh;
};
#endif // SHOTLABEL_H

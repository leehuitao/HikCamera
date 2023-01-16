#include "shot_label.h"
#include <QPainter>
#include <QPixmap>

ShotLabel::ShotLabel(QWidget *wid)
{

}


void ShotLabel::mouseMoveEvent(QMouseEvent *e)
{
    if(build){
        QRect r(x*scalew,y*scaleh,(e->x()-x)*scalew,(e->y()-y)*scaleh);
        rect = r;
        rectMap[selectNumber] = rect;
        update();
    }
}

void ShotLabel::mouseReleaseEvent(QMouseEvent *e)
{
    rlist.append(rect);

    build = false;
}

void ShotLabel::paintEvent(QPaintEvent *event)
{
    if(startbuild){
        QPainter painter(this);  //QPainter对象
        QPen pen; //画笔
        pen.setColor(QColor(255, 0, 0));  //设置画笔颜色
        painter.setPen(pen);
        painter.setBrush(Qt::NoBrush); //添加画刷
        painter.drawImage(0,0,img);
        for(auto iter = rectMap.begin(); iter != rectMap.end(); ++iter){
            QRect r(iter.value().x()/scalew,iter.value().y()/scaleh,iter.value().width()/scalew,iter.value().height()/scaleh);
            painter.drawText(r,QString("检测位%1,坐标:x %2 y %3 w %4 h %5").arg(iter.key()).arg(iter.value().x()).arg(iter.value().y()).arg(iter.value().width()).arg(iter.value().height()));//填充文字
            painter.drawRect(r); //绘制矩形
        }

        painter.drawRect(rect.x()/scalew,rect.y()/scaleh,rect.width()/scalew,rect.height()/scaleh); //绘制矩形
    }
}

#ifndef IMAGE_FORMAT_H
#define IMAGE_FORMAT_H

#include <QImage>
#include <QApplication>
#include <QDebug>
#include <QFileDialog>
#include <QFuture>
#include <QtConcurrent>
#include "HdvpEngine/hdvpengine.h"
/**
     * @brief QImage2HImage 将 Qt QImage 转换为 Halcon 的 HImage
     * @param from 输入的 QImage
     * @param to 输出的 HImage ，from 和 to 不共享内存数据。 每次都会为 to 重新分配内存。
     * @return true 表示转换成功，false 表示转换失败。
     */
static bool qtimage2HImage(const QImage &from, HalconCpp::HImage &to)
{
    if(from.isNull()) return false;

    int width = from.width(), height = from.height();
    QImage::Format format = from.format();

    if(format == QImage::Format_RGB32 ||
            format == QImage::Format_ARGB32 ||
            format == QImage::Format_ARGB32_Premultiplied)
    {
        to.GenImageInterleaved((char*)from.bits(), "rgbx", width, height, 0,  "byte", width, height, 0, 0, 8, 0);
        return true;
    }
    else if(format == QImage::Format_RGB888)
    {
        to.GenImageInterleaved((char*)from.bits(), "rgb", width, height, 0,  "byte", width, height, 0, 0, 8, 0);
        return true;
    }
    else if(format == QImage::Format_Grayscale8 || format == QImage::Format_Indexed8)
    {
        to.GenImage1("byte", width, height, (char*)from.bits());
        return true;
    }
    return false;
}

/**
    * @brief HImage2QImage 将 Halcon 的 HImage 转换为 Qt 的 QImage
    * @param from HImage ，暂时只支持 8bits 灰度图像和 8bits 的 3 通道彩色图像
    * @param to QImage ，这里 from 和 to 不共享内存。如果 to 的内存大小合适，那么就不用重新分配内存。所以可以加快速度。
    * @return  true 表示转换成功，false 表示转换失败
    */
static bool halconimage2QImage(const HalconCpp::HImage &from, QImage &to)
{
    Hlong width;
    Hlong height;
    from.GetImageSize(&width, &height);
    HTuple channels = from.CountChannels();
    HTuple type = from.GetImageType();

    if( strcmp(type[0].S(), "byte" )) // 如果不是 byte 类型，则失败
    {
        return false;
    }

    QImage::Format format;
    switch(channels[0].I())
    {
    case 1:
        format = QImage::Format_Grayscale8;
        break;
    case 3:
        format = QImage::Format_RGB32;
        break;
    default:
        return false;
    }

    if(to.width() != width || to.height() != height || to.format() != format)
    {
        to = QImage(static_cast<int>(width),
                    static_cast<int>(height),
                    format);
    }
    HString Type = "byte";
    if(channels[0].I() == 1)
    {
        unsigned char * pSrc = reinterpret_cast<unsigned char *>( from.GetImagePointer1(&Type, &width, &height) );
        memcpy( to.bits(), pSrc, static_cast<size_t>(width) * static_cast<size_t>(height) );
        return true;
    }
    else if(channels[0].I() == 3)
    {
        uchar *R, *G, *B;
        from.GetImagePointer3(reinterpret_cast<void **>(&R),
                              reinterpret_cast<void **>(&G),
                              reinterpret_cast<void **>(&B), &Type, &width, &height);

        for(int row = 0; row < height; row ++)
        {
            QRgb* line = reinterpret_cast<QRgb*>(to.scanLine(row));
            for(int col = 0; col < width; col ++)
            {
                line[col] = qRgb(*R++, *G++, *B++);
            }
        }
        return true;
    }

    return false;
}

static void HObjectToQImage(HObject himage,QImage **qimage)
{
    HTuple hChannels;
    HTuple   width,height;
    width=height=0;
    HTuple htype;
    ConvertImageType(himage,&himage,"byte");//将图片转化成byte类型
    CountChannels(himage,&hChannels);       //判断图像通道数

    if(hChannels[0].I()==1)//单通道图
    {
        HTuple hv_pointer;
        unsigned char *ptr;
        GetImagePointer1(himage,&hv_pointer,&htype,&width,&height);

        ptr=(unsigned char *)hv_pointer[0].L();

         *(*qimage)=QImage(ptr,width,height,width,QImage::Format_Indexed8);//不知道是否已自动4字节对齐
    }
    else if(hChannels[0].I()==3)//三通道图
    {
        HTuple hv_ptrRed,hv_ptrGreen,hv_ptrBlue;
        GetImagePointer3(himage,&hv_ptrRed,&hv_ptrGreen,&hv_ptrBlue,&htype,&width,&height);

        uchar *ptrRed=(uchar*)hv_ptrRed[0].L();
        uchar *ptrGreen=(uchar*)hv_ptrGreen[0].L();
        uchar *ptrBlue=(uchar*)hv_ptrBlue[0].L();
        int bytesperline=(width*8*3+31)/32*4;//针对位深为8的三通道图进行每行4字节对齐补齐
        int bytecount=bytesperline*height;//整个图像数据需要的字节数
        uchar* data8=new uchar[bytecount];
        int lineheadid,pixid;
        for(int i=0;i<height;i++)
        {
            lineheadid=bytesperline*i;//计算出图像第i行的行首在图像数据中的地址
            for(int j=0;j<width;j++)
            {
                pixid=lineheadid+j*3;//计算坐标为（i，j）的像素id
                data8[pixid]=ptrRed[width*i+j];
                data8[pixid+1]=ptrGreen[width*i+j];
                data8[pixid+2]=ptrBlue[width*i+j];
            }
        }

        *(*qimage)=QImage(data8,width,height,QImage::Format_RGB888);
    }
}
#endif // IMAGE_FORMAT_H

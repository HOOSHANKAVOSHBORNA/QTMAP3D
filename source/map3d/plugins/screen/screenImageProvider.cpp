#include "screenImageProvider.h"


ScreenImageProvider::ScreenImageProvider() : QQuickImageProvider(QQuickImageProvider::Pixmap) {}

QPixmap ScreenImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    const uchar *qImageBuffer = (const uchar*)mImage->data();
    QImage img(qImageBuffer, mImage->s(), mImage->t(), mImage->getRowSizeInBytes(), QImage::Format_RGB888);
    img.mirror(false,true);
    QPixmap pixmap(400,220);
    pixmap.convertFromImage(img);
    return pixmap;
}

void ScreenImageProvider::setImage(osg::ref_ptr<osg::Image> img)
{
    mImage = img;
}


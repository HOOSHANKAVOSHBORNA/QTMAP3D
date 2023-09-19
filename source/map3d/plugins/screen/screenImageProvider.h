#ifndef SCREENIMAGEPROVIDER_H
#define SCREENIMAGEPROVIDER_H

#include <osg/Image>
#include <QQuickImageProvider>


class ScreenImageProvider : public QQuickImageProvider
{
public:
    ScreenImageProvider();

    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize ) override;
    void setImage(osg::ref_ptr<osg::Image> img);


private:
    osg::ref_ptr<osg::Image> mImage;
};

#endif // SCREENIMAGEPROVIDER_H

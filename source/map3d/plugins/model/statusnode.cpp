#include "statusnode.h"

#include <QColor>
#include <QPainter>
#include <osg/AutoTransform>
#include <osg/Depth>
#include <osgEarthAnnotation/AnnotationUtils>
#include <osgEarth/GLUtils>
#include "model.h"
StatusNode::StatusNode(MapItem *mapControler, QObject *parent)
    : QObject{parent},
    osgEarth::Annotation::ModelNode(mapControler->getMapNode(), Model::getDefaultStyle()),
    mMapItem(mapControler)
{
    Data data0;
    data0.name = "Mehrdad";
    data0.value = 85;
    Data data1;
    data1.name = "changiz";
    data1.value = 69;
    Data data2;
    data2.name = "abbas";
    data2.value = 6985;
    Data data3;
    data3.name = "torab";
    data3.value = "BooB";


    mTitle = "Moos";
    mDataList.push_back(data0);
    mDataList.push_back(data1);
    mDataList.push_back(data2);
    mDataList.push_back(data3);

    updateStatusData();
    osg::AutoTransform *at = new osg::AutoTransform;
    mImgNode = new osg::Geode();
    osg::ref_ptr<osg::StateSet> geodeStateSet = new osg::StateSet();
    geodeStateSet->setAttributeAndModes(new osg::Depth(osg::Depth::ALWAYS, 0, 1, false), 1);
    osg::ref_ptr<osg::Image> image = mStatusImg;
    osg::ref_ptr<osg::Geometry> imgGeom = osgEarth::Annotation::AnnotationUtils::createImageGeometry(image, osg::Vec2s(0,0), 0, 0, 1);
    mImgNode->setStateSet(geodeStateSet);
    mImgNode->addDrawable(imgGeom);
    at->addChild(mImgNode);
    at->setAutoRotateMode(osg::AutoTransform::ROTATE_TO_CAMERA);
    osgEarth::Symbology::Style  rootStyle;
    rootStyle.getOrCreate<osgEarth::Symbology::ModelSymbol>()->setModel(at);
    rootStyle.getOrCreate<osgEarth::Symbology::ModelSymbol>()->autoScale() = true;
    setStyle(rootStyle);
//    getGeoTransform()->addChild(mImgNode);

}

void StatusNode::setData(QString title, std::list<Data> dataList)
{
    mTitle = title;
    mDataList = dataList;
    updateStatusData();
    setStyle(getStyle());
}

void StatusNode::updateStatusData()
{
    int imageHeight = 36;
    int pos = 5;
    if (mDataList.size() > 1){
        imageHeight += (mDataList.size()-1)*22;
    }
        //pos = mDataList.size()-1*16;

    if (!mRenderImage) {
        mRenderImage = new QImage(
            LABEL_IMAGE_WIDTH,
            imageHeight,
            QImage::Format_RGBA8888
            );
    }
    if(!mStatusImg.valid())
        mStatusImg = new osg::Image;

    {

        mRenderImage->fill(QColor(Qt::transparent));
        QPainter *painter = new QPainter(mRenderImage);
        painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

        static const QBrush backgroundBrush = QBrush(QColor(30, 30, 30, int(255 * 0.3f)));
        static const QFont textFont("SourceSansPro", 7, QFont::Normal);
        static const QPen  textPen(QColor(255, 255, 255));

        static const QFont tTextFont("SourceSansPro", 8, QFont::Bold);
        static const QPen  tTextPen(QColor(255, 0, 0));


        static const QPen linePen(QColor(255, 255, 255),
                                  1,
                                  Qt::PenStyle::DashLine
                                  );

        painter->setPen(linePen);
        painter->setBrush(Qt::NoBrush);
        painter->drawLine(0, 14, LABEL_IMAGE_WIDTH, 14);

        painter->setPen(Qt::NoPen);
        painter->setBrush(backgroundBrush);
        painter->drawRoundedRect(
            mRenderImage->rect(),
            10,2);


        painter->setPen(tTextPen);
        painter->setFont(tTextFont);

        painter->drawText(0, 0, LABEL_IMAGE_WIDTH, imageHeight,
                          Qt::AlignHCenter,
                          mTitle);

        for (const auto& data: mDataList){
            pos += 16 ;
            painter->setPen(textPen);
            painter->setFont(textFont);
            painter->drawText(2, pos, LABEL_IMAGE_WIDTH, 22,
                              Qt::AlignLeft,
                              data.name);

            static const QPen  gTextPen(QColor(0, 255, 0));
            painter->setPen(gTextPen);
            painter->drawText(-2, pos, LABEL_IMAGE_WIDTH, 22,
                              Qt::AlignRight,
                              data.value.toString());
        }

        *mRenderImage = mRenderImage->mirrored(false, true);

        mStatusImg->setImage(LABEL_IMAGE_WIDTH,
                              imageHeight,
                              1,
                              GL_RGBA,
                              GL_RGBA,
                              GL_UNSIGNED_BYTE,
                              mRenderImage->bits(),
                              osg::Image::AllocationMode::NO_DELETE);
    }
}

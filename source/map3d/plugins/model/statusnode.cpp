#include "statusnode.h"
#include <QColor>
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
    mAutoTransform = new osg::AutoTransform;
    mAutoTransform->setAutoRotateMode(osg::AutoTransform::ROTATE_TO_CAMERA);
    osgEarth::Symbology::Style  rootStyle;
    rootStyle.getOrCreate<osgEarth::Symbology::ModelSymbol>()->setModel(mAutoTransform);
    rootStyle.getOrCreate<osgEarth::Symbology::ModelSymbol>()->autoScale() = true;
    setStyle(rootStyle);
}

void StatusNode::setData(QString title, std::list<Data> *dataList)
{
    mTitle = title;
    mDataList = dataList;

    createImageNode();
    osgEarth::Symbology::Style  rootStyle;
    rootStyle.getOrCreate<osgEarth::Symbology::ModelSymbol>()->setModel(mAutoTransform);
    rootStyle.getOrCreate<osgEarth::Symbology::ModelSymbol>()->autoScale() = true;
    setStyle(rootStyle);
}

void StatusNode::createImageNode()
{
    int cellHeight = 22;
    int height = cellHeight + 2 + 2;
    int width = 100;
    int pos = 0;
    if (mDataList){
        height += mDataList->size() * cellHeight;
    }

    if (!mRenderImage) {
        mRenderImage = new QImage(
            width,
            height,
            QImage::Format_RGBA8888
            );
    } else {
        mRenderImage->~QImage();
        mRenderImage = new(mRenderImage) QImage(
            width,
            height,
            QImage::Format_RGBA8888
            );
    }
    //--------------------------------------------------------------------------
    mRenderImage->fill(QColor(Qt::transparent));
    QPainter painter(mRenderImage);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    QBrush backgroundBrush = QBrush(QColor(30, 30, 30, int(255 * 0.3f)));
    QFont titleTextFont("SourceSansPro", 10, QFont::Bold);
    QPen  titleTextPen(QColor(204, 204, 51, 255));

    QFont textFont("SourceSansPro", 9, QFont::Normal);
    QPen  textPen(QColor(255, 255, 255));

    QPen linePen(QColor(255, 255, 255), 1, Qt::PenStyle::DashLine);

    painter.setPen(Qt::NoPen);
    painter.setBrush(backgroundBrush);
    painter.drawRoundedRect(mRenderImage->rect(), 8, 8);
    painter.setBrush(QBrush(QColor(26, 77, 46, int(255 * 0.2f))));
    painter.drawRoundedRect(
        QRect(0, 0, width, cellHeight + 1),
        8,8);
    //--title---------------------------------------------------------------
    painter.setPen(titleTextPen);
    painter.setFont(titleTextFont);
    painter.drawText(0, pos, width, cellHeight + 2,
                     Qt::AlignCenter,
                     mTitle);
    pos += cellHeight + 2;

    painter.setPen(linePen);
    painter.setBrush(Qt::NoBrush);
    painter.drawLine(0, pos + 1, width, pos + 1);
    pos += 2;
    //--data--------------------------------------------------------------
    painter.setPen(textPen);
    painter.setFont(textFont);
    if(mDataList)
        for (const auto& data: *mDataList){
            painter.setOpacity(0.8);
            painter.drawText(2, pos, width/2, cellHeight,
                             Qt::AlignLeft|Qt::AlignVCenter,
                             data.name + ": ");


            painter.setOpacity(1);
            painter.drawText(2 + width/2, pos, width, cellHeight,
                             Qt::AlignLeft|Qt::AlignVCenter,
                             data.value.toString());
            pos += cellHeight;
        }
    painter.end();

    *mRenderImage = mRenderImage->mirrored(false, true);

    osg::ref_ptr<osg::Image> osgImage = new osg::Image;
    osgImage->setImage(width,
                         height,
                         1,
                         GL_RGBA,
                         GL_RGBA,
                         GL_UNSIGNED_BYTE,
                         mRenderImage->bits(),
                         osg::Image::AllocationMode::NO_DELETE);

    osg::ref_ptr<osg::Geode> imageNode = new osg::Geode();
    osg::ref_ptr<osg::StateSet> geodeStateSet = new osg::StateSet();
    geodeStateSet->setAttributeAndModes(new osg::Depth(osg::Depth::ALWAYS, 0, 1, false), 1);
    osg::ref_ptr<osg::Geometry> imgGeom = osgEarth::Annotation::AnnotationUtils::createImageGeometry(osgImage, osg::Vec2s(0,0), 0, 0, 1);
    imageNode->setStateSet(geodeStateSet);
    imageNode->addDrawable(imgGeom);
    mAutoTransform->removeChildren(0, mAutoTransform->getNumChildren());
    mAutoTransform->addChild(imageNode);
}

StatusNodeData *StatusNode::nodeData() const
{
    return mNodeData;
}

void StatusNode::setNodeData(StatusNodeData *newNodeData)
{
    mNodeData = newNodeData;
}

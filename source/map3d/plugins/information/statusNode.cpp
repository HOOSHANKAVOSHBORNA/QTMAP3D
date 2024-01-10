#include "statusNode.h"
#include <QColor>
#include <osg/AutoTransform>
#include <osg/Depth>
#include <osgEarthAnnotation/AnnotationUtils>
#include <osgEarth/GLUtils>

StatusNode::StatusNode(MapItem *mapControler)
    : osgEarth::Annotation::PlaceNode(),
    mMapItem(mapControler)
{
    mFont.setFamily("Roboto");
    mFont.setPointSize(12);
    osgEarth::Symbology::Style  rootStyle;
    rootStyle.getOrCreate<osgEarth::Symbology::IconSymbol>()->alignment() = osgEarth::Symbology::IconSymbol::ALIGN_LEFT_TOP;
    rootStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->font() = mFont.family().toStdString();
    rootStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->fill()->color() = osgEarth::Color::White;
    rootStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->size() = mFont.pointSize();
//    rootStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->halo() = osgEarth::Color("#5f5f5f");
    rootStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->alignment() = osgEarth::Symbology::TextSymbol::ALIGN_CENTER_TOP;
    rootStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->occlusionCull() = false;
    rootStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->occlusionCullAltitude() = 20000000000000;
    rootStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->declutter() = true;
//    rootStyle.getOrCreate<osgEarth::Symbology::BBoxSymbol>()->fill() = osgEarth::Color::Black;
//    rootStyle.getOrCreate<osgEarth::Symbology::BBoxSymbol>()->margin() = 3;
//    rootStyle.getOrCreate<osgEarth::Symbology::BBoxSymbol>()->border() = osgEarth::Color::Red;
//    rootStyle.getOrCreate<osgEarth::Symbology::BBoxSymbol>()->border()->width() = 1;
    setStyle(rootStyle);
//    setOcclusionCulling(true);
//    setOcclusionCullingMaxAltitude(20000000);
    //    setPriority(FLT_MAX);
}

StatusNode::~StatusNode()
{
    delete mTitleImage;
    mTitleImage = nullptr;
}

void StatusNode::setFieldData(QString title, const std::vector<NodeFieldData>& data)
{
    mTitle = title;
    compile(data);
}

void StatusNode::compile(const std::vector<NodeFieldData>& data)
{
//    int cellHeight = 22;
//    int height = cellHeight + 2 + 2;
//    int width = 100;
//    int pos = 0;
//    if (mDataList){
//        height += mDataList->size() * cellHeight;
//    }

//    if (!mTitleImage) {
//        mTitleImage = new QImage(
//            width,
//            height,
//            QImage::Format_RGBA8888
//            );
//    } else {
//        mTitleImage->~QImage();
//        mTitleImage = new(mTitleImage) QImage(
//            width,
//            height,
//            QImage::Format_RGBA8888
//            );
//    }
//    //--------------------------------------------------------------------------
//    mTitleImage->fill(QColor(Qt::transparent));
//    QPainter painter(mTitleImage);
//    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

//    QBrush backgroundBrush = QBrush(QColor(30, 30, 30, int(255 * 0.3f)));
//    QFont titleTextFont("SourceSansPro", 10, QFont::Bold);
//    QPen  titleTextPen(QColor(204, 204, 51, 255));

//    QFont textFont("SourceSansPro", 9, QFont::Normal);
//    QPen  textPen(QColor(255, 255, 255));

//    QPen linePen(QColor(255, 255, 255), 1, Qt::PenStyle::DashLine);

//    painter.setPen(Qt::NoPen);
//    painter.setBrush(backgroundBrush);
//    painter.drawRoundedRect(mTitleImage->rect(), 8, 8);
//    painter.setBrush(QBrush(QColor(26, 77, 46, int(255 * 0.2f))));
//    painter.drawRoundedRect(
//        QRect(0, 0, width, cellHeight + 1),
//        8,8);
//    //--title---------------------------------------------------------------
//    painter.setPen(titleTextPen);
//    painter.setFont(titleTextFont);
//    painter.drawText(0, pos, width, cellHeight + 2,
//                     Qt::AlignCenter,
//                     mTitle);
//    pos += cellHeight + 2;

//    painter.setPen(linePen);
//    painter.setBrush(Qt::NoBrush);
//    painter.drawLine(0, pos + 1, width, pos + 1);
//    pos += 2;
//    //--data--------------------------------------------------------------
//    std::string str = mTitle.toStdString() + "\n";
//    str += "------------- \n";
//    painter.setPen(textPen);
//    painter.setFont(textFont);
//    if(mDataList)
//        for (const auto& data: *mDataList){
//            painter.setOpacity(0.8);
//            painter.drawText(2, pos, width/2, cellHeight,
//                             Qt::AlignLeft|Qt::AlignVCenter,
//                             data.name + ": ");


//            painter.setOpacity(1);
//            painter.drawText(2 + width/2, pos, width, cellHeight,
//                             Qt::AlignLeft|Qt::AlignVCenter,
//                             data.value.toString());
//            pos += cellHeight;

//            str += (data.name + ": "+ data.value.toString() +"\n").toStdString();
//        }
//    painter.end();

//    *mTitleImage = mTitleImage->mirrored(false, true);

//    osg::ref_ptr<osg::Image> osgImage = new osg::Image;
//    osgImage->setImage(width,
//                       height,
//                       1,
//                       GL_RGBA,
//                       GL_RGBA,
//                       GL_UNSIGNED_BYTE,
//                       mTitleImage->bits(),
//                       osg::Image::AllocationMode::NO_DELETE);

    int height = 22;
    int width;
    //--data--------------------------------------------------------------
    QFontMetrics fm(mFont);
    width = fm.horizontalAdvance(mTitle);;
    std::string text = "\n";
    for (const auto& data: data){
        QString str = (data.name + ": "+ data.value.toString() +"\n");
        int widthStr = fm.horizontalAdvance(str);
        if(widthStr > width)
            width = widthStr;
        text += str.toStdString();
    }
   //-----------------------------------------------------------------------
    if (!mTitleImage) {
        mTitleImage = new QImage(
            width,
            height,
            QImage::Format_RGBA8888
            );
    } else {
        mTitleImage->~QImage();
        mTitleImage = new(mTitleImage) QImage(
            width,
            height,
            QImage::Format_RGBA8888
            );
    }
    //--------------------------------------------------------------------------
    mTitleImage->fill(QColor(Qt::transparent));
    QPainter painter(mTitleImage);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    QBrush backgroundBrush = QBrush(QColor(30, 30, 30, int(255 * 0.1f)));
    QFont titleTextFont(mFont.family(), mFont.pointSize() - 3, QFont::Bold);
    QPen  titleTextPen(QColor(237, 226, 20, 255));

    painter.setPen(Qt::NoPen);
    painter.setBrush(backgroundBrush);
    painter.drawRoundedRect(mTitleImage->rect(), 5, 5);
    painter.setBrush(QBrush(QColor(26, 77, 46, int(255 * 0.1f))));
    painter.drawRoundedRect(QRect(0, 0, width, height), 5, 5);
    //--title---------------------------------------------------------------
    painter.setPen(titleTextPen);
    painter.setFont(titleTextFont);
    painter.drawText(0, 0, width, height + 2,
                     Qt::AlignCenter,
                     mTitle);
    painter.end();

    *mTitleImage = mTitleImage->mirrored(false, true);

    osg::ref_ptr<osg::Image> osgImage = new osg::Image;
    osgImage->setImage(width,
                       height,
                       1,
                       GL_RGBA,
                       GL_RGBA,
                       GL_UNSIGNED_BYTE,
                       mTitleImage->bits(),
                       osg::Image::AllocationMode::NO_DELETE);
    setIconImage(osgImage);
    setText(text);
}

StatusNodeData StatusNode::nodeData() const
{
    return mNodeData;
}

void StatusNode::setNodeData(const StatusNodeData &newNodeData)
{
    mNodeData = newNodeData;
    setFieldData(newNodeData.name, newNodeData.fieldData);
}

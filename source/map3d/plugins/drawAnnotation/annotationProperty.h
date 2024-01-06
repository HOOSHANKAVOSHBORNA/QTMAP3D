#ifndef LINEPROPERTIES_H
#define LINEPROPERTIES_H

#include "lineNode.h"
#include "property.h"
#include <QAbstractListModel>
#include <QObject>
#include <QVariant>
#include <QVector3D>
#include <QQuickItem>
#include <QQmlEngine>
#include "annotatedNode.h"

class AnnotationProperty: public Property
{
    Q_OBJECT
public:
    AnnotationProperty(QQuickItem *parent=nullptr);
    void setStrokeWidth(double width)        override;
    void setStrokeColor(const QColor &color) override;
    void setFillColor(const QColor &color) override;
    void setPointsVisible(bool visible)      override;
    void setPointsColor(const QColor &point) override;
    void setPointsWidth(double point)        override;
    void setPointsSmooth(bool point)         override;
    void setShowBearing(bool showBearing)    override;
    void setShowSlop(bool slop)              override;
    void setTesselation(double tesselation)  override;
    void setClamp(int clamp)                 override;
    void setShowLen(bool showLen)            override;
    void setHeight(double height)            override;
    void setShowAltitude(bool showAltitude)  override;


    AnnotatedNode *getAnnotatedNode() const;

    void setAnnotatedNode(AnnotatedNode *newLine);
    void setRuler(AnnotatedNode *newLine);
    void setMesureSlope(AnnotatedNode *newLine);
    void setMeasureHeight(AnnotatedNode *newLine);

private:
    AnnotatedNode *mAnnotatedNode = nullptr;
};



#endif // LINEPROPERTIES_H

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


class LineProperty: public Property
{
    Q_OBJECT
public:
    LineProperty(QQuickItem *parent=nullptr);
    void setStrokeWidth(double width) override;
    void setStrokeColor(const QColor &color) override;
    void setPointsVisible(bool visible)override;
    void setPointsColor(const QColor &point) override;
    void setPointsWidth(double point) override;
    void setPointsSmooth(bool point) override;
    void setShowBearing(bool showBearing) override;
    void setShowSlop(bool slop)override;
    void setTesselation(double tesselation)override;
    void setClamp(int clamp) override;
    void setShowLen(bool showLen)override;


    LineNode *getLine() const;

    void setLine(LineNode *newLine);
    void setRuler(LineNode *newLine);
    void setMesureSlope(LineNode *newLine);
    void setMeasureHeight(LineNode *newLine);

private:
    LineNode *mLineNode = nullptr;
};
#endif // LINEPROPERTIES_H

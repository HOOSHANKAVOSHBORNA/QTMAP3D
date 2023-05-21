#ifndef BASEMODEL_H
#define BASEMODEL_H

#include"mapcontroller.h"

#include <osgEarthAnnotation/ModelNode>
#include <osgEarthAnnotation/PlaceNode>
#include <osgParticle/FireEffect>
#include <osgParticle/SmokeTrailEffect>
#include <osgParticle/SmokeEffect>
#include <osgParticle/ExplosionEffect>
#include <osgParticle/ExplosionDebrisEffect>
#include <osgEarth/Registry>
#include <osgGA/GUIEventHandler>
#include <osgViewer/Viewer>
#include <osgFX/Outline>

#include <QMouseEvent>
#include <QObject>

//class DefenseModelNode;

class ModelAnimationPathCallback: public osg::AnimationPathCallback
{
public:
    void operator()(osg::Node* node, osg::NodeVisitor* nv) override;
};

class DefenseModelNode: public QObject, public osgEarth::Annotation::ModelNode
{
    Q_OBJECT

public:

    enum SelectionMode {
        UNSELECTED = 0,
        SELECTED   = 1
    };

    enum HoverMode {
        UNHOVERED = 0,
        HOVERED   = 1
    };


public:
    DefenseModelNode(MapController* mapControler, QObject* parent = nullptr);
    void setType(const QString &value);
    QString getType() const;
    void setQStringName(QString name);
    QString getQStringName();
    virtual void setGeographicPosition(const osgEarth::GeoPoint& pos, double heading);
    osg::Vec3d getGeographicPosition() const;



    virtual void stop(){}
    void collision();
    osgEarth::Annotation::ModelNode* getDragModelNode();


	static osg::Image* createColoredImage(osg::Image* sourceImage, osgEarth::Color color);
	static osg::Image* createDarkerImage(osg::Image *sourceImage, float factor);

	static void copyColoredImage(osg::Image* sourceImage, osg::Image* destImage, osgEarth::Color color);
	static void copyDarkerImage(osg::Image *sourceImage, osg::Image* destImage, float factor);


signals:
//    void selectionModeChanged();
//    void hoverModeChanged();

protected:
	virtual void setSelectionMode(SelectionMode sm);
	void setHoverMode(HoverMode hm);


protected:
    virtual void playExplosionEffect(float scale);
public:
    virtual void frameEvent() {}
    virtual void mousePressEvent(QMouseEvent* event, bool onModel);
    virtual void mouseMoveEvent(QMouseEvent* event, bool onModel);
    //void select(bool val);
    //virtual void hover(bool val);
    virtual void updateColors();

protected:
    //bool mIsSelected{false};
    bool mIs3D{false};
    osg::ref_ptr<osg::LOD> mRootNode;
    osg::ref_ptr<osg::Group> mNode3D;
    osg::ref_ptr<osg::Switch> mNode2D;
	osg::ref_ptr<osgEarth::Annotation::PlaceNode> mStatusNode{nullptr};
    osgEarth::Color mModelColor{osgEarth::Color::White};

private:
    friend class ModelAnimationPathCallback;
    friend class PickHandler;

private:
    QString mType;
    osg::ref_ptr<osgFX::Outline> mSelectOutline;
    MapController *mMapControler;
    osg::ref_ptr<osgEarth::Annotation::PlaceNode> mSelectedNode{nullptr};

protected:
    SelectionMode mSelectionMode = UNSELECTED;
    HoverMode     mHoverMode     = UNHOVERED;
    class DefenseModelNodeAutoScaler *mDefenseModeNodeAutoScaler = nullptr;
};

#endif // BASEMODEL_H

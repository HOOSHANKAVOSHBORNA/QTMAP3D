#ifndef SIMPLEMODELNODE_H
#define SIMPLEMODELNODE_H


#include "mapItem.h"
#include <QObject>
#include <osgEarthAnnotation/ModelNode>
#include "modelAutoScaler.h"
#include "nodeInformationManager.h"
#include <osgFX/Outline>
#include <osgFX/Scribe>
#include <QQmlEngine>
#include <bookmark.h>
#include <circle.h>
#include <cone.h>
#include <osg/ComputeBoundsVisitor>


class MoveableModelNode;
class FlyableModelNode;
class AttackManager;
class TargetManager;

class SimpleModelNode : public QObject, public osgEarth::Annotation::ModelNode
{
    Q_OBJECT
public:
    SimpleModelNode(MapItem* mapControler, const std::string& url3D, const std::string& url2D, QObject *parent = nullptr);

    ~SimpleModelNode();
    void updateUrl(const std::string& url3D, const std::string& url2D);
    MapItem *mapItem() const;
    std::string url2D() const;
    std::string url3D() const;

    virtual SimpleModelNode* asSimpleModelNode(){return this;}
    virtual MoveableModelNode* asMoveableModelNode(){return nullptr;}
    virtual FlyableModelNode* asFlyableModelNode(){return nullptr;}

    void selectModel();
    bool isAutoScale() const;
    void setAutoScale(bool newIsAutoScale);

    NodeData *nodeData() const;
    void setNodeData(NodeData *newNodeData);
    void setModelColor(osgEarth::Color color);

    void setBookmark(BookmarkManager *bookmark);
    void setQQmlEngine(QQmlEngine *engine);

    bool getIsBookmarked() const;
    void setIsBookmarked(bool newIsBookmarked);

    bool isAttacker();
    void makeAttacker(ParenticAnnotationLayer *layer, int bulletCount);
    TargetManager *getTargetManager();
    AttackManager *getAttackManager();
    osgEarth::Annotation::ModelNode *getDragModelNode();

private slots:
    void compile();
    void onModeChanged(bool is3DView);
    void onBookmarkChecked(bool status);

private:
    osg::ref_ptr<osg::Image> mImage;
    osg::ref_ptr<osg::Node> mSimpleNode;
    osg::ref_ptr<osg::Switch> mSwitchNode;
    osg::ref_ptr<osg::LOD> m3DNode;
    osg::ref_ptr<osg::Geode> m2DNode;
    osg::ref_ptr<Circle> mCircleSelectNode;
    osg::ref_ptr<Cone> mConeSelecteNode;
    AttackManager* mAttackManager;
    TargetManager* mTargetManager;


    osg::ref_ptr<ModelAutoScaler> mAutoScaler;
    std::string mUrl2D;
    std::string mUrl3D;
    MapItem *mMapItem;
    bool mIs3D{false};
    bool mIsAutoScale{true};
    bool mIsSelected{false};
    NodeData* mNodeData{nullptr};
    osgEarth::Color mColor{osgEarth::Color::White};
    NodeInformationManager* mNodeInformation{nullptr};
    bool mIsBookmarked{false};
    QQmlEngine *mEnigine{nullptr};
    BookmarkManager *mBookmark;
    BookmarkItem *mBookmarkItem{nullptr};
    bool mIsAttacker{false};


private:
    static QMap<std::string, osg::ref_ptr<osg::Node>> mNodes3D;
    static QMap<std::string, osg::ref_ptr<osg::Image>> mImages2D;
};

#endif // SIMPLEMODELNODE_H

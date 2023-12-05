#ifndef SIMPLEMODELNODE_H
#define SIMPLEMODELNODE_H



#include "mapItem.h"
#include <QObject>
#include <osg/CullFace>
#include <osg/PolygonMode>
#include <osgEarthAnnotation/ModelNode>
#include "modelAutoScaler.h"
#include "nodeInformation.h"
#include <osgFX/Outline>
#include <osgFX/Scribe>
#include <QQmlEngine>
#include <bookmark.h>
#include <qmlNode.h>
#include <osg/ComputeBoundsVisitor>
#include <osg/Stencil>
#include "circle.h"
#include "cone.h"
#include "circularMenu.h"

class HighlightLine;
class MoveableModelNode;
class FlyableModelNode;

class SimpleModelNode : public QObject, public osgEarth::Annotation::ModelNode
{
    Q_OBJECT
public:
    SimpleModelNode(MapItem* mapItem, const std::string& url3D, const std::string& url2D, QObject *parent = nullptr);
    ~SimpleModelNode();

    virtual SimpleModelNode* asSimpleModelNode(){return this;}
    virtual MoveableModelNode* asMoveableModelNode(){return nullptr;}
    virtual FlyableModelNode* asFlyableModelNode(){return nullptr;}

    MapItem *mapItem() const;

    void updateUrl(const std::string& url3D, const std::string& url2D);
    std::string url3D() const;
    std::string url2D() const;

    BookmarkManager *bookmarkManager() const;
    void setBookmarkManager(BookmarkManager *bookmarkManager);

    bool isSelect() const;
    void select();
    void showMenu();


    bool isHighlight() const;
    void highlight(bool isHighlight);

    bool isAutoScale() const;
    void setAutoScale(bool newIsAutoScale);

    NodeData *nodeData() const;
    void setNodeData(NodeData *newNodeData);

    osgEarth::Color color() const;
    void setColor(osgEarth::Color color);

    bool isAttacker() const;
    void setAttacker(bool attacker);

signals:
    void onTargetChecked();
    void onAttackChecked();
//    osgEarth::Annotation::ModelNode *getDragModelNode();


private slots:
    void onModeChanged(bool is3DView);
    void onInfoClicked();
    void onBookmarkChecked(bool status);
    void onGoToPosition();
    void onTrack();

private:
    void compile();
    void createCircularMenu();
    void createNodeInformation();
    void createBookmarkItem();
    void setOutline(bool state);

private:
    osg::ref_ptr<osg::Image> mImage;
    osg::ref_ptr<osg::Node> mSimpleNode;
    osg::ref_ptr<osg::Switch> mSwitchNode;
    osg::ref_ptr<osg::LOD> m3DNode;
    osg::ref_ptr<HighlightLine> mHighlightLine;
    osg::ref_ptr<osg::Geode> m2DNode;
    osg::ref_ptr<Circle> mCircleSelectNode;
    osg::ref_ptr<Cone> mConeHighliteNode;
    osg::ref_ptr<Circle> mCircleHighlightNode;
    CircularMenuItem *mAttackerMenuItem;

    osg::ref_ptr<ModelAutoScaler> mAutoScaler;
    std::string mUrl2D;
    std::string mUrl3D;
    MapItem *mMapItem;
    bool mIs3D{false};
    bool mIsHighlight{false};
    bool mIsAttacker{false};
    bool mIsAutoScale{true};
    bool mIsSelected{false};
    bool mIsMenuVisible{false};
    NodeData* mNodeData{nullptr};
    osgEarth::Color mColor{osgEarth::Color::White};
    NodeInformation* mNodeInformation{nullptr};
    bool mIsBookmarked{false};
    QQmlEngine *mEnigine{nullptr};
    BookmarkManager *mBookmarkManager;
    BookmarkItem *mBookmarkItem{nullptr};
    CircularMenu *mCircularMenu{nullptr};
    CircularMenuItem *mBookmarkMenuItem{nullptr};

private:
    static QMap<std::string, osg::ref_ptr<osg::Node>> mNodes3D;
    static QMap<std::string, osg::ref_ptr<osg::Image>> mImages2D;
};

#endif // SIMPLEMODELNODE_H

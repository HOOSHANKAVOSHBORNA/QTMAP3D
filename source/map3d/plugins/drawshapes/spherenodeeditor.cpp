#include "spherenodeeditor.h"

#include <osgEarth/GeoMath>

class SphereRadiusCallback : public osgEarth::Annotation::Dragger::PositionChangedCallback
{
public:
    SphereRadiusCallback(SphereNode* node, SphereNodeEditor* editor):
        mNode(node),
        mEditor(editor)
    {
    }
    ~SphereRadiusCallback() override { }

    void onPositionChanged(const osgEarth::Annotation::Dragger* /*sender*/, const osgEarth::GeoPoint& position) override
    {
        //        osg::Vec3d wordPos;
        //        position.toWorld(wordPos);
        //        osg::Vec3d wordCenter;
        //        mNode->getPosition().toWorld(wordCenter);
        //        double distance = (wordPos - wordCenter).length();
        const osg::EllipsoidModel* em = mNode->getMapNode()->getMapSRS()->getEllipsoid();

        osgEarth::GeoPoint radiusLocation(position);
        radiusLocation.makeGeographic();

        //Figure out the distance between the center of the circle and this new location
        osgEarth::GeoPoint center = mNode->getPosition();
        center.makeGeographic();

        double distance = osgEarth::GeoMath::distance(osg::DegreesToRadians( center.y() ), osg::DegreesToRadians( center.x() ),
                                                      osg::DegreesToRadians( radiusLocation.y() ), osg::DegreesToRadians( radiusLocation.x() ),
                                                      em->getRadiusEquator());
        mNode->setRadius( osgEarth::Distance(distance, osgEarth::Units::METERS ) );
        mEditor->computeBearing();

    }
private:
    SphereNode* mNode{nullptr};
    SphereNodeEditor* mEditor{nullptr};
};

SphereNodeEditor::SphereNodeEditor(SphereNode *sphereNode):
    osgEarth::Annotation::GeoPositionNodeEditor( sphereNode ),
    mRadiusDragger( nullptr ),
    mSphereNode(sphereNode)
{
    mRadiusDragger  = new osgEarth::Annotation::SphereDragger(_node->getMapNode());
    mRadiusDragger->addPositionChangedCallback(new SphereRadiusCallback(sphereNode, this));
    mRadiusDragger->setColor(osg::Vec4(0,0,1,1));
    addChild(mRadiusDragger);
    updateDraggers();

    //    getPositionDragger()->setDefaultDragMode(osgEarth::Annotation::Dragger::DRAGMODE_VERTICAL);
}

void SphereNodeEditor::computeBearing()
{
    //--update z location------------------------------------------
    if(mSphereNode->getSphereShape() != SphereNode::SphereTopHalf)
    {
        osgEarth::GeoPoint location = mRadiusDragger->getPosition();
        double r = mSphereNode->getRadius().as(osgEarth::Units::METERS);
        location.z() = r;
        mRadiusDragger->setPosition(location, false);
    }
    //------------------------------------------------------------
    mBearing = osg::DegreesToRadians( 90.0 );

    //Get the radius dragger's position
    if ( mRadiusDragger->getPosition().isValid() )
    {
        // Get the current location of the center of the circle (in lat/long)
        osgEarth::GeoPoint location = _node->getPosition();
        location.makeGeographic();

        // location of the radius dragger (in lat/long)
        osgEarth::GeoPoint radiusLocation = mRadiusDragger->getPosition();
        if ( !radiusLocation.getSRS()->isGeographic() )
            radiusLocation = radiusLocation.transform( location.getSRS() );

        // calculate the bearing b/w the
        mBearing = osgEarth::GeoMath::bearing(
                    osg::DegreesToRadians(location.y()), osg::DegreesToRadians(location.x()),
                    osg::DegreesToRadians(radiusLocation.y()), osg::DegreesToRadians(radiusLocation.x()));
    }
}

void SphereNodeEditor::updateDraggers()
{
    GeoPositionNodeEditor::updateDraggers();
    if (mRadiusDragger && mSphereNode->getMapNode())
    {
        const osg::EllipsoidModel* em = mSphereNode->getMapNode()->getMapSRS()->getEllipsoid();

        // Get the current location of the center of the circle (in lat/long, absolute Z)
        osgEarth::GeoPoint location = mSphereNode->getPosition();
        location.makeGeographic();

        //Get the radius of the sphere in meters
        double r = mSphereNode->getRadius().as(osgEarth::Units::METERS);

        double lat, lon;
        osgEarth::GeoMath::destination(
                    osg::DegreesToRadians(location.y()), osg::DegreesToRadians(location.x()),
                    mBearing, r, lat, lon, em->getRadiusEquator());

        osgEarth::GeoPoint draggerLocation(
                    location.getSRS(),
                    osg::RadiansToDegrees(lon),
                    osg::RadiansToDegrees(lat));

        if(mSphereNode->getSphereShape() == SphereNode::SphereTopHalf)
            draggerLocation.z() = 0;
        else
            draggerLocation.z() = r;

        mRadiusDragger->setPosition( draggerLocation, false );
    }
    //    double radius = mSphereNode->getRadius().getValue();
    //    osg::Vec3d wordPos;
    //    getPositionDragger()->getPosition().toWorld(wordPos);
    //    wordPos += osg::Vec3d(-radius, -radius, -radius);
    //    osgEarth::GeoPoint draggerPosition;
    //    draggerPosition.fromWorld(mSphereNode->getPosition().getSRS(), wordPos);

    //    mRadiusDragger->setPosition(draggerPosition, false);
    //    getPositionDragger()->setPosition()
}

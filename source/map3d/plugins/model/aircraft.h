#ifndef FLYINGMODEL_H
#define FLYINGMODEL_H

#include "basemodel.h"
#include "mapcontroller.h"
#include <plugininterface.h>

#include <osgEarthAnnotation/ModelNode>
#include <osgEarthAnnotation/PlaceNode>
#include <osgParticle/FireEffect>
#include <osgParticle/SmokeTrailEffect>
#include <osgParticle/SmokeEffect>
#include <osgParticle/ExplosionEffect>
#include <osgParticle/ExplosionDebrisEffect>

#include <QObject>
#include <QMouseEvent>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

class MapAnimationPathCallback;
class EventCallback;
class Aircraft: public BaseModel
{
    Q_OBJECT
public:
    struct Information
    {
        QString TN;
        QString IFFCode;
        QString CallSign;
        QString Type;
        QString MasterRadar;
        QString Identification;
        QString IdentificationMethod;
        QString Time;
        QString Pos;

        QString Latitude;
        QString Longitude;
        QString Altitude;
        QString Heading;
        QString Speed;

        QStringList DetectionSystems;
        QStringList Sends;

        QJsonDocument toJson()
        {
            QJsonObject jsonObject;
            jsonObject.insert("TN", TN);
            jsonObject.insert("IFFCode", IFFCode);
            jsonObject.insert("CallSign", CallSign);
            jsonObject.insert("Type", Type);
            jsonObject.insert("MasterRadar", MasterRadar);
            jsonObject.insert("Identification", Identification);// F, K, Z, X, U, H
            jsonObject.insert("IdentificationMethod", IdentificationMethod);//3 char
            jsonObject.insert("Time", Time);//epoch
            jsonObject.insert("Pos", Pos);

            jsonObject.insert("Latitude", Latitude);
            jsonObject.insert("Longitude", Longitude);
            jsonObject.insert("Altitude", Altitude);//meter
            jsonObject.insert("Heading", Heading);
            jsonObject.insert("Speed", Speed);//m/s
            //
            QJsonArray detectSystems;
            for(auto detectSystem: DetectionSystems)
                detectSystems.push_back(detectSystem);
            jsonObject.insert("DetectionSystem", detectSystems);
            //
            QJsonArray sends;
            for(auto send: Sends)
                sends.push_back(send);
            jsonObject.insert("Send", sends);

            QJsonDocument jsonDoc;
            jsonDoc.setObject(jsonObject);
            return jsonDoc;
        }

        void fromJson(QJsonDocument jsonDoc)
        {
            QJsonObject data = jsonDoc.object();
            TN = data.value("TN").toString();
            IFFCode = data.value("IFFCode").toString();
            CallSign = data.value("CallSign").toString();
            Type = data.value("Type").toString();
            MasterRadar = data.value("MasterRadar").toString();
            Identification = data.value("Identification").toString();
            IdentificationMethod = data.value("IdentificationMethod").toString();
            Time = data.value("Time").toString();
            Pos = data.value("Pos").toString();

            Latitude = data.value("Latitude").toString();
            Longitude = data.value("Longitude").toString();
            Altitude = data.value("Altitude").toString();
            Heading = data.value("Heading").toString();
            Speed = data.value("Speed").toString();

            QJsonArray detectSystems = data.value("DetectionSystems").toArray();
            QStringList DetectionSystems;
            QStringList Sends;

        }
    };

public:
    Aircraft(MapController *mapControler, QQmlEngine *qmlEngine, UIHandle* uiHandle, QObject* parent = nullptr);
    void flyTo(const osg::Vec3d& pos, double heading, double speed);
    void stop() override;
    void setTruckModel(osgEarth::Annotation::ModelNode* truckModel);
    osgEarth::Annotation::ModelNode *getTruckModel() const;
    void setInformation(Information info);
    Q_INVOKABLE
    void iw2D3DButtonClicked();
    Q_INVOKABLE
    void iwRouteButtonClicked();
    Q_INVOKABLE
    void iwFollowButtonClicked();
    Q_INVOKABLE
    void iwMoreButtonClicked();
public slots:
    void onModeChanged(bool is3DView);
    void onContextmenuItemClicked(int index, QString systemName);
public:
    virtual void frameEvent()override;
    virtual void mousePressEvent(QMouseEvent *event, bool onModel) override;
    virtual void curentPosition(osgEarth::GeoPoint pos) override;
private:
    void addEffect(double emitterDuration);
    void removeEffect();
private:
    MapController* mMapController{nullptr};
    ModelAnimationPathCallback* mAnimationPathCallback{nullptr};
    osgEarth::Annotation::ModelNode* mTruckModel;
    //    osg::ref_ptr<osg::Geode> mGeodeParticle;
    osg::ref_ptr<osgParticle::SmokeTrailEffect> mSmoke;
    osg::ref_ptr<osgParticle::FireEffect> mFire;

    bool mIsStop{false};
    bool mIsRoute{false};
    UIHandle* mUIHandle;
    Information mInformation;
    osg::ref_ptr<osg::Vec3Array> mLocationPoints;
    osg::ref_ptr<osg::Vec3Array> mTempLocationPoints;
    QQmlEngine *mQmlEngine;
    QQuickItem *mCurrentContextMenuItem;
};

#endif // FLYINGMODEL_H

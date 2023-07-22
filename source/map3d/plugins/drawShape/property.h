#ifndef PROPERTY_H
#define PROPERTY_H

#include <QObject>
#include <QQuickItem>
#include <QColor>

class Property :  public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(bool fillColorStatus   READ getFillColorStatus WRITE setFillColorStatus    NOTIFY propretyChanged)
    Q_PROPERTY(QColor fillColor       READ getFillColor       WRITE setFillColor          NOTIFY propretyChanged)


    Q_PROPERTY(bool strokeStatus      READ getStrokeStatus     WRITE setStrokeStatus      NOTIFY propretyChanged)
    Q_PROPERTY(QColor stroke          READ getStroke           WRITE setStroke            NOTIFY propretyChanged)
    Q_PROPERTY(double strokeWidth     READ getStrokeWidth      WRITE setStrokeWidth       NOTIFY propretyChanged)


    Q_PROPERTY(bool locationStatus    READ getLocationStatus   WRITE setLocationStatus    NOTIFY propretyChanged)
    Q_PROPERTY(QVector3D location     READ getLocation         WRITE setLocation          NOTIFY propretyChanged)
    Q_PROPERTY(bool locationRelative  READ getLocationRelative WRITE setLocationRelative  NOTIFY propretyChanged)


    Q_PROPERTY(bool centerStatus      READ getCenterStatus     WRITE setCenterStatus      NOTIFY propretyChanged)
    Q_PROPERTY(QVector3D center       READ getCenter           WRITE setCenter            NOTIFY propretyChanged)


    Q_PROPERTY(bool arcStatus         READ getArcStatus                  WRITE setArcStatus         NOTIFY propretyChanged)
    Q_PROPERTY(QVector2D arc          READ getArc                        WRITE setArc               NOTIFY propretyChanged)


    Q_PROPERTY(bool radiusStatus      READ getRadiusStatus               WRITE setRadiusStatus      NOTIFY propretyChanged)
    Q_PROPERTY(double radius          READ getRadius                     WRITE setRadius            NOTIFY propretyChanged)


    Q_PROPERTY(bool heightStatus      READ getHeightStatus               WRITE setHeightStatus      NOTIFY propretyChanged)
    Q_PROPERTY(double heighT          READ getHeight                     WRITE setHeight            NOTIFY propretyChanged)


    Q_PROPERTY(bool lenghtStatus           READ getLenghtStatus          WRITE setLenghtStatus      NOTIFY propretyChanged)
    Q_PROPERTY(double lenghT               READ getLenght                WRITE setLenght            NOTIFY propretyChanged)


    Q_PROPERTY(bool widthStatus            READ getWidthStatus           WRITE setWidthStatus      NOTIFY propretyChanged)
    Q_PROPERTY(double widtH                READ getWidth                 WRITE setWidth            NOTIFY propretyChanged)


    Q_PROPERTY(bool tesselationStatus      READ getTesselationStatus     WRITE setTesselationStatus      NOTIFY propretyChanged)
    Q_PROPERTY(double tesselation          READ getTesselation           WRITE setTesselation            NOTIFY propretyChanged)


    Q_PROPERTY(bool clampStatus      READ getClampStatus     WRITE setClampStatus      NOTIFY propretyChanged)
    Q_PROPERTY(double clamp          READ getClamp           WRITE setClamp            NOTIFY propretyChanged)

    Q_PROPERTY(bool   showLenStatus          READ getShowLenStatus     WRITE setShowLenStatus      NOTIFY propretyChanged)
    Q_PROPERTY(bool   showLen                READ getShowLen           WRITE setShowLen           NOTIFY propretyChanged)

    Q_PROPERTY(bool   bearingStatus          READ getBearingStatus     WRITE setBearingStatus      NOTIFY propretyChanged)
    Q_PROPERTY(bool   bearing                READ getBearing           WRITE setBearing           NOTIFY propretyChanged)

    Q_PROPERTY(bool   showSlopStatus          READ getShowSlopStatus     WRITE setShowSlopStatus      NOTIFY propretyChanged)
    Q_PROPERTY(bool   showSlop                READ getShowSlop           WRITE setShowSlop           NOTIFY propretyChanged)


    Q_PROPERTY(bool pointsStatus      READ getPointsStatus     WRITE setPointsStatus      NOTIFY propretyChanged)
    Q_PROPERTY(QColor points       READ getPoints           WRITE setPoints            NOTIFY propretyChanged)
    Q_PROPERTY(double pointsWidth                READ getPointsWidth                 WRITE setPointsWidth            NOTIFY propretyChanged)
    Q_PROPERTY(bool pointsSmooth      READ getPointsSmooth       WRITE setPointsSmooth          NOTIFY propretyChanged)


public:
    explicit Property(QQuickItem *parent = nullptr);
    // ----------  fill color -------- //


    Q_INVOKABLE bool getFillColorStatus() const;
    void setFillColorStatus(bool status);

    QColor getFillColor() const;
    virtual void setFillColor(const QColor &color);


    // ----------  stroke -------- //
    bool getStrokeStatus() const;
    void setStrokeStatus(bool status);

    double getStrokeWidth () const;
    virtual void setStrokeWidth(const double &opacity);

    QColor getStroke() const;
    virtual void setStroke(const QColor &color);


    // ---------- Location  --------- //
    bool getLocationStatus() const;
    void setLocationStatus (bool status);

    QVector3D getLocation() const;
    virtual void setLocation(const QVector3D &status);

    bool getLocationRelative() const;
    virtual void setLocationRelative(const bool &relative);


    // ---------- Center --------- //

    bool getCenterStatus() const;
    void setCenterStatus(bool center);

    QVector3D getCenter() const;
    virtual void setCenter(const QVector3D &center);

    // ---------- arc --------- //

    bool getArcStatus() const;
    void setArcStatus(bool arc);

    QVector2D getArc() const;
    virtual void setArc(const QVector2D &arc);


    // ---------- radius  --------- //

    bool getRadiusStatus() const;
    void setRadiusStatus(bool radius);

    double getRadius() const;
    virtual void setRadius(const double &radius);

    // ---------- height  --------- //

    bool getHeightStatus() const;
    void setHeightStatus(bool height);

    double getHeight() const;
    virtual void setHeight(const double &height);


    // ---------- Lenght  --------- //

    bool getLenghtStatus() const;
    void setLenghtStatus(bool lenght);

    double getLenght() const;
    virtual void setLenght(const double &lenght);


    // ---------- Width  --------- //

    bool getWidthStatus() const;
    void setWidthStatus(bool width);

    double getWidth() const;
    virtual void setWidth(const double &width);


    // ---------- Tesselation  --------- //

    bool getTesselationStatus() const;
    void setTesselationStatus(bool tesselation);

    double getTesselation() const;
    virtual void setTesselation(const double &tesselation);


    // ---------- Clamp  --------- //

    bool getClampStatus() const;
    void setClampStatus(bool clamp);

    double getClamp() const;
    virtual void setClamp(const int &clamp);


    // ---------- showlen  --------- //

    bool getShowLenStatus() const;
    void setShowLenStatus(bool clamp);

    bool getShowLen() const;
    virtual void setShowLen(const bool &clamp);


    // ---------- Bearing  --------- //

    bool getBearingStatus() const;
    void setBearingStatus(bool bearing);

    bool getBearing() const;
    virtual void setBearing(const bool &bearing);


    // ---------- showSlop  --------- //

    bool getShowSlopStatus() const;
    void setShowSlopStatus(bool slop);

    bool getShowSlop() const;
    virtual void setShowSlop(const bool &slop);


    // ---------- Points --------- //

    bool getPointsStatus() const;
    void setPointsStatus(bool point);

    QColor getPoints() const;
    virtual void setPoints(const QColor &point);

    double getPointsWidth() const;
    virtual void setPointsWidth(const double &point);

    bool getPointsSmooth() const;
    virtual void setPointsSmooth(const bool &point);


signals:
    void propretyChanged();



private:
    // ----------  fill color -------- //
    bool mFillColorStatus            {true};
    QColor mFillColor = QColor(0, 0, 250, 255);


    // ----------  stroke //
    bool mStrokeStatus               {false};
    QColor mStroke;
    double  mStrokeWidth;

    // ---------- Location- //
    bool mLocationStatus             {false};
    QVector3D mLocation;
    bool mRelative;


    // ---------- Center  //

    bool mCenterStatus               {false};
    QVector3D mCenter ;

    // ---------- Arc  //

    bool mArcStatus                  {false};
    QVector2D mArc ;

    // ---------- radius //

    bool mRadiusStatus               {false};
    double mRadius;

    // ---------- height //

    bool mHeightStatus               {false};
    double mHeight;


    // ---------- lenght //

    bool mLenghtStatus               {false};
    double mLenght;

    // ---------- width //

    bool mWidthStatus                {false};
    double mWidth;

    // ---------- tesselation //

    bool mTesselationStatus          {false};
    double mTesselation;

    //----------- clamp  //

    bool mClampStatus                {false};
    int mClamp;


    //----------- showlen  //

    bool mShowLenStatus              {false};
    bool  mShowLen;

    //----------- Bearing  //

    bool mBearingStatus              {false};
    bool mBearing;


    //----------- showSlop  //

    bool mShowSlopStatus             {false};
    bool mShowSlop;

    // ---------- Points  //

    bool mPointsStatus               {false};
    QColor mPoints ;
    bool mPointsSmooth;
    double mPointsWidth;




};

#endif // PROPERTY_H

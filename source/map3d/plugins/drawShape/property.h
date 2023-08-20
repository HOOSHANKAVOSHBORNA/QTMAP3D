#ifndef PROPERTY_H
#define PROPERTY_H

#include <QObject>
#include <QQuickItem>
#include <QColor>

class Property :  public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QString name          READ getName               WRITE setName               NOTIFY propretyChanged)
    Q_PROPERTY(bool fillColorStatus   READ getFillColorStatus WRITE setFillColorStatus    NOTIFY propretyChanged)
    Q_PROPERTY(QColor fillColor       READ getFillColor       WRITE setFillColor          NOTIFY propretyChanged)


    Q_PROPERTY(bool strokeStatus      READ getStrokeStatus     WRITE setStrokeStatus      NOTIFY propretyChanged)
    Q_PROPERTY(QColor strokeColor     READ getStrokeColor      WRITE setStrokeColor       NOTIFY propretyChanged)
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


    Q_PROPERTY(bool radiusMinorStatus      READ getRadiusMinorStatus               WRITE setRadiusMinorStatus      NOTIFY propretyChanged)
    Q_PROPERTY(double radiusMinor          READ getRadiusMinor                     WRITE setRadiusMinor            NOTIFY propretyChanged)


    Q_PROPERTY(bool radiusMajorStatus      READ getRadiusMajorStatus               WRITE setRadiusMajorStatus      NOTIFY propretyChanged)
    Q_PROPERTY(double radiusMajor          READ getRadiusMajor                     WRITE setRadiusMajor            NOTIFY propretyChanged)


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
    Q_PROPERTY(bool   showBearing            READ getShowBearing       WRITE setShowBearing        NOTIFY propretyChanged)

    Q_PROPERTY(bool   showSlopStatus          READ getShowSlopStatus     WRITE setShowSlopStatus      NOTIFY propretyChanged)
    Q_PROPERTY(bool   showSlop                READ getShowSlop           WRITE setShowSlop           NOTIFY propretyChanged)


    Q_PROPERTY(bool pointsStatus      READ getPointsStatus     WRITE setPointsStatus      NOTIFY propretyChanged)
    Q_PROPERTY(bool pointsVisible     READ getPointsVisible    WRITE setPointsVisible     NOTIFY propretyChanged)
    Q_PROPERTY(QColor pointsColor     READ getPointsColor      WRITE setPointsColor       NOTIFY propretyChanged)
    Q_PROPERTY(double pointsWidth     READ getPointsWidth      WRITE setPointsWidth       NOTIFY propretyChanged)
    Q_PROPERTY(bool pointsSmooth      READ getPointsSmooth     WRITE setPointsSmooth      NOTIFY propretyChanged)


public:
    explicit Property(QQuickItem *parent = nullptr);


    // ----------  fill color -------- //


    Q_INVOKABLE bool getFillColorStatus() const;
    void setFillColorStatus(bool status);

    QColor getFillColor() const;
    virtual void setFillColor(const QColor &color);


    // -------- name --------------//
    QString getName() const;
    virtual void setName(const QString& name);

    // ----------  stroke -------- //
    bool getStrokeStatus() const;
    void setStrokeStatus(bool status);

    double getStrokeWidth () const;
    virtual void setStrokeWidth(double width);

    QColor getStrokeColor() const;
    virtual void setStrokeColor(const QColor &color);


    // ---------- Location  --------- //
    bool getLocationStatus() const;
    void setLocationStatus (bool status);

    QVector3D getLocation() const;
    virtual void setLocation(const QVector3D &location);

    bool getLocationRelative() const;
    virtual void setLocationRelative(bool relative);


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
    virtual void setRadius(double radius);

    // ---------- RadiusMinor  --------- //

    bool getRadiusMinorStatus() const;
    void setRadiusMinorStatus(bool radius);

    double getRadiusMinor() const;
    virtual void setRadiusMinor(double radius);


    // ---------- RadiusMajor  --------- //

    bool getRadiusMajorStatus() const;
    void setRadiusMajorStatus(bool radius);

    double getRadiusMajor() const;
    virtual void setRadiusMajor(double radius);


    // ---------- height  --------- //

    bool getHeightStatus() const;
    void setHeightStatus(bool status);

    double getHeight() const;
    virtual void setHeight(double height);


    // ---------- Lenght  --------- //

    bool getLenghtStatus() const;
    void setLenghtStatus(bool status);

    double getLenght() const;
    virtual void setLenght(double lenght);


    // ---------- Width  --------- //

    bool getWidthStatus() const;
    void setWidthStatus(bool status);

    double getWidth() const;
    virtual void setWidth(double width);


    // ---------- Tesselation  --------- //

    bool getTesselationStatus() const;
    void setTesselationStatus(bool status);

    double getTesselation() const;
    virtual void setTesselation(double tesselation);


    // ---------- Clamp  --------- //

    bool getClampStatus() const;
    void setClampStatus(bool status);

    int getClamp() const;
    virtual void setClamp(int clamp);


    // ---------- showlen  --------- //

    bool getShowLenStatus() const;
    void setShowLenStatus(bool status);

    bool getShowLen() const;
    virtual void setShowLen(bool showLen);


    // ---------- Bearing  --------- //

    bool getBearingStatus() const;
    void setBearingStatus(bool status);

    bool getShowBearing() const;
    virtual void setShowBearing(bool showBearing);


    // ---------- showSlop  --------- //

    bool getShowSlopStatus() const;
    void setShowSlopStatus(bool status);

    bool getShowSlop() const;
    virtual void setShowSlop(bool showSlop);


    // ---------- Points --------- //

    bool getPointsStatus() const;
    void setPointsStatus(bool status);

    bool getPointsVisible() const;
    virtual void setPointsVisible(bool visible);

    QColor getPointsColor() const;
    virtual void setPointsColor(const QColor &pointColor);

    double getPointsWidth() const;
    virtual void setPointsWidth(double pointWidth);

    bool getPointsSmooth() const;
    virtual void setPointsSmooth(bool point);


    QQuickItem *item() const;

signals:
    void propretyChanged();
protected:
//    void createProperty(QString name, QVariant property, QQmlEngine *qmlEngine);


private:
    QQuickItem *mItem;
    // ----------  fill color -------- //
    bool mFillColorStatus{true};
    QColor mFillColor = QColor(0, 0, 250, 255);
    QString mName;


    // ----------  stroke //
    bool mStrokeStatus{false};
    QColor mStroke = QColor(0, 250, 0, 255);
    double  mStrokeWidth{7};

    // ---------- Location- //
    bool mLocationStatus{false};
    QVector3D mLocation;
    bool mRelative;


    // ---------- Center  //

    bool mCenterStatus{false};
    QVector3D mCenter ;

    // ---------- Arc  //

    bool mArcStatus{false};
    QVector2D mArc= QVector2D(0,360) ;

    // ---------- radius //

    bool mRadiusStatus{false};
    double mRadius{1000};

    // ---------- RadiusMinor //

    bool mRadiusMinorStatus{false};
    double mRadiusMinor{10};

    // ---------- RadiusMajor //

    bool mRadiusMajorStatus{false};
    double mRadiusMajor{100};

    // ---------- height //

    bool mHeightStatus{false};
    double mHeight {1000};


    // ---------- lenght //

    bool mLenghtStatus{false};
    double mLenght {1000};

    // ---------- width //

    bool mWidthStatus{false};
    double mWidth {1000};

    // ---------- tesselation //

    bool mTesselationStatus{false};
    double mTesselation{1};

    //----------- clamp  //

    bool mClampStatus{false};
    int mClamp{0};


    //----------- showlen  //

    bool mShowLenStatus{false};
    bool  mShowLen{true};

    //----------- Bearing  //

    bool mBearingStatus{false};
    bool mShowBearing{true};


    //----------- showSlop  //

    bool mShowSlopStatus{false};
    bool mShowSlop{true};

    // ---------- Points  //

    bool mPointsStatus{false};
    bool mPointsVisible{false};
    QColor mPointsColor{QColor(250, 0, 0, 255)};
    bool mPointsSmooth{true};
    double mPointsWidth{10};




};

#endif // PROPERTY_H

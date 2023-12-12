#ifndef PROPERTY_H
#define PROPERTY_H

#include <QColor>
#include <QObject>
#include <QVector3D>

class PropertyItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged FINAL)
    Q_PROPERTY(bool isMovable READ isMovable WRITE setIsMovable NOTIFY isMovableChanged FINAL)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged FINAL)
    Q_PROPERTY(QVector3D location READ getLocation WRITE setLocation NOTIFY propretyChanged FINAL)
    Q_PROPERTY(QVector3D moveTo READ getMoveTo WRITE setMoveTo NOTIFY propretyChanged FINAL)
    Q_PROPERTY(double speed READ speed WRITE setSpeed NOTIFY speedChanged FINAL)

public:
    PropertyItem();
    QString name() const;
    void setName(const QString &newName);

    bool isMovable() const;
    void setIsMovable(bool newIsMovable);

    QColor color() const;
    void setColor(const QColor &newColor);

    QVector3D getLocation() const;
    void setLocation(const QVector3D &newLocation);

    QVector3D getMoveTo() const;
    void setMoveTo(const QVector3D &newmoveTo);

    double speed() const;
    void setSpeed(double newSpeed);

signals:
    void nameChanged();

    void isMovableChanged();

    void colorChanged();

    void propretyChanged();

    void speedChanged();

private:
    QString mName;
    bool mIsMovable;
    QColor mColor;
    QVector3D mLocation;
    QVector3D mMoveTo;
    double mSpeed;
};

#endif // PROPERTY_H

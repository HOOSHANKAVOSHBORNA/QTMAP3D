#ifndef PROPERTY_H
#define PROPERTY_H

#include <QColor>
#include <QObject>
#include <QVector3D>

class PropertyItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY propretyChanged FINAL)
    Q_PROPERTY(bool isMovable READ isMovable WRITE setIsMovable NOTIFY propretyChanged FINAL)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY propretyChanged FINAL)
    Q_PROPERTY(QVector3D location READ getLocation WRITE setLocation NOTIFY propretyChanged FINAL)
    Q_PROPERTY(QVector3D moveTo READ getMoveTo WRITE setMoveTo NOTIFY propretyChanged FINAL)
    Q_PROPERTY(double speed READ speed WRITE setSpeed NOTIFY propretyChanged FINAL)

public:
    PropertyItem();
    QString name();
    PropertyItem *setName(const QString &newName);

    bool isMovable() const;
    PropertyItem *setIsMovable(bool newIsMovable);

    QColor color() const;
    PropertyItem *setColor(const QColor &newColor);

    QVector3D getLocation() const;
    PropertyItem *setLocation(const QVector3D &newLocation);

    QVector3D getMoveTo() const;
    PropertyItem *setMoveTo(const QVector3D &newmoveTo);

    double speed() const;
    PropertyItem *setSpeed(double newSpeed);

signals:
    void propretyChanged();

private:
    QString mName;
    bool mIsMovable;
    QColor mColor;
    QVector3D mLocation;
    QVector3D mMoveTo;
    double mSpeed;
};

#endif // PROPERTY_H

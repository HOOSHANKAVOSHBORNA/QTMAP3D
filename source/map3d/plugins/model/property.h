#ifndef PROPERTY_H
#define PROPERTY_H

#include <QColor>
#include <QObject>

class PropertyItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged FINAL)
    Q_PROPERTY(bool isMovable READ isMovable WRITE setIsMovable NOTIFY isMovableChanged FINAL)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged FINAL)

public:
    PropertyItem();
    QString name() const;
    void setName(const QString &newName);

    bool isMovable() const;
    void setIsMovable(bool newIsMovable);

    QColor color() const;
    void setColor(const QColor &newColor);

signals:
    void nameChanged();

    void isMovableChanged();

    void colorChanged();

private:
    QString mName;
    bool mIsMovable;
    QColor m_color;
};

#endif // PROPERTY_H

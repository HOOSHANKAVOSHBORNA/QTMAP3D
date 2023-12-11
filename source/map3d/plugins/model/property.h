#ifndef PROPERTY_H
#define PROPERTY_H

#include <QObject>

class PropertyItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged FINAL)
    Q_PROPERTY(bool isMovable READ isMovable WRITE setIsMovable NOTIFY isMovableChanged FINAL)

public:
    PropertyItem();
    QString name() const;
    void setName(const QString &newName);

    bool isMovable() const;
    void setIsMovable(bool newIsMovable);

signals:
    void nameChanged();

    void isMovableChanged();

private:
    QString mName;
    bool m_isMovable;
};

#endif // PROPERTY_H

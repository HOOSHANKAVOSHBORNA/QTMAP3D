#ifndef PROPERTYITEM_H
#define PROPERTYITEM_H

#include <QObject>

class propertyItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged FINAL)

public:
    propertyItem();
    QString name() const;
    void setName(const QString &newName);

signals:
    void nameChanged();

private:
    QString mName;
};

#endif // PROPERTYITEM_H

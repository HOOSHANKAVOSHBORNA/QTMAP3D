#ifndef FILTERMANAGER_H
#define FILTERMANAGER_H
#include "serviceManager.h"
#include <QObject>
#include <QMap>
#include <QSet>

struct Tag {
    bool equalCheck;
    QVariant value;

    QString comparision;
    QPair<double, double> values;

    bool operator==(const Tag& t) const{
        if (equalCheck == t.equalCheck) {
            return value == t.value;
        }
        return (t.comparision == comparision) && ((t.value == value) || (t.values == values));
    }

};

class FilterManager : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:

    FilterManager(QObject* parent = nullptr);
    void addFilterField(NodeData *nodeData);
    void addFilterField(QString field, QString type);
    bool checkNodeToShow(NodeData *nodeData);

    Q_INVOKABLE void addFilterTag(QString key, QString value);
    Q_INVOKABLE void addFilterTag(QString key, double value, QString comp);
    Q_INVOKABLE void addFilterTag(QString key, double value1, double value2, QString comp);
    Q_INVOKABLE void removeFilterTag(QString key, QString value);
    Q_INVOKABLE void removeFilterTag(QString key, double value, QString comp);
    Q_INVOKABLE void removeFilterTag(QString key, double value1, double value2, QString comp);

    Q_INVOKABLE QSet<QString> getAllFilterFields();
    Q_INVOKABLE QSet<QString> getIntFilterFields();

private:
    QSet<QString> mFilterFields;                    // all tags
    QSet<QString> mFilterFieldsNum;                 // int tags
    QMap<QString, QVector<Tag>> mFilterTags;        // map: < key, condistion>
};

#endif // FILTERMANAGER_H

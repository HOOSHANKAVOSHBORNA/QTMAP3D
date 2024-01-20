#ifndef FILTERMANAGER_H
#define FILTERMANAGER_H
#include "qqmlintegration.h"
#include "qvariant.h"
#include <QObject>
#include <QMap>
#include <QSet>

struct NodeData;

struct Tag {
    enum Comparision{
        Equal,
        NotEqual,
        Greater,
        Less,
        GreaterEqual,
        LessEqual
    };
    enum LogicalOperator{
        And,
        Or
//        Not
    };

    QString field;
    QVariant value;

    Comparision comparision;
    LogicalOperator logicalOperator;

    bool operator==(const Tag& t) const {
//        if (equalCheck == t.equalCheck) {
//            return value == t.value;
//        }
        return (t.comparision == comparision) && ((t.value == value) /*|| (t.value2 == value2)*/);
    }

};

class FilterManager : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QSet<QString> stringFilterFields READ stringFilterFields NOTIFY filterFieldsChanged)
    Q_PROPERTY(QSet<QString> numFilterFields READ numFilterFields NOTIFY filterFieldsChanged)
    Q_PROPERTY(QSet<QString> colorFilterFields READ colorFilterFields NOTIFY filterFieldsChanged)
public:

    FilterManager(QObject* parent = nullptr);
    void addFilterField(NodeData *nodeData);
    bool checkNodeToShow(NodeData *nodeData);

    Q_INVOKABLE void addFilterTag(QString key, QString value);
    Q_INVOKABLE void addFilterTag(QString key, double value, QString comp);
    Q_INVOKABLE void addFilterTag(QString key, double value1, double value2, QString comp);

    Q_INVOKABLE void removeFilterTag(QString key, QString value);
    Q_INVOKABLE void removeFilterTag(QString key, double value, QString comp);
    Q_INVOKABLE void removeFilterTag(QString key, double value1, double value2, QString comp);

    QSet<QString> stringFilterFields() const;
    QSet<QString> colorFilterFields() const;
    QSet<QString> numFilterFields() const;

signals:
    void filterFieldsChanged();
    void filterTagsEdited();
private:
    void addFilterField(QString field, QVariant value);

private:
    QSet<QString> mFilterFieldsColor;               // color fields
    QSet<QString> mFilterFieldsStr;                    // all   fields
    QSet<QString> mFilterFieldsNum;                 // int   fields
    QMap<QString, QVector<Tag>> mFilterTags;        // map: < key, condistion>
};

#endif // FILTERMANAGER_H

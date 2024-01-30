#ifndef FILTERMANAGER_H
#define FILTERMANAGER_H
#include "qqmlintegration.h"
#include "qvariant.h"
#include <QObject>
#include <QMap>
#include <QSet>
#include <QQmlEngine>

struct NodeData;

struct Tag : public QObject {
    Q_OBJECT
public:
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
    Q_ENUM(LogicalOperator)
    Q_ENUM(Comparision)

    Tag(QString field, QVariant value, Comparision comp = Greater, LogicalOperator op = And, QObject* parent = nullptr):
        QObject(parent),
        field(field),
        value(value),
        comparision(comp),
        logicalOperator(op)
    {}

    QString field;
    QVariant value;

    Comparision comparision;
    LogicalOperator logicalOperator;

    bool operator==(const Tag* t) const {
        return (t->field == field) && (t->comparision == comparision) && (t->value == value);
    }

};

class FilterManager : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QVector<QString> stringFilterFields READ stringFilterFields NOTIFY filterFieldsChanged)
    Q_PROPERTY(QVector<QString> numFilterFields READ numFilterFields NOTIFY filterFieldsChanged)
    Q_PROPERTY(QVector<QString> colorFilterFields READ colorFilterFields NOTIFY filterFieldsChanged)
    Q_PROPERTY(QVector<Tag*> filterTags READ getFilterTags NOTIFY filterTagsEdited)
public:

    FilterManager(QObject* parent = nullptr);
    void addFilterField(NodeData *nodeData);
    bool checkNodeToShow(NodeData *nodeData);

    Q_INVOKABLE void addFilterTag(QString field, QVariant value, Tag::Comparision comp, Tag::LogicalOperator op = Tag::And);
    Q_INVOKABLE void removeFilterTag(QString field, QVariant value, Tag::Comparision comp, Tag::LogicalOperator op = Tag::And);
    Q_INVOKABLE const QVector<Tag*> getFilterTags() const;

    const QVector<QString> &stringFilterFields() const;
    const QVector<QString> &colorFilterFields() const;
    const QVector<QString> &numFilterFields() const;

signals:
    void filterFieldsChanged();
    void filterTagsEdited();
private:
    void addFilterField(QString field, QVariant value);
    bool checkNodeToShow(NodeData *nodeData, Tag* tag);

private:
    QVector<QString> mFilterFieldsColor;               // color fields
    QVector<QString> mFilterFieldsStr;                    // all   fields
    QVector<QString> mFilterFieldsNum;                 // int   fields
    QVector<Tag*> mFilterTags;        // map: < key, condistion>
};

#endif // FILTERMANAGER_H

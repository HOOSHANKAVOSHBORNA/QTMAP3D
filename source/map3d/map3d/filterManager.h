#ifndef FILTERMANAGER_H
#define FILTERMANAGER_H
#include "qqmlintegration.h"
#include "qsettings.h"
#include "qvariant.h"
#include <QObject>
#include <QMap>
#include <QSet>
#include <QQmlEngine>
#include <QSortFilterProxyModel>

struct NodeData;

class FilterFieldModel: public QAbstractListModel
{
    Q_OBJECT
public:
    FilterFieldModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void addFilterField(QString field);
    void addFilterField(QSet<QString> &fields);
    void setFilterField(QSet<QString> &fields);
    void removeFilterField(QString field);
private:
    QVector<QString> mFilterFields;
};


struct Tag : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString field MEMBER field CONSTANT)
    Q_PROPERTY(QVariant value MEMBER value CONSTANT)
    Q_PROPERTY(bool isEnabled MEMBER isEnabled NOTIFY tagChanged)
    Q_PROPERTY(Comparision comparision MEMBER comparision CONSTANT)
    Q_PROPERTY(LogicalOperator logicalOperator MEMBER logicalOperator CONSTANT)

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
        And = 0,
        Or = 1
//        Not
    };
    Q_ENUM(LogicalOperator)
    Q_ENUM(Comparision)

    Tag(QString field, QVariant value, Comparision comp = Greater, LogicalOperator op = And, bool isEnabled = true, QObject* parent = nullptr):
        QObject(parent),
        field{field},
        value{value},
        comparision{comp},
        logicalOperator{op},
        isEnabled{isEnabled}
    {}

    QString field;
    QVariant value;
    bool isEnabled;
    Comparision comparision;
    LogicalOperator logicalOperator;

    Q_INVOKABLE void toggleIsEnabled() {
        isEnabled = !isEnabled;
        emit tagChanged();
    }
    Q_INVOKABLE QString comparisionToString() const {
        switch (comparision) {
        case Comparision::Equal:
            return "=";
        case Comparision::NotEqual:
            return "!=";
        case Comparision::Greater:
            return ">";
        case Comparision::Less:
            return "<";
        case Comparision::GreaterEqual:
            return ">=";
        case Comparision::LessEqual:
            return "<=";
        }
        return "";
    }

    static Comparision srtingToComparison(QString s) {
        if (s == "=")
            return Comparision::Equal;
        else if (s == "!=")
            return Comparision::NotEqual;
        else if (s == ">=")
            return Comparision::GreaterEqual;
        else if (s == "<=")
            return Comparision::LessEqual;
        else if (s == ">")
            return Comparision::Greater;
        else
            return Comparision::Greater;
    }

    bool operator==(const Tag* t) const {
        return (t->field.toLower() == field.toLower()) && (t->comparision == comparision) && (t->value == value) && t->logicalOperator == logicalOperator;
    }

signals:
    void tagChanged();
};

class FilterManager : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QSortFilterProxyModel * stringFilterFields READ stringFilterFields NOTIFY filterFieldsChanged)
    Q_PROPERTY(QSortFilterProxyModel * numFilterFields READ numFilterFields NOTIFY filterFieldsChanged)
    Q_PROPERTY(QSortFilterProxyModel * colorFilterFields READ colorFilterFields NOTIFY filterFieldsChanged)
    Q_PROPERTY(QVector<Tag*> filterTags READ getFilterTags NOTIFY filterTagsEdited)
public:

    FilterManager(QObject* parent = nullptr);
    ~FilterManager();
    void addColorFilterField(QSet<QString> &fields);
    void setColorFilterField(QSet<QString> &fields);
    void addColorFilterField(QString fields);
    void addNumFilterField(QSet<QString> &fields);
    void setNumFilterField(QSet<QString> &fields);
    void addNumFilterField(QString fields);
    void addStringFilterField(QSet<QString> &fields);
    void setStringFilterField(QSet<QString> &fields);
    void addStringFilterField(QString fields);

    void removeColorFilterField(QString field);
    void removeNumFilterField(QString field);
    void removeStringFilterField(QString field);
    bool checkNodeToShow(NodeData *nodeData);

    Q_INVOKABLE void addFilterTag(QString field, QVariant value, QString comp, Tag::LogicalOperator op = Tag::And);
    Q_INVOKABLE void removeFilterTag(QString field, QVariant value, QString comp, Tag::LogicalOperator op = Tag::And);
    Q_INVOKABLE void removeFilterTag(int index);
    Q_INVOKABLE const QVector<Tag*> getFilterTags() const;
    Q_INVOKABLE const Tag * const getFilterTagAt(int index);

    Q_INVOKABLE QSortFilterProxyModel *colorFilterFields() const;
    Q_INVOKABLE QSortFilterProxyModel *stringFilterFields() const;
    Q_INVOKABLE QSortFilterProxyModel *numFilterFields() const;

signals:
    void filterFieldsChanged();
    void filterTagsEdited();
private:
    bool checkNodeToShow(NodeData *nodeData, Tag* tag);

private:
    QSortFilterProxyModel *mFilterFieldsColor;
    FilterFieldModel *mFilterFieldsStrModel;
    QSortFilterProxyModel *mFilterFieldsStr;
    FilterFieldModel *mFilterFieldsNumModel;
    QSortFilterProxyModel *mFilterFieldsNum;
    FilterFieldModel *mFilterFieldsColorModel;
    QSettings *mFilterSettings;

    QVector<Tag*> mFilterTags{};
};

#endif // FILTERMANAGER_H

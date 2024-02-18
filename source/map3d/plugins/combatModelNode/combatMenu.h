#ifndef COMBATLISTMODEL_H
#define COMBATLISTMODEL_H

#include "combatManager.h"
#include "mapControllerItem.h"
#include <QObject>

class AssignmentListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum CustomRoles {
        Name = Qt::UserRole + 1,
        Icon,
        State,
        StateColor,
        Select
    };

    explicit AssignmentListModel(CombatManager *combatManager, QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    virtual QHash<int, QByteArray> roleNames() const override;
    void setOperator(SimpleModelNode *node, bool isAttacker);
    Q_INVOKABLE void onAddButtonChecked(bool check);
    Q_INVOKABLE void onRemoveButtonChecked(bool check);
    Q_INVOKABLE void onAttackButtonClicked();
    Q_INVOKABLE void onCancelButtonClicked();
    Q_INVOKABLE void onMenuItemSelect(int row);
    Q_INVOKABLE void onItemHovered(int row, bool hover);
    Q_INVOKABLE void selectAll(bool select);
    QList<Assignment*> getSelectedAssignmentList();

signals:
    void addAssignmentChecked(bool check, SimpleModelNode *node, bool isAttacker);
    void removeAssignmentChecked(bool check, SimpleModelNode *node, bool isAttacker);
    void sendAssignRequested(SimpleModelNode *attacker, SimpleModelNode *target);
    void cancelAssignRequested(SimpleModelNode *attacker, SimpleModelNode *target);
    Q_INVOKABLE void onCloseMenuClicked();

private:
    CombatManager *mCombatManager;
    QList<Assignment*> mAssignmentList;
    QList<Assignment*> mSelectedAssignmentList;
    bool mIsAttacker{false};
    SimpleModelNode *mOperatorNode{nullptr};

};

class OperatorListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString operatorName       READ getOperatorName       WRITE setOperatorName       NOTIFY operatorChanged FINAL)
    Q_PROPERTY(QUrl    operatorIcon       READ getOperatorIcon       WRITE setOperatorIcon       NOTIFY operatorChanged FINAL)
    Q_PROPERTY(bool    operatorIsAttacker READ getOperatorIsAttacker WRITE setOperatorIsAttacker NOTIFY operatorChanged FINAL)

public:
    enum CustomRoles {
        Name = Qt::UserRole + 1,
        Icon,
        Color,
        Select
    };

    explicit OperatorListModel(AssignmentListModel *assignmentListModel, QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    virtual QHash<int, QByteArray> roleNames() const override;
    void addAttacker(SimpleModelNode* node);
    void addTarget(SimpleModelNode* node);
    Q_INVOKABLE void select(int row);
    Q_INVOKABLE void operatorToggle(bool isAttacker);

    QString getOperatorName();
    QUrl getOperatorIcon();
    bool getOperatorIsAttacker();
    void setOperatorName(QString name);
    void setOperatorIcon(QUrl url);
    void setOperatorIsAttacker(bool attacker);

signals:
    void operatorChanged();

private:
    AssignmentListModel *mAssignmentListModel;
    QList<SimpleModelNode*> mAttackerList;
    QList<SimpleModelNode*> mTargetList;
    SimpleModelNode* mSelectedNode{nullptr};
    bool mIsAttacker{false};

    QString mOperatorName;
    QUrl mOperatorIcon;
    int mOperatorIsAttacker;

};

class CombatMenu:public QObject
{
    Q_OBJECT
public:
    explicit CombatMenu(CombatManager *combatManager,
                        QQmlEngine *engine,
                        MapControllerItem *map = nullptr);
    ~CombatMenu();
    void selectAttacker(SimpleModelNode* node);
    void selectTarget(SimpleModelNode* node);
    AssignmentListModel *assignmentListModel() const;

private:
    MapControllerItem *mMapItem;
    AssignmentListModel *mAssignmentListModel;
    OperatorListModel *mOperatorListModel;



};


#endif // COMBATLISTMODEL_H


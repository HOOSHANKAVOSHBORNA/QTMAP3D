#include "combatListModel.h"
#include "qqmlcomponent.h"

CombatListModel::CombatListModel(QObject *parent) {

    mRootItem = invisibleRootItem();
}

QHash<int, QByteArray> CombatListModel::roleNames() const
{
    QHash<int, QByteArray> hash = QAbstractItemModel::roleNames();
    hash[ID] = "objectID";
    hash[icon] = "objectIcon";
    hash[selection] = "objectSelection";
    return hash;
}


CombatList::CombatList(QQmlEngine *Engine, QQuickWindow *parent)
{
        mCombatListModel = new CombatListModel(Engine);

        QQmlComponent* comp = new QQmlComponent(Engine, this);
        QObject::connect(comp, &QQmlComponent::statusChanged, [&](const QQmlComponent::Status &status){
            if(status == QQmlComponent::Error){
                qDebug()<<"Can not load this: "<<comp->errorString();
            }
            if(status == QQmlComponent::Ready){
                mWindow = qobject_cast<QQuickWindow*>(comp->create());
                mWindow->setProperty("combatModel", QVariant::fromValue<CombatListModel*>(mCombatListModel));
                connect(parent, &QQuickWindow::closing, mWindow, &QQuickWindow::close);
            }
        });
        comp->loadUrl(QUrl("qrc:/CombatMenu.qml"));
}

CombatList::~CombatList(){}

CombatListModel *CombatList::model() const
{
    return mCombatListModel;
}

QQuickWindow *CombatList::window() const
{
    return mWindow;
}

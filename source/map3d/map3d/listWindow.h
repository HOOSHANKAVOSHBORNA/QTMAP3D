#ifndef LISTWINDOW_H
#define LISTWINDOW_H

#include <QQuickWindow>
#include <QQuickItem>
#include <QAbstractListModel>

class TabbarModel;
class ListWindow;

// --------------------------------------------- struct
struct TabbarItem
{
public:
    QString title;
    QQuickItem *item;
};

enum {
    TitleRole = Qt::UserRole + 1,
    ItemRolee
};

// --------------------------------------------- model
class TabbarModel : public QAbstractListModel
{
    Q_OBJECT


public:
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void appendRow(TabbarItem *newLocationItem);
    Q_INVOKABLE void removeRow(const QModelIndex &index);

private:
    QVector<TabbarItem *> mTabbarItems;
};

// --------------------------------------------- list window
class ListWindow : public QQuickWindow
{
    Q_OBJECT
    Q_PROPERTY(TabbarModel *tabbarModel READ tabbarModel WRITE setTabbarModel NOTIFY tabbarModelChanged FINAL)

public:
    ListWindow(QQuickWindow *parent = nullptr);
    TabbarModel *tabbarModel() const;
    void setTabbarModel(TabbarModel *newTabbarModel);

    // TEST
    Q_INVOKABLE void appendTest(TabbarItem *newTabbarItem) {
        mTabbarModel->appendRow(newTabbarItem);
    }

    Q_INVOKABLE void appendTest(QString newTitle)
    {
        QQmlComponent *comp = new QQmlComponent(qmlEngine(this));

        QObject::connect(comp, &QQmlComponent::statusChanged, [&](QQmlComponent::Status status) {
            if(status == QQmlComponent::Error) {
                qDebug() << "Can not load this: " << comp->errorString();
            }

            if(status == QQmlComponent::Ready) {
                QQuickItem *item = qobject_cast<QQuickItem*>(comp->create());
                item->setProperty("title", newTitle);

                appendTest(new TabbarItem{newTitle, item});

                //                                    ToolboxManager::createSingletonInstance(nullptr, nullptr)->addPropertyItem(item, QStringLiteral("Test Item"));
            }
        });

        comp->loadUrl(QUrl("qrc:/TestItem.qml"));
    }
    // ENDTEST

signals:
    void tabbarModelChanged();

private:
    TabbarModel *mTabbarModel = nullptr;
};

#endif // LISTWINDOW_H

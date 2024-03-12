#ifndef LOADINGPAGE_H
#define LOADINGPAGE_H
#include <QAbstractListModel>
#include <QTimer>

struct LoadingInfoItem
{
    QString text;
    bool isError;
};

class LoadingPage : public QObject
{
    Q_OBJECT
    Q_PROPERTY(float pluginFraction READ pluginFraction WRITE setpluginFraction NOTIFY pluginFractionChanged)

public:
    explicit LoadingPage(QObject* parent = nullptr);

    Q_INVOKABLE void addItem(const QString &message, bool isError);
    Q_INVOKABLE void removeItem(int index);
    Q_INVOKABLE QString firstItem();
    Q_INVOKABLE bool firstItemErrorStatus();

    float pluginFraction() const;
    void setpluginFraction(float pluginFrac);
    void setPluginsCount(int count);

signals:
    void pluginFractionChanged();
    void itemAdded();

private:
    QVector<LoadingInfoItem> mLoadingDataItem;
    float mPluginFraction{0.0};
    int mPluginsCount{0};
};

#endif // LOADINGPAGE_H

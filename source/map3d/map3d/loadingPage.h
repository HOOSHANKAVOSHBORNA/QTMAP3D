#ifndef LOADINGPAGE_H
#define LOADINGPAGE_H
#include <QAbstractListModel>
#include <QTimer>

struct LoadingInfoItem
{
    QString text;
    bool isError;

    bool operator <(const LoadingInfoItem& item) const {
        return isError < item.isError;
    }

    bool operator ==(const LoadingInfoItem& item) const {
        return isError == item.isError && text == item.text;
    }
};

class LoadingPage : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(float pluginFraction READ pluginFraction WRITE setpluginFraction NOTIFY pluginFractionChanged)
    enum CustomRoles {
       errorStatus = Qt::UserRole,
    };
public:
    explicit LoadingPage(QObject* parent = nullptr);

    Q_INVOKABLE virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QHash<int, QByteArray> roleNames() const override;
    Q_INVOKABLE void addItem(const QString &message, bool isError);
    Q_INVOKABLE void removeItem(int index);
    void swapItem(int sourceIndex, int destinationIndex);
    void updateData(int index);

    float pluginFraction() const;
    void setpluginFraction(float pluginFrac);
    void setPluginsCount(int count);

signals:
    void pluginFractionChanged();

private:
    QVector<LoadingInfoItem> mLoadingDataItem;
    float mPluginFraction{0.0};
    int mPluginsCount{0};
};

#endif // LOADINGPAGE_H

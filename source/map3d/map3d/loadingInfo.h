#ifndef LOADINGINFO_H
#define LOADINGINFO_H
#include <QAbstractListModel>

struct LoadingInfoItem
{
    QString text;
    bool acceptionState;

    bool operator ==(LoadingInfoItem other){
        return (text == other.text && acceptionState == other.acceptionState);
    }
};

class LoadingInfo : public QAbstractListModel
{
    Q_OBJECT
    enum CustomRoles {
       acceptionState = Qt::UserRole,
    };
public:
   explicit LoadingInfo(QObject* parent = nullptr);

   virtual int rowCount(const QModelIndex &parent) const override;
   virtual QVariant data(const QModelIndex &index, int role) const override;
   virtual QHash<int, QByteArray> roleNames() const override;
   Q_INVOKABLE void addItem(const QString &loadingData, bool acceptionState);
   Q_INVOKABLE void removeItem(int index);

private:
   std::vector<LoadingInfoItem> mLoadingDataItem;
};

#endif // LOADINGINFO_H

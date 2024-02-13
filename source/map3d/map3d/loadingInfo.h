#ifndef LOADINGINFO_H
#define LOADINGINFO_H
#include <QAbstractListModel>
#include <QTimer>
struct LoadingInfoItem
{
    QString text;
    bool acceptionState;

    bool operator <(const LoadingInfoItem& item) const {
        return acceptionState < item.acceptionState;
    }

    bool operator ==(const LoadingInfoItem& item) const {
        return acceptionState == item.acceptionState && text == item.text;
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
   Q_INVOKABLE void changeAcceptionState(int index, bool state);
   void swapItem(int sourceIndex, int destinationIndex);
   void updateData(int index);

private:
   std::vector<LoadingInfoItem> mLoadingDataItem;
    QTimer* mTimer;
};

#endif // LOADINGINFO_H

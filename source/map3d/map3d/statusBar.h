#ifndef STATUSBAR_H
#define STATUSBAR_H
#include <QAbstractListModel>
#include <QSortFilterProxyModel>
#include <QItemSelectionModel>
#include <QDateTime>
#include <QObject>

struct Message {
    QString text;
    QDateTime dateTime;
    bool isNew{true};
    bool isCheck{false};
};


class StatusBar: public QAbstractListModel
{
    Q_OBJECT
public:
    enum myRoles{
        messageText = Qt::UserRole + 100,
        dateText,
        timeText,
        textChecked
    };

    StatusBar(QObject *parent = nullptr);
    Q_INVOKABLE int rowCount(const QModelIndex & parent = QModelIndex()) const override;
    QVariant data(const QModelIndex & index,int role =Qt::DisplayRole)const override;
    void removeMessage();
//    void deleteMarkedMessage(const QModelIndex &index);
    void selectAllMessages();
    QHash<int, QByteArray> roleNames() const override;

    void addMessage(Message* m);





private:
    std::vector<Message*> mMessages;
//    bool isAllSelected{false};


    // QAbstractItemModel interface
public:
    Q_INVOKABLE void toggleCheck(const QModelIndex &index, bool check);
};




//----------------------------------------------



class StatusBarSearchModel : public QSortFilterProxyModel
{
    Q_OBJECT

    Q_PROPERTY(int range READ getRange NOTIFY rangeChanged)
    Q_PROPERTY(double scale READ getScale)

public:
    explicit StatusBarSearchModel(QObject *parent = nullptr);
    QString filterString() const;
    int getRange() const;
    double getScale() const;
    void setRange(const int range);
    void setScale(const double scale);
    void addMessage(QString Text, QDateTime);
    Q_INVOKABLE void removeMessage();
    Q_INVOKABLE void toggleCheck(const QModelIndex &index, bool check);
    Q_INVOKABLE void selectAllMessages();


public slots:
    void setFilterString(const QString &filterString);
protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

signals:
    void filterStringChanged();
    void rangeChanged();

private:
    QString mFilterString = "";

    int mRange{2};
    double mScale{1000};
};

#endif // STATUSBAR_H

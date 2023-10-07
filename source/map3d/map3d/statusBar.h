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
        isnewMessage,
        textChecked
    };

//    Q_PROPERTY(QString messagesToShow NOTIFY messagesToShowChanged)

    StatusBar(QObject *parent = nullptr);
    Q_INVOKABLE int rowCount(const QModelIndex & parent = QModelIndex()) const override;
    QVariant data(const QModelIndex & index,int role =Qt::DisplayRole)const override;
    QHash<int, QByteArray> roleNames() const override;

    void addMessage(Message* m);
    void removeMessage();



private:
    std::vector<Message*> mMessages;

    // QAbstractItemModel interface
public:
    Q_INVOKABLE void toggleCheck(const QModelIndex &index, bool check);
    Q_INVOKABLE void isNewMessage(const QModelIndex &index);
};



//---------------------------------------------- PROXYMODEL



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
    void addMessage(QString Text);
    Q_INVOKABLE void removeMessage();
    Q_INVOKABLE void toggleCheck(const QModelIndex &index, bool check);
    Q_INVOKABLE void selectAllMessages(bool check);
    Q_INVOKABLE void isNewMessage(const QModelIndex &index);




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

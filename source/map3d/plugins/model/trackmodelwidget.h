#ifndef TRACKMODELWIDGET_H
#define TRACKMODELWIDGET_H

#include <QJsonValue>
#include <QWidget>
#include <QQuickWidget>

class QQuickWidget;

class TrackModelWidget : public QQuickWidget
{
    Q_OBJECT

public:
    TrackModelWidget(QWidget *parent = nullptr);
    void addModel(QString type, QString name);
    void setModelPosition(QString name,QString type, double latitude , double longitude , double altitude);
    void setClose();
    void setModelInfo(QString type ,QString name, QJsonObject info);
    void setMinimaizeWidget(bool isMax);

public slots:

signals :
    void onModelClicked(QString type, QString name , bool isClick);
protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    QString stringListToString(QStringList key,QStringList value);

private slots:

signals:
    Q_INVOKABLE void modelAdded (QString type ,QString name);
    Q_INVOKABLE void onPin (bool t);

    Q_INVOKABLE void isDock (int height);
    Q_INVOKABLE void modelInfo (QString t ,QString n,QString str);
    Q_INVOKABLE void close();
    Q_INVOKABLE void minimize( bool isMax);
    Q_INVOKABLE void modelPosition(QString t,QString n, double latitude , double longitude , double altitude);
    Q_INVOKABLE void changeSize(bool t);

private:
    QQuickWidget *mQQuickWidget;


};
#endif // TRACKWIDGET_H

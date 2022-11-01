#ifndef OBJECTINFOWIDGET_H
#define OBJECTINFOWIDGET_H

#include <QMainWindow>
#include <QObject>
#include <QtQuickWidgets/QQuickWidget>
#include <QWidget>

class ObjectInfoWidget : public QQuickWidget
{
    Q_OBJECT
public:
    ObjectInfoWidget(QWidget *parent = nullptr);

signals:

public slots:
};

#endif // OBJECTINFOWIDGET_H

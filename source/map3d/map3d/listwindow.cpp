#include "listwindow.h"
#include "application.h"
#include <QTimer>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QQuickItem>
#include <QVariant>

ListWindow::ListWindow(QQuickWindow *parent) :
    QQuickWindow(parent)
{


//    QTimer::singleShot(4000, [this](){
//
//        QQmlComponent *comp = new QQmlComponent(Application::instance()->qmlEngine());
//
//        QObject::connect(comp, &QQmlComponent::statusChanged, [this, comp](){
//
//            QQuickItem *tab = (QQuickItem*)comp->create(nullptr);
//
//            QMetaObject::invokeMethod(this, "addTab",
//                                      Q_ARG(QVariant, QVariant::fromValue<QString>("OOPS")),
//                                      Q_ARG(QVariant, QVariant::fromValue<QQuickItem*>(tab))
//                                      );
//
//        });
//
//        comp->setData(R"(
//        import QtQuick 2.13
//
//        Rectangle {
//        color: 'darkblue'
//        }
//        )", QUrl());
//
    //    });
}

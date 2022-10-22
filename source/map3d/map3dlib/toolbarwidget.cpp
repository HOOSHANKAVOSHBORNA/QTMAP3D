#include "toolbarwidget.h"
#include <QDockWidget>
#include <QQmlEngine>
#include <QQuickWidget>
#include <QQmlContext>
ToolBarWidget::ToolBarWidget(QWidget *parent) : QQuickWidget(parent)
{
    //mQQuickWidget = new QQuickWidget(QUrl(QStringLiteral("qrc:/toolbarwidget/MenuSlide.qml")),this);
    QQmlContext  *context = this->rootContext();
    context->setContextProperty("NamePlugin", this);
    setSource(QUrl("qrc:/toolbarwidget/MenuSlide.qml"));
    this->setResizeMode(QQuickWidget::SizeViewToRootObject);
    this->setAttribute(Qt::WA_AlwaysStackOnTop);
    this->setClearColor(Qt::transparent);
    connect(this ,&ToolBarWidget::onPin,[=](bool t){
        if(t){
            emit isDock(parent->height());
        }
    });


}




void ToolBarWidget::addItem(Category category, QString name, QString icon ,bool checkable)
{
    if(icon == "")
        icon = "qrc:/res/icon/puzzle.png";
    mItemName = name;
    mItemIcon = icon;
    mItemCategory = categoryString(category);
    emit itemAdded(checkable);
}

QVariant ToolBarWidget::getItemName()
{
    return mItemName;
}




QVariant ToolBarWidget::getItemIcon()
{
    return mItemIcon;
}

QVariant ToolBarWidget::getItemCategory()
{
    return mItemCategory;
}

void ToolBarWidget::setClose()
{

    emit isclose();
}

void ToolBarWidget::onGetItemClicked(QString category, QString name, bool ischeck)
{
    emit onItemClicked(categoryEnum(category),name,ischeck);
}



void ToolBarWidget::setSizeWidget(bool t)
{
    emit changeSize(t);
}

void ToolBarWidget::resizeEvent(QResizeEvent *event)
{
        QQuickWidget::resizeEvent(event);
}

QString ToolBarWidget::categoryString(ToolBarWidget::Category category)
{
    switch (category) {
    case File:
        return "File";
    case Draw:
        return "Draw";
    case Image:
        return "Image";
    case Model:
        return "Model";
    case Effect:
        return "Effect";
    case Feature:
        return "Feature";
    case Measure:
        return "Measure";
    case Terrain:
        return "Terrain";
    case Analyze:
        return "Analyze";
    case Other:
        return "Other";
    }
    return "Other";
}

ToolBarWidget::Category ToolBarWidget::categoryEnum(QString category)
{
    if( category == "File")
        return File;
    else if( category == "Draw")
        return Draw;
    else if( category == "Image")
        return Image;
    else if( category == "Model")
        return Model;
    else if( category == "Effect")
        return Effect;
    else if( category == "Feature")
        return Feature;
    else if( category == "Measure")
        return Measure;
    else if( category == "Terrain")
        return Terrain;
    else if( category == "Analyze")
        return Analyze;
    return Other;
}

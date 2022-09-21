#include "toolbarwidget.h"
#include <QDockWidget>
#include <QQmlEngine>
#include <QQuickWidget>
#include <QQmlContext>

ToolBarWidget::ToolBarWidget(QWidget *parent)
    : QWidget(parent)

{
    mQQuickWidget = new QQuickWidget(QUrl(QStringLiteral("qrc:/toolbarwidget/MenuSlide.qml")),this);
    mQQuickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    mQQuickWidget->setAttribute(Qt::WA_AlwaysStackOnTop);
    mQQuickWidget->setClearColor(Qt::transparent);
    mQQuickWidget->resize(200, 450);
    mQQuickWidget->raise();

    mQQuickWidget->engine()->rootContext()->setContextProperty("NamePlugin",this);
    setMinimumSize(200,450);
    setMaximumSize(200,450);

}




void ToolBarWidget::addItem(Category category, QString name, QString icon)
{
    if(icon == "")
        icon = "qrc:/res/icon/puzzle.png";
    mItemName = name;
    mItemIcon = icon;
    mItemCategory = categoryString(category);
    emit itemAdded();
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

void ToolBarWidget::onGetItemClicked(QString category, QString name)
{
    emit onItemClicked(categoryEnum(category),name);
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

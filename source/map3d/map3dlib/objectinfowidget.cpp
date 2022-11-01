
#include "objectinfowidget.h"

ObjectInfoWidget::ObjectInfoWidget(QWidget *parent) :
    QQuickWidget(parent)
{

    setSource(QUrl("qrc:///ObjectInfoItem.qml"));
    setClearColor(QColor("transparent"));

    setResizeMode(QQuickWidget::ResizeMode::SizeRootObjectToView);
    setAttribute(Qt::WA_AlwaysStackOnTop);
    resize(300, 200);
    raise();

    setFocusPolicy(Qt::FocusPolicy::NoFocus);

}

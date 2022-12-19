#ifndef LISTWINDOW_H
#define LISTWINDOW_H

#include "QQuickWindow"

class ListWindow : public QQuickWindow
{
    Q_OBJECT

public:
    ListWindow(QQuickWindow *parent = nullptr);
};

#endif // LISTWINDOW_H

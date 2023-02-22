#ifndef LISTWINDOW_H
#define LISTWINDOW_H

#include "QQuickWindow"

class ListWindow : public QQuickWindow
{
    Q_OBJECT

public:
    ListWindow(QQuickWindow *parent = nullptr);

signals:
    void tabChanged(int indx);
};

#endif // LISTWINDOW_H

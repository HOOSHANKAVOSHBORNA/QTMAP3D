#ifndef URLDIALOG_H
#define URLDIALOG_H

#include <QDialog>

namespace Ui {
class URLDialog;
}

class URLDialog : public QDialog
{
    Q_OBJECT

public:
    explicit URLDialog(QMap<QString, QString> example, QWidget *parent = nullptr);

    ~URLDialog();

    QString getURL() const;

private:
    QString mSelectedText;
    Ui::URLDialog *ui;
};

#endif // URLDIALOG_H

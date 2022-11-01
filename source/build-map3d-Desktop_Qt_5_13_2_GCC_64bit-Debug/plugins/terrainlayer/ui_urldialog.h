/********************************************************************************
** Form generated from reading UI file 'urldialog.ui'
**
** Created by: Qt User Interface Compiler version 5.13.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_URLDIALOG_H
#define UI_URLDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_URLDialog
{
public:
    QVBoxLayout *verticalLayout;
    QFormLayout *formLayout;
    QLineEdit *lineEdit;
    QComboBox *comboBox;
    QLabel *label;
    QLabel *label_2;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *URLDialog)
    {
        if (URLDialog->objectName().isEmpty())
            URLDialog->setObjectName(QString::fromUtf8("URLDialog"));
        URLDialog->resize(400, 128);
        verticalLayout = new QVBoxLayout(URLDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        formLayout = new QFormLayout();
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        lineEdit = new QLineEdit(URLDialog);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        formLayout->setWidget(2, QFormLayout::FieldRole, lineEdit);

        comboBox = new QComboBox(URLDialog);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));

        formLayout->setWidget(1, QFormLayout::FieldRole, comboBox);

        label = new QLabel(URLDialog);
        label->setObjectName(QString::fromUtf8("label"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label);

        label_2 = new QLabel(URLDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_2);


        verticalLayout->addLayout(formLayout);

        buttonBox = new QDialogButtonBox(URLDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(URLDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), URLDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), URLDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(URLDialog);
    } // setupUi

    void retranslateUi(QDialog *URLDialog)
    {
        URLDialog->setWindowTitle(QCoreApplication::translate("URLDialog", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("URLDialog", "Example:", nullptr));
        label_2->setText(QCoreApplication::translate("URLDialog", "URL:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class URLDialog: public Ui_URLDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_URLDIALOG_H

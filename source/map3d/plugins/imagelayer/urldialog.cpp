#include "urldialog.h"
#include "ui_urldialog.h"

URLDialog::URLDialog(QMap<QString, QString> example, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::URLDialog)
{
    ui->setupUi(this);
    ui->comboBox->addItem("");
    for(const auto &key: example.keys())
        ui->comboBox->addItem(key);
    QObject::connect(ui->comboBox, &QComboBox::currentTextChanged, [=](const QString& selectedItem){
        ui->lineEdit->setText(example[selectedItem]);
    });
}

URLDialog::~URLDialog()
{
    //delete ui;
}

QString URLDialog::getURL() const
{
    return ui->lineEdit->text();
}


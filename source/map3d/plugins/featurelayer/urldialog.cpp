#include "urldialog.h"
#include "ui_urldialog.h"

URLDialog::URLDialog(QMap<QString, QString> example, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::URLDialog)
{
    ui->setupUi(this);
    ui->comboBox->addItem("");
    for(auto key: example.keys())
        ui->comboBox->addItem(key);
    QObject::connect(ui->comboBox, QOverload<const QString &>::of(&QComboBox::currentIndexChanged), [=](const QString& selectedItem){
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

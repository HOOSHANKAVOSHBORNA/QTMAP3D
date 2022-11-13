#include "MultiChooseDlg.h"

#include <QHeaderView>
#include <QCheckBox>
#include <QTableWidget>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QVBoxLayout>

MultiChooseDlg::MultiChooseDlg(QWidget *parent, QStringList& itemToChoose)
    : QDialog(parent)
{
    QVBoxLayout* vlayout = new QVBoxLayout(this);

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                         );


    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);


    setWindowTitle(tr("Layers"));
    mTable = new QTableWidget(this);
//    mTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    mTable->setRowCount(itemToChoose.size());
    mTable->setColumnCount(2);
    mTable->verticalHeader()->hide();
    mTable->horizontalHeader()->hide();
//    mTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    mTable->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);

    mTable->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    mTable->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);



    for (int i = 0; i < itemToChoose.size(); i++)
    {
        QTableWidgetItem* tableItem = new QTableWidgetItem(itemToChoose[i]);
        mTable->setItem(i, 0, tableItem);
        mTable->setCellWidget(i, 1, new QCheckBox);
    }

    mTable->adjustSize();
    vlayout->addWidget(mTable);
    vlayout->addWidget(buttonBox);
}

MultiChooseDlg::~MultiChooseDlg()
{

}

QStringList MultiChooseDlg::getCheckedItems()
{
    QStringList checkedItems;

    for (int i = 0; i < mTable->rowCount(); i++)
    {
        if (static_cast<QCheckBox*>(mTable->cellWidget(i, 1))->isChecked())
        {
            checkedItems.push_back(mTable->item(i, 0)->text());
        }
    }
    return checkedItems;
}

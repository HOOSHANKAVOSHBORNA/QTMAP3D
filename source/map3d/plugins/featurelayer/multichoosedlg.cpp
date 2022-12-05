#include "multichoosedlg.h"

#include <QHeaderView>
#include <QCheckBox>
#include <QTableWidget>

MultiChooseDlg::MultiChooseDlg(QWidget *parent, QStringList& itemToChoose)
    : QDialog(parent)
{
    setWindowTitle(tr("Layers"));

    mtable = new QTableWidget(this);
    mtable->setRowCount(itemToChoose.size());
    mtable->setColumnCount(2);
    mtable->verticalHeader()->hide();
    mtable->horizontalHeader()->hide();
    mtable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    mtable->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    mtable->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);

    for (int i = 0; i < itemToChoose.size(); i++)
    {
        QTableWidgetItem* tableItem = new QTableWidgetItem(itemToChoose[i]);
        mtable->setItem(i, 0, tableItem);
        mtable->setCellWidget(i, 1, new QCheckBox);
    }

    mtable->adjustSize();
}

MultiChooseDlg::~MultiChooseDlg()
{

}

QStringList MultiChooseDlg::getCheckedItems()
{
    QStringList checkedItems;

    for (int i = 0; i < mtable->rowCount(); i++)
    {
        if (static_cast<QCheckBox*>(mtable->cellWidget(i, 1))->isChecked())
        {
            checkedItems.push_back(mtable->item(i, 0)->text());
        }
    }
    return checkedItems;
}

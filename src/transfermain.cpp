#include "transfermain.h"
#include "ui_transfermain.h"

Maintransfer::Maintransfer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Maintransfer)
{
    ui->setupUi(this);

    transfer = new TransferWidget();
    ui->gridLayout_all->addWidget(transfer);

    transfer_stockTudsa = new TransferStockTudsaWidget();
    ui->gridLayout_stockTudsa->addWidget(transfer_stockTudsa);

    transfer_stock = new TransferStockWidget();
    ui->gridLayout_stock->addWidget(transfer_stock);

    transfer_kabinet = new TransferKabinetWidget();
    ui->gridLayout_kabinet->addWidget(transfer_kabinet);

    connect(ui->transfer_tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));
}

void Maintransfer::tabChanged(int tab) {
    switch (tab) {
        case 0:
            transfer->refreshData();
            break;
        case 1:
            transfer_stockTudsa->refreshData();
            break;
        case 2:
            transfer_stock->refreshData();
            break;
        case 3:
            transfer_kabinet->refreshData();
            break;
    }
}

Maintransfer::~Maintransfer()
{
    delete ui;
}

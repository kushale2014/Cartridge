#ifndef MAINTRANSFER_H
#define MAINTRANSFER_H

#include <QWidget>
#include "transferview.h"
#include "transferstocktudsaview.h"
#include "transferstockview.h"
#include "transferkabinetview.h"

namespace Ui {
class Maintransfer;
}

class Maintransfer : public QWidget
{
    Q_OBJECT

public:
    explicit Maintransfer(QWidget *parent = 0);
    ~Maintransfer();

private:
    Ui::Maintransfer *ui;
    TransferWidget *transfer;
    TransferStockTudsaWidget *transfer_stockTudsa;
    TransferStockWidget *transfer_stock;
    TransferKabinetWidget *transfer_kabinet;
private slots:
    void tabChanged(int);
};

#endif // MAINTRANSFER_H

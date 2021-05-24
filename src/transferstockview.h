#ifndef __TRANSFERSTOCKVIEW_H__
#define __TRANSFERSTOCKVIEW_H__

#include <QtGui>
#include <QSqlQuery>
#include "ui_transferstockview.h"
#include "transferstockadd.h"
#include "datamodel_cc.h"
#include <QMessageBox>
#include <QSqlError>

class TransferStockWidget : public QWidget, private Ui::transferstock_view
{
    Q_OBJECT
public:
    //!A constructor
    TransferStockWidget();
    void refreshData();
private:
    QString qu;
    QSqlQuery sqlquery;
    void LoadData1();
    void LoadData2();
    void SetModelsParam(DataModel*& usemodel);
    DataModel *usemodel1;
    DataModel *usemodel2;
    QModelIndex useindex;
private slots:
        void AddTudsa_data();
        void AddKabinet_data();
        void edit_data();
        void delete_data();
        void text_changed_1();
        void clear_filter_1();
        void text_changed_2();
        void clear_filter_2();
};

#endif // __TRANSFERSTOCKVIEW_H__

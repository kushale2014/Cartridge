#ifndef __TRANSFERSTOCKTUDSAVIEW_H__
#define __TRANSFERSTOCKTUDSAVIEW_H__

#include <QtGui>
#include <QSqlQuery>
#include "ui_transferstocktudsaview.h"
#include "transferstockadd.h"
#include "transfertudsatoadd.h"
#include "transfertudsaview.h"
#include "datamodel_cc.h"
#include <QMessageBox>
#include <QSqlError>

class TransferStockTudsaWidget : public QWidget, private Ui::transferstocktudsa_view
{
    Q_OBJECT
public:
    //!A constructor
    TransferStockTudsaWidget();
    void refreshData();
private:
    QString qu;
    QSqlQuery sqlquery;
    void LoadData1();
    void LoadData2();
    void SetModelsParam1();
    void SetModelsParam2();
    DataModel *usemodel1;
    DataModel *usemodel2;
    QModelIndex useindex;
private slots:
        void stockTudsa_add();
        void tudsa_add();
        void tudsa_arhiv();
        void edit_data();  void delete_data();
        void text_changed_1();  void clear_filter_1();
        void edit_data_2();  void delete_data_2();
        void text_changed_2();  void clear_filter_2();
};

#endif // __TRANSFERSTOCKTUDSAVIEW_H__

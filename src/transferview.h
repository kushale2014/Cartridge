#ifndef __TRANSFERVIEW_H__
#define __TRANSFERVIEW_H__

#include <QtGui>
#include <QSqlQuery>
#include "ui_transferview.h"
#include "transfercartradd.h"
#include "datamodel_cc.h"
#include <QMessageBox>
#include <QSqlError>
#include <QSqlRecord>
#include "basecombomodel.h"

class TransferWidget : public QWidget, private Ui::transfer_view
{
    Q_OBJECT
public:
    //!A constructor
    TransferWidget();
private:
    QString qu;
    QSqlQuery sqlquery;
    QString useOrder;
    void LoadData();
    void SetModelsParam(DataModel*& usemodel);
    DataModel *usemodel;
    QSqlQueryModel *usemodel_kabinet;
    QModelIndex useindex;
    QString getFromSQL();
    void setInitFilterKabinet();
public slots:
    void refreshData();
private slots:
        void add_data();
        void edit_data();
        void delete_data();
        void clear_filter();
        void clear_filter_2();
        void clear_filter_kabinet1();
        void clear_filter_kabinet4();
        void sortByColumn(int);
};

#endif // __TRANSFERVIEW_H__

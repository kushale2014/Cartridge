#ifndef __TRANSFERKABINETVIEW_H__
#define __TRANSFERKABINETVIEW_H__

#include <QtGui>
#include <QSqlQuery>
#include "ui_transferkabinetview.h"
#include "transferkabinetadd.h"
#include "transfertudsafromadd.h"
#include "transferstockadd.h"
#include "datamodel_cc.h"
#include <QMessageBox>
#include <QSqlError>

class TransferKabinetWidget : public QWidget, private Ui::transfer_kabinet_view
{
    Q_OBJECT
public:
    //!A constructor
    TransferKabinetWidget();
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
    QString getFromSQL();
    QString useOrder1;
private slots:
    void AddFromTudsa_data();
    void AddFromKabinet_data();
    void add_data();
        void edit_data();
        void add_data_with_id();
        void delete_data();
        void text_changed_1();
        void clear_filter_1();
        void text_changed_2();
        void clear_filter_2();
        void All_RB_clicked();
        void Tek_RB_clicked();
        void sortByColumn1(int);
};

#endif // __TRANSFERKABINETVIEW_H__

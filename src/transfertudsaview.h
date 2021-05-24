#ifndef __TRANSFERTUDSAVIEW_H__
#define __TRANSFERTUDSAVIEW_H__

#include <QtGui>
#include <QSqlQuery>
#include "ui_transfertudsaview.h"
#include "transfertudsafromadd.h"
#include "transfertudsatoadd.h"
#include "datamodel_cc.h"
#include <QMessageBox>
#include <QSqlError>

class TransferTudsaWidget : public QDialog, private Ui::transfer_tudsa_view
{
    Q_OBJECT
public:
    //!A constructor
    TransferTudsaWidget();
private:
    QString qu;
    QSqlQuery sqlquery;
    void LoadData();
    void SetModelsParam();
    DataModel *usemodel;
    QModelIndex useindex;
private slots:
        void text_changed();
        void clear_filter();
};

#endif // __TRANSFERTUDSAVIEW_H__

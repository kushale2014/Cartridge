#ifndef __CARTRVIEW_H__
#define __CARTRVIEW_H__

#include <QtGui>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include "ui_cartrview.h"
#include "datamodel_cc.h"
#include "cartradd.h"
#include "cartrview_arhiv.h"

class CartrWidget : public QWidget, private Ui::cartr_view
{
    Q_OBJECT
public:
    //!A constructor
    CartrWidget();
private:
    QString qu;
    QSqlQuery sqlquery;
    void LoadData();
    void SetModelsParam();
    DataModel *usemodel;
    QModelIndex useindex;
    QStringList tables;
    QString useOrder;
private slots:
        void add_data();
        void delete_data();
        void edit_data();
        void clear_filter();
        void text_changed();
        void sortByColumn(int);
        void arhiv_add();
        void arhiv_view();
public slots:
        void show();
};

#endif // __CARTRVIEW_H__

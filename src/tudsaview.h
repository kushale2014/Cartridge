#ifndef __TUDSAVIEW_H__
#define __TUDSAVIEW_H__

#include <QtGui>
#include <QSqlQuery>
#include "ui_tudsaview.h"
#include "datamodel_cc.h"
#include <QMessageBox>
#include <QSqlError>
#include <QSqlRecord>

class TudsaWidget : public QWidget, private Ui::tudsa_view
{
    Q_OBJECT
public:
    //!A constructor
    TudsaWidget();
    void refreshData();
private:
    QString qu;
    QSqlQuery sqlquery;
    QString mes_error;
    void LoadData();
    void SetModelsParam(DataModel*& usemodel);
    DataModel *usemodel1;
    DataModel *usemodel2;
    QSqlQueryModel *usemodel_fio;
    QModelIndex useindex;
    void setInit_fio();
    int getFioID();
    bool validate();
    QString getDate() {return date_DE->date().toString(Qt::ISODate);}
private slots:
        void add_to_down();
        void add_to_up();
        void add_fio_default();
        void delete_data();
        void save_data();
};

#endif // __TUDSAVIEW_H__

#ifndef __CARTRVIEWARHIV_H__
#define __CARTRVIEWARHIV_H__

#include <QtGui>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include "ui_cartrview_arhiv.h"
#include "datamodel_cc.h"

class CartrArhivWidget : public QDialog, private Ui::cartr_view_arhiv
{
    Q_OBJECT
public:
    //!A constructor
    CartrArhivWidget();
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
        void clear_filter();
        void sortByColumn(int);
        void arhiv_delete();
public slots:
        void show();
};

#endif // __CARTRVIEWARHIV_H__

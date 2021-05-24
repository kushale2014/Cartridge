#ifndef __KABINETVIEW_H__
#define __KABINETVIEW_H__

#include <QtGui>
#include "ui_kabinetview.h"
#include "datamodel_cc.h"
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>

class KabinetWidget : public QWidget, private Ui::kabinet_view
{
    Q_OBJECT
public:
    //!A constructor
    KabinetWidget();
private:
    QString qu;
    QSqlQuery sqlquery;
    void LoadData();
    void SetModelsParam(DataModel*&);
    DataModel *usemodel;
    DataModel *usemodel_arhiv;
    QModelIndex useindex;
private slots:
        void add_data();
        void delete_data();
        void edit_data();
        void arhiv_add();
        void arhiv_delete();
public slots:
        void show();
};

#endif // __KABINETVIEW_H__

#ifndef __PERSONVIEW_H__
#define __PERSONVIEW_H__

#include <QtGui>
#include "ui_personview.h"
#include "personadd.h"
#include "datamodel_cc.h"
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>

class PersonWidget : public QWidget, private Ui::person_view
{
    Q_OBJECT
public:
    //!A constructor
    PersonWidget();
private:
    QString qu;
    QSqlQuery sqlquery;
    QString useOrder;
    void LoadData();
    void SetModelsParam(DataModel*&, QString);
    DataModel *usemodel;
    DataModel *usemodel_arhiv;
    QModelIndex useindex;
    QStringList tables;
private slots:
        void add_data();
        void delete_data();
        void edit_data();
        void arhiv_add();
        void arhiv_delete();
        void text_changed();
        void clear_filter();
        void sortByColumn(int);
public slots:
        void show();
};

#endif // __PERSONVIEW_H__

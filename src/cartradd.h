#ifndef __CARTRADD_H__
#define __CARTRADD_H__

#include "ui_cartradd.h"
#include <QtGui>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QMessageBox>
#include "basecombomodel.h"

class CartrAddForm : public QDialog, private Ui::cartr_add_Dialog
{
     Q_OBJECT
public:
        CartrAddForm(QWidget *parent);
        CartrAddForm(QWidget *parent, int par_id);
private:
    QString qu;  QSqlQuery sqlquery;
    int useID; QString invNomer;
    int getMaxID();
    void initComboBox();
    bool validate();
    bool isNew;
    void add_data();
    void edit_data();
    QString getModel() {return model_LE->text();}
    QString getSerialNo1() {return serialNo1_LE->text();}
    QString getSerialNo2() {return serialNo2_LE->text();}
    QString getComboID(QComboBox*& combobox);
private slots:
        void save_data();
};

#endif // __CARTRADD_H__

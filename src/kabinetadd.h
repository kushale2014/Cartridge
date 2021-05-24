#ifndef __KABINETADD_H__
#define __KABINETADD_H__

#include "ui_kabinetadd.h"
#include <QtGui>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QMessageBox>

class KabinetAddForm : public QDialog, private Ui::dialog_add_kabinet
{
     Q_OBJECT
public:
        KabinetAddForm(QWidget *parent);        // add
        KabinetAddForm(QWidget *parent, int);   // edit
        int getLastID() {return lastID;};
private:
    QString qu;  QSqlQuery sqlquery;  bool ok;
    bool isNew;
    int useID;
    int lastID;
    QString mes_error;
    bool validate();
    QSqlQueryModel *model;
    void add_data();
    void edit_data();
    int getNomer() {return nomer_SPB->value();}
    QString getTitle() {return title_LE->text();}
private slots:
        void save_data();
};

#endif // __KABINETADD_H__

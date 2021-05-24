#ifndef __PERSONADD_H__
#define __PERSONADD_H__

#include "ui_personadd.h"
#include <QtGui>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QMessageBox>

class PersonAddForm : public QDialog, private Ui::dialog_add_person
{
     Q_OBJECT
public:
    PersonAddForm(QWidget *parent);      //add
    PersonAddForm(QWidget *parent, int); //edit
    int getLastID() {return lastID;};
private:
    QString qu;  QSqlQuery sqlquery;  bool ok;
    bool isNew;
    int useID;
    int lastID;
    QString mes_error;
    void init_new();
    void init_edit(int);
    bool validate();
    QSqlQueryModel *model;
    void add_data();
    void edit_data();
private slots:
        void save_data();
};

#endif // __PERSONADD_H__

#ifndef __TRANSFERTUDSAFROMADD_H__
#define __TRANSFERTUDSAFROMADD_H__

#include <QDialog>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QComboBox>
#include <QCheckBox>
#include "dateeditbox.h"
#include "cartredit.h"

namespace Ui {
class Transfer_tudsa_add;
}

class TransferTudsaFromAdd : public QDialog
{
    Q_OBJECT

public:
    explicit TransferTudsaFromAdd(QWidget *parent = 0);
    explicit TransferTudsaFromAdd(int id, QWidget *parent = 0);
    ~TransferTudsaFromAdd();
private:
    Ui::Transfer_tudsa_add *ui;
    QString qu;  QSqlQuery sqlquery;
    bool isNew;
    int useID;
    QString mes_error;
    void setInit();
    void loadInfo();
    void setInit_fio();
    void setInit_cartr();
    void setInit_cartr_update();
    QMap<int, CartrEdit *> map_cartr;
    QSqlQueryModel *usemodel_fio;
    QString getFioID();
    QString getDate();
    QString getWhere();
    DateEditBox *date_edit;
    bool validate();
    void add_data();
    void edit_data();
private slots:
    void save_data();
    void find_fio();
    void add_fio();
    void add_fio_default();
    void stateChanged(int);
};

#endif // __TRANSFERTUDSAFROMADD_H__

#ifndef __TRANSFERTUDSATOADD_H__
#define __TRANSFERTUDSATOADD_H__

#include <QDialog>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QComboBox>
#include "dateeditbox.h"
#include "cartredit.h"

namespace Ui {
class Transfer_tudsa_add;
}

class TransferTudsaToAdd : public QDialog
{
    Q_OBJECT

public:
    explicit TransferTudsaToAdd(QWidget *parent = 0);
    explicit TransferTudsaToAdd(int id, QWidget *parent = 0);
    ~TransferTudsaToAdd();
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
    QMap<int, CartrEdit *> map_cartr;
    QSqlQueryModel *usemodel_cartr;
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
};

#endif // __TRANSFERTUDSATOADD_H__

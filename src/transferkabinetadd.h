#ifndef __TRANSFERKABINETADD_H__
#define __TRANSFERKABINETADD_H__

#include <QDialog>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QComboBox>
#include "dateeditbox.h"

namespace Ui {
class Transfer_kabinet_add;
}

class TransferKabinetAdd : public QDialog
{
    Q_OBJECT

public:
    explicit TransferKabinetAdd(QWidget *parent = 0);
    explicit TransferKabinetAdd(int id, bool pr_new, QWidget *parent = 0);
    ~TransferKabinetAdd();

private:
    Ui::Transfer_kabinet_add *ui;
    QString qu;  QSqlQuery sqlquery;
    bool isNew;
    int useID;
    QString mes_error;
    void setInit();
    void loadInfo();
    void setInit_fio();
    void setInit_cartr();
    void setInit_kabinet();
    QSqlQueryModel *usemodel_fio;
    QSqlQueryModel *usemodel_kabinet;
    QSqlQueryModel *usemodel_cartr;
    QString getCartrID();
    QString getFioID();
    QString getKabinetID();
    QString getDate();
    QString getWhere();
    DateEditBox *date_edit_4;
    bool validate();
    void add_data();
    void edit_data();
private slots:
    void save_data();
    void find_model();
    void find_fio();
    void find_kabinet();
    void add_fio();
    void add_kabinet();
};

#endif // __TRANSFERKABINETADD_H__

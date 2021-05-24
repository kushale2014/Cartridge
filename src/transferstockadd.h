#ifndef TRANSFERSTOCKADD_H
#define TRANSFERSTOCKADD_H

#include <QDialog>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QComboBox>
#include "dateeditbox.h"

namespace Ui {
class Transferstockadd;
}

class Transferstockadd : public QDialog
{
    Q_OBJECT

public:
    explicit Transferstockadd(QString pr, QWidget *parent = 0);
    explicit Transferstockadd(int id, QWidget *parent = 0);
    ~Transferstockadd();

private:
    Ui::Transferstockadd *ui;
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
    QSqlQueryModel *usemodel_model;
    QString getCartrID();
    QString getFioID();
    QString getKabinetID();
    QString getDate();
    QString getWhere();
    DateEditBox *date_edit_1;
    bool validate();
    void add_data();
    void edit_data();
    QString useTudsa;
private slots:
    void save_data();
    void find_model();
    void find_fio();
    void find_kabinet();
    void add_fio();
    void add_kabinet();
};

#endif // TRANSFERSTOCKADD_H

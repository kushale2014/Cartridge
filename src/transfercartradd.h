#ifndef TRANSFERCARTRADD_H
#define TRANSFERCARTRADD_H

#include <QDialog>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QComboBox>
#include "dateeditbox.h"

namespace Ui {
class Transfer_Cartr_Add;
}

class TransferCartrAdd : public QDialog
{
    Q_OBJECT

public:
    explicit TransferCartrAdd(QWidget *parent = 0);
    explicit TransferCartrAdd(int id, QWidget *parent = 0);
    ~TransferCartrAdd();

private:
    Ui::Transfer_Cartr_Add *ui;
    QString qu;  QSqlQuery sqlquery;
    bool isNew;
    int useID;
    QString mes_error;
    void setInit();
    void loadInfo();
    void setData(int, QString, QString, QString);
    void setInit_fio();
    void refresh_fio(int&, int&);
    void setInit_cartr();
    void setInit_kabinet();
    void refresh_kabinet(int&, int&);
    QSqlQueryModel *usemodel_fio;
    QSqlQueryModel *usemodel_kabinet;
    QSqlQueryModel *usemodel_model;
    QMap<int, QComboBox *> map_fio_combo;
    QMap<int, QLineEdit *> map_fio_lineEdit;
    QMap<int, QComboBox *> map_kabinet_combo;
    QMap<int, QLineEdit *> map_kabinet_lineEdit;
    QMap<int, DateEditBox *> map_date_DateEditBox;
    QString getCartrID();
    QString getFioID(int);
    QString getKabinetID(int);
    QString getDate(int);
    QString getWhere();
    DateEditBox *date_edit_1;
    DateEditBox *date_edit_2;
    DateEditBox *date_edit_3;
    DateEditBox *date_edit_4;
    bool validate();
    void add_data();
    void edit_data();
private slots:
    void save_data();
    void find_model();
    void find_in_combo(QString);
    void add_fio(int);
    void add_kabinet(int);
    void add_fio_default(int);
};

#endif // TRANSFERCARTRADD_H

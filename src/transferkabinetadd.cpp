#include "transferkabinetadd.h"
#include "ui_transferkabinetadd.h"
#include "personadd.h"
#include "kabinetadd.h"

TransferKabinetAdd::TransferKabinetAdd(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Transfer_kabinet_add)
{
    ui->setupUi(this);
    isNew = true;
    setWindowTitle("Передать в кабинет");
    setWindowIcon(QPixmap(":/resources/0721.ico"));
    setInit();
}

TransferKabinetAdd::TransferKabinetAdd(int id, bool pr_new, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Transfer_kabinet_add)
{
    ui->setupUi(this);
    isNew = pr_new;
    useID = id;
    ui->model_GB->setEnabled(false);
    if (isNew) {
        setWindowTitle("Передать в кабинет");
        setWindowIcon(QPixmap(":/resources/0721.ico"));
    }  else{
        setWindowTitle("Редактирование записи");
        setWindowIcon(QPixmap(":/buttonEdit"));
    }
    setInit();
    loadInfo();
}

TransferKabinetAdd::~TransferKabinetAdd()
{
    delete ui;
}

void TransferKabinetAdd::setInit() {
    date_edit_4 = new DateEditBox();
    date_edit_4->setText("Дата передачи:");
    ui->verticalLayout_date->addWidget(date_edit_4);

    usemodel_fio = new QSqlQueryModel();
    usemodel_kabinet = new QSqlQueryModel();
    usemodel_cartr = new QSqlQueryModel();

    setInit_fio();
    setInit_kabinet();
    setInit_cartr();

    connect(ui->model_LE, SIGNAL(textChanged(QString)), this, SLOT(find_model()));

    connect(ui->fio_LE, SIGNAL(textChanged(QString)), this, SLOT(find_fio()));
    connect(ui->fio_TB, SIGNAL(clicked()), this, SLOT(add_fio()));

    connect(ui->kabinet_LE, SIGNAL(textChanged(QString)), this, SLOT(find_kabinet()));
    connect(ui->kabinet_TB, SIGNAL(clicked()), this, SLOT(add_kabinet()));

    connect(ui->pbSave, SIGNAL(clicked()), this, SLOT(save_data()));

}

void TransferKabinetAdd::setInit_fio() {
    qu = "SELECT id, family || ' ' || name FROM Person"
           " %1 ORDER by family, name";
    qu = qu.arg(getWhere());
    sqlquery.exec(qu);
    usemodel_fio->setQuery(sqlquery);
    ui->fio_CB->setModel(usemodel_fio);;
    ui->fio_CB->setModelColumn(1);
    ui->fio_CB->setCurrentIndex(-1);
}

void TransferKabinetAdd::setInit_cartr() {
    if (isNew) {
        qu = "SELECT Transfer.id, Cartr.invNomer || '-->' || Cartr.model FROM Cartr, Transfer"
            " WHERE Cartr.id = Transfer.cartr_id AND Transfer.date4 IS NULL"
            " ORDER by Cartr.invNomer";
    } else {
        qu = "SELECT Transfer.id, Cartr.invNomer || '-->' || Cartr.model FROM Cartr, Transfer"
                " WHERE Cartr.id = Transfer.cartr_id AND Transfer.id = %1";
        qu = qu.arg(useID);
    }
    sqlquery.exec(qu);
    usemodel_cartr->setQuery(sqlquery);
    ui->model_CB->setModel(usemodel_cartr);
    ui->model_CB->setModelColumn(1);
    ui->model_CB->setCurrentIndex(-1);
}

void TransferKabinetAdd::setInit_kabinet() {
    qu = "SELECT id, nomer || ' - ' || title FROM Kabinet"
            " %1 ORDER by nomer";
    qu = qu.arg(getWhere());
    sqlquery.exec(qu);
    usemodel_kabinet->setQuery(sqlquery);
    ui->kabinet_CB->setModel(usemodel_kabinet);;
    ui->kabinet_CB->setModelColumn(1);
    ui->kabinet_CB->setCurrentIndex(-1);
}

QString TransferKabinetAdd::getWhere() {
    if (isNew) return "WHERE pr_arhiv <> 1";
    else return "";
}

void TransferKabinetAdd::find_model() {
    if (ui->model_LE->text().isEmpty()) {
        ui->model_CB->setCurrentIndex(-1);
    } else {
        QString tt = QString("0000%1").arg(ui->model_LE->text());
        tt = tt.right(5);
        ui->model_CB->setCurrentIndex(ui->model_CB->findData(tt, 0, Qt::MatchStartsWith));
    }
}

void TransferKabinetAdd::find_fio() {
    QString tt = ui->fio_LE->text();
    if (tt.isEmpty()) ui->fio_CB->setCurrentIndex(-1);
    else ui->fio_CB->setCurrentIndex(ui->fio_CB->findData(tt, 0, Qt::MatchStartsWith));
}

void TransferKabinetAdd::find_kabinet() {
    QString tt = ui->kabinet_LE->text();
    if (tt.isEmpty()) ui->kabinet_CB->setCurrentIndex(-1);
    else ui->kabinet_CB->setCurrentIndex(ui->kabinet_CB->findData(tt, 0, Qt::MatchStartsWith));
}

void TransferKabinetAdd::add_fio() {
    PersonAddForm *dlg = new PersonAddForm(this);
    dlg->exec();
    int id = dlg->getLastID();
    delete dlg;
    if (id == -1) return;

    setInit_fio();
    ui->fio_CB->setModelColumn(0);
    int ind = ui->fio_CB->findText(QString("%1").arg(id));
    ui->fio_CB->setModelColumn(1);
    ui->fio_CB->setCurrentIndex(ind);
}

void TransferKabinetAdd::add_kabinet() {
    KabinetAddForm *dlg = new KabinetAddForm(this);
    dlg->exec();
    int id = dlg->getLastID();
    delete dlg;
    if (id == -1) return;

    setInit_kabinet();
    ui->kabinet_CB->setModelColumn(0);
    int ind = ui->kabinet_CB->findText(QString("%1").arg(id));
    ui->kabinet_CB->setModelColumn(1);
    ui->kabinet_CB->setCurrentIndex(ind);
}

bool TransferKabinetAdd::validate() {
    if (getCartrID()=="NULL") {
        mes_error = "Выберите картридж.";
        return false;
    }
    if (getDate()=="NULL") {
        mes_error = "Заполните поле 'Дата передачи'";
        return false;
    }
    if (getKabinetID()=="NULL") {
        mes_error = "Заполните поле 'В какой кабинет'";
        return false;
    }
    return true;
}

void TransferKabinetAdd::save_data() {
    mes_error = "";
    if (isNew) add_data();
    else edit_data();
    if (mes_error.isEmpty()) reject();
    else QMessageBox::critical(this, "Ошибка", mes_error);
}

void TransferKabinetAdd::add_data() {
    if (!validate()) return;
    edit_data();
}

void TransferKabinetAdd::edit_data() {
    if (!validate()) return;

    qu = "UPDATE Transfer SET date4 = %2, kabinet_id_4 = %3, person_id_4 = %4"
        " WHERE id = %1";
    qu = qu.arg(getCartrID()).arg(getDate()).arg(getKabinetID()).arg(getFioID());
    if (!sqlquery.exec(qu)) {
        mes_error = sqlquery.lastError().databaseText();
        return;
    }
}

QString TransferKabinetAdd::getCartrID() {
    int ind = ui->model_CB->currentIndex();
    if (ind < 0) return "NULL";
    QSqlRecord rec = usemodel_cartr->record(ind);
    return rec.value(0).toString();
}

QString TransferKabinetAdd::getFioID() {
    int ind = ui->fio_CB->currentIndex();
    if (ind < 0) return "NULL";
    QSqlRecord rec = usemodel_fio->record(ind);
    return rec.value(0).toString();
}

QString TransferKabinetAdd::getKabinetID() {
    int ind = ui->kabinet_CB->currentIndex();
    if (ind < 0) return "NULL";
    QSqlRecord rec = usemodel_kabinet->record(ind);
    return rec.value(0).toString();
}

QString TransferKabinetAdd::getDate() {
    QDate DD;
    DD = date_edit_4->getDate();
    if (DD.isNull()) return "NULL";
    else return QString("'%1'").arg(DD.toString(Qt::ISODate));
}

void TransferKabinetAdd::loadInfo() {
    qu = "SELECT cartr_id, date4, kabinet_id_4, person_id_4"
         " FROM transfer WHERE id = %1";
    qu = qu.arg(useID);
    if(!sqlquery.exec(qu)){
        QMessageBox::warning(this, "Ошибка!", sqlquery.lastError().databaseText());
        return;
    }
    sqlquery.first();
    ui->model_LE->setText(sqlquery.value(0).toString());

    QString DD = sqlquery.value(1).toString();
    if (!DD.isEmpty()) {
        QDate d;
        d.setDate(DD.mid(0,4).toInt(), DD.mid(5,2).toInt(), DD.mid(8,2).toInt());
        date_edit_4->setDate(d);
    }

    QString kabinet = sqlquery.value(2).toString();
    if (!kabinet.isEmpty()) {
        ui->kabinet_CB->setModelColumn(0);
        ui->kabinet_CB->setCurrentIndex(ui->kabinet_CB->findData(kabinet, Qt::MatchStartsWith));
        ui->kabinet_CB->setModelColumn(1);
    }

    QString fio = sqlquery.value(3).toString();
    if (!fio.isEmpty()) {
        ui->fio_CB->setModelColumn(0);
        ui->fio_CB->setCurrentIndex(ui->fio_CB->findData(fio, Qt::MatchStartsWith));
        ui->fio_CB->setModelColumn(1);
    }

}

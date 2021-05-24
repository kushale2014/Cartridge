#include "transferstockadd.h"
#include "ui_transferstockadd.h"
#include "personadd.h"
#include "kabinetadd.h"

Transferstockadd::Transferstockadd(QString pr, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Transferstockadd)
{
    ui->setupUi(this);
    isNew = true;
    useTudsa = pr;  if (useTudsa != "1") useTudsa = "0";
    if (useTudsa == "1") {
        setWindowTitle("Принять картридж на отправку в ТуДСА.");
        setWindowIcon(QPixmap(":/resources/0415.ico"));
    } else {
        setWindowTitle("Принять картридж на склад для передачи в другой кабинет.");
        setWindowIcon(QPixmap(":/resources/0417.ico"));
    }
    setInit();
}

Transferstockadd::Transferstockadd(int id, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Transferstockadd)
{
    ui->setupUi(this);
    isNew = false;
    useID = id;
    ui->model_GB->setEnabled(false);
    setWindowTitle("Редактирование записи");
    setWindowIcon(QPixmap(":/buttonEdit"));
    setInit();
    loadInfo();
}

Transferstockadd::~Transferstockadd()
{
    delete ui;
}

void Transferstockadd::setInit() {
    date_edit_1 = new DateEditBox();
    date_edit_1->setText("Дата получения:");
    ui->dataLayout_1->addWidget(date_edit_1);

    usemodel_fio = new QSqlQueryModel();
    usemodel_kabinet = new QSqlQueryModel();
    usemodel_model = new QSqlQueryModel();

    setInit_fio();
    setInit_cartr();
    setInit_kabinet();

    connect(ui->model_LE, SIGNAL(textChanged(QString)), this, SLOT(find_model()));

    connect(ui->fio_1_LE, SIGNAL(textChanged(QString)), this, SLOT(find_fio()));
    connect(ui->fio_1_TB, SIGNAL(clicked()), this, SLOT(add_fio()));

    connect(ui->kabinet_1_LE, SIGNAL(textChanged(QString)), this, SLOT(find_kabinet()));
    connect(ui->kabinet_1_TB, SIGNAL(clicked()), this, SLOT(add_kabinet()));

    connect(ui->pbSave, SIGNAL(clicked()), this, SLOT(save_data()));

}

void Transferstockadd::setInit_fio() {
    qu = "SELECT id, family || ' ' || name FROM Person"
           " %1 ORDER by family, name";
    qu = qu.arg(getWhere());
    sqlquery.exec(qu);
    usemodel_fio->setQuery(sqlquery);
    ui->fio_1_CB->setModel(usemodel_fio);;
    ui->fio_1_CB->setModelColumn(1);
    ui->fio_1_CB->setCurrentIndex(-1);
}

void Transferstockadd::setInit_cartr() {
    qu = "SELECT id, invNomer || '-->' || model FROM Cartr"
            " %1 ORDER by invNomer";
    qu = qu.arg(getWhere());
    sqlquery.exec(qu);
    usemodel_model->setQuery(sqlquery);
    ui->model_CB->setModel(usemodel_model);
    ui->model_CB->setModelColumn(1);
    ui->model_CB->setCurrentIndex(-1);
}

void Transferstockadd::setInit_kabinet() {
    qu = "SELECT id, nomer || ' - ' || title FROM Kabinet"
            " %1 ORDER by nomer";
    qu = qu.arg(getWhere());
    sqlquery.exec(qu);
    usemodel_kabinet->setQuery(sqlquery);
    ui->kabinet_1_CB->setModel(usemodel_kabinet);;
    ui->kabinet_1_CB->setModelColumn(1);
    ui->kabinet_1_CB->setCurrentIndex(-1);
}

QString Transferstockadd::getWhere() {
    if (isNew) return "WHERE pr_arhiv <> 1";
    else return "";
}

void Transferstockadd::find_model() {
    if (ui->model_LE->text().isEmpty()) {
        ui->model_CB->setCurrentIndex(-1);
    } else {
        QString tt = QString("0000%1").arg(ui->model_LE->text());
        tt = tt.right(5);
        ui->model_CB->setCurrentIndex(ui->model_CB->findData(tt, 0, Qt::MatchStartsWith));
    }
}

void Transferstockadd::find_fio() {
    QString tt = ui->fio_1_LE->text();
    if (tt.isEmpty()) ui->fio_1_CB->setCurrentIndex(-1);
    else ui->fio_1_CB->setCurrentIndex(ui->fio_1_CB->findData(tt, 0, Qt::MatchStartsWith));
}

void Transferstockadd::find_kabinet() {
    QString tt = ui->kabinet_1_LE->text();
    if (tt.isEmpty()) ui->kabinet_1_CB->setCurrentIndex(-1);
    else ui->kabinet_1_CB->setCurrentIndex(ui->kabinet_1_CB->findData(tt, 0, Qt::MatchStartsWith));
}

void Transferstockadd::add_fio() {
    PersonAddForm *dlg = new PersonAddForm(this);
    dlg->exec();
    int id = dlg->getLastID();
    delete dlg;
    if (id == -1) return;

    setInit_fio();
    ui->fio_1_CB->setModelColumn(0);
    int ind = ui->fio_1_CB->findText(QString("%1").arg(id));
    ui->fio_1_CB->setModelColumn(1);
    ui->fio_1_CB->setCurrentIndex(ind);
}

void Transferstockadd::add_kabinet() {
    KabinetAddForm *dlg = new KabinetAddForm(this);
    dlg->exec();
    int id = dlg->getLastID();
    delete dlg;
    if (id == -1) return;

    setInit_kabinet();
    ui->kabinet_1_CB->setModelColumn(0);
    int ind = ui->kabinet_1_CB->findText(QString("%1").arg(id));
    ui->kabinet_1_CB->setModelColumn(1);
    ui->kabinet_1_CB->setCurrentIndex(ind);
}

bool Transferstockadd::validate() {
    if (getCartrID()=="NULL") {
        mes_error = "Выберите картридж.";
        return false;
    }
    if (getDate()=="NULL") {
        mes_error = "Заполните поле 'Дата получения'";
        return false;
    }
    if (getKabinetID()=="NULL") {
        mes_error = "Заполните поле 'С какого кабинета'";
        return false;
    }
    return true;
}

void Transferstockadd::save_data() {
    mes_error = "";
    if (isNew) add_data();
    else edit_data();
    if (mes_error.isEmpty()) reject();
    else QMessageBox::critical(this, "Ошибка", mes_error);
}

void Transferstockadd::add_data() {
    if (!validate()) return;

    qu = "INSERT INTO Transfer"
            " (cartr_id, date1, kabinet_id_1, person_id_1, comment, pr_tudsa)"
            " VALUES(%1, %2, %3, %4, '%5', '%6')";
    qu = qu.arg(getCartrID()).arg(getDate()).arg(getKabinetID()).arg(getFioID()).arg(ui->comment_LE->text()).arg(useTudsa);
    if (!sqlquery.exec(qu)) {
        mes_error = sqlquery.lastError().databaseText();
        return;
    }
}

void Transferstockadd::edit_data() {
    if (!validate()) return;

    qu = "UPDATE Transfer SET date1 = %2, kabinet_id_1 = %3, person_id_1 = %4, comment = '%5'"
        " WHERE id = %1";
    qu = qu.arg(useID).arg(getDate()).arg(getKabinetID()).arg(getFioID()).arg(ui->comment_LE->text());
    if (!sqlquery.exec(qu)) {
        mes_error = sqlquery.lastError().databaseText();
        return;
    }
}

QString Transferstockadd::getCartrID() {
    int ind = ui->model_CB->currentIndex();
    if (ind < 0) return "NULL";
    QSqlRecord rec = usemodel_model->record(ind);
    return rec.value(0).toString();
}

QString Transferstockadd::getFioID() {
    int ind = ui->fio_1_CB->currentIndex();
    if (ind < 0) return "NULL";
    QSqlRecord rec = usemodel_fio->record(ind);
    return rec.value(0).toString();
}

QString Transferstockadd::getKabinetID() {
    int ind = ui->kabinet_1_CB->currentIndex();
    if (ind < 0) return "NULL";
    QSqlRecord rec = usemodel_kabinet->record(ind);
    return rec.value(0).toString();
}

QString Transferstockadd::getDate() {
    QDate DD;
    DD = date_edit_1->getDate();
    if (DD.isNull()) return "NULL";
    else return QString("'%1'").arg(DD.toString(Qt::ISODate));
}

void Transferstockadd::loadInfo() {
    qu = "SELECT cartr_id, date1, kabinet_id_1, person_id_1, comment"
         " FROM transfer WHERE id = %1";
    qu = qu.arg(useID);
    if(!sqlquery.exec(qu)){
        QMessageBox::warning(this, "Ошибка!", sqlquery.lastError().databaseText());
        return;
    }
    sqlquery.first();
    ui->model_LE->setText(sqlquery.value(0).toString());
    ui->comment_LE->setText(sqlquery.value(4).toString());

    QDate d;
    QString DD = sqlquery.value(1).toString();
    d.setDate(DD.mid(0,4).toInt(), DD.mid(5,2).toInt(), DD.mid(8,2).toInt());
    date_edit_1->setDate(d);

    QString kabinet = sqlquery.value(2).toString();
    if (!kabinet.isEmpty()) {
        ui->kabinet_1_CB->setModelColumn(0);
        ui->kabinet_1_CB->setCurrentIndex(ui->kabinet_1_CB->findData(kabinet, Qt::MatchStartsWith));
        ui->kabinet_1_CB->setModelColumn(1);
    }

    QString fio = sqlquery.value(3).toString();
    if (!fio.isEmpty()) {
        ui->fio_1_CB->setModelColumn(0);
        ui->fio_1_CB->setCurrentIndex(ui->fio_1_CB->findData(fio, Qt::MatchStartsWith));
        ui->fio_1_CB->setModelColumn(1);
    }

}

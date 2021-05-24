#include "transfertudsatoadd.h"
#include "ui_TransferTudsaAdd.h"
#include "personadd.h"
#include "kabinetadd.h"

TransferTudsaToAdd::TransferTudsaToAdd(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Transfer_tudsa_add)
{
    ui->setupUi(this);
    isNew = true;
    setWindowTitle("Добавление записи");
    setWindowIcon(QPixmap(":/resources/0415.ico"));
    setInit();
}

TransferTudsaToAdd::TransferTudsaToAdd(int id, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Transfer_tudsa_add)
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

TransferTudsaToAdd::~TransferTudsaToAdd()
{
    delete ui;
}

void TransferTudsaToAdd::setInit() {

    ui->fio_label->setText("Кто отвез: ");
    date_edit = new DateEditBox();
    date_edit->setText("Дата отправки в ТуДСА:");
    ui->verticalLayoutDate->addWidget(date_edit);

    usemodel_fio = new QSqlQueryModel();
    usemodel_cartr = new QSqlQueryModel();

    setInit_fio();
    setInit_cartr();

    connect(ui->fio_LE, SIGNAL(textChanged(QString)), this, SLOT(find_fio()));
    connect(ui->fio_TB, SIGNAL(clicked()), this, SLOT(add_fio()));
    connect(ui->fio_default_TB, SIGNAL(clicked()), this, SLOT(add_fio_default()));

    connect(ui->pbSave, SIGNAL(clicked()), this, SLOT(save_data()));

}

void TransferTudsaToAdd::setInit_fio() {
    qu = "SELECT id, family || ' ' || name FROM Person"
           " %1 ORDER by family, name";
    qu = qu.arg(getWhere());
    sqlquery.exec(qu);
    usemodel_fio->setQuery(sqlquery);
    ui->fio_CB->setModel(usemodel_fio);;

    QSettings settings("config.ini", QSettings::IniFormat);
    QString id_fio_default = settings.value("id_fio_default").toString();
    if (!id_fio_default.isEmpty()) {
        int ind = ui->fio_CB->findText(QString("%1").arg(id_fio_default));
        ui->fio_CB->setModelColumn(1);
        ui->fio_CB->setCurrentIndex(ind);
    } else {
        ui->fio_CB->setModelColumn(1);
        ui->fio_CB->setCurrentIndex(-1);
    }
}

void TransferTudsaToAdd::add_fio_default() {
    QString id = getFioID();
    if (id != "NULL") {
        QSettings settings("config.ini", QSettings::IniFormat);
        settings.setValue("id_fio_default", id);
        QMessageBox::information(this, "Сообщение", QString("Сотрудник: %1\nустановлен по умолчанию успешно!").arg(ui->fio_CB->currentText()));
    }
}

void TransferTudsaToAdd::setInit_cartr() {
    if (isNew) {
        qu = "SELECT Transfer.id, Transfer.cartr_id, Cartr.invNomer || '-->' || Cartr.model FROM Cartr, Transfer";
        qu += " WHERE Cartr.id = Transfer.cartr_id AND pr_tudsa = 1 AND Transfer.date2 IS NULL";
        qu += " ORDER by Cartr.invNomer";
    } else {
        qu = "SELECT Transfer.id, Transfer.cartr_id, Cartr.invNomer || '-->' || Cartr.model FROM Cartr, Transfer"
                " WHERE Cartr.id = Transfer.cartr_id AND Transfer.id = %1";
        qu = qu.arg(useID);
    }
    sqlquery.exec(qu);
    usemodel_cartr->setQuery(sqlquery);
    for (int i = 0; i < usemodel_cartr->rowCount(); i++) {
        QSqlRecord record = usemodel_cartr->record(i);
        int id = record.value(0).toInt();
        int cartr_id = record.value(1).toInt();
        QString cartr_model = record.value(2).toString();
        CartrEdit *ccc = new CartrEdit(id, cartr_id);
        ccc->setText(cartr_model);
        map_cartr.insert(i, ccc);

        QLabel *label = new QLabel();
        label->setText(QString("%1").arg(i+1));
        QHBoxLayout *horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(3, 3, 3, 1);
        horizontalLayout->addWidget(label);
        horizontalLayout->addWidget(ccc);

        ui->verticalLayoutCartr->addLayout(horizontalLayout);
    }
}

QString TransferTudsaToAdd::getWhere() {
    if (isNew) return "WHERE pr_arhiv <> 1";
    else return "";
}

void TransferTudsaToAdd::find_fio() {
    QString tt = ui->fio_LE->text();
    if (tt.isEmpty()) ui->fio_CB->setCurrentIndex(-1);
    else ui->fio_CB->setCurrentIndex(ui->fio_CB->findData(tt, 0, Qt::MatchStartsWith));
}

void TransferTudsaToAdd::add_fio() {
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

bool TransferTudsaToAdd::validate() {
    int ii = 0;
    QMapIterator<int, CartrEdit *> i(map_cartr);
    while (i.hasNext()) {
        i.next();
        QString id = i.value()->getID();
        if (id == "NULL") continue;
        ii++;
    }

    if (ii == 0) {
        mes_error = "Нет выбранных картриджей.";
        return false;
    }
    if (getDate()=="NULL") {
        mes_error = QString("Заполните поле '%1'").arg("Кто отвез: ");
        return false;
    }
    return true;
}

void TransferTudsaToAdd::save_data() {
    mes_error = "";
    if (isNew) add_data();
    else edit_data();
    if (mes_error.isEmpty()) reject();
    else QMessageBox::critical(this, "Ошибка", mes_error);
}

void TransferTudsaToAdd::add_data() {
    if (!validate()) return;
    edit_data();
}

void TransferTudsaToAdd::edit_data() {
    if (!validate()) return;

    qu = "UPDATE Transfer SET date2 = %2, person_id_2 = %3";
    qu += " WHERE id = %1";
    QSqlDatabase::database().transaction();
    QMapIterator<int, CartrEdit *> i(map_cartr);
    while (i.hasNext()) {
        i.next();
        QString id = i.value()->getID();
        if (id == "NULL") continue;
        QString query = qu.arg(id).arg(getDate()).arg(getFioID());
        sqlquery.exec(query);
    }
    QSqlDatabase::database().commit();
}

QString TransferTudsaToAdd::getFioID() {
    int ind = ui->fio_CB->currentIndex();
    if (ind < 0) return "NULL";
    QSqlRecord rec = usemodel_fio->record(ind);
    return rec.value(0).toString();
}

QString TransferTudsaToAdd::getDate() {
    QDate DD;
    DD = date_edit->getDate();
    if (DD.isNull()) return "NULL";
    else return QString("'%1'").arg(DD.toString(Qt::ISODate));
}

void TransferTudsaToAdd::loadInfo() {
    qu = "SELECT cartr_id, date2, person_id_2";
    qu += " FROM transfer WHERE id = %1";
    qu = qu.arg(useID);
    if(!sqlquery.exec(qu)){
        QMessageBox::warning(this, "Ошибка!", sqlquery.lastError().databaseText());
        return;
    }
    sqlquery.first();

    QDate d;
    QString DD = sqlquery.value(1).toString();
    d.setDate(DD.mid(0,4).toInt(), DD.mid(5,2).toInt(), DD.mid(8,2).toInt());
    date_edit->setDate(d);

    QString fio = sqlquery.value(2).toString();
    if (!fio.isEmpty()) {
        ui->fio_CB->setModelColumn(0);
        ui->fio_CB->setCurrentIndex(ui->fio_CB->findData(fio, Qt::MatchStartsWith));
        ui->fio_CB->setModelColumn(1);
    }

}

#include "transfercartradd.h"
#include "ui_transfercartradd.h"
#include "personadd.h"
#include "kabinetadd.h"

TransferCartrAdd::TransferCartrAdd(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Transfer_Cartr_Add)
{
    ui->setupUi(this);
    isNew = true;
    setWindowTitle("Добавление записи");
    setWindowIcon(QPixmap(":/resources/0415.ico"));
    setInit();
}

TransferCartrAdd::TransferCartrAdd(int id, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Transfer_Cartr_Add)
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

TransferCartrAdd::~TransferCartrAdd()
{
    delete ui;
}

void TransferCartrAdd::setInit() {
    date_edit_1 = new DateEditBox();
    date_edit_1->setText("Дата получения:");
    ui->dataLayout_1->addWidget(date_edit_1);
    date_edit_2 = new DateEditBox();
    date_edit_2->setText("Дата отправки:");
    ui->dataLayout_2->addWidget(date_edit_2);
    date_edit_3 = new DateEditBox();
    date_edit_3->setText("Дата прихода:");
    ui->dataLayout_3->addWidget(date_edit_3);
    date_edit_4 = new DateEditBox();
    date_edit_4->setText("Дата передачи:");
    ui->dataLayout_4->addWidget(date_edit_4);
    map_date_DateEditBox.insert(1, date_edit_1);
    map_date_DateEditBox.insert(2, date_edit_2);
    map_date_DateEditBox.insert(3, date_edit_3);
    map_date_DateEditBox.insert(4, date_edit_4);

    usemodel_fio = new QSqlQueryModel();
    usemodel_kabinet = new QSqlQueryModel();
    usemodel_model = new QSqlQueryModel();

    map_fio_lineEdit.insert(1, ui->fio_1_LE);
    map_fio_lineEdit.insert(4, ui->fio_4_LE);
    map_kabinet_lineEdit.insert(1, ui->kabinet_1_LE);
    map_kabinet_lineEdit.insert(4, ui->kabinet_4_LE);

    map_fio_combo.insert(1, ui->fio_1_CB);
    map_fio_combo.insert(2, ui->fio_2_CB);
    map_fio_combo.insert(3, ui->fio_3_CB);
    map_fio_combo.insert(4, ui->fio_4_CB);
    setInit_fio();

    setInit_cartr();

    map_kabinet_combo.insert(1, ui->kabinet_1_CB);
    map_kabinet_combo.insert(4, ui->kabinet_4_CB);
    setInit_kabinet();

    connect(ui->model_LE, SIGNAL(textChanged(QString)), this, SLOT(find_model()));

    QSignalMapper *mapper_LE = new QSignalMapper(0);
    connect(ui->fio_1_LE, SIGNAL(textChanged(QString)), mapper_LE, SLOT(map()));
    mapper_LE->setMapping(ui->fio_1_LE, "fio:1");
    connect(ui->fio_4_LE, SIGNAL(textChanged(QString)), mapper_LE, SLOT(map()));
    mapper_LE->setMapping(ui->fio_4_LE, "fio:4");
    connect(ui->kabinet_1_LE, SIGNAL(textChanged(QString)), mapper_LE, SLOT(map()));
    mapper_LE->setMapping(ui->kabinet_1_LE, "kabinet:1");
    connect(ui->kabinet_4_LE, SIGNAL(textChanged(QString)), mapper_LE, SLOT(map()));
    mapper_LE->setMapping(ui->kabinet_4_LE, "kabinet:4");
    connect(mapper_LE, SIGNAL(mapped(QString)), this, SLOT(find_in_combo(QString)));

    QSignalMapper *mapper_fio_default_TB = new QSignalMapper(0);
    connect(ui->fio_default_2_TB, SIGNAL(clicked()), mapper_fio_default_TB, SLOT(map()));
    mapper_fio_default_TB->setMapping(ui->fio_default_2_TB, 2);
    connect(ui->fio_default_3_TB, SIGNAL(clicked()), mapper_fio_default_TB, SLOT(map()));
    mapper_fio_default_TB->setMapping(ui->fio_default_3_TB, 3);
    connect(mapper_fio_default_TB, SIGNAL(mapped(int)), this, SLOT(add_fio_default(int)));

    QSignalMapper *mapper_fio_TB = new QSignalMapper(0);
    connect(ui->fio_1_TB, SIGNAL(clicked()), mapper_fio_TB, SLOT(map()));
    mapper_fio_TB->setMapping(ui->fio_1_TB, 1);
    connect(ui->fio_4_TB, SIGNAL(clicked()), mapper_fio_TB, SLOT(map()));
    mapper_fio_TB->setMapping(ui->fio_4_TB, 4);
    connect(mapper_fio_TB, SIGNAL(mapped(int)), this, SLOT(add_fio(int)));

    QSignalMapper *mapper_kabinet_TB = new QSignalMapper(0);
    connect(ui->kabinet_1_TB, SIGNAL(clicked()), mapper_kabinet_TB, SLOT(map()));
    mapper_kabinet_TB->setMapping(ui->kabinet_1_TB, 1);
    connect(ui->kabinet_4_TB, SIGNAL(clicked()), mapper_kabinet_TB, SLOT(map()));
    mapper_kabinet_TB->setMapping(ui->kabinet_4_TB, 4);
    connect(mapper_kabinet_TB, SIGNAL(mapped(int)), this, SLOT(add_kabinet(int)));

    connect(ui->pbSave, SIGNAL(clicked()), this, SLOT(save_data()));

}

void TransferCartrAdd::setInit_fio() {
    qu = "SELECT id, family || ' ' || name FROM Person"
           " %1 ORDER by family, name";
    qu = qu.arg(getWhere());
    sqlquery.exec(qu);
    usemodel_fio->setQuery(sqlquery);
    QMapIterator<int, QComboBox *> i(map_fio_combo);
    while (i.hasNext()) {
        i.next();
        i.value()->setModel(usemodel_fio);
        if (i.key() == 2 or i.key() == 3) {
            QSettings settings("config.ini", QSettings::IniFormat);
            QString id_fio_default = settings.value("id_fio_default").toString();
            if (!id_fio_default.isEmpty()) {
                int ind = i.value()->findText(QString("%1").arg(id_fio_default));
                i.value()->setModelColumn(1);
                i.value()->setCurrentIndex(ind);
                continue;
            }
        }
        i.value()->setModelColumn(1);
        i.value()->setCurrentIndex(-1);
    }
}

void TransferCartrAdd::add_fio_default(int nn) {
    QString id = getFioID(nn);
    if (id != "NULL") {
        QSettings settings("config.ini", QSettings::IniFormat);
        settings.setValue("id_fio_default", id);
        QMessageBox::information(this, "Сообщение", QString("Сотрудник: %1\nустановлен по умолчанию успешно!").arg(map_fio_combo.value(nn)->currentText()));
    }
}

void TransferCartrAdd::setInit_cartr() {
    qu = "SELECT id, invNomer || '-->' || model FROM Cartr"
            " %1 ORDER by invNomer";
    qu = qu.arg(getWhere());
    sqlquery.exec(qu);
    usemodel_model->setQuery(sqlquery);
    ui->model_CB->setModel(usemodel_model);
    ui->model_CB->setModelColumn(1);
    ui->model_CB->setCurrentIndex(-1);
}

void TransferCartrAdd::setInit_kabinet() {
    qu = "SELECT id, nomer || ' - ' || title FROM Kabinet"
            " %1 ORDER by nomer";
    qu = qu.arg(getWhere());
    sqlquery.exec(qu);
    usemodel_kabinet->setQuery(sqlquery);
    QMapIterator<int, QComboBox *> i(map_kabinet_combo);
    while (i.hasNext()) {
        i.next();
        i.value()->setModel(usemodel_kabinet);;
        i.value()->setModelColumn(1);
        i.value()->setCurrentIndex(-1);
    }
}

QString TransferCartrAdd::getWhere() {
    if (isNew) return "WHERE pr_arhiv <> 1";
    else return "";
}

void TransferCartrAdd::find_model() {
    if (ui->model_LE->text().isEmpty()) {
        ui->model_CB->setCurrentIndex(-1);
    } else {
        QString tt = QString("0000%1").arg(ui->model_LE->text());
        tt = tt.right(5);
        ui->model_CB->setCurrentIndex(ui->model_CB->findData(tt, 0, Qt::MatchStartsWith));
    }
}

void TransferCartrAdd::find_in_combo(QString param) {
    int nn = param.right(1).toInt();
    QComboBox *combo_var;
    QString text;
    if (param.contains("fio")) {
        combo_var = map_fio_combo.value(nn);
        text = map_fio_lineEdit.value(nn)->text();
    }
    if (param.contains("kabinet")) {
        combo_var = map_kabinet_combo.value(nn);
        text = map_kabinet_lineEdit.value(nn)->text();
    }
    if (text.isEmpty()) combo_var->setCurrentIndex(-1);
    else combo_var->setCurrentIndex(combo_var->findData(text, 0, Qt::MatchStartsWith));
}

void TransferCartrAdd::add_fio(int nn) {
    PersonAddForm *dlg = new PersonAddForm(this);
    dlg->exec();
    int id = dlg->getLastID();
    delete dlg;
    if (id != -1) refresh_fio(nn, id);
}

void TransferCartrAdd::refresh_fio(int& nn, int& id) {
    if (id == -1) return;

    QMap<int, QString> map_fio_text;
    QMapIterator<int, QComboBox *> ii(map_fio_combo);
    while (ii.hasNext()) {
        ii.next();
        if (ii.value()->currentIndex() != -1) map_fio_text.insert(ii.key(), ii.value()->currentText());
    }
    setInit_fio();
    int ind;
    QMapIterator<int, QComboBox *> i(map_fio_combo);
    while (i.hasNext()) {
        i.next();
        QComboBox *fio = i.value();
        if (i.key() == nn) {
            fio->setModelColumn(0);
            ind = fio->findText(QString("%1").arg(id));
            fio->setModelColumn(1);
            fio->setCurrentIndex(ind);
        } else {
            if (map_fio_text.contains(i.key())) {
                QString text = map_fio_text.value(i.key());
                fio->setCurrentIndex(fio->findData(text, Qt::MatchStartsWith));
            }
        }
    }
}

void TransferCartrAdd::add_kabinet(int nn) {
    KabinetAddForm *dlg = new KabinetAddForm(this);
    dlg->exec();
    int id = dlg->getLastID();
    delete dlg;
    if (id != -1) refresh_kabinet(nn, id);
}

void TransferCartrAdd::refresh_kabinet(int& nn, int& id) {
    if (id == -1) return;

    QMap<int, QString> map_kabinet_text;
    QMapIterator<int, QComboBox *> ii(map_kabinet_combo);
    while (ii.hasNext()) {
        ii.next();
        if (ii.value()->currentIndex() != -1) map_kabinet_text.insert(ii.key(), ii.value()->currentText());
    }
    setInit_kabinet();
    int ind;
    QMapIterator<int, QComboBox *> i(map_kabinet_combo);
    while (i.hasNext()) {
        i.next();
        QComboBox *kabinet = i.value();
        if (i.key() == nn) {
            kabinet->setModelColumn(0);
            ind = kabinet->findText(QString("%1").arg(id));
            kabinet->setModelColumn(1);
            kabinet->setCurrentIndex(ind);
        } else {
            if (map_kabinet_text.contains(i.key())) {
                QString text = map_kabinet_text.value(i.key());
                kabinet->setCurrentIndex(kabinet->findData(text, Qt::MatchStartsWith));
            }
        }
    }
}

bool TransferCartrAdd::validate() {
    if (getCartrID()=="NULL") {
        mes_error = "Выберите картридж.";
        return false;
    }
    if (getDate(1)=="NULL") {
        mes_error = "Заполните поле 'Дата получения'";
        return false;
    }
    if (getKabinetID(1)=="NULL") {
        mes_error = "Заполните поле 'С какого кабинета'";
        return false;
    }
    return true;
}

void TransferCartrAdd::save_data() {
    mes_error = "";
    if (isNew) add_data();
    else edit_data();
    if (mes_error.isEmpty()) reject();
    else QMessageBox::critical(this, "Ошибка", mes_error);
}

void TransferCartrAdd::add_data() {
    if (!validate()) return;

    qu = "INSERT INTO Transfer"
            " (cartr_id, date1, kabinet_id_1, person_id_1, comment, "
            " date2, person_id_2, date3, person_id_3, date4, kabinet_id_4, person_id_4)"
            " VALUES(%1, %2, %3, %4, '%5', %6, %7, %8, %9, %10, %11, %12)";
    qu = qu.arg(getCartrID()).arg(getDate(1)).arg(getKabinetID(1)).arg(getFioID(1)).arg(ui->comment_LE->text()).arg(getDate(2)).arg(getFioID(2)).arg(getDate(3)).arg(getFioID(3)).arg(getDate(4)).arg(getKabinetID(4)).arg(getFioID(4));
    if (!sqlquery.exec(qu)) {
        mes_error = sqlquery.lastError().databaseText();
        return;
    }
}

void TransferCartrAdd::edit_data() {
    if (!validate()) return;

    qu = "UPDATE Transfer SET date1 = %2, kabinet_id_1 = %3, person_id_1 = %4, comment = '%5', "
        " date2 = %6, person_id_2 = %7, date3 = %8, person_id_3 = %9,"
        " date4 = %10, kabinet_id_4 = %11, person_id_4 = %12"
        " WHERE id = %1";
    qu = qu.arg(useID).arg(getDate(1)).arg(getKabinetID(1)).arg(getFioID(1)).arg(ui->comment_LE->text()).arg(getDate(2)).arg(getFioID(2)).arg(getDate(3)).arg(getFioID(3)).arg(getDate(4)).arg(getKabinetID(4)).arg(getFioID(4));
    if (!sqlquery.exec(qu)) {
        mes_error = sqlquery.lastError().databaseText();
        return;
    }
}

QString TransferCartrAdd::getCartrID() {
    int ind = ui->model_CB->currentIndex();
    if (ind < 0) return "NULL";
    QSqlRecord rec = usemodel_model->record(ind);
    return rec.value(0).toString();
}

QString TransferCartrAdd::getFioID(int nn) {
    if ((getDate(nn)) == "NULL") return "NULL";
    int ind = map_fio_combo.value(nn)->currentIndex();
    if (ind < 0) return "NULL";
    QSqlRecord rec = usemodel_fio->record(ind);
    return rec.value(0).toString();
}

QString TransferCartrAdd::getKabinetID(int nn) {
    int ind = map_kabinet_combo.value(nn)->currentIndex();
    if (ind < 0) return "NULL";;
    QSqlRecord rec = usemodel_kabinet->record(ind);
    return rec.value(0).toString();
}

QString TransferCartrAdd::getDate(int nn) {
    QDate DD;
    DD = map_date_DateEditBox.value(nn)->getDate();
    if (DD.isNull()) return "NULL";
    else return QString("'%1'").arg(DD.toString(Qt::ISODate));
}

void TransferCartrAdd::loadInfo() {
    qu = "SELECT cartr_id, date1, kabinet_id_1, person_id_1, comment, "
         " date2, person_id_2, date3, person_id_3, date4, kabinet_id_4, person_id_4"
         " FROM transfer WHERE id = %1";
    qu = qu.arg(useID);
    if(!sqlquery.exec(qu)){
        QMessageBox::warning(this, "Ошибка!", sqlquery.lastError().databaseText());
        return;
    }
    sqlquery.first();
    ui->model_LE->setText(sqlquery.value(0).toString());
    ui->comment_LE->setText(sqlquery.value(4).toString());

    setData(1, sqlquery.value(1).toString(), sqlquery.value(2).toString(), sqlquery.value(3).toString());
    setData(2, sqlquery.value(5).toString(), "", sqlquery.value(6).toString());
    setData(3, sqlquery.value(7).toString(), "", sqlquery.value(8).toString());
    setData(4, sqlquery.value(9).toString(), sqlquery.value(10).toString(), sqlquery.value(11).toString());
}

void TransferCartrAdd::setData(int nn, QString DD, QString kabinet, QString fio) {

    if (!DD.isEmpty()) {
        QDate d;
        d.setDate(DD.mid(0,4).toInt(), DD.mid(5,2).toInt(), DD.mid(8,2).toInt());
        map_date_DateEditBox.value(nn)->setDate(d);
    }

    if (!kabinet.isEmpty()) {
        map_kabinet_combo.value(nn)->setModelColumn(0);
        map_kabinet_combo.value(nn)->setCurrentIndex(map_kabinet_combo.value(nn)->findData(kabinet, Qt::MatchStartsWith));
        map_kabinet_combo.value(nn)->setModelColumn(1);
    }

    if (!fio.isEmpty()) {
        map_fio_combo.value(nn)->setModelColumn(0);
        map_fio_combo.value(nn)->setCurrentIndex(map_fio_combo.value(nn)->findData(fio, Qt::MatchStartsWith));
        map_fio_combo.value(nn)->setModelColumn(1);
    }
}

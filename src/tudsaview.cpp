#include "tudsaview.h"

TudsaWidget::TudsaWidget() {
    setupUi(this);
//    connect(add_PB, SIGNAL(clicked()), this, SLOT(add_data()));
    connect(del_PB, SIGNAL(clicked()), this, SLOT(delete_data()));
    connect(addTudsa_PB, SIGNAL(clicked()), this, SLOT(add_to_down()));
    connect(addStock_PB, SIGNAL(clicked()), this, SLOT(add_to_up()));
    connect(fio_default_TB, SIGNAL(clicked()), this, SLOT(add_fio_default()));
    connect(save_PB, SIGNAL(clicked()), this, SLOT(save_data()));

    connect(tableView1, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(add_to_down()));
    connect(tableView2, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(add_to_up()));

    usemodel1 = new DataModel();
    usemodel2 = new DataModel();
    usemodel_fio = new QSqlQueryModel();

    date_DE->setDate(QDate::currentDate());
    setInit_fio();

    QFont font("Helvetica", 9, QFont::Bold);
    tableView1->horizontalHeader()->setFont(font);
    tableView1->verticalHeader()->setHidden(true);
    tableView2->horizontalHeader()->setFont(font);
    tableView2->verticalHeader()->setHidden(true);
    LoadData();
}

void TudsaWidget::setInit_fio() {
    qu = "SELECT id, family || ' ' || name"
           " FROM Person WHERE pr_arhiv<>1 ORDER by family, name";
    sqlquery.exec(qu);
    usemodel_fio->setQuery(sqlquery);
    fio_CB->setModel(usemodel_fio);

    QSettings settings("config.ini", QSettings::IniFormat);
    QString id_fio_default = settings.value("id_fio_default").toString();
    if (!id_fio_default.isEmpty()) {
        int ind = fio_CB->findText(QString("%1").arg(id_fio_default));
        fio_CB->setModelColumn(1);
        fio_CB->setCurrentIndex(ind);
    } else {
        fio_CB->setModelColumn(1);
        fio_CB->setCurrentIndex(-1);
    }
}

void TudsaWidget::add_fio_default() {
    QSettings settings("config.ini", QSettings::IniFormat);
    int id = getFioID();
    if (id>-1) {
        settings.setValue("id_fio_default", id);
    }
}

int TudsaWidget::getFioID() {
    int nn = fio_CB->currentIndex();
    if (nn>-1) {
        QSqlRecord rec = usemodel_fio->record(nn);
        nn = rec.value(0).toInt();
    }
    return nn;
}

void TudsaWidget::save_data() {
    mes_error = "";
    if (!validate()) {
        QMessageBox::warning(this, "Ошибка!!!", mes_error);
        return;
    }

    for (int ii = 0; ii < usemodel2->rowCount(); ii++) {
        int id = usemodel2->data(usemodel2->index(ii, 0)).toInt();
        qu = "INSERT INTO Transfer(cartr_id, date, person_id, status) VALUES(%1, '%2', %3, %4)";
        qu = qu.arg(id).arg(getDate()).arg(getFioID()).arg(21);
        sqlquery.exec(qu);

        qu = "UPDATE Transfer SET pr_arhiv = 1 WHERE id = %1";
        qu = qu.arg(id);
        sqlquery.exec(qu);
    }
    LoadData();
}

bool TudsaWidget::validate() {
    if (getFioID() == -1) {
        mes_error = "Необходимо выбрать сотрудника !";
        return false;
    }
    return true;
}

void TudsaWidget::add_to_down() {
    useindex = tableView1->currentIndex();
    QString str_id = usemodel1->data(usemodel1->index(useindex.row(), 0), 0).toString();
    if (str_id.isEmpty()) return;

    int id = usemodel1->data(usemodel1->index(useindex.row(), 0), 0).toInt();
    qu = "UPDATE Transfer SET status = 12 WHERE id = %1";
    qu = qu.arg(id);
    sqlquery.exec(qu);
    if (!sqlquery.exec(qu)) {
        QMessageBox::warning(this, "Ошибка!!!", sqlquery.lastError().databaseText());
        return;
    }
    LoadData();
}

void TudsaWidget::add_to_up() {
    useindex = tableView2->currentIndex();
    QString str_id = usemodel2->data(usemodel2->index(useindex.row(), 0), 0).toString();
    if (str_id.isEmpty()) return;

    int id = usemodel2->data(usemodel2->index(useindex.row(), 0), 0).toInt();
    qu = "UPDATE Transfer SET status = 11 WHERE id = %1";
    qu = qu.arg(id);
    sqlquery.exec(qu);
    if (!sqlquery.exec(qu)) {
        QMessageBox::warning(this, "Ошибка!!!", sqlquery.lastError().databaseText());
        return;
    }
    LoadData();
}

void TudsaWidget::delete_data(){
    useindex = tableView1->currentIndex();
    QString str_id = usemodel1->data(usemodel1->index(useindex.row(), 0), 0).toString();
    if (str_id.isEmpty()) return;

    int id = usemodel1->data(usemodel1->index(useindex.row(), 0), 0).toInt();
    QString date = usemodel1->data(usemodel1->index(useindex.row(), 2), 0).toString();
    QString invNomer = usemodel1->data(usemodel1->index(useindex.row(), 3), 0).toString();
    QString model = usemodel1->data(usemodel1->index(useindex.row(), 4), 0).toString();

    QString strhtml;
    strhtml = "<big style=\"color: red; font-weight: 10px;\"><big>Вы действительно хотите удалить?</big></big><br>";
    strhtml += "<table style=\"text-align: left; width: 100%;\" border=0 cellpadding=2 cellspacing=2><tbody>";
    strhtml += "<tr><td style=\"width: 50%;\" align=\"right\" valign=\"top\"><big style=\"color: black;\">%1</big></td><td style=\"width: 50%;\" valign=\"top\"><big style=\"color: blue;\">%2</big></td></tr>";
    strhtml = strhtml.arg("Дата:").arg(date);
    strhtml += "<tr><td style=\"width: 50%;\" align=\"right\" valign=\"top\"><big style=\"color: black;\">%1</big></td><td style=\"width: 50%;\" valign=\"top\"><big style=\"color: blue;\">%2</big></td></tr>";
    strhtml = strhtml.arg("Инвентарный номер:").arg(invNomer);
    strhtml += "<tr><td style=\"width: 50%;\" align=\"right\" valign=\"top\"><big style=\"color: black;\">%1</big></td><td style=\"width: 50%;\" valign=\"top\"><big style=\"color: blue;\">%2</big></td></tr>";
    strhtml = strhtml.arg("Модель:").arg(model);
    strhtml += "</tbody></table><br>";

    int r = QMessageBox::question(this,
        "Вопрос", strhtml,
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No);
    if (r == QMessageBox::No) return;

    qu = "DELETE from Transfer WHERE id = %1";
    qu = qu.arg(id);
    sqlquery.exec(qu);
    if (!sqlquery.exec(qu)) {
        QMessageBox::warning(this, "Ошибка!!!", sqlquery.lastError().databaseText());
        return;
    }
    LoadData();
}

void TudsaWidget::LoadData() {
    qu = "SELECT Tr.id, Tr.cartr_id, Tr.date, Cartr.invNomer, Cartr.model, Cartr.serialNo1, Cartr.serialNo2, Person.family || ' ' ||Person.name"
                    " FROM (SELECT * FROM Transfer WHERE pr_arhiv = 0 AND status = 11) as Tr"
                    " LEFT OUTER JOIN Person ON Tr.person_id = Person.id"
                    " LEFT OUTER JOIN Cartr ON Tr.cartr_id = Cartr.id"
                    " ORDER by Tr.date DESC, Person.family, Cartr.invNomer";
    usemodel1->setQuery(qu);
    if (usemodel1->lastError().isValid()) {
        QMessageBox::warning(this, "Ошибка!!!", usemodel1->lastError().databaseText());
        return;
    }
    while (usemodel1->canFetchMore()) {
         usemodel1->fetchMore();
    }

    SetModelsParam(usemodel1);
    tableView1->setModel(usemodel1);
    tableView1->hideColumn(0); tableView1->hideColumn(1);
    tableView1->resizeColumnsToContents();
    tableView1->selectRow(0);

//**************************************************************

    qu = "SELECT Tr.id, Tr.cartr_id, Tr.date, Cartr.invNomer, Cartr.model, Cartr.serialNo1, Cartr.serialNo2, Person.family || ' ' ||Person.name"
                    " FROM (SELECT * FROM Transfer WHERE pr_arhiv = 0 AND status = 12) as Tr"
                    " LEFT OUTER JOIN Person ON Tr.person_id = Person.id"
                    " LEFT OUTER JOIN Cartr ON Tr.cartr_id = Cartr.id"
                    " ORDER by Tr.date DESC, Person.family, Cartr.invNomer";
    usemodel2->setQuery(qu);
    if (usemodel2->lastError().isValid()) {
        QMessageBox::warning(this, "Ошибка!!!", usemodel2->lastError().databaseText());
        return;
    }
    while (usemodel2->canFetchMore()) {
         usemodel2->fetchMore();
    }

    SetModelsParam(usemodel2);
    tableView2->setModel(usemodel2);
    tableView2->hideColumn(0); tableView2->hideColumn(1);
    tableView2->resizeColumnsToContents();
    tableView2->selectRow(0);

}


void TudsaWidget::SetModelsParam(DataModel*& usemodel) {
    int ii = 1;
    ii++;
    usemodel->setHeaderData(ii, Qt::Horizontal, "Дата");
    usemodel->setHeaderData(ii, Qt::Horizontal, QSize(100, 36), Qt::SizeHintRole);
    usemodel->setHAlign(ii, "center");
    usemodel->setColor(ii, "red");
    usemodel->setBold(ii);

    ii++;
    usemodel->setHeaderData(ii, Qt::Horizontal, "Инв.номер");
    usemodel->setHeaderData(ii, Qt::Horizontal, QSize(80, 36), Qt::SizeHintRole);
    usemodel->setHAlign(ii, "center");
    usemodel->setColor(ii, "blue");
    usemodel->setBold(ii);

    ii++;
    usemodel->setHeaderData(ii, Qt::Horizontal, "Модель");
    usemodel->setHeaderData(ii, Qt::Horizontal, QSize(130, 36), Qt::SizeHintRole);
    usemodel->setHAlign(ii, "center");
    usemodel->setPointSize(ii, 9);

    ii++;
    usemodel->setHeaderData(ii, Qt::Horizontal,tr("Серийный номер"));
    usemodel->setHeaderData(ii, Qt::Horizontal, QSize(200, 36), Qt::SizeHintRole);
    usemodel->setHAlign(ii, "left");
    usemodel->setPointSize(ii, 10);

    ii++;
    usemodel->setHeaderData(ii, Qt::Horizontal,tr("Серийный номер\n(без букв)"));
    usemodel->setHeaderData(ii, Qt::Horizontal, QSize(130, 36), Qt::SizeHintRole);
    usemodel->setHAlign(ii, "center");
    usemodel->setPointSize(ii, 10);

    ii++;
    usemodel->setHeaderData(ii, Qt::Horizontal, "Сотрудник,\nкоторый передал");
    usemodel->setHeaderData(ii, Qt::Horizontal, QSize(140, 36), Qt::SizeHintRole);
    usemodel->setHAlign(ii, "left");
    usemodel->setPointSize(ii, 10);

}

void TudsaWidget::refreshData() {
    LoadData();
}

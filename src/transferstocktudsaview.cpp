#include "transferstocktudsaview.h"

TransferStockTudsaWidget::TransferStockTudsaWidget() {
    setupUi(this);
    connect(stockTudsa_add_PB, SIGNAL(clicked()), this, SLOT(stockTudsa_add()));
    connect(tudsa_add_PB, SIGNAL(clicked()), this, SLOT(tudsa_add()));
    connect(edit_PB, SIGNAL(clicked()), this, SLOT(edit_data()));
    connect(del_PB, SIGNAL(clicked()), this, SLOT(delete_data()));
    connect(nomer_1_LE, SIGNAL(textChanged(QString)), this, SLOT(text_changed_1()));
    connect(clear_1_TB, SIGNAL(clicked()), this, SLOT(clear_filter_1()));
    connect(tableView1, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(edit_data()));
    connect(tudsa_arhiv_PB, SIGNAL(clicked()), this, SLOT(tudsa_arhiv()));
    connect(edit_tudsa_PB, SIGNAL(clicked()), this, SLOT(edit_data_2()));
    connect(del_tudsa_PB, SIGNAL(clicked()), this, SLOT(delete_data_2()));
    connect(tableView2, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(edit_data_2()));
    connect(nomer_2_LE, SIGNAL(textChanged(QString)), this, SLOT(text_changed_2()));
    connect(clear_2_TB, SIGNAL(clicked()), this, SLOT(clear_filter_2()));

    usemodel1 = new DataModel();
    usemodel2 = new DataModel();

    QFont font("Helvetica", 9, QFont::Bold);
    tableView1->horizontalHeader()->setFont(font);
    LoadData1();

    tableView2->horizontalHeader()->setFont(font);
    LoadData2();
}

void TransferStockTudsaWidget::tudsa_add() {
    TransferTudsaToAdd *dlg = new TransferTudsaToAdd(this);
    dlg->exec();
    delete dlg;
    LoadData1();
    LoadData2();
}

void TransferStockTudsaWidget::stockTudsa_add() {
    Transferstockadd *dlg = new Transferstockadd("1", this);
    dlg->exec();
    delete dlg;
    LoadData1();
    LoadData2();
}

void TransferStockTudsaWidget::edit_data() {
    useindex = tableView1->currentIndex();
    QString str_id = usemodel1->data(usemodel1->index(useindex.row(), 0), 0).toString();
    if (str_id.isEmpty()) return;

    int id = usemodel1->data(usemodel1->index(useindex.row(), 0), 0).toInt();
    Transferstockadd *dlg = new Transferstockadd(id, this);
    dlg->exec();
    delete dlg;
    LoadData1();
}

void TransferStockTudsaWidget::delete_data(){
    useindex = tableView1->currentIndex();
    QString str_id = usemodel1->data(usemodel1->index(useindex.row(), 0), 0).toString();
    if (str_id.isEmpty()) return;

    int id = usemodel1->data(usemodel1->index(useindex.row(), 0), 0).toInt();
    QString invNomer = usemodel1->data(usemodel1->index(useindex.row(), 1), 0).toString();
    QString model = usemodel1->data(usemodel1->index(useindex.row(), 2), 0).toString();

    QString strhtml;
    strhtml = "<big style=\"color: red; font-weight: 10px;\"><big>Вы действительно хотите удалить?</big></big><br>";
    strhtml += "<table style=\"text-align: left; width: 100%;\" border=0 cellpadding=2 cellspacing=2><tbody>";
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
    LoadData1();
}

void TransferStockTudsaWidget::text_changed_1() {
    LoadData1();
}

void TransferStockTudsaWidget::clear_filter_1() {
    nomer_1_LE->setText("");
}

void TransferStockTudsaWidget::text_changed_2() {
    LoadData2();
}

void TransferStockTudsaWidget::clear_filter_2() {
    nomer_2_LE->setText("");
}

void TransferStockTudsaWidget::LoadData1() {

    qu = "SELECT tr.id, Cartr.invNomer, Cartr.model,"
           " k1.nomer || ' - ' || k1.title, p1.family || ' ' ||p1.name, tr.date1, tr.comment";
    if (nomer_1_LE->text().isEmpty()) qu += " FROM (SELECT * FROM Transfer WHERE pr_tudsa = 1 and date2 IS NULL and date4 IS NULL) as tr";
    else {
    QString str = " FROM (SELECT ttt.* FROM (SELECT * FROM Transfer WHERE pr_tudsa = 1 and date2 IS NULL and date4 IS NULL) as ttt,"
                       " (SELECT * FROM Cartr WHERE Cartr.invNomer LIKE '%%1') as ccc"
                       " WHERE ttt.cartr_id = ccc.id) as tr";
    qu += str.arg(nomer_1_LE->text());
    }
    qu +=  " LEFT OUTER JOIN Cartr ON tr.cartr_id = Cartr.id"
            " LEFT OUTER JOIN Person  as p1 ON tr.person_id_1 = p1.id"
            " LEFT OUTER JOIN Kabinet as k1 ON tr.kabinet_id_1 = k1.id"
            " ORDER by 2";
    usemodel1->setQuery(qu);
    if (usemodel1->lastError().isValid()) {
        QMessageBox::warning(this, "Ошибка!!!", usemodel1->lastError().databaseText());
        return;
    }
    while (usemodel1->canFetchMore()) {
         usemodel1->fetchMore();
    }

    SetModelsParam1();
    tableView1->setModel(usemodel1);
    tableView1->hideColumn(0);
    tableView1->resizeColumnsToContents();
}

void TransferStockTudsaWidget::SetModelsParam1() {
    int ii = 0;

    ii++;
    usemodel1->setHeaderData(ii, Qt::Horizontal, "Инв.номер");
    usemodel1->setHeaderData(ii, Qt::Horizontal, QSize(80, 36), Qt::SizeHintRole);
    usemodel1->setHAlign(ii, "center");
    usemodel1->setColor(ii, "blue");
    usemodel1->setPointSize(ii, 11);
    usemodel1->setBold(ii);

    ii++;
    usemodel1->setHeaderData(ii, Qt::Horizontal, "Модель");
    usemodel1->setHeaderData(ii, Qt::Horizontal, QSize(180, 36), Qt::SizeHintRole);
    usemodel1->setPointSize(ii, 10);

    ii++;
    usemodel1->setHeaderData(ii, Qt::Horizontal, "С какого\nкабинета");
    usemodel1->setHeaderData(ii, Qt::Horizontal, QSize(140, 36), Qt::SizeHintRole);
    usemodel1->setHAlign(ii, "left");
    usemodel1->setPointSize(ii, 10);

    ii++;
    usemodel1->setHeaderData(ii, Qt::Horizontal, "Кто передал");
    usemodel1->setHeaderData(ii, Qt::Horizontal, QSize(180, 36), Qt::SizeHintRole);
    usemodel1->setHAlign(ii, "left");
    usemodel1->setPointSize(ii, 10);

    ii++;
    usemodel1->setHeaderData(ii, Qt::Horizontal, "Дата\nполучения");
    usemodel1->setHeaderData(ii, Qt::Horizontal, QSize(100, 36), Qt::SizeHintRole);
    usemodel1->setHAlign(ii, "center");
    usemodel1->setColor(ii, "red");
    usemodel1->setDate(ii, "");
    usemodel1->setBold(ii);

    ii++;
    usemodel1->setHeaderData(ii, Qt::Horizontal, "Примечания");
    usemodel1->setHeaderData(ii, Qt::Horizontal, QSize(300, 36), Qt::SizeHintRole);
    usemodel1->setHeaderData(ii, Qt::Horizontal, Qt::AlignVCenter, Qt::TextAlignmentRole);
}

void TransferStockTudsaWidget::tudsa_arhiv() {
    TransferTudsaWidget *dlg = new TransferTudsaWidget();
    dlg->exec();
    delete dlg;
}

void TransferStockTudsaWidget::edit_data_2() {
    useindex = tableView2->currentIndex();
    QString str_id = usemodel2->data(usemodel2->index(useindex.row(), 0), 0).toString();
    if (str_id.isEmpty()) return;

    int id = usemodel2->data(usemodel2->index(useindex.row(), 0), 0).toInt();
    TransferTudsaToAdd *dlg = new TransferTudsaToAdd(id, this);
    dlg->exec();
    delete dlg;
    LoadData1();
    LoadData2();
}

void TransferStockTudsaWidget::delete_data_2(){
    useindex = tableView2->currentIndex();
    QString str_id = usemodel2->data(usemodel2->index(useindex.row(), 0), 0).toString();
    if (str_id.isEmpty()) return;

    int id = usemodel2->data(usemodel2->index(useindex.row(), 0), 0).toInt();
    QString date = usemodel2->data(usemodel2->index(useindex.row(), 1), 0).toString();
    QString invNomer = usemodel2->data(usemodel2->index(useindex.row(), 2), 0).toString();

    QString strhtml;
    strhtml = "<big style=\"color: red; font-weight: 10px;\"><big>Вы действительно хотите удалить?</big></big><br>";
    strhtml += "<table style=\"text-align: left; width: 100%;\" border=0 cellpadding=2 cellspacing=2><tbody>";
    strhtml += "<tr><td style=\"width: 50%;\" align=\"right\" valign=\"top\"><big style=\"color: black;\">%1</big></td><td style=\"width: 50%;\" valign=\"top\"><big style=\"color: blue;\">%2</big></td></tr>";
    strhtml = strhtml.arg("Дата:").arg(date);
    strhtml += "<tr><td style=\"width: 50%;\" align=\"right\" valign=\"top\"><big style=\"color: black;\">%1</big></td><td style=\"width: 50%;\" valign=\"top\"><big style=\"color: blue;\">%2</big></td></tr>";
    strhtml = strhtml.arg("Инвентарный номер:").arg(invNomer);
    strhtml += "</tbody></table><br>";

    int r = QMessageBox::question(this,
        "Вопрос", strhtml,
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No);
    if (r == QMessageBox::No) return;

    qu = "UPDATE Transfer SET date2 = NULL, person_id_2 = NULL"
              " WHERE id = %1";
    qu = qu.arg(id);
    sqlquery.exec(qu);
    if (!sqlquery.exec(qu)) {
        QMessageBox::warning(this, "Ошибка!!!", sqlquery.lastError().databaseText());
        return;
    }
    LoadData1();
    LoadData2();
}

void TransferStockTudsaWidget::LoadData2() {

    qu = "SELECT tr.id, tr.date2, Cartr.invNomer, Cartr.model, p2.family || ' ' ||p2.name";
    if (nomer_2_LE->text().isEmpty()) qu += " FROM (SELECT * FROM Transfer WHERE date2 NOT NULL AND date3 IS NULL) as tr";
    else {
    QString str = " FROM (SELECT ttt.* FROM (SELECT * FROM Transfer WHERE date2 NOT NULL AND date3 IS NULL) as ttt,"
                       " (SELECT * FROM Cartr WHERE Cartr.invNomer LIKE '%%1') as ccc"
                       " WHERE ttt.cartr_id = ccc.id) as tr";
    qu += str.arg(nomer_2_LE->text());
    }
    qu +=  " LEFT OUTER JOIN Cartr ON tr.cartr_id = Cartr.id"
           " LEFT OUTER JOIN Person  as p2 ON tr.person_id_2 = p2.id"
           " ORDER by 2, 3";
    usemodel2->setQuery(qu);
    if (usemodel2->lastError().isValid()) {
        QMessageBox::warning(this, "Ошибка!!!", usemodel2->lastError().databaseText());
        return;
    }
    while (usemodel2->canFetchMore()) {
         usemodel2->fetchMore();
    }

    SetModelsParam2();
    tableView2->setModel(usemodel2);
    tableView2->hideColumn(0);
    tableView2->resizeColumnsToContents();
    tableView2->horizontalHeader()->setStretchLastSection(true);
}

void TransferStockTudsaWidget::SetModelsParam2() {
    int ii = 0;

    ii++;
    usemodel2->setHeaderData(ii, Qt::Horizontal, "Дата отправки");
    usemodel2->setHeaderData(ii, Qt::Horizontal, QSize(100, 36), Qt::SizeHintRole);
    usemodel2->setHAlign(ii, "center");
    usemodel2->setColor(ii, "red");
    usemodel2->setDate(ii,"");
    usemodel2->setBold(ii);

    ii++;
    usemodel2->setHeaderData(ii, Qt::Horizontal, "Инв.номер");
    usemodel2->setHeaderData(ii, Qt::Horizontal, QSize(80, 36), Qt::SizeHintRole);
    usemodel2->setHAlign(ii, "center");
    usemodel2->setColor(ii, "blue");
    usemodel2->setPointSize(ii, 11);
    usemodel2->setBold(ii);

    ii++;
    usemodel2->setHeaderData(ii, Qt::Horizontal, "Модель");
    usemodel2->setHeaderData(ii, Qt::Horizontal, QSize(220, 36), Qt::SizeHintRole);
    usemodel2->setHAlign(ii, "left");
    usemodel2->setPointSize(ii, 10);

    ii++;
    usemodel2->setHeaderData(ii, Qt::Horizontal, "Кто отвез");
    usemodel2->setHeaderData(ii, Qt::Horizontal, QSize(180, 36), Qt::SizeHintRole);
    usemodel2->setHAlign(ii, "left");
}

void TransferStockTudsaWidget::refreshData() {
    LoadData1();
    LoadData2();
}

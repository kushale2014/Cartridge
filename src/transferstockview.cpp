#include "transferstockview.h"

TransferStockWidget::TransferStockWidget() {
    setupUi(this);
    connect(AddKabinet_PB, SIGNAL(clicked()), this, SLOT(AddKabinet_data()));
    connect(del_PB, SIGNAL(clicked()), this, SLOT(delete_data()));
    connect(nomer_1_LE, SIGNAL(textChanged(QString)), this, SLOT(text_changed_1()));
    connect(clear_1_TB, SIGNAL(clicked()), this, SLOT(clear_filter_1()));
    connect(tableView1, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(edit_data()));
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


void TransferStockWidget::AddTudsa_data() {
    Transferstockadd *dlg = new Transferstockadd("1", this);
    dlg->exec();
    delete dlg;
    LoadData1();
}

void TransferStockWidget::AddKabinet_data() {
    Transferstockadd *dlg = new Transferstockadd("0", this);
    dlg->exec();
    delete dlg;
    LoadData1();
}

void TransferStockWidget::edit_data() {
    useindex = tableView1->currentIndex();
    QString str_id = usemodel1->data(usemodel1->index(useindex.row(), 0), 0).toString();
    if (str_id.isEmpty()) return;

    int id = usemodel1->data(usemodel1->index(useindex.row(), 0), 0).toInt();
    Transferstockadd *dlg = new Transferstockadd(id, this);
    dlg->exec();
    delete dlg;
    LoadData1();
}

void TransferStockWidget::delete_data(){
    useindex = tableView1->currentIndex();
    QString str_id = usemodel1->data(usemodel1->index(useindex.row(), 0), 0).toString();
    if (str_id.isEmpty()) return;

    int id = usemodel1->data(usemodel1->index(useindex.row(), 0), 0).toInt();
    QString date = usemodel1->data(usemodel1->index(useindex.row(), 1), 0).toString();
    QString invNomer = usemodel1->data(usemodel1->index(useindex.row(), 2), 0).toString();
    QString model = usemodel1->data(usemodel1->index(useindex.row(), 3), 0).toString();

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
    LoadData1();
}

void TransferStockWidget::text_changed_1() {
    LoadData1();
}

void TransferStockWidget::clear_filter_1() {
    nomer_1_LE->setText("");
}

void TransferStockWidget::text_changed_2() {
    LoadData2();
}

void TransferStockWidget::clear_filter_2() {
    nomer_2_LE->setText("");
}

void TransferStockWidget::LoadData1() {

    qu = "SELECT tr.id, Cartr.invNomer,"
           " k1.nomer || ' - ' || k1.title, p1.family || ' ' ||p1.name, tr.date1, tr.comment";
    if (nomer_1_LE->text().isEmpty()) qu += " FROM (SELECT * FROM Transfer WHERE pr_tudsa = 0 and date2 IS NULL and date4 IS NULL) as tr";
    else {
    QString str = " FROM (SELECT ttt.* FROM (SELECT * FROM Transfer WHERE pr_tudsa = 0 and date2 IS NULL and date4 IS NULL) as ttt,"
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

    SetModelsParam(usemodel1);
    tableView1->setModel(usemodel1);
    tableView1->hideColumn(0);
    tableView1->resizeColumnsToContents();
}

void TransferStockWidget::LoadData2() {

    qu = "SELECT tr.id, Cartr.invNomer,"
           " k1.nomer || ' - ' || k1.title, p1.family || ' ' ||p1.name,"
           " tr.date1, tr.comment";
    if (nomer_2_LE->text().isEmpty()) qu += " FROM (SELECT * FROM Transfer WHERE date2 NOT NULL or date4 NOT NULL) as tr";
    else {
    QString str = " FROM (SELECT ttt.*"
            " FROM (SELECT * FROM Transfer WHERE date2 NOT NULL or date4 NOT NULL) as ttt,"
                 " (SELECT * FROM Cartr WHERE Cartr.invNomer LIKE '%%1') as ccc"
            " WHERE ttt.cartr_id = ccc.id) as tr";
    qu += str.arg(nomer_2_LE->text());
    }
    qu +=  " LEFT OUTER JOIN Cartr ON tr.cartr_id = Cartr.id"
            " LEFT OUTER JOIN Person  as p1 ON tr.person_id_1 = p1.id"
            " LEFT OUTER JOIN Kabinet as k1 ON tr.kabinet_id_1 = k1.id"
            " ORDER by 2";
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
    tableView2->hideColumn(0);
    tableView2->resizeColumnsToContents();
}

void TransferStockWidget::SetModelsParam(DataModel*& usemodel) {
    int ii = 0;

    ii++;
    usemodel->setHeaderData(ii, Qt::Horizontal, "Инв.номер");
    usemodel->setHeaderData(ii, Qt::Horizontal, QSize(80, 36), Qt::SizeHintRole);
    usemodel->setHAlign(ii, "center");
    usemodel->setColor(ii, "blue");
    usemodel->setPointSize(ii, 11);
    usemodel->setBold(ii);

    ii++;
    usemodel->setHeaderData(ii, Qt::Horizontal, "С какого\nкабинета");
    usemodel->setHeaderData(ii, Qt::Horizontal, QSize(140, 36), Qt::SizeHintRole);
    usemodel->setHAlign(ii, "left");
    usemodel->setPointSize(ii, 10);

    ii++;
    usemodel->setHeaderData(ii, Qt::Horizontal, "Кто передал");
    usemodel->setHeaderData(ii, Qt::Horizontal, QSize(180, 36), Qt::SizeHintRole);
    usemodel->setHAlign(ii, "left");
    usemodel->setPointSize(ii, 10);

    ii++;
    usemodel->setHeaderData(ii, Qt::Horizontal, "Дата\nполучения");
    usemodel->setHeaderData(ii, Qt::Horizontal, QSize(100, 36), Qt::SizeHintRole);
    usemodel->setHAlign(ii, "center");
    usemodel->setColor(ii, "red");
    usemodel->setDate(ii, "");
    usemodel->setBold(ii);

    ii++;
    usemodel->setHeaderData(ii, Qt::Horizontal, "Примечания");
    usemodel->setHeaderData(ii, Qt::Horizontal, QSize(300, 36), Qt::SizeHintRole);
    usemodel->setHeaderData(ii, Qt::Horizontal, Qt::AlignVCenter, Qt::TextAlignmentRole);
}

void TransferStockWidget::refreshData() {
    LoadData1();
    LoadData2();
}

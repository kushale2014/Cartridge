#include "transferkabinetview.h"

TransferKabinetWidget::TransferKabinetWidget() {
    setupUi(this);
    connect(AddFromTudsa_PB, SIGNAL(clicked()), this, SLOT(AddFromTudsa_data()));
    connect(AddFromKabinet_PB, SIGNAL(clicked()), this, SLOT(AddFromKabinet_data()));
    connect(AddToKabinet_PB, SIGNAL(clicked()), this, SLOT(add_data()));
    connect(tableView1, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(add_data_with_id()));
    connect(tableView1->horizontalHeader(),SIGNAL(sectionClicked(int)), this, SLOT(sortByColumn1(int)));
    connect(del_PB, SIGNAL(clicked()), this, SLOT(delete_data()));
    connect(edit_PB, SIGNAL(clicked()), this, SLOT(edit_data()));
    connect(nomer_1_LE, SIGNAL(textChanged(QString)), this, SLOT(text_changed_1()));
    connect(clear_1_TB, SIGNAL(clicked()), this, SLOT(clear_filter_1()));
    connect(nomer_2_LE, SIGNAL(textChanged(QString)), this, SLOT(text_changed_2()));
    connect(clear_2_TB, SIGNAL(clicked()), this, SLOT(clear_filter_2()));
    connect(Tek_RB, SIGNAL(clicked()), this, SLOT(Tek_RB_clicked()));
    connect(All_RB, SIGNAL(clicked()), this, SLOT(All_RB_clicked()));

    usemodel1 = new DataModel();
    usemodel2 = new DataModel();

    QFont font("Helvetica", 9, QFont::Bold);
    tableView1->horizontalHeader()->setFont(font);
    tableView1->horizontalHeader()->setSortIndicator(1, Qt::AscendingOrder);
    sortByColumn1(1);

    tableView2->horizontalHeader()->setFont(font);
    LoadData2();
}


void TransferKabinetWidget::AddFromTudsa_data() {
    TransferTudsaFromAdd *dlg = new TransferTudsaFromAdd(this);
    dlg->exec();
    delete dlg;
    LoadData1();
    LoadData2();
}

void TransferKabinetWidget::AddFromKabinet_data() {
    Transferstockadd *dlg = new Transferstockadd("0", this);
    dlg->exec();
    delete dlg;
    LoadData1();
    LoadData2();
}

void TransferKabinetWidget::add_data() {
    TransferKabinetAdd *dlg = new TransferKabinetAdd(this);
    dlg->exec();
    delete dlg;
    LoadData1();
    LoadData2();
}

void TransferKabinetWidget::edit_data() {
    useindex = tableView2->currentIndex();
    QString str_id = usemodel2->data(usemodel2->index(useindex.row(), 0), 0).toString();
    if (str_id.isEmpty()) return;

    int id = usemodel2->data(usemodel2->index(useindex.row(), 0), 0).toInt();
    TransferKabinetAdd *dlg = new TransferKabinetAdd(id, false, this);
    dlg->exec();
    delete dlg;
    LoadData2();
}

void TransferKabinetWidget::add_data_with_id() {
    useindex = tableView1->currentIndex();
    QString str_id = usemodel1->data(usemodel1->index(useindex.row(), 0), 0).toString();
    if (str_id.isEmpty()) return;

    int id = usemodel1->data(usemodel1->index(useindex.row(), 0), 0).toInt();
    TransferKabinetAdd *dlg = new TransferKabinetAdd(id, true, this);
    dlg->exec();
    delete dlg;
    LoadData1();
    LoadData2();
}

void TransferKabinetWidget::delete_data(){
    useindex = tableView2->currentIndex();
    QString str_id = usemodel2->data(usemodel2->index(useindex.row(), 0), 0).toString();
    if (str_id.isEmpty()) return;

    int id = usemodel2->data(usemodel2->index(useindex.row(), 0), 0).toInt();
    QString invNomer = usemodel2->data(usemodel2->index(useindex.row(), 1), 0).toString();
    QString kabinet = usemodel2->data(usemodel2->index(useindex.row(), 2), 0).toString();
    QString date = usemodel2->data(usemodel2->index(useindex.row(), 4), 0).toString();

    QString strhtml;
    strhtml = "<big style=\"color: red; font-weight: 10px;\"><big>Вы действительно хотите удалить?</big></big><br>";
    strhtml += "<table style=\"text-align: left; width: 100%;\" border=0 cellpadding=2 cellspacing=2><tbody>";
    strhtml += "<tr><td style=\"width: 50%;\" align=\"right\" valign=\"top\"><big style=\"color: black;\">%1</big></td><td style=\"width: 50%;\" valign=\"top\"><big style=\"color: blue;\">%2</big></td></tr>";
    strhtml = strhtml.arg("Дата:").arg(date);
    strhtml += "<tr><td style=\"width: 50%;\" align=\"right\" valign=\"top\"><big style=\"color: black;\">%1</big></td><td style=\"width: 50%;\" valign=\"top\"><big style=\"color: blue;\">%2</big></td></tr>";
    strhtml = strhtml.arg("Инвентарный номер:").arg(invNomer);
    strhtml += "<tr><td style=\"width: 50%;\" align=\"right\" valign=\"top\"><big style=\"color: black;\">%1</big></td><td style=\"width: 50%;\" valign=\"top\"><big style=\"color: blue;\">%2</big></td></tr>";
    strhtml = strhtml.arg("Кабинет:").arg(kabinet);
    strhtml += "</tbody></table><br>";

    int r = QMessageBox::question(this,
        "Вопрос", strhtml,
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No);
    if (r == QMessageBox::No) return;

    qu = "UPDATE Transfer SET date4 = NULL, kabinet_id_4 = NULL, person_id_4 = NULL"
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

void TransferKabinetWidget::text_changed_1() {
    LoadData1();
}

void TransferKabinetWidget::clear_filter_1() {
    nomer_1_LE->setText("");
}

void TransferKabinetWidget::text_changed_2() {
    LoadData2();
}

void TransferKabinetWidget::clear_filter_2() {
    nomer_2_LE->setText("");
}

void TransferKabinetWidget::All_RB_clicked() {
    stackedWidget->setCurrentIndex(1);
    LoadData2();
}

void TransferKabinetWidget::Tek_RB_clicked() {
    stackedWidget->setCurrentIndex(0);
    LoadData2();
}

void TransferKabinetWidget::sortByColumn1(int nn) {
    QString order = tableView1->horizontalHeader()->sortIndicatorOrder() == Qt::AscendingOrder ? "" : "DESC";
    useOrder1 = QString(" ORDER by %1 %2, 2 %2").arg(nn+1).arg(order);
    LoadData1();
}

void TransferKabinetWidget::LoadData1() {
    qu = "SELECT tr.id, Cartr.invNomer, Cartr.model, pr_tudsa, tr.date3 as dateFrom, Kabinet.nomer || ' - ' || Kabinet.title as kabinet"
             " FROM Transfer as tr"
             " LEFT OUTER JOIN Cartr ON tr.cartr_id = Cartr.id"
             " LEFT OUTER JOIN Kabinet ON tr.kabinet_id_1 = Kabinet.id"
             " WHERE date4 IS NULL AND (pr_tudsa = 1 AND date3 NOT NULL)";
    if ( ! nomer_1_LE->text().isEmpty() )  qu += QString(" AND Cartr.invNomer LIKE '%%1%'").arg(nomer_1_LE->text());
    qu += "UNION"
            " SELECT tr.id, Cartr.invNomer, Cartr.model, pr_tudsa, tr.date1 as dateFrom, Kabinet.nomer || ' - ' || Kabinet.title as kabinet"
             " FROM Transfer as tr"
             " LEFT OUTER JOIN Cartr ON tr.cartr_id = Cartr.id"
             " LEFT OUTER JOIN Kabinet ON tr.kabinet_id_1 = Kabinet.id"
             " WHERE date4 IS NULL AND pr_tudsa = 0";
    if ( ! nomer_1_LE->text().isEmpty() )  qu += QString(" AND Cartr.invNomer LIKE '%%1%'").arg(nomer_1_LE->text());
    qu += useOrder1;

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

void TransferKabinetWidget::SetModelsParam1() {
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
    usemodel1->setHAlign(ii, "left");
    usemodel1->setPointSize(ii, 10);

    ii++;
    usemodel1->setHeaderData(ii, Qt::Horizontal, "Откуда\nполучен");
    usemodel1->setHeaderData(ii, Qt::Horizontal, QSize(70, 36), Qt::SizeHintRole);
    usemodel1->setHAlign(ii, "center");
    usemodel1->setYesNo(ii, "Склад@ТуДСА");
    usemodel1->setColor(ii, "#B34EE9@blue");

    ii++;
    usemodel1->setHeaderData(ii, Qt::Horizontal, "Дата\nполучения");
    usemodel1->setHeaderData(ii, Qt::Horizontal, QSize(100, 36), Qt::SizeHintRole);
    usemodel1->setHAlign(ii, "center");
    usemodel1->setDate(ii, "");
    usemodel1->setColor(ii, "red");
    usemodel1->setBold(ii);

    ii++;
    usemodel1->setHeaderData(ii, Qt::Horizontal, "С какого\nкабинета");
    usemodel1->setHeaderData(ii, Qt::Horizontal, QSize(120, 36), Qt::SizeHintRole);
    usemodel1->setPointSize(ii, 10);
}

QString TransferKabinetWidget::getFromSQL() {
    QString str;
    if (All_RB->isChecked()) {
        if (nomer_2_LE->text().isEmpty()) return " FROM (SELECT * FROM Transfer WHERE date4 NOT NULL) as tr";
        else {
            str = " FROM (SELECT ttt.*"
                    " FROM (SELECT * FROM Transfer WHERE date4 NOT NULL) as ttt,"
                         " (SELECT * FROM Cartr WHERE Cartr.invNomer LIKE '%%1') as ccc"
                    " WHERE ttt.cartr_id = ccc.id) as tr";
            return str.arg(nomer_2_LE->text());
        }
    }
    if (Tek_RB->isChecked()) {
        if (nomer_2_LE->text().isEmpty()) return " FROM (SELECT id, cartr_id, max(date4) as date4, kabinet_id_4, person_id_4 FROM Transfer Group By cartr_id) as tr";
        else {
            str = " FROM (SELECT ttt.*"
                    " FROM (SELECT id, cartr_id, max(date4) as date4, kabinet_id_4, person_id_4 FROM Transfer Group By cartr_id) as ttt,"
                         " (SELECT * FROM Cartr WHERE Cartr.invNomer LIKE '%%1') as ccc"
                    " WHERE ttt.cartr_id = ccc.id) as tr";
        return str.arg(nomer_2_LE->text());
        }
    }
}

void TransferKabinetWidget::LoadData2() {

    qu = "SELECT tr.id, Cartr.invNomer,"
           " k4.nomer || ' - ' || k4.title, p4.family || ' ' ||p4.name, tr.date4";
    qu += getFromSQL();
    qu +=  " LEFT OUTER JOIN Cartr ON tr.cartr_id = Cartr.id"
           " LEFT OUTER JOIN Person  as p4 ON tr.person_id_4 = p4.id"
           " LEFT OUTER JOIN Kabinet as k4 ON tr.kabinet_id_4 = k4.id"
           " ORDER by 2";
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
}

void TransferKabinetWidget::SetModelsParam2() {
    int ii = 0;

    ii++;
    usemodel2->setHeaderData(ii, Qt::Horizontal, "Инв.номер");
    usemodel2->setHeaderData(ii, Qt::Horizontal, QSize(80, 36), Qt::SizeHintRole);
    usemodel2->setHAlign(ii, "center");
    usemodel2->setColor(ii, "blue");
    usemodel2->setPointSize(ii, 11);
    usemodel2->setBold(ii);

    ii++;
    usemodel2->setHeaderData(ii, Qt::Horizontal, "В какой\nкабинет");
    usemodel2->setHeaderData(ii, Qt::Horizontal, QSize(140, 36), Qt::SizeHintRole);
    usemodel2->setHAlign(ii, "left");
    usemodel2->setPointSize(ii, 10);

    ii++;
    usemodel2->setHeaderData(ii, Qt::Horizontal, "Через кого");
    usemodel2->setHeaderData(ii, Qt::Horizontal, QSize(180, 36), Qt::SizeHintRole);
    usemodel2->setHAlign(ii, "left");
    usemodel2->setPointSize(ii, 10);

    ii++;
    usemodel2->setHeaderData(ii, Qt::Horizontal, "Дата\nпередачи");
    usemodel2->setHeaderData(ii, Qt::Horizontal, QSize(100, 36), Qt::SizeHintRole);
    usemodel2->setHAlign(ii, "center");
    usemodel2->setDate(ii, "");
    usemodel2->setColor(ii, "red");
    usemodel2->setBold(ii);
}

void TransferKabinetWidget::refreshData() {
    LoadData1();
    LoadData2();
}

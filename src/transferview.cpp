#include "transferview.h"

TransferWidget::TransferWidget() {
    setupUi(this);
    connect(add_PB, SIGNAL(clicked()), this, SLOT(add_data()));
    connect(edit_PB, SIGNAL(clicked()), this, SLOT(edit_data()));
    connect(del_PB, SIGNAL(clicked()), this, SLOT(delete_data()));
    connect(nomer_LE, SIGNAL(textChanged(QString)), this, SLOT(refreshData()));
    connect(clear_TB, SIGNAL(clicked()), this, SLOT(clear_filter()));
    connect(date2_LE, SIGNAL(textChanged(QString)), this, SLOT(refreshData()));
    connect(clear_2_TB, SIGNAL(clicked()), this, SLOT(clear_filter_2()));
    connect(filter_kabinet1_CB, SIGNAL(currentIndexChanged(int)), this, SLOT(refreshData()));
    connect(clear_kabinet1_TB, SIGNAL(clicked()), this, SLOT(clear_filter_kabinet1()));
    connect(filter_kabinet4_CB, SIGNAL(currentIndexChanged(int)), this, SLOT(refreshData()));
    connect(clear_kabinet4_TB, SIGNAL(clicked()), this, SLOT(clear_filter_kabinet4()));
    connect(tableView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(edit_data()));
    connect(tableView->horizontalHeader(),SIGNAL(sectionClicked(int)), this, SLOT(sortByColumn(int)));

    usemodel = new DataModel();
    usemodel_kabinet = new QSqlQueryModel();
    setInitFilterKabinet();

    QFont font("Helvetica", 9, QFont::Bold);
    tableView->horizontalHeader()->setFont(font);
    tableView->horizontalHeader()->setSortIndicator(1, Qt::AscendingOrder);
    sortByColumn(1);
}

void TransferWidget::setInitFilterKabinet() {
    qu = "SELECT id, nomer || ' - '||  title FROM `Kabinet` Where pr_arhiv <> 1 ORDER by nomer, title";
    sqlquery.exec(qu);
    usemodel_kabinet->setQuery(sqlquery);

    filter_kabinet1_CB->setModel(usemodel_kabinet);;
    filter_kabinet1_CB->setModelColumn(1);
    filter_kabinet1_CB->setCurrentIndex(-1);

    filter_kabinet4_CB->setModel(usemodel_kabinet);;
    filter_kabinet4_CB->setModelColumn(1);
    filter_kabinet4_CB->setCurrentIndex(-1);
}

void TransferWidget::add_data() {
    TransferCartrAdd *dlg = new TransferCartrAdd(this);
    dlg->exec();
    delete dlg;
    LoadData();
}

void TransferWidget::edit_data() {
    useindex = tableView->currentIndex();
    QString str_id = usemodel->data(usemodel->index(useindex.row(), 0), 0).toString();
    if (str_id.isEmpty()) return;

    int id = usemodel->data(usemodel->index(useindex.row(), 0), 0).toInt();
    TransferCartrAdd *dlg = new TransferCartrAdd(id, this);
    dlg->exec();
    delete dlg;
    LoadData();
}

void TransferWidget::delete_data(){
    useindex = tableView->currentIndex();
    QString str_id = usemodel->data(usemodel->index(useindex.row(), 0), 0).toString();
    if (str_id.isEmpty()) return;

    int id = usemodel->data(usemodel->index(useindex.row(), 0), 0).toInt();
    QString date = usemodel->data(usemodel->index(useindex.row(), 1), 0).toString();
    QString invNomer = usemodel->data(usemodel->index(useindex.row(), 2), 0).toString();
    QString model = usemodel->data(usemodel->index(useindex.row(), 3), 0).toString();

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

void TransferWidget::clear_filter() {
    nomer_LE->setText("");
}

void TransferWidget::clear_filter_2() {
    date2_LE->setText("");
}

void TransferWidget::clear_filter_kabinet1() {
    filter_kabinet1_CB->setCurrentIndex(-1);
}

void TransferWidget::clear_filter_kabinet4() {
    filter_kabinet4_CB->setCurrentIndex(-1);
}

void TransferWidget::sortByColumn(int nn) {
    QString order = tableView->horizontalHeader()->sortIndicatorOrder() == Qt::AscendingOrder ? "" : "DESC";
    if (nn==1) useOrder = QString(" ORDER by %1 %2, tr.date1 %2").arg(nn+1).arg(order);
    else useOrder = QString(" ORDER by %1 %2, 2 %2").arg(nn+1).arg(order);
    LoadData();
}

void TransferWidget::LoadData() {

    qu = "SELECT tr.id, Cartr.invNomer,"
           " k1.nomer || ' - ' || k1.title, p1.family || ' ' || p1.name,"
           " tr.comment, tr.date1, tr.date2, tr.date3, tr.date4,"
           " k4.nomer || ' - ' || k4.title, p4.family || ' ' || p4.name";
    qu += getFromSQL();
    qu +=  " LEFT OUTER JOIN Cartr ON tr.cartr_id = Cartr.id"
            " LEFT OUTER JOIN Person  as p1 ON tr.person_id_1 = p1.id"
            " LEFT OUTER JOIN Kabinet as k1 ON tr.kabinet_id_1 = k1.id"
            " LEFT OUTER JOIN Person  as p4 ON tr.person_id_4 = p4.id"
            " LEFT OUTER JOIN Kabinet as k4 ON tr.kabinet_id_4 = k4.id"
            " WHERE 1";
    int ind;
    ind = filter_kabinet1_CB->currentIndex();  if (ind>=0) {
        int kabinet_id = usemodel_kabinet->record(ind).value(0).toInt();
        qu += QString(" AND kabinet_id_1 = %1").arg(kabinet_id);
    }
    ind = filter_kabinet4_CB->currentIndex();  if (ind>=0) {
        int kabinet_id = usemodel_kabinet->record(ind).value(0).toInt();
        qu += QString(" AND kabinet_id_4 = %1").arg(kabinet_id);
    }
    qu += useOrder;
    usemodel->setQuery(qu);
    if (usemodel->lastError().isValid()) {
        QMessageBox::warning(this, "Ошибка!!!", usemodel->lastError().databaseText());
        return;
    }
    while (usemodel->canFetchMore()) {
         usemodel->fetchMore();
    }

    SetModelsParam(usemodel);
    tableView->setModel(usemodel);
    tableView->hideColumn(0);
    tableView->resizeColumnsToContents();
}

QString TransferWidget::getFromSQL() {

    QString f1 = nomer_LE->text();
    QString f2 = date2_LE->text();

    if (!f2.isEmpty()) {
        f2 = f2.replace("/","-");
        f2 = f2.replace("\\","-");
        f2 = f2.replace(".","-");
        f2 = f2.replace(",","-");
        QDate dd = QDate::fromString(f2, "dd-MM-yyyy");
        if (dd.isValid()) f2 = dd.toString(Qt::ISODate);
        else f2 = "";
    }

    if (f1.isEmpty() and f2.isEmpty()) return " FROM Transfer as tr";

    if (!f1.isEmpty() and f2.isEmpty()) {
        return QString(" FROM (SELECT Transfer.*"
                          " FROM Transfer, (SELECT * FROM Cartr WHERE Cartr.invNomer LIKE '%%1') as ccc"
                          " WHERE Transfer.cartr_id = ccc.id) as tr").arg(f1);
    }

    if (f1.isEmpty() and !f2.isEmpty()) {
        return QString(" FROM (SELECT Transfer.* FROM Transfer WHERE date2 = '%1') as tr").arg(f2);
    }

    if (!f1.isEmpty() and !f2.isEmpty()) {
        return QString(" FROM (SELECT Transfer.*"
                      " FROM Transfer, (SELECT * FROM Cartr WHERE Cartr.invNomer LIKE '%%1') as ccc"
                      " WHERE Transfer.cartr_id = ccc.id AND date2 = '%2') as tr").arg(f1).arg(f2);
    }
    return "";
}

void TransferWidget::SetModelsParam(DataModel*& usemodel) {
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
    usemodel->setHeaderData(ii, Qt::Horizontal, QSize(150, 36), Qt::SizeHintRole);
    usemodel->setHAlign(ii, "left");
    usemodel->setPointSize(ii, 10);

    ii++;
    usemodel->setHeaderData(ii, Qt::Horizontal, "Примечание");
    usemodel->setHeaderData(ii, Qt::Horizontal, QSize(140, 36), Qt::SizeHintRole);
    usemodel->setHAlign(ii, "left");
    usemodel->setPointSize(ii, 9);

    ii++;
    usemodel->setHeaderData(ii, Qt::Horizontal, "Дата\nполучения");
    usemodel->setHeaderData(ii, Qt::Horizontal, QSize(100, 36), Qt::SizeHintRole);
    usemodel->setHAlign(ii, "center");
    usemodel->setColor(ii, "red");
    usemodel->setDate(ii, "");
    usemodel->setBold(ii);

    ii++;
    usemodel->setHeaderData(ii, Qt::Horizontal, "Дата\nотправки");
    usemodel->setHeaderData(ii, Qt::Horizontal, QSize(100, 36), Qt::SizeHintRole);
    usemodel->setHAlign(ii, "center");
    usemodel->setColor(ii, "red");
    usemodel->setDate(ii, "");
    usemodel->setBold(ii);

    ii++;
    usemodel->setHeaderData(ii, Qt::Horizontal, "Дата\nприхода");
    usemodel->setHeaderData(ii, Qt::Horizontal, QSize(100, 36), Qt::SizeHintRole);
    usemodel->setHAlign(ii, "center");
    usemodel->setColor(ii, "red");
    usemodel->setDate(ii, "");
    usemodel->setBold(ii);

    ii++;
    usemodel->setHeaderData(ii, Qt::Horizontal, "Дата\nпередачи");
    usemodel->setHeaderData(ii, Qt::Horizontal, QSize(100, 36), Qt::SizeHintRole);
    usemodel->setHAlign(ii, "center");
    usemodel->setColor(ii, "red");
    usemodel->setDate(ii, "");
    usemodel->setBold(ii);

    ii++;
    usemodel->setHeaderData(ii, Qt::Horizontal, "В какой\nкабинет");
    usemodel->setHeaderData(ii, Qt::Horizontal, QSize(140, 36), Qt::SizeHintRole);
    usemodel->setHAlign(ii, "left");
    usemodel->setPointSize(ii, 10);

    ii++;
    usemodel->setHeaderData(ii, Qt::Horizontal, "Через кого");
    usemodel->setHeaderData(ii, Qt::Horizontal, QSize(150, 36), Qt::SizeHintRole);
    usemodel->setHAlign(ii, "left");
    usemodel->setPointSize(ii, 10);
}

void TransferWidget::refreshData() {
    LoadData();
}

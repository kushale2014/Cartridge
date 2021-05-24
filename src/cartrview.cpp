#include "cartrview.h"

CartrWidget::CartrWidget() {
    setupUi(this);
    connect(add_PB, SIGNAL(clicked()), this, SLOT(add_data()));
    connect(delete_PB, SIGNAL(clicked()), this, SLOT(delete_data()));
    connect(edit_PB, SIGNAL(clicked()), this, SLOT(edit_data()));
    connect(arhiv_add_PB, SIGNAL(clicked()), this, SLOT(arhiv_add()));
    connect(arhiv_view_PB, SIGNAL(clicked()), this, SLOT(arhiv_view()));
    connect(tableView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(edit_data()));
    connect(tableView->horizontalHeader(),SIGNAL(sectionClicked(int)), this, SLOT(sortByColumn(int)));
    connect(nomer_LE, SIGNAL(textChanged(QString)), this, SLOT(text_changed()));
    connect(clear_TB, SIGNAL(clicked()), this, SLOT(clear_filter()));

    usemodel = new DataModel();

    QFont font("Helvetica", 10, QFont::Bold);
    tableView->horizontalHeader()->setFont(font);
    tableView->verticalHeader()->setHidden(true);
    tableView->horizontalHeader()->setSortIndicator(1, Qt::AscendingOrder);
    sortByColumn(1);
    QString styleSheet;
    styleSheet = "QHeaderView::section {"
                "background-color: #27ae60;"
                "color: white;"
                "font-weight: bold;"
                "border: 1px solid black;"
                "}";
    tableView->horizontalHeader()->setStyleSheet(styleSheet);

   QString activeDB = QSqlDatabase::connectionNames().value(0);
    tables = QSqlDatabase::database(activeDB, false).tables();
    LoadData();
}

void CartrWidget::arhiv_add() {
    useindex = tableView->currentIndex();
    int id = usemodel->data(usemodel->index(useindex.row(), 0), 0).toInt();
    if (id==0) return;

    QString invNomer = usemodel->data(usemodel->index(useindex.row(), 1), 0).toString();
    QString model = usemodel->data(usemodel->index(useindex.row(), 2), 0).toString();
    QString serialNo1 = usemodel->data(usemodel->index(useindex.row(), 3), 0).toString();
    QString serialNo2 = usemodel->data(usemodel->index(useindex.row(), 4), 0).toString();
    QString kabinet = usemodel->data(usemodel->index(useindex.row(), 5), 0).toString();

    QString strhtml;
    strhtml = "<big style=\"color: red;\"><big>Перевести в архив ?</big></big><br>";
    strhtml += "<table style=\"text-align: left; width: 100%;\" border=0 cellpadding=2 cellspacing=2><tbody>";
    strhtml += "<tr><td style=\"width: 50%;\" align=\"right\" valign=\"top\"><big style=\"color: black;\">%1</big></td><td style=\"width: 50%;\" valign=\"top\"><big style=\"color: blue;\">%2</big></td></tr>";
    strhtml = strhtml.arg("Инвентарный номер:").arg(invNomer);
    strhtml += "<tr><td style=\"width: 50%;\" align=\"right\" valign=\"top\"><big style=\"color: black;\">%1</big></td><td style=\"width: 50%;\" valign=\"top\"><big style=\"color: black;\">%2</big></td></tr>";
    strhtml = strhtml.arg("Модель:").arg(model);
    strhtml += "<tr><td style=\"width: 50%;\" align=\"right\" valign=\"top\"><big style=\"color: black;\">%1</big></td><td style=\"width: 50%;\" valign=\"top\"><big style=\"color: black;\">%2</big></td></tr>";
    strhtml = strhtml.arg("Серийный номер:").arg(serialNo1);
    strhtml += "<tr><td style=\"width: 50%;\" align=\"right\" valign=\"top\"><big style=\"color: black;\">%1</big></td><td style=\"width: 50%;\" valign=\"top\"><big style=\"color: black;\">%2</big></td></tr>";
    strhtml = strhtml.arg("Серийный номер(без букв):").arg(serialNo2);
    strhtml += "<tr><td style=\"width: 50%;\" align=\"right\" valign=\"top\"><big style=\"color: black;\">%1</big></td><td style=\"width: 50%;\" valign=\"top\"><big style=\"color: black;\">%2</big></td></tr>";
    strhtml = strhtml.arg("Кабинет:").arg(kabinet);
    strhtml += "</tbody></table><br>";

    int r = QMessageBox::question(this,
        "Вопрос", strhtml,
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No);
    if (r == QMessageBox::No) return;

    qu = "UPDATE Cartr SET pr_arhiv=1 WHERE id = %1";
    qu = qu.arg(id);
    if (!sqlquery.exec(qu)) {
        QMessageBox::warning(this, "Ошибка", sqlquery.lastError().databaseText());
        return;
    }
    LoadData();
}

void CartrWidget::arhiv_view() {
    CartrArhivWidget *dlg = new CartrArhivWidget();
    dlg->exec();
    delete dlg;
    LoadData();
}

void CartrWidget::add_data() {
    CartrAddForm *dlg=new CartrAddForm(this);
    dlg->exec();
    delete dlg;
    LoadData();
}

void CartrWidget::edit_data() {
    useindex = tableView->currentIndex();
    int id = usemodel->data(usemodel->index(useindex.row(), 0), 0).toInt();
    if (id==0) return;
    CartrAddForm *dlg=new CartrAddForm(this, id);
    dlg->exec();
    delete dlg;
    LoadData();
}

void CartrWidget::delete_data(){
    useindex = tableView->currentIndex();
    int id = usemodel->data(usemodel->index(useindex.row(), 0), 0).toInt();
    if (id==0) return;

    QString invNomer = usemodel->data(usemodel->index(useindex.row(), 1), 0).toString();
    QString model = usemodel->data(usemodel->index(useindex.row(), 2), 0).toString();
    QString serialNo1 = usemodel->data(usemodel->index(useindex.row(), 3), 0).toString();
    QString serialNo2 = usemodel->data(usemodel->index(useindex.row(), 4), 0).toString();
    QString kabinet = usemodel->data(usemodel->index(useindex.row(), 5), 0).toString();

    QString strhtml;
    strhtml = "<big style=\"color: red;\"><big>Вы действительно хотите удалить?</big></big><br>";
    strhtml += "<table style=\"text-align: left; width: 100%;\" border=0 cellpadding=2 cellspacing=2><tbody>";
    strhtml += "<tr><td style=\"width: 50%;\" align=\"right\" valign=\"top\"><big style=\"color: black;\">%1</big></td><td style=\"width: 50%;\" valign=\"top\"><big style=\"color: blue;\">%2</big></td></tr>";
    strhtml = strhtml.arg("Инвентарный номер:").arg(invNomer);
    strhtml += "<tr><td style=\"width: 50%;\" align=\"right\" valign=\"top\"><big style=\"color: black;\">%1</big></td><td style=\"width: 50%;\" valign=\"top\"><big style=\"color: black;\">%2</big></td></tr>";
    strhtml = strhtml.arg("Модель:").arg(model);
    strhtml += "<tr><td style=\"width: 50%;\" align=\"right\" valign=\"top\"><big style=\"color: black;\">%1</big></td><td style=\"width: 50%;\" valign=\"top\"><big style=\"color: black;\">%2</big></td></tr>";
    strhtml = strhtml.arg("Серийный номер:").arg(serialNo1);
    strhtml += "<tr><td style=\"width: 50%;\" align=\"right\" valign=\"top\"><big style=\"color: black;\">%1</big></td><td style=\"width: 50%;\" valign=\"top\"><big style=\"color: black;\">%2</big></td></tr>";
    strhtml = strhtml.arg("Серийный номер(без букв):").arg(serialNo2);
    strhtml += "<tr><td style=\"width: 50%;\" align=\"right\" valign=\"top\"><big style=\"color: black;\">%1</big></td><td style=\"width: 50%;\" valign=\"top\"><big style=\"color: black;\">%2</big></td></tr>";
    strhtml = strhtml.arg("Кабинет:").arg(kabinet);
    strhtml += "</tbody></table><br>";

    int r = QMessageBox::question(this,
        "Вопрос", strhtml,
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No);
    if (r == QMessageBox::No) return;

    qu = "SELECT id from Transfer WHERE cartr_id = %1 LIMIT 1";
    qu = qu.arg(id);
    sqlquery.exec(qu);
    if (sqlquery.first()) {
        strhtml = "<p style=\"color: red; font-weight: 14px;\">УДАЛИТЬ  НЕВОЗМОЖНО,  так как</p>";
        strhtml += "<p style=\"color: red; font-weight: 14px;\">данный картридж уже используется в базе.</p><br>";
        QMessageBox::information(this, "Сообщение", strhtml);
        return;
    }

    qu = "DELETE from Cartr WHERE id = %1";
    qu = qu.arg(id);
    if (!sqlquery.exec(qu)) {
        QMessageBox::warning(this, "Ошибка", sqlquery.lastError().databaseText());
        return;
    }
    LoadData();
}

void CartrWidget::clear_filter() {
    nomer_LE->setText("");
}

void CartrWidget::text_changed() {
    LoadData();
}

void CartrWidget::sortByColumn(int nn) {
    QString order = tableView->horizontalHeader()->sortIndicatorOrder() == Qt::AscendingOrder ? "" : "DESC";
    useOrder = QString(" ORDER by %1 %2, 2 %2").arg(nn+1).arg(order);
    LoadData();
}

void CartrWidget::LoadData() {
    qu = "SELECT cc.id, cc.invNomer, cc.model, cc.serialNo1, cc.serialNo2, kk.nomer || ' - ' || kk.title, pp.family"
            " FROM (SELECT * From Cartr WHERE pr_arhiv<>1) as cc"
            " LEFT OUTER JOIN Kabinet as kk ON cc.home_kabinet_id = kk.id"
            " LEFT OUTER JOIN Person as pp ON cc.person_id = pp.id";
    if (!nomer_LE->text().isEmpty()) qu += QString(" WHERE cc.invNomer LIKE '%%1'").arg(nomer_LE->text());
    qu += useOrder;
    usemodel->setQuery(qu);
    if (usemodel->lastError().isValid()) {
        QMessageBox::warning(this, "Ошибка", usemodel->lastError().databaseText());
        return;
    }
    while (usemodel->canFetchMore()) {
         usemodel->fetchMore();
    }

    SetModelsParam();
    tableView->setModel(usemodel);
    tableView->hideColumn(0);
    tableView->resizeColumnsToContents();
}

void CartrWidget::SetModelsParam() {
    int ii = 0;

    ii++;
    usemodel->setHeaderData(ii, Qt::Horizontal,"Инвентарный\n номер");
    usemodel->setHeaderData(ii, Qt::Horizontal, QSize(100, 36), Qt::SizeHintRole);
    usemodel->setHAlign(ii, "center");
    usemodel->setPointSize(ii, 11);
    usemodel->setBold(ii);
    usemodel->setColor(ii, "blue");

    ii++;
    usemodel->setHeaderData(ii, Qt::Horizontal,"Модель");
    usemodel->setHeaderData(ii, Qt::Horizontal, QSize(200, 36), Qt::SizeHintRole);
    usemodel->setHAlign(ii, "left");
    usemodel->setBold(ii);
    usemodel->setPointSize(ii, 9);

    ii++;
    usemodel->setHeaderData(ii, Qt::Horizontal,"Серийный номер");
    usemodel->setHeaderData(ii, Qt::Horizontal, QSize(240, 36), Qt::SizeHintRole);
    usemodel->setHAlign(ii, "left");
    usemodel->setBold(ii);
    usemodel->setPointSize(ii, 9);

    ii++;
    usemodel->setHeaderData(ii, Qt::Horizontal,"Серийный номер\n (без букв)");
    usemodel->setHeaderData(ii, Qt::Horizontal, QSize(150, 36), Qt::SizeHintRole);
    usemodel->setHAlign(ii, "center");
    usemodel->setBold(ii);
    usemodel->setPointSize(ii, 9);

    ii++;
    usemodel->setHeaderData(ii, Qt::Horizontal,"Начальный кабинет");
    usemodel->setHeaderData(ii, Qt::Horizontal, QSize(150, 36), Qt::SizeHintRole);
    usemodel->setHAlign(ii, "left");
    usemodel->setPointSize(ii, 9);

    ii++;
    usemodel->setHeaderData(ii, Qt::Horizontal,"Кому принадлежит");
    usemodel->setHeaderData(ii, Qt::Horizontal, QSize(150, 36), Qt::SizeHintRole);
    usemodel->setHAlign(ii, "left");
    usemodel->setPointSize(ii, 9);
}

void CartrWidget::show() {
    LoadData();
}

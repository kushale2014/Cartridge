#include "personview.h"

PersonWidget::PersonWidget() {
    setupUi(this);
    connect(add_PB, SIGNAL(clicked()), this, SLOT(add_data()));
    connect(delete_PB, SIGNAL(clicked()), this, SLOT(delete_data()));
    connect(edit_PB, SIGNAL(clicked()), this, SLOT(edit_data()));
    connect(arhiv_add_PB, SIGNAL(clicked()), this, SLOT(arhiv_add()));
    connect(arhiv_delete_PB, SIGNAL(clicked()), this, SLOT(arhiv_delete()));
    connect(fio_LE, SIGNAL(textChanged(QString)), this, SLOT(text_changed()));
    connect(clear_TB, SIGNAL(clicked()), this, SLOT(clear_filter()));
    connect(tableView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(edit_data()));
    connect(tableView->horizontalHeader(),SIGNAL(sectionClicked(int)), this, SLOT(sortByColumn(int)));

    usemodel = new DataModel();
    usemodel_arhiv = new DataModel();

    QFont font("Helvetica", 10, QFont::Bold);
    tableView->horizontalHeader()->setFont(font);
    tableView_arhiv->horizontalHeader()->setFont(font);
    QString styleSheet;
    styleSheet = "QHeaderView::section {"
                "background-color: #2980b9;"
                "color: white;"
                "font-weight: bold;"
                "border: 1px solid black;"
                "}";
    tableView->horizontalHeader()->setStyleSheet(styleSheet);
    styleSheet = "QHeaderView::section {"
                "background-color: #E9843B;"
                "color: white;"
                "font-weight: bold;"
                "border: 1px solid black;"
                "}";
    tableView_arhiv->horizontalHeader()->setStyleSheet(styleSheet);

    tableView->horizontalHeader()->setSortIndicator(1, Qt::AscendingOrder);
    sortByColumn(1);
    LoadData();
}

void PersonWidget::arhiv_add() {
    useindex = tableView->currentIndex();
    QString str_id = usemodel->data(usemodel->index(useindex.row(), 0), 0).toString();
    if (str_id.isEmpty()) return;

    int id = usemodel->data(usemodel->index(useindex.row(), 0), 0).toInt();
    QString family = usemodel->data(usemodel->index(useindex.row(), 1), 0).toString();
    QString name = usemodel->data(usemodel->index(useindex.row(), 2), 0).toString();
    QString surname = usemodel->data(usemodel->index(useindex.row(), 3), 0).toString();

    QString strhtml;
    strhtml = "<big style=\"color: red; font-weight: 10px;\"><big>Перевести в архив ?</big></big><br>";
    strhtml += "<table style=\"text-align: left; width: 100%;\" border=0 cellpadding=2 cellspacing=2><tbody>";
    strhtml += "<tr><td style=\"width: 50%;\" align=\"right\" valign=\"top\"><big style=\"color: black;\">%1</big></td><td style=\"width: 50%;\" valign=\"top\"><big style=\"color: blue;\">%2</big></td></tr>";
    strhtml = strhtml.arg("Сотрудник:").arg(family);
    strhtml += "<tr><td style=\"width: 50%;\" align=\"right\" valign=\"top\"><big style=\"color: black;\">%1</big></td><td style=\"width: 50%;\" valign=\"top\"><big style=\"color: blue;\">%2</big></td></tr>";
    strhtml = strhtml.arg("").arg(name);
    strhtml += "<tr><td style=\"width: 50%;\" align=\"right\" valign=\"top\"><big style=\"color: black;\">%1</big></td><td style=\"width: 50%;\" valign=\"top\"><big style=\"color: blue;\">%2</big></td></tr>";
    strhtml = strhtml.arg("").arg(surname);
    strhtml += "</tbody></table><br>";

    int r = QMessageBox::question(this,
        "Вопрос", strhtml,
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No);
    if (r == QMessageBox::No) return;

    qu = "UPDATE Person SET pr_arhiv=1 WHERE id = %1";
    qu = qu.arg(id);
    if (!sqlquery.exec(qu)) {
        QMessageBox::warning(this, "Ошибка", sqlquery.lastError().databaseText());
        return;
    }
    LoadData();
}

void PersonWidget::arhiv_delete() {
    useindex = tableView_arhiv->currentIndex();
    QString str_id = usemodel_arhiv->data(usemodel_arhiv->index(useindex.row(), 0), 0).toString();
    if (str_id.isEmpty()) return;

    int id = usemodel_arhiv->data(usemodel_arhiv->index(useindex.row(), 0), 0).toInt();
    QString family = usemodel_arhiv->data(usemodel_arhiv->index(useindex.row(), 1), 0).toString();
    QString name = usemodel_arhiv->data(usemodel_arhiv->index(useindex.row(), 2), 0).toString();
    QString surname = usemodel_arhiv->data(usemodel_arhiv->index(useindex.row(), 3), 0).toString();

    QString strhtml;
    strhtml = "<big style=\"color: red; font-weight: 10px;\"><big>Перевести из архива ?</big></big><br>";
    strhtml += "<table style=\"text-align: left; width: 100%;\" border=0 cellpadding=2 cellspacing=2><tbody>";
    strhtml += "<tr><td style=\"width: 50%;\" align=\"right\" valign=\"top\"><big style=\"color: black;\">%1</big></td><td style=\"width: 50%;\" valign=\"top\"><big style=\"color: blue;\">%2</big></td></tr>";
    strhtml = strhtml.arg("Сотрудник:").arg(family);
    strhtml += "<tr><td style=\"width: 50%;\" align=\"right\" valign=\"top\"><big style=\"color: black;\">%1</big></td><td style=\"width: 50%;\" valign=\"top\"><big style=\"color: blue;\">%2</big></td></tr>";
    strhtml = strhtml.arg("").arg(name);
    strhtml += "<tr><td style=\"width: 50%;\" align=\"right\" valign=\"top\"><big style=\"color: black;\">%1</big></td><td style=\"width: 50%;\" valign=\"top\"><big style=\"color: blue;\">%2</big></td></tr>";
    strhtml = strhtml.arg("").arg(surname);
    strhtml += "</tbody></table><br>";

    int r = QMessageBox::question(this,
        "Вопрос", strhtml,
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No);
    if (r == QMessageBox::No) return;

    qu = "UPDATE Person SET pr_arhiv=0 WHERE id = %1";
    qu = qu.arg(id);
    if (!sqlquery.exec(qu)) {
        QMessageBox::warning(this, "Ошибка", sqlquery.lastError().databaseText());
        return;
    }
    LoadData();
}

void PersonWidget::add_data() {
    PersonAddForm *dlg=new PersonAddForm(this);
    dlg->exec();
    delete dlg;
    LoadData();
}

void PersonWidget::edit_data() {
    useindex = tableView->currentIndex();
    QString str_id = usemodel->data(usemodel->index(useindex.row(), 0), 0).toString();
    if (str_id.isEmpty()) return;

    int id = usemodel->data(usemodel->index(useindex.row(), 0), 0).toInt();
    PersonAddForm *dlg=new PersonAddForm(this, id);
    dlg->exec();
    delete dlg;
    LoadData();
}

void PersonWidget::delete_data(){
    useindex = tableView->currentIndex();
    QString str_id = usemodel->data(usemodel->index(useindex.row(), 0), 0).toString();
    if (str_id.isEmpty()) return;

    int id = usemodel->data(usemodel->index(useindex.row(), 0), 0).toInt();
    QString family = usemodel->data(usemodel->index(useindex.row(), 1), 0).toString();
    QString name = usemodel->data(usemodel->index(useindex.row(), 2), 0).toString();
    QString surname = usemodel->data(usemodel->index(useindex.row(), 3), 0).toString();

    QString strhtml;
    strhtml = "<big style=\"color: red; font-weight: 10px;\"><big>Вы действительно хотите удалить?</big></big><br>";
    strhtml += "<table style=\"text-align: left; width: 100%;\" border=0 cellpadding=2 cellspacing=2><tbody>";
    strhtml += "<tr><td style=\"width: 50%;\" align=\"right\" valign=\"top\"><big style=\"color: black;\">%1</big></td><td style=\"width: 50%;\" valign=\"top\"><big style=\"color: blue;\">%2</big></td></tr>";
    strhtml = strhtml.arg("Сотрудник:").arg(family);
    strhtml += "<tr><td style=\"width: 50%;\" align=\"right\" valign=\"top\"><big style=\"color: black;\">%1</big></td><td style=\"width: 50%;\" valign=\"top\"><big style=\"color: blue;\">%2</big></td></tr>";
    strhtml = strhtml.arg("").arg(name);
    strhtml += "<tr><td style=\"width: 50%;\" align=\"right\" valign=\"top\"><big style=\"color: black;\">%1</big></td><td style=\"width: 50%;\" valign=\"top\"><big style=\"color: blue;\">%2</big></td></tr>";
    strhtml = strhtml.arg("").arg(surname);
    strhtml += "</tbody></table><br>";

    int r = QMessageBox::question(this,
        "Вопрос", strhtml,
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No);
    if (r == QMessageBox::No) return;

    qu = "SELECT id from Transfer WHERE person_id_1 = %1 OR person_id_2 = %1 OR person_id_3 = %1 OR person_id_4 = %1 LIMIT 1";
    qu = qu.arg(id);
    sqlquery.exec(qu);
    if (sqlquery.first()) {
        strhtml = "<p style=\"color: red; font-weight: 14px;\">УДАЛИТЬ  НЕВОЗМОЖНО,  так как</p>";
        strhtml += "<p style=\"color: red; font-weight: 14px;\">данный сотрудник уже используется в базе.</p><br>";
        QMessageBox::information(this, "Сообщение", strhtml);
        return;
    }

    qu = "DELETE from Person WHERE id = %1";
    qu = qu.arg(id);
    if (!sqlquery.exec(qu)) {
        QMessageBox::warning(this, "Ошибка", sqlquery.lastError().databaseText());
        return;
    }
    LoadData();
}

void PersonWidget::clear_filter() {
    fio_LE->setText("");
}

void PersonWidget::text_changed() {
    LoadData();
}

void PersonWidget::sortByColumn(int nn) {
    QString order = tableView->horizontalHeader()->sortIndicatorOrder() == Qt::AscendingOrder ? "" : "DESC";
    useOrder = QString(" ORDER by %1 %2, 2 %2").arg(nn+1).arg(order);
    LoadData();
}

void PersonWidget::LoadData() {
    qu = "SELECT id, family, name, surname FROM Person WHERE pr_arhiv<>1";
    if (!fio_LE->text().isEmpty()) qu += QString(" AND family LIKE '%%1%'").arg(fio_LE->text());
    qu += useOrder;
    usemodel->setQuery(qu);
    if (usemodel->lastError().isValid()) {
        QMessageBox::information(this, tr(""), usemodel->lastError().databaseText());
        return;
    }
    while (usemodel->canFetchMore()) {
         usemodel->fetchMore();
    }

    SetModelsParam(usemodel, "blue");
    tableView->setModel(usemodel);
    tableView->hideColumn(0);
    tableView->resizeColumnsToContents();

//  **************************************************

    qu = "SELECT id, family, name, surname FROM Person WHERE pr_arhiv=1 ORDER by family ASC";
    usemodel_arhiv->setQuery(qu);
    if (usemodel_arhiv->lastError().isValid()) {
        QMessageBox::warning(this, "Ошибка", usemodel_arhiv->lastError().databaseText());
        return;
    }
    while (usemodel_arhiv->canFetchMore()) {
         usemodel_arhiv->fetchMore();
    }

    SetModelsParam(usemodel_arhiv, "black");
    tableView_arhiv->setModel(usemodel_arhiv);
    tableView_arhiv->hideColumn(0);
    tableView_arhiv->resizeColumnsToContents();
}

void PersonWidget::SetModelsParam(DataModel*& usemodel, QString color) {
    int ii = 0;

    ii++;
    usemodel->setHeaderData(ii, Qt::Horizontal, "Фамилия");
    usemodel->setHeaderData(ii, Qt::Horizontal, QSize(200, 28), Qt::SizeHintRole);
    usemodel->setHAlign(ii, "left");
    usemodel->setPointSize(ii, 11);
    usemodel->setBold(ii);
    usemodel->setColor(ii, color);

    ii++;
    usemodel->setHeaderData(ii, Qt::Horizontal, "Имя");
    usemodel->setHeaderData(ii, Qt::Horizontal, QSize(150, 28), Qt::SizeHintRole);
    usemodel->setHAlign(ii, "left");
    usemodel->setBold(ii);
    usemodel->setPointSize(ii, 11);
    usemodel->setColor(ii, color);

    ii++;
    usemodel->setHeaderData(ii, Qt::Horizontal, "Отчество");
    usemodel->setHeaderData(ii, Qt::Horizontal, QSize(150, 28), Qt::SizeHintRole);
    usemodel->setHAlign(ii, "left");
    usemodel->setBold(ii);
    usemodel->setPointSize(ii, 10);
    usemodel->setColor(ii, color);
}

void PersonWidget::show() {
    LoadData();
}

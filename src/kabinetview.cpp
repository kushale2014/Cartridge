#include "kabinetview.h"
#include "kabinetadd.h"

KabinetWidget::KabinetWidget() {
    setupUi(this);
    connect(add_PB, SIGNAL(clicked()), this, SLOT(add_data()));
    connect(delete_PB, SIGNAL(clicked()), this, SLOT(delete_data()));
    connect(edit_PB, SIGNAL(clicked()), this, SLOT(edit_data()));
    connect(arhiv_add_PB, SIGNAL(clicked()), this, SLOT(arhiv_add()));
    connect(arhiv_delete_PB, SIGNAL(clicked()), this, SLOT(arhiv_delete()));
    connect(tableView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(edit_data()));

    usemodel = new DataModel();
    usemodel_arhiv = new DataModel();

    QFont font("Helvetica", 10, QFont::Bold);
    tableView->horizontalHeader()->setFont(font);
    tableView_arhiv->horizontalHeader()->setFont(font);
    tableView->verticalHeader()->setHidden(true);
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
    LoadData();
}

void KabinetWidget::arhiv_add() {
    useindex = tableView->currentIndex();
    QString str_id = usemodel->data(usemodel->index(useindex.row(), 0), 0).toString();
    if (str_id.isEmpty()) return;

    int id = usemodel->data(usemodel->index(useindex.row(), 0), 0).toInt();
    QString nomer = usemodel->data(usemodel->index(useindex.row(), 1), 0).toString();
    QString title = usemodel->data(usemodel->index(useindex.row(), 2), 0).toString();

    QString strhtml;
    strhtml = "<big style=\"color: red; font-weight: 10px;\"><big>Перевести в архив ?</big></big><br>";
    strhtml += "<table style=\"text-align: left; width: 100%;\" border=0 cellpadding=2 cellspacing=2><tbody>";
    strhtml += "<tr><td style=\"width: 50%;\" align=\"right\" valign=\"top\"><big style=\"color: black;\">%1</big></td><td style=\"width: 50%;\" valign=\"top\"><big style=\"color: blue;\">%2</big></td></tr>";
    strhtml = strhtml.arg("Кабинет:").arg(nomer);
    strhtml += "<tr><td style=\"width: 50%;\" align=\"right\" valign=\"top\"><big style=\"color: black;\">%1</big></td><td style=\"width: 50%;\" valign=\"top\"><big style=\"color: blue;\">%2</big></td></tr>";
    strhtml = strhtml.arg("").arg(title);
    strhtml += "</tbody></table><br>";

    int r = QMessageBox::question(this,
        "Вопрос", strhtml,
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No);
    if (r == QMessageBox::No) return;

    qu = "UPDATE Kabinet SET pr_arhiv=1 WHERE id = %1";
    qu = qu.arg(id);
    if (!sqlquery.exec(qu)) {
        QMessageBox::warning(this, "Ошибка", sqlquery.lastError().databaseText());
        return;
    }
    LoadData();
}

void KabinetWidget::arhiv_delete() {
    useindex = tableView_arhiv->currentIndex();
    QString str_id = usemodel_arhiv->data(usemodel_arhiv->index(useindex.row(), 0), 0).toString();
    if (str_id.isEmpty()) return;

    int id = usemodel_arhiv->data(usemodel_arhiv->index(useindex.row(), 0), 0).toInt();
    QString nomer = usemodel_arhiv->data(usemodel_arhiv->index(useindex.row(), 1), 0).toString();
    QString title = usemodel_arhiv->data(usemodel_arhiv->index(useindex.row(), 2), 0).toString();

    QString strhtml;
    strhtml = "<big style=\"color: red; font-weight: 10px;\"><big>Перевести из архива ?</big></big><br>";
    strhtml += "<table style=\"text-align: left; width: 100%;\" border=0 cellpadding=2 cellspacing=2><tbody>";
    strhtml += "<tr><td style=\"width: 50%;\" align=\"right\" valign=\"top\"><big style=\"color: black;\">%1</big></td><td style=\"width: 50%;\" valign=\"top\"><big style=\"color: blue;\">%2</big></td></tr>";
    strhtml = strhtml.arg("Кабинет:").arg(nomer);
    strhtml += "<tr><td style=\"width: 50%;\" align=\"right\" valign=\"top\"><big style=\"color: black;\">%1</big></td><td style=\"width: 50%;\" valign=\"top\"><big style=\"color: blue;\">%2</big></td></tr>";
    strhtml = strhtml.arg("").arg(title);
    strhtml += "</tbody></table><br>";

    int r = QMessageBox::question(this,
        "Вопрос", strhtml,
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No);
    if (r == QMessageBox::No) return;

    qu = "UPDATE Kabinet SET pr_arhiv=0 WHERE id = %1";
    qu = qu.arg(id);
    if (!sqlquery.exec(qu)) {
        QMessageBox::warning(this, "Ошибка", sqlquery.lastError().databaseText());
    }
    LoadData();
}

void KabinetWidget::add_data() {
    KabinetAddForm *dlg=new KabinetAddForm(this);
    dlg->exec();
    delete dlg;
    LoadData();
}

void KabinetWidget::edit_data() {
    useindex = tableView->currentIndex();
    QString str_id = usemodel->data(usemodel->index(useindex.row(), 0), 0).toString();
    if (str_id.isEmpty()) return;

    int id = usemodel->data(usemodel->index(useindex.row(), 0), 0).toInt();
    KabinetAddForm *dlg=new KabinetAddForm(this, id);
    dlg->exec();
    delete dlg;
    LoadData();
}

void KabinetWidget::delete_data(){
    useindex = tableView->currentIndex();
    QString str_id = usemodel->data(usemodel->index(useindex.row(), 0), 0).toString();
    if (str_id.isEmpty()) return;

    int id = usemodel->data(usemodel->index(useindex.row(), 0), 0).toInt();
    int nomer = usemodel->data(usemodel->index(useindex.row(), 1), 0).toInt();
    QString title = usemodel->data(usemodel->index(useindex.row(), 2), 0).toString();

    QString strhtml;
    strhtml = "<big style=\"color: red; font-weight: 10px;\"><big>Вы действительно хотите удалить?</big></big><br>";
    strhtml += "<table style=\"text-align: left; width: 100%;\" border=0 cellpadding=2 cellspacing=2><tbody>";
    strhtml += "<tr><td style=\"width: 50%;\" align=\"right\" valign=\"top\"><big style=\"color: black;\">%1</big></td><td style=\"width: 50%;\" valign=\"top\"><big style=\"color: blue;\">%2</big></td></tr>";
    strhtml = strhtml.arg("Кабинет:").arg(nomer);
    strhtml += "<tr><td style=\"width: 50%;\" align=\"right\" valign=\"top\"><big style=\"color: black;\">%1</big></td><td style=\"width: 50%;\" valign=\"top\"><big style=\"color: blue;\">%2</big></td></tr>";
    strhtml = strhtml.arg("").arg(title);
    strhtml += "</tbody></table><br>";

    int r = QMessageBox::question(this,
        "Вопрос", strhtml,
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No);
    if (r == QMessageBox::No) return;

    qu = "SELECT id from Transfer WHERE kabinet_id_1 = %1 OR kabinet_id_4 = %1 LIMIT 1";
    qu = qu.arg(id);
    sqlquery.exec(qu);
    if (sqlquery.first()) {
        strhtml = "<p style=\"color: red; font-weight: 14px;\">УДАЛИТЬ  НЕВОЗМОЖНО,  так как</p>";
        strhtml += "<p style=\"color: red; font-weight: 14px;\">данный кабинет уже используется в базе.</p><br>";
        QMessageBox::information(this, "Сообщение", strhtml);
        return;
    }

    qu = "DELETE from Kabinet WHERE id = %1";
    qu = qu.arg(id);
    if (!sqlquery.exec(qu)) {
        QMessageBox::warning(this, "Ошибка", sqlquery.lastError().databaseText());
        return;
    }
    LoadData();
}

void KabinetWidget::LoadData() {
    qu = "SELECT id, nomer, title FROM Kabinet WHERE pr_arhiv<>1 ORDER by nomer, title";
    usemodel->setQuery(qu);
    if (usemodel->lastError().isValid()) {
        QMessageBox::warning(this, "Ошибка", usemodel->lastError().databaseText());
        return;
    }
    while (usemodel->canFetchMore()) {
         usemodel->fetchMore();
    }

    SetModelsParam(usemodel);
    tableView->setModel(usemodel);
    tableView->hideColumn(0);
    tableView->resizeColumnsToContents();

    //  **************************************************

    qu = "SELECT id, nomer, title FROM Kabinet WHERE pr_arhiv=1 ORDER by nomer, title";
    usemodel_arhiv->setQuery(qu);
    if (usemodel_arhiv->lastError().isValid()) {
        QMessageBox::warning(this, "Ошибка", usemodel_arhiv->lastError().databaseText());
        return;
    }
    while (usemodel_arhiv->canFetchMore()) {
         usemodel_arhiv->fetchMore();
    }

    SetModelsParam(usemodel_arhiv);
    tableView_arhiv->setModel(usemodel_arhiv);
    tableView_arhiv->hideColumn(0);
    tableView_arhiv->resizeColumnsToContents();
}

void KabinetWidget::SetModelsParam(DataModel*& usemodel) {
    int ii = 0;

    ii++;
    usemodel->setHeaderData(ii, Qt::Horizontal, "Номер кабинета");
    usemodel->setHeaderData(ii, Qt::Horizontal, QSize(150, 28), Qt::SizeHintRole);
    usemodel->setHAlign(ii, "center");
    usemodel->setPointSize(ii, 10);
    usemodel->setBold(ii);
    usemodel->setColor(ii, "red");

    ii++;
    usemodel->setHeaderData(ii, Qt::Horizontal, "Название кабинета");
    usemodel->setHeaderData(ii, Qt::Horizontal, QSize(200, 28), Qt::SizeHintRole);
    usemodel->setHAlign(ii, "left");
    usemodel->setBold(ii);
    usemodel->setPointSize(ii, 9);
    usemodel->setColor(ii, "blue");
}

void KabinetWidget::show() {
    LoadData();
}

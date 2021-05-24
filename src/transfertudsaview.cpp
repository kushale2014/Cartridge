#include "transfertudsaview.h"

TransferTudsaWidget::TransferTudsaWidget() {
    setupUi(this);
    connect(nomer_LE, SIGNAL(textChanged(QString)), this, SLOT(text_changed()));
    connect(clear_TB, SIGNAL(clicked()), this, SLOT(clear_filter()));

    usemodel = new DataModel();

    QFont font("Helvetica", 9, QFont::Bold);
    tableView->horizontalHeader()->setFont(font);
    LoadData();

}

void TransferTudsaWidget::text_changed() {
    LoadData();
}

void TransferTudsaWidget::clear_filter() {
    nomer_LE->setText("");
}

void TransferTudsaWidget::LoadData() {

    qu = "SELECT tr.id, Cartr.invNomer, tr.date2, p2.family || ' ' ||p2.name, tr.date3, p3.family || ' ' ||p3.name";
    if (nomer_LE->text().isEmpty()) qu += " FROM (SELECT * FROM Transfer WHERE date2 NOT NULL AND date3 NOT NULL) as tr";
    else {
    QString str = " FROM (SELECT ttt.*"
            " FROM (SELECT * FROM Transfer WHERE date2 NOT NULL AND date3 NOT NULL) as ttt,"
                 " (SELECT * FROM Cartr WHERE Cartr.invNomer LIKE '%%1') as ccc"
            " WHERE ttt.cartr_id = ccc.id) as tr";
    qu += str.arg(nomer_LE->text());
    }
    qu +=  " LEFT OUTER JOIN Cartr ON tr.cartr_id = Cartr.id"
           " LEFT OUTER JOIN Person as p2 ON tr.person_id_2 = p2.id"
           " LEFT OUTER JOIN Person as p3 ON tr.person_id_3 = p3.id"
           " ORDER by 2, 3";
    usemodel->setQuery(qu);
    if (usemodel->lastError().isValid()) {
        QMessageBox::warning(this, "Ошибка!!!", usemodel->lastError().databaseText());
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

void TransferTudsaWidget::SetModelsParam() {
    int ii = 0;

    ii++;
    usemodel->setHeaderData(ii, Qt::Horizontal, "Инв.номер");
    usemodel->setHeaderData(ii, Qt::Horizontal, QSize(80, 36), Qt::SizeHintRole);
    usemodel->setHAlign(ii, "center");
    usemodel->setColor(ii, "blue");
    usemodel->setPointSize(ii, 11);
    usemodel->setBold(ii);

    ii++;
    usemodel->setHeaderData(ii, Qt::Horizontal, "Дата\nотправки");
    usemodel->setHeaderData(ii, Qt::Horizontal, QSize(100, 36), Qt::SizeHintRole);
    usemodel->setHAlign(ii, "center");
    usemodel->setColor(ii, "red");
    usemodel->setDate(ii,"");
    usemodel->setBold(ii);

    ii++;
    usemodel->setHeaderData(ii, Qt::Horizontal, "Кто отвез");
    usemodel->setHeaderData(ii, Qt::Horizontal, QSize(180, 36), Qt::SizeHintRole);
    usemodel->setHAlign(ii, "left");

    ii++;
    usemodel->setHeaderData(ii, Qt::Horizontal, "Дата\nприхода");
    usemodel->setHeaderData(ii, Qt::Horizontal, QSize(100, 36), Qt::SizeHintRole);
    usemodel->setHAlign(ii, "center");
    usemodel->setColor(ii, "red");
    usemodel->setDate(ii,"");
    usemodel->setBold(ii);

    ii++;
    usemodel->setHeaderData(ii, Qt::Horizontal, "Кто привез");
    usemodel->setHeaderData(ii, Qt::Horizontal, QSize(180, 36), Qt::SizeHintRole);
    usemodel->setHAlign(ii, "left");
}

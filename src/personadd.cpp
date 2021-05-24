#include "personadd.h"

PersonAddForm::PersonAddForm(QWidget *parent):QDialog(parent) {
    setupUi(this);
    isNew = true; lastID = -1;
    setWindowTitle("Добавление сотрудника");
    setWindowIcon(QPixmap(":/resources/0415.ico"));
    connect(pbSave, SIGNAL(clicked()), this, SLOT(save_data()));
}

PersonAddForm::PersonAddForm(QWidget *parent, int par_id):QDialog(parent) {
    setupUi(this);
    isNew = false; lastID = -1;
    setWindowTitle("Редактирование сотрудника");
    setWindowIcon(QPixmap(":/buttonEdit"));
    connect(pbSave, SIGNAL(clicked()), this, SLOT(save_data()));
    useID = par_id;

    qu = "SELECT family, name, surname FROM Person WHERE id=%1";
    qu = qu.arg(useID);
    if (!sqlquery.exec(qu)) {
       QMessageBox::warning(this, "Ошибка", sqlquery.lastError().databaseText());
       return;
    }
    sqlquery.first();
    family_LE->setText(sqlquery.value(0).toString());
    name_LE->setText(sqlquery.value(1).toString());
    surname_LE->setText(sqlquery.value(2).toString());
}

bool PersonAddForm::validate() {

    return true;
}

void PersonAddForm::save_data() {
    mes_error = "";
    if (isNew) add_data();
    else edit_data();
    if (mes_error.isEmpty()) reject();
    else QMessageBox::warning(this, "Ошибка", mes_error);
}

void PersonAddForm::add_data() {
    if (!validate()) return;

    qu = "INSERT INTO Person(family, name, surname, pr_arhiv) VALUES('%1', '%2', '%3', %4)";
    qu = qu.arg(family_LE->text()).arg(name_LE->text()).arg(surname_LE->text()).arg(0);
    ok = sqlquery.exec(qu);
    if (!ok) {
        mes_error = sqlquery.lastError().databaseText();
        return;
    }
    qu = "SELECT last_insert_rowid()";
    sqlquery.exec(qu);
    sqlquery.first();
    lastID = sqlquery.value(0).toInt();
}

void PersonAddForm::edit_data() {
    qu = "UPDATE Person SET family = '%2', name = '%3', surname = '%4' WHERE id = %1";
    qu = qu.arg(useID).arg(family_LE->text()).arg(name_LE->text()).arg(surname_LE->text());
    ok = sqlquery.exec(qu);
    if (!ok) {
        mes_error = sqlquery.lastError().databaseText();
        return;
    }
    lastID = useID;
}

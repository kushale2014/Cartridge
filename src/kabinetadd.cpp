#include "kabinetadd.h"

KabinetAddForm::KabinetAddForm(QWidget *parent):QDialog(parent) {
    setupUi(this);
    setWindowTitle("Добавление кабинета");
    setWindowIcon(QPixmap(":/resources/0415.ico"));
    connect(pbSave, SIGNAL(clicked()), this, SLOT(save_data()));
    isNew = true; lastID = -1;
}

KabinetAddForm::KabinetAddForm(QWidget *parent, int id):QDialog(parent) {
    setupUi(this);
    setWindowTitle("Редактирование кабинета");
    setWindowIcon(QPixmap(":/buttonEdit"));
    connect(pbSave, SIGNAL(clicked()), this, SLOT(save_data()));
    nomer_SPB->setReadOnly(true);
    isNew = false; lastID = -1;
    useID = id;

    qu = "SELECT nomer, title FROM Kabinet WHERE id = %1";
    qu = qu.arg(useID);
    if (!sqlquery.exec(qu)) {
       QMessageBox::warning(this, "Ошибка", sqlquery.lastError().databaseText());
       return;
    }
    sqlquery.first();
    nomer_SPB->setValue(sqlquery.value(0).toInt());
    title_LE->setText(sqlquery.value(1).toString());
}

bool KabinetAddForm::validate() {
    qu = "SELECT nomer, title FROM Kabinet WHERE nomer = %1 AND title = '%2' LIMIT 1";
    qu = qu.arg(getNomer()).arg(getTitle());
    sqlquery.exec(qu);
    if (sqlquery.first()) {
        mes_error = "Кабинет с таким номером и названием уже существует.";
        return false;
    }
    if (isNew) {
        qu = "SELECT id, nomer, title FROM Kabinet WHERE nomer = %1 AND pr_arhiv<>1";
        qu = qu.arg(getNomer());
        sqlquery.exec(qu);
        if (sqlquery.first()) {
            int id = sqlquery.value(0).toInt();
            QString nomer = sqlquery.value(1).toString();
            QString title = sqlquery.value(2).toString();
            QString strhtml;
            strhtml = "<div style=\"color: red; font-size: 12pt;\"><p>Кабинет с таким номером уже существует.</p>";
            strhtml += "<p>Перевести его в архив ?</p></div>";
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
            if (r == QMessageBox::Yes) {
                qu = "UPDATE Kabinet SET pr_arhiv=1 WHERE id = %1";
                qu = qu.arg(id);
                sqlquery.exec(qu);
            } else {
                mes_error = "Кабинет с таким номером уже существует.\nИзмените номер кабинета.";
                return false;
            }
        }
    }

    return true;
}

void KabinetAddForm::save_data() {
    mes_error = "";
    if (isNew) add_data();
    else edit_data();
    if (mes_error.isEmpty()) reject();
    else QMessageBox::warning(this, "Ошибка", mes_error);
}

void KabinetAddForm::add_data() {
    if (!validate()) return;

    qu = "INSERT INTO Kabinet(nomer, title) VALUES(%1, '%2')";
    qu = qu.arg(getNomer()).arg(getTitle());
    if (!sqlquery.exec(qu)) {
        QMessageBox::warning(this, "Ошибка", sqlquery.lastError().databaseText());
        return;
    }
    qu = "SELECT last_insert_rowid()";
    sqlquery.exec(qu);
    sqlquery.first();
    lastID = sqlquery.value(0).toInt();
}

void KabinetAddForm::edit_data() {
    if (!validate()) return;
    qu = "UPDATE Kabinet SET title = '%2' WHERE `id` = %1";
    qu = qu.arg(useID).arg(getTitle());
    if (!sqlquery.exec(qu)) {
        QMessageBox::warning(this, "Ошибка", sqlquery.lastError().databaseText());
        return;
    }
    lastID = useID;
}

#include "cartradd.h"

CartrAddForm::CartrAddForm(QWidget *parent):QDialog(parent) {
    setupUi(this);
    setWindowTitle("Добавление картриджа");
    setWindowIcon(QPixmap(":/resources/0415.ico"));
    connect(pbSave, SIGNAL(clicked()), this, SLOT(save_data()));
    isNew = true;
    useID = getMaxID(); invNomer = "";
    if (useID > -1) {
        QString tt = QString("0000%1").arg(useID);
        invNomer = tt.right(5);
        invNomer_LE->setText(QString("Б-Днестр. %1").arg(invNomer));
    }
    initComboBox();
}

CartrAddForm::CartrAddForm(QWidget *parent, int id):QDialog(parent) {
    setupUi(this);
    setWindowTitle("Редактирование картриджа");
    setWindowIcon(QPixmap(":/buttonEdit"));
    connect(pbSave, SIGNAL(clicked()), this, SLOT(save_data()));
    isNew = false;
    useID = id;
    initComboBox();

    qu = "SELECT `invNomer`, `model`, `serialNo1`, `serialNo2`, `home_kabinet_id`, `person_id` FROM `Cartr` WHERE `id` = %1";
    qu = qu.arg(useID);
    if (!sqlquery.exec(qu)) {
       QMessageBox::warning(this, "Ошибка", sqlquery.lastError().databaseText());
       return;
    }
    sqlquery.first();

    invNomer = sqlquery.value(0).toString();
    invNomer_LE->setText(QString("Б-Днестр. %1").arg(invNomer));
    model_LE->setText(sqlquery.value(1).toString());
    serialNo1_LE->setText(sqlquery.value(2).toString());
    serialNo2_LE->setText(sqlquery.value(3).toString());

    if (!sqlquery.value(4).toString().isEmpty()) {
        int kabinet = sqlquery.value(4).toInt();
        int ind = home_kabinet_CB->findData(kabinet, Qt::UserRole, Qt::MatchExactly);
        home_kabinet_CB->setCurrentIndex(ind);
    }

    if (!sqlquery.value(5).toString().isEmpty()) {
        int person = sqlquery.value(5).toInt();
        int ind = person_CB->findData(person, Qt::UserRole, Qt::MatchExactly);
        person_CB->setCurrentIndex(ind);
    }
}

void CartrAddForm::initComboBox() {
    if (isNew) {
        home_kabinet_CB->setModel( new BaseComboModel( "nomer || ' - '||  title", "Kabinet WHERE pr_arhiv<>1 ORDER by nomer, title", this ) );
        person_CB->setModel( new BaseComboModel( "family || '  ' ||  name", "Person WHERE pr_arhiv<>1 ORDER by 2", this ) );
    } else {
        QString qu1 = "nomer || ' - '||  title";
        QString qu2 = "Kabinet ORDER by nomer, title";
        qu = "SELECT pr_arhiv FROM Kabinet WHERE id = %1 AND pr_arhiv = 1 LIMIT 1";
        qu = qu.arg(useID);
        sqlquery.exec(qu);
        if (sqlquery.first()) {
            qu2 =  "Kabinet"
                        " UNION"
                        " SELECT nomer || ' - '||  title || ' ' || (в архиве) FROM Kabinet WHERE id = %1 LIMIT 1"
                        " ORDER by nomer, title";
            qu2 = qu2.arg(useID);
        }
        home_kabinet_CB->setModel( new BaseComboModel( qu1, qu2, this ) );
        person_CB->setModel( new BaseComboModel( "family || '  ' ||  name", "Person ORDER by 2", this ) );
    }
}

bool CartrAddForm::validate() {

    return true;
}

void CartrAddForm::save_data() {
    if (isNew) add_data();
    else edit_data();
    reject();
}

void CartrAddForm::add_data() {
    if (!validate()) return;

    qu = "INSERT INTO Cartr("
            "id, model, serialNo1, serialNo2, invNomer, home_kabinet_id, person_id)"
            " VALUES(%1, '%2', '%3', '%4', '%5', %6, %7)";
    qu = qu.arg(useID).arg(getModel()).arg(getSerialNo1()).arg(getSerialNo2()).arg(invNomer)
            .arg(getComboID(home_kabinet_CB)).arg(getComboID(person_CB));
    if (!sqlquery.exec(qu)) {
        QMessageBox::warning(this, "Ошибка", sqlquery.lastError().databaseText());
        return;
    }
}

void CartrAddForm::edit_data() {
    if (!validate()) return;

    qu = "UPDATE Cartr"
            " SET model = '%2', serialNo1 = '%3', serialNo2 = '%4', invNomer = '%5', home_kabinet_id = %6, person_id=%7"
            " WHERE id = %1";
    qu = qu.arg(useID).arg(getModel()).arg(getSerialNo1()).arg(getSerialNo2()).arg(invNomer)
            .arg(getComboID(home_kabinet_CB)).arg(getComboID(person_CB));
    if (!sqlquery.exec(qu)) {
        QMessageBox::warning(this, "Ошибка", sqlquery.lastError().databaseText());
        return;
    }
}

QString CartrAddForm::getComboID(QComboBox*& combobox) {
    int id = combobox->itemData( combobox->currentIndex(), Qt::UserRole ).toInt();
    if (id < 1) return "NULL";
   else return QString("%1").arg(id);
}

int CartrAddForm::getMaxID()
{
    qu = "SELECT MAX(`id`) FROM `Cartr`";
    if (!sqlquery.exec(qu)) {
        QMessageBox::warning(this, "Ошибка", sqlquery.lastError().databaseText());
        return -1;
    }
    sqlquery.first();
    int max = sqlquery.value(0).toInt() + 1;
    return max;
}

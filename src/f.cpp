#include "f.h"

F::F()
{
}

int F::getComboID(QComboBox*& widget) {
    int nn = widget->currentIndex();
    if (nn>-1) {
//        QSqlRecord rec = widget->model()->record(nn);
//        nn = rec.value(0).toInt();
    }
    return nn;
}

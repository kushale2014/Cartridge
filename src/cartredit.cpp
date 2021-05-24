#include "cartredit.h"

CartrEdit::CartrEdit(const int id, const int cartr_id, QWidget *parent) :
    QWidget(parent)
{
    useID = id;
    useCartrID = cartr_id;
    horizontalLayout_2 = new QHBoxLayout(this);
    horizontalLayout_2->setContentsMargins(0, 0, 0, 3);
    groupBox = new QGroupBox(this);
    QFont font;
    font.setPointSize(10);
    groupBox->setFont(font);
    groupBox->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
    groupBox->setCheckable(true);
    groupBox->setChecked(false);
    horizontalLayout = new QHBoxLayout(groupBox);
    horizontalLayout->setSpacing(6);
    horizontalLayout->setContentsMargins(3, 3, 3, 1);

    cartr_LE = new QLineEdit(groupBox);
    cartr_LE->setReadOnly(true);
    cartr_LE->setMinimumSize(QSize(250, 0));
//    cartr_LE->setMaximumSize(QSize(50, 16777215));
    QPalette palette;
    QBrush brush(QColor(85, 255, 127, 255));
    brush.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::Base, brush);
    palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
    palette.setBrush(QPalette::Disabled, QPalette::Base, brush);
    cartr_LE->setPalette(palette);
    cartr_LE->setFont(font);
    horizontalLayout->addWidget(cartr_LE);

    horizontalLayout_2->addWidget(groupBox);
    horizontalSpacer = new QSpacerItem(0, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    horizontalLayout_2->addItem(horizontalSpacer);

}

void CartrEdit::setText(const QString text) {
    cartr_LE->setText(text);
    if (!groupBox->isChecked()) groupBox->setChecked(true);
}

QString CartrEdit::getID() {
    if (groupBox->isChecked()) return QString("%1").arg(useID);
    else return "NULL";
}

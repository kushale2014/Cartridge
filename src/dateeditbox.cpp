#include "dateeditbox.h"

DateEditBox::DateEditBox(QWidget *parent) :
    QWidget(parent)
{
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
    currentDate_TB = new QToolButton(groupBox);
    currentDate_TB->setText("ТД");
    currentDate_TB->setToolTip("Установить текущую дату");
    QFont font1;
    font1.setPointSize(8);
    currentDate_TB->setFont(font1);

    horizontalLayout->addWidget(currentDate_TB);

    dateEdit = new QDateEdit(groupBox);
    dateEdit->setCalendarPopup(true);

    horizontalLayout->addWidget(dateEdit);

    horizontalLayout_2->addWidget(groupBox);
    horizontalSpacer = new QSpacerItem(0, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    horizontalLayout_2->addItem(horizontalSpacer);

    useDate = QDate();

    connect(groupBox, SIGNAL(toggled(bool)), this, SLOT(groupBox_toggled()));
    connect(currentDate_TB, SIGNAL(clicked()), this, SLOT(setCurrentDate()));
    connect(dateEdit, SIGNAL(dateChanged(QDate)), this, SLOT(setUseDate(QDate)));

}

void DateEditBox::groupBox_toggled() {
    if (groupBox->isChecked()) {
        if (useDate==QDate()) setCurrentDate();
    } else useDate = QDate();
}

void DateEditBox::setCurrentDate() {
    useDate = QDate::currentDate();
    dateEdit->setDate(useDate);
}

void DateEditBox::setDate(const QDate &date)
{
    groupBox->setChecked(!date.isNull());
    if (!date.isNull()){
        dateEdit->setDate(date);
    }
}

void DateEditBox::setUseDate(const QDate &date)
{
    useDate = date;
}

QDate DateEditBox::getDate() const  {
    return useDate;
}

void DateEditBox::setText(QString text){
    groupBox->setTitle(text);
}

void DateEditBox::setChecked(bool fl) {
    if (fl) {
        if (!groupBox->isChecked()) groupBox->setChecked(fl);
    } else {
        if (groupBox->isChecked()) groupBox->setChecked(fl);
    }

}

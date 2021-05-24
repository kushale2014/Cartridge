#ifndef DATEEDITBOX_H
#define DATEEDITBOX_H

#include <QWidget>
#include <QDateEdit>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QToolButton>
#include <QFont>
#include <QDebug>

class DateEditBox : public QWidget
{
    Q_OBJECT

public:
    explicit DateEditBox(QWidget *parent = 0);
    void setText(QString);
    void setChecked(bool);
    void setDate ( const QDate &date );
    QDate getDate() const;
private:
    QHBoxLayout *horizontalLayout_2;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout;
    QDateEdit *dateEdit;
    QToolButton *currentDate_TB;
    QSpacerItem *horizontalSpacer;
    QDate useDate;
public slots:
    void groupBox_toggled();
    void setCurrentDate();
    void setUseDate ( const QDate &date );
};

#endif // DATEEDITBOX_H

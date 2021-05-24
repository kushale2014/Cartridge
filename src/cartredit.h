#ifndef CARTREDIT_H
#define CARTREDIT_H

#include <QWidget>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QFont>

class CartrEdit : public QWidget
{
    Q_OBJECT

public:
    explicit CartrEdit(const int, const int, QWidget *parent = 0);
    void setText(const QString);
    QString getID();
    int getCartrID() {return useCartrID;};
    void setChecked(bool ch) {groupBox->setChecked(ch);};
//    QString getText() {return cartr_LE->text();};

private:
    int useID;
    int useCartrID;
    QHBoxLayout *horizontalLayout_2;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout;
    QLineEdit *cartr_LE;
    QSpacerItem *horizontalSpacer;
};

#endif // CARTREDIT_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H
//
#include <QMainWindow>
#include "ui_mainwindow.h"
#include "src/transfermain.h"
#include "src/cartrview.h"
#include "src/personview.h"
#include "src/kabinetview.h"

class MainWindow : public QMainWindow, public Ui::MainWindow
{
Q_OBJECT
public:
    MainWindow( QWidget * parent = 0, Qt::WindowFlags f = 0 );
private:
    Maintransfer *transfer;
    CartrWidget *cartr;
    PersonWidget *person;
    KabinetWidget *kabinet;
protected:
    void closeEvent(QCloseEvent *event);
};
#endif





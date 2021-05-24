#include "mainwindow.h"

MainWindow::MainWindow( QWidget * parent, Qt::WindowFlags f)
    : QMainWindow(parent, f)
{
    setupUi(this);
    setWindowState(Qt::WindowMaximized);

    transfer = new Maintransfer();
    gridLayout_transfer->addWidget(transfer);

    cartr = new CartrWidget();
    gridLayout_cartr->addWidget(cartr);

    person = new PersonWidget();
    gridLayout_person->addWidget(person);

    kabinet = new KabinetWidget();
    gridLayout_kabinet->addWidget(kabinet);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    event->accept();
}

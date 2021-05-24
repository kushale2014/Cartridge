#include <QApplication>
#include <QtGui>

#include "mainwindow.h"
#include "src/connection.h"

int main(int argc, char ** argv)
{
    QApplication app( argc, argv );

    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(),
    QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&qtTranslator);

    if (!createConnection())
         return 1;
    MainWindow win;
    win.show(); 
    app.connect( &app, SIGNAL( lastWindowClosed() ), &app, SLOT( quit() ) );
    return app.exec();
}

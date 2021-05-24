QT = gui core sql widgets
TEMPLATE = app
DESTDIR  = ../cartridge

HEADERS += mainwindow.h \
    src/cartrview.h \
    src/cartradd.h \
    src/personview.h \
    src/personadd.h \
    src/kabinetview.h \
    src/kabinetadd.h \
    src/datamodel_cc.h \
    src/connection.h \
    src/transferview.h \
    src/transferstockadd.h \
    src/transferstockview.h \
    src/transfertudsatoadd.h \
    src/transfertudsafromadd.h \
    src/transfertudsaview.h \
    src/dateeditbox.h \
    src/transfercartradd.h \
    src/cartredit.h \
    src/transferkabinetadd.h \
    src/transferkabinetview.h \
    src/transferstocktudsaview.h \
    src/transfermain.h \
    src/basecombomodel.h \
    src/cartrview_arhiv.h
SOURCES += main.cpp \
    mainwindow.cpp \
    src/cartrview.cpp \
    src/cartradd.cpp \
    src/personview.cpp \
    src/personadd.cpp \
    src/kabinetview.cpp \
    src/kabinetadd.cpp \
    src/datamodel_cc.cpp \
    src/transferview.cpp \
    src/transferstockadd.cpp \
    src/transferstockview.cpp \
    src/transfertudsatoadd.cpp \
    src/transfertudsafromadd.cpp \
    src/transfertudsaview.cpp \
    src/dateeditbox.cpp \
    src/transfercartradd.cpp \
    src/cartredit.cpp \
    src/transferkabinetadd.cpp \
    src/transferkabinetview.cpp \
    src/transferstocktudsaview.cpp \
    src/transfermain.cpp \
    src/basecombomodel.cpp \
    src/cartrview_arhiv.cpp
FORMS += mainwindow.ui \
    src/cartrview.ui \
    src/cartradd.ui \
    src/personview.ui \
    src/personadd.ui \
    src/kabinetview.ui \
    src/kabinetadd.ui \
    src/transferview.ui \
    src/transferstockadd.ui \
    src/transferstockview.ui \
    src/transfertudsaadd.ui \
    src/transfertudsaview.ui \
    src/transfercartradd.ui \
    src/transferkabinetadd.ui \
    src/transferkabinetview.ui \
    src/transferstocktudsaview.ui \
    src/transfermain.ui \
    src/cartrview_arhiv.ui
MOC_DIR = build
OBJECTS_DIR = build
RCC_DIR = build
UI_DIR = build

RESOURCES += src/resources.qrc
RC_ICONS = $$PWD/src/resources/printer.ico

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addawarddialog.cpp \
    addpaperdialog.cpp \
    addstudentdialog.cpp \
    adminwindow.cpp \
    changepasswddialog.cpp \
    dataProcess.c \
    findstudentdialog.cpp \
    globalVar.c \
    gpatable.cpp \
    logInCheck.c \
    loginwidget.cpp \
    main.cpp \
    mainwindow.cpp \
    mypushbutton.cpp \
    projecttable.cpp \
    scoretable.cpp \
    siftstudentdialog.cpp \
    statistics.c \
    student-grademanager.c \
    student.c \
    studentwindow.cpp \
    verifypasswddialog.cpp

HEADERS += \
    Worker.h \
    addawarddialog.h \
    addpaperdialog.h \
    addstudentdialog.h \
    adminwindow.h \
    changepasswddialog.h \
    dataProcess.h \
    findstudentdialog.h \
    globalVar.h \
    gpatable.h \
    logInCheck.h \
    loginwidget.h \
    mainwindow.h \
    mypushbutton.h \
    projecttable.h \
    scoretable.h \
    siftstudentdialog.h \
    student-grademanager.h \
    student.h \
    studentwindow.h \
    verifypasswddialog.h

FORMS += \
    addawarddialog.ui \
    addpaperdialog.ui \
    addstudentdialog.ui \
    changepasswddialog.ui \
    findstudentdialog.ui \
    gpatable.ui \
    loginwidget.ui \
    mainwindow.ui \
    projecttable.ui \
    scoretable.ui \
    siftstudentdialog.ui \
    verifypasswddialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

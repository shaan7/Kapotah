# -------------------------------------------------
# Project created by QtCreator 2009-07-22T11:50:41
# -------------------------------------------------
QT += network \
    webkit \
    xml
TARGET = Kapotah
TEMPLATE = app
SOURCES += main.cpp \
    dialog.cpp \
    peermanager.cpp \
    peerinfo.cpp \
    chatdialog.cpp \
    messagesender.cpp \
    server.cpp \
    filesenderthread.cpp \
    fileserver.cpp \
    fileserverthread.cpp \
    filereceiverthread.cpp \
    dirsenderthread.cpp \
    serverthread.cpp \
    messagesenderthread.cpp \
    dirreceiverthread.cpp \
    filestatusdialog.cpp \
    aboutdialog.cpp \
    transfermanager.cpp
HEADERS += dialog.h \
    peermanager.h \
    peerinfo.h \
    chatdialog.h \
    messagesender.h \
    server.h \
    filesenderthread.h \
    fileserver.h \
    fileserverthread.h \
    filereceiverthread.h \
    dirsenderthread.h \
    serverthread.h \
    messagesenderthread.h \
    dirreceiverthread.h \
    filestatusdialog.h \
    pointers.h \
    aboutdialog.h \
    transfermanager.h
FORMS += dialog.ui \
    chatdialog.ui \
    filestatusdialog.ui \
    aboutdialog.ui
RESOURCES += systray.qrc

#-------------------------------------------------
#
# Project created by QtCreator 2012-01-16T17:27:13
#
#-------------------------------------------------

QT       += core gui webkit network

INCLUDEPATH += ../qjson/include

linux* {
LIBS += ../qjson/build/lib/libqjson.so
}

win32-g++ {
    LIBS += ../qjson/build/lib/qjson0.dll
}

win32-msvc* {
    DEFINES += CISO646
    LIBS += ../qjson/build/lib/qjson0.lib
}

macx* {
LIBS += -F../qjson/build/lib -framework qjson
}

unix {
    QMAKE_CXXFLAGS += \
        -Wall \
        -Wshadow \
        -Wextra \
        -Wno-long-long \
        -pedantic
}

TARGET = google_api_plus_clent
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    oauth2.cpp \
    logindialog.cpp \
    form.cpp \
    plus_data_manager.cpp \
    PeopleWidget.cpp \
    ActivitiesWidget.cpp \
    HtmlGenerator.cpp \
    PeopleListModel.cpp \
    PeopleItemDelegate.cpp \
    ImageDownloader.cpp \
    PlusWidget.cpp \
    PlusDialog.cpp \
    SimplyActivitiesWidget.cpp \
    CommentsWidget.cpp \
    PageNavigatorWidget.cpp

HEADERS  += mainwindow.h \
    oauth2.h \
    logindialog.h \
    form.h \
    plus_data_manager.h \
    PeopleWidget.h \
    ActivitiesWidget.h \
    HtmlGenerator.h \
    PeopleListModel.h \
    PeopleItemDelegate.h \
    ImageDownloader.h \
    PlusWidget.h \
    PlusDialog.h \
    SimplyActivitiesWidget.h \
    CommentsWidget.h \
    PageNavigatorWidget.h

FORMS    += mainwindow.ui \
    logindialog.ui \
    form.ui \
    PeopleWidget.ui \
    ActivitiesWidget.ui \
    PlusDialog.ui \
    PageNavigatorWidget.ui

OTHER_FILES += \
    tools/person.js \
    tools/generator.js \
    tools/generate.html \
    CHANGELOG.txt \
    GOOGLE_PROBLEMS.txt

RESOURCES += \
    resources.qrc

















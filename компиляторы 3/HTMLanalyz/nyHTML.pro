TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    lexer.cpp \
    parser.cpp \
    node.cpp \
    block.cpp \
    root.cpp \
    text.cpp

HEADERS += \
    lexer.h \
    parser.h \
    node.h \
    block.h \
    root.h \
    text.h

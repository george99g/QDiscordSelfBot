QT += core network websockets
QT -= gui

CONFIG += c++11

TARGET = QDiscordSelfBot
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    selfbot.cpp \
    utils.cpp \
    command.cpp \
    interpreter.cpp \
    commandmanager.cpp \
    config.cpp

HEADERS += \
    selfbot.hpp \
    utils.hpp \
    command.hpp \
    interpreter.hpp \
    commandmanager.hpp \
    config.hpp

include(QDiscord/QDiscord/QDiscord.pri)

include(commands/commands.pri)

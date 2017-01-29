# NOTICE:
#
# Application name defined in TARGET has a corresponding QML filename.
# If name defined in TARGET is changed, the following needs to be done
# to match new name:
#   - corresponding QML filename must be changed
#   - desktop icon filename must be changed
#   - desktop filename must be changed
#   - icon definition filename in desktop file must be changed
#   - translation filenames have to be changed

# The name of your application
TARGET = harbour-pingyou

CONFIG += sailfishapp

PKGCONFIG += TelepathyQt5

QT += quick qml dbus

SOURCES +=  src/harbour-pingyou.cpp \
    src/accounts-model.cpp \
    src/roster-model.cpp \
    src/pingyou-settings.cpp \
    src/filemodel.cpp


HEADERS += \
    src/accounts-model.h \
    src/roster-model.h \
    src/pingyou-settings.h \
    src/account-properties-dialog-data.h \
    src/filemodel.h
    #src/account-properties-dialog-data.h


OTHER_FILES += \
    rpm/harbour-pingyou.changes.in \
    rpm/harbour-pingyou.spec \
    rpm/harbour-pingyou.yaml \
    translations/*.ts \
    harbour-pingyou.desktop

SAILFISHAPP_ICONS = 86x86 108x108 128x128 256x256

# to disable building translations every time, comment out the
# following CONFIG line
CONFIG += sailfishapp_i18n

# German translation is enabled as an example. If you aren't
# planning to localize your app, remember to comment out the
# following TRANSLATIONS line. And also do not forget to
# modify the localized app name in the the .desktop file.
TRANSLATIONS += translations/harbour-pingyou-de.ts

INCLUDEPATH += /usr/include/telepathy-qt5

INCLUDEPATH += /usr/include/qt5/QtDBus

DISTFILES += \
    qml/harbour-pingyou.qml \
    qml/cover/CoverPage.qml \
    qml/pages/AccountsPage.qml \
    qml/delegates/AccountEntry.qml \
    qml/pages/RosterPage.qml \
    qml/delegates/RosterEntry.qml \
    qml/dialogs/AddContactDialog.qml \
    qml/dialogs/AccountPropertiesDialog.qml \
    qml/pages/SettingsPage.qml \
    qml/pages/AboutPage.qml \
    qml/dialogs/ImagePicker.qml

RESOURCES += \
    harbour-pingyou.qrc


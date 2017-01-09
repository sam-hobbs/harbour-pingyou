# PingYou Development

This document sets out some notes about PingYou's structure and design decisions.

Please bear in mind that I am a novice programmer with limited experience using C++, Qt and QML. If you spot an easier way to organise the project, or any mistakes/misunderstandings, please let me know!

## Telepathy

PingYou uses Telepathy as a backend. Other C++ XMPP libraries exist, but Telepathy is well integrated in to SailfishOS as it provides the cellular telephony capabilities and XMPP support. This makes it the obvious choice for an XMPP app on SFOS, even though understanding the Telepathy architecture can be quite daunting in comparison to using simpler libraries.

I hope that using Telepathy to handle connections will minimise the effect of the app on battery life, and also result in more reliable connection management.

The connection manager responsible for handling XMPP connections in SFOS is **Telepathy Gabble**. PingYou makes use of Telepathy via the **TelepathyQt** library.

Some useful resources are below:

* The wiki pagess on sailfishos.org relating to Telepathy have improved recently and are worth reading:
* * [Telepathy Framework](https://sailfishos.org/wiki/Telepathy_Framework)
* * [Messaging](https://sailfishos.org/wiki/Messaging)

* The Qt bindings for Telepathy are provided by a project called [TelepathyQt](https://github.com/TelepathyIM/telepathy-qt). There is an examples subdirectory containing some simple Qt4-style programs that I have found very useful in understanding the various types and how to use them.

* The [Telepathy project homepage](https://telepathy.freedesktop.org/).

## Use of Telepathy-Qt types within QML

The data in PingYou is presented using models. I have encapsulated the Telepathy-Qt types in my own classes to use as elements in the model (e.g. _AccountsItem_) because some of the Telepathy types such as [Tp::Account](https://github.com/TelepathyIM/telepathy-qt/blob/master/TelepathyQt/account.h) have Q_PROPERTYs that return types that cannot be registered with the QML engine, so they require some processing before the data can be passed to QML.

For example, the presence properties of _Tp::Account_ including _currentPresence_ have type [Tp::Presence](https://github.com/TelepathyIM/telepathy-qt/blob/master/TelepathyQt/presence.h), which is not derived from qobject and does not contain the Q_OBJECT macro.

It is possible to register the _Tp::Account_ type like this:

    qmlRegisterType<Tp::Account>();

The above enables the use of _Tp::Account_ types from within qml when they are created within C++ and exposed to QML like this (where AccountsModel is a QAbstractListModel containing a QList of Tp::AccountPtr):

    AccountsModel *accountsModel = new AccountsModel();
    view->rootContext()->setContextProperty("accountsModel", accountsModel);

However, the _Tp::Account_ type cannot be initialised within qml. If you try to register the type so that it can be imported from within qml like this:

    qmlRegisterType<Tp::Account>("harbour.pingyou",1,0,"Account");

... you will get the error _"no matching function for call to Tp::Account::Account()"_ because _Tp::Account_ has no default constructor (required for qml to initialise the object).


Similarly, the _Tp::Account_ type cannot be registered as a metatype like this:

    qRegisterMetaType<Tp::Account>("Tp::Account");

...because the class contains the Q_DISABLE_COPY macro (you will get an error like _"'Tp::Account::Account(const Tp::Account&)' is private"_. Presumably this is because you are supposed to access the account using a shared pointer returned using the static Account::create functions.

The [docs for qRegisterMetaType](https://doc.qt.io/qt-5/qmetatype.html#qRegisterMetaType) say _"any class or struct that has a public default constructor, a public copy constructor and a public destructor can be registered"_, so it probably wouldn't work even if Tp::Account didn't contain the Q_DISABLE_COPY macro.

I found the following references useful when experimenting with TelepathyQt types and qml:

* [https://doc.qt.io/qt-5/model-view-programming.html](https://doc.qt.io/qt-5/model-view-programming.html)

* [https://doc.qt.io/qt-5/qtqml-tutorials-extending-qml-example.html](https://doc.qt.io/qt-5/qtqml-tutorials-extending-qml-example.html)

* [https://forum.qt.io/topic/6751/solved-passing-qabstractitemmodel-subclasses-to-qml-using-a-qobject-property](https://forum.qt.io/topic/6751/solved-passing-qabstractitemmodel-subclasses-to-qml-using-a-qobject-property)

## Building & Testing PingYou
### Emulator
During development, I build PingYou using the SDK and deploy to a physical device (Jolla 1), because using a mouse to emulate gestures is a pain.

If you want to test PingYou in the emulator, I suggest you install some additional packages, at least until PingYou supports accounts management (if you don't, then the accounts page will be empty and you will not be able to add any accounts).

* **jolla-settings**, **jolla-settings-accounts** and **jolla-settings-accounts-extensions-jabber** (accounts application and plugin for settings, which will allow you to create an XMPP account)
* **jolla-messages** (Jolla's messages application with XMPP support)
* **jolla-contacts** (this will show you some of your roster - only people who you have accepted presence subscriptions for with another client)

### Aspect ratios

The device I use daily is a Fairphone 2. In theory, this means the app gets tested on two different displays with different aspect ratios and pixel densities, and should scale reasonably well. However, I do not have any tablet hardware running SFOS so the app has not been tested on very large screens.

### Architectures

At present I only build for **armv7hl**. If you would like an **i486** build for the Jolla tablet, please let me know and I will do my best.


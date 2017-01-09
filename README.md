# PingYou
An XMPP client for SailfishOS implemented using [TelepathyQt](https://github.com/TelepathyIM/telepathy-qt).

## Screenshots

None yet.

## Features

PingYou is still in the very early stages of development. The following features are currently supported:

* Listing existing account details

## Development Priorities

The following list describes the development priorites for PingYou. If the order looks strange, bear in mind that some features are already supported by Jolla's built-in XMPP client (jolla-messages), and the immediate priority is to address these shortfalls before adding other features, since the two apps can complement each other:

- [ ] Allow the user to view their roster, and also:
    - [ ]  Authorize subscription requests
    - [ ] Deny subscription requests
    - [ ] Remove contact
    - [ ] Block contact
    - [ ] Add contact / send subscription request
- [ ] Send and receive messages to individual contacts
- [ ] Add notifications
- [ ] Add support for group chats (MUC)
- [ ] Ability to create new MUC room from PingYou
- [ ] File transfer support
- [ ] Add new contacts
- [ ] Local message storage
- [ ] Retrieve messages from the server (MAM) - if possible with Telepathy
- [ ] Support for message carbons

Additional tasks:

- [ ] Work out how to disable jolla-messages handling of incoming XMPP messages. Since both clients use Telepathy, accounts created from one client will show up in the other and we don't want double notifications for each incoming message.
- [ ] Create a nice logo

## Development Readme

There is a separate readme for [development](DEVELOPMENT.md), which has some tips for building, explains some design decisions and has some links to resources for learning about Telepathy.

## License (GPLv3+)

PingYou is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

You can find the GNU GPLv3 along with FAQs and guides at  [https://www.gnu.org/licenses/gpl-3.0.html](https://www.gnu.org/licenses/gpl-3.0.html)

PingYou is distributed in the hope that it will be useful, but _without any warranty_; without even the implied warranty of _merchantability_ or _fitness for a particular purpose_.  See the GNU General Public License for more details.
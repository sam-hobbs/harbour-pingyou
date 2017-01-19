/*

PingYou

Copyright (C) 2015 Sam Hobbs

This file is part of PingYou.

PingYou is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

PingYou is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with PingYou.  If not, see <http://www.gnu.org/licenses/>

*/

#include <QSettings>
#include <QDebug>
#include "pingyou-settings.h"

PingYouSettings::PingYouSettings(QObject *parent) : QObject(parent), mSettings(new QSettings(this)) {
    qDebug() << "Initialising PingYouSettings object";
}


bool PingYouSettings::displayDevInfo() const {
    return mSettings->value("displayDevInfo",false).toBool();
}

void PingYouSettings::setDisplayDevInfo(const bool &newValue) {
    if(displayDevInfo() != newValue) {
        qDebug() << "setting displayDevInfo to " << newValue;
        mSettings->setValue("displayDevInfo",newValue);
        emit displayDevInfoChanged(newValue);
    } else {
        qDebug() << "displayDevInfo is not changed (" << !newValue << ")";
    }
}

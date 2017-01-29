/*

PingYou

Copyright (C) 2017 Sam Hobbs

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

#ifndef PINGYOUSETTINGS_H
#define PINGYOUSETTINGS_H

#include <QObject>
#include <QSettings>

class PingYouSettings : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool displayDevInfo READ displayDevInfo WRITE setDisplayDevInfo NOTIFY displayDevInfoChanged)

public:
    explicit PingYouSettings(QObject *parent = 0);

    bool displayDevInfo() const;

    void setDisplayDevInfo(const bool &);

signals:
    void displayDevInfoChanged(bool);

private:
    QSettings * mSettings;
};


#endif // PINGYOUSETTINGS_H

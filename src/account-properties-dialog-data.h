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

#ifndef ACCOUNTPROPERTIESDIALOGDATA_H
#define ACCOUNTPROPERTIESDIALOGDATA_H

#include <QObject>
#include <QStringList>

// All Qt classes are noncopyabe by design
// https://stackoverflow.com/questions/19092513/copy-constructor-of-derived-qt-class#19092698

//class AccountPropertiesDialogData : public QObject
//{
//    Q_OBJECT

//    Q_PROPERTY(bool ignoreSSLErrors MEMBER m_ignoreSSLErrors NOTIFY ignoreSSLErrorsChanged)
//    Q_PROPERTY(uint fallbackStunPort MEMBER m_fallbackStunPort NOTIFY fallbackStunPortChanged)
//    Q_PROPERTY(uint keepaliveInterval MEMBER m_keepaliveInterval NOTIFY keepaliveIntervalChanged)
//    Q_PROPERTY(bool register MEMBER m_register NOTIFY registerChanged)
//    Q_PROPERTY(QString server MEMBER m_server NOTIFY serverChanged)
//    Q_PROPERTY(bool requireEncryption MEMBER m_requireEncryption NOTIFY requireEncryptionChanged)
//    Q_PROPERTY(bool downloadAtConnection MEMBER m_downloadAtConnection NOTIFY downloadAtConnectionChanged)
//    Q_PROPERTY(bool fallbackStunServer MEMBER m_fallbackStunServer NOTIFY fallbackStunServerChanged)
//    Q_PROPERTY(int priority MEMBER m_priority NOTIFY priorityChanged)
//    Q_PROPERTY(QStringList fallbackSocks5Proxies MEMBER m_fallbackSocks5Proxies NOTIFY fallbackSocks5ProxiesChanged)
//    Q_PROPERTY(bool oldSSL MEMBER m_oldSSL NOTIFY oldSSLChanged)
//    Q_PROPERTY(uint port MEMBER m_port NOTIFY portChanged)
//    Q_PROPERTY(QString httpsProxyServer MEMBER m_httpsProxyServer NOTIFY httpsProxyServerChanged)
//    Q_PROPERTY(QString stunServer MEMBER m_stunServer NOTIFY stunServerChanged)
//    Q_PROPERTY(QString resource MEMBER m_resource NOTIFY resourceChanged)
//    Q_PROPERTY(bool decloakAutomatically MEMBER m_decloakAutomatically NOTIFY decloakAutomaticallyChanged)
//    Q_PROPERTY(uint httpsProxyPort MEMBER m_httpsProxyPort NOTIFY httpsProxyPortChanged)
//    Q_PROPERTY(QString account MEMBER m_account NOTIFY accountChanged)
//    Q_PROPERTY(bool lowBandwidth MEMBER m_lowBandwidth NOTIFY lowBandwidthChanged)
//    Q_PROPERTY(QString fallbackServers MEMBER m_fallbackServers NOTIFY fallbackServersChanged)
//    Q_PROPERTY(QString password MEMBER m_password NOTIFY passwordChanged)
//    Q_PROPERTY(uint stunPort MEMBER m_stunPort NOTIFY stunPortChanged)
//    Q_PROPERTY(QString alias MEMBER m_alias NOTIFY aliasChanged)
//    Q_PROPERTY(QString extraCertificateIdentities MEMBER m_extraCertificateIdentities NOTIFY extraCertificateIdentitiesChanged)
//    Q_PROPERTY(QString fallbackConferenceServer MEMBER m_fallbackConferenceServer NOTIFY fallbackConferenceServerChanged)

//signals:
//    void ignoreSSLErrorsChanged();
//    void fallbackStunPortChanged();
//    void keepaliveIntervalChanged();
//    void registerChanged();
//    void serverChanged();
//    void requireEncryptionChanged();
//    void downloadAtConnectionChanged();
//    void fallbackStunServerChanged();
//    void priorityChanged();
//    void fallbackSocks5ProxiesChanged();
//    void oldSSLChanged();
//    void portChanged();
//    void httpsProxyServerChanged();
//    void stunServerChanged();
//    void resourceChanged();
//    void decloakAutomaticallyChanged();
//    void httpsProxyPortChanged();
//    void accountChanged();
//    void lowBandwidthChanged();
//    void fallbackServersChanged();
//    void passwordChanged();
//    void stunPortChanged();
//    void aliasChanged();
//    void extraCertificateIdentitiesChanged();
//    void fallbackConferenceServerChanged();

//private:
//    bool m_ignoreSSLErrors;
//    uint m_fallbackStunPort;
//    uint m_keepaliveInterval;
//    bool m_register;
//    QString m_server;
//    bool m_requireEncryption;
//    bool m_downloadAtConnection;
//    bool m_fallbackStunServer;
//    int m_priority;
//    QStringList m_fallbackSocks5Proxies;
//    bool m_oldSSL;
//    uint m_port;
//    QString m_httpsProxyServer;
//    QString m_stunServer;
//    QString m_resource;
//    bool m_decloakAutomatically;
//    uint m_httpsProxyPort;
//    QString m_account;
//    bool m_lowBandwidth;
//    QString m_fallbackServers;
//    QString m_password;
//    uint m_stunPort;
//    QString m_alias;
//    QString m_extraCertificateIdentities;
//    QString m_fallbackConferenceServer;
//};

struct AccountPropertiesDialogData
{
    bool ignoreSSLErrors;
    uint fallbackStunPort;
    uint keepaliveInterval;
    bool Register;
    QString server;
    bool requireEncryption;
    bool downloadAtConnection;
    bool fallbackStunServer;
    int priority;
    QStringList fallbackSocks5Proxies;
    bool oldSSL;
    uint port;
    QString httpsProxyServer;
    QString stunServer;
    QString resource;
    bool decloakAutomatically;
    uint httpsProxyPort;
    QString account;
    bool lowBandwidth;
    QString fallbackServers;
    QString password;
    uint stunPort;
    QString alias;
    QString extraCertificateIdentities;
    QString fallbackConferenceServer;
};

Q_DECLARE_METATYPE(AccountPropertiesDialogData);


#endif // ACCOUNTPROPERTIESDIALOGDATA_H

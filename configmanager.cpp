#include "configmanager.h"
#include <QDebug>

ConfigManager::ConfigManager(QObject *parent)
    : QObject(parent), settings(nullptr)
{

    settings = new QSettings("config.ini", QSettings::IniFormat, this);

    loadConfig();
}

void ConfigManager::loadConfig()
{
    targetAddress = settings->value("NetworkSettings/targetAddress", "127.0.0.1").toString();
    targetPort = settings->value("NetworkSettings/targetPort", 12346).toUInt();
}

QString ConfigManager::getTargetAddress() const
{
    return targetAddress;
}

quint16 ConfigManager::getTargetPort() const
{
    return targetPort;
}



/*
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    Author: Othmar Ehrhardt, 2020
*/

#include "indiclient.h"

IndiClient::IndiClient()
{
    m_profile = 0;
    m_mount_dp = 0;
    m_focuser_dp = 0;
    QSettings settings("Stroblhofwarte", "TargetClient");
    settings.beginGroup("setup");
    QString selectedProfile = settings.value("Profile").value<QString>();
    settings.endGroup();
    settings.beginGroup("profileNames");
    QStringList keys = settings.allKeys();
    settings.endGroup();
    for (int i = 0; i < keys.size(); ++i)
    {
        Profile *profile = new Profile();
        settings.beginGroup("profile/" + keys.at(i));
        profile->Name = keys.at(i);
        profile->Host = settings.value("Host").value<QString>();
        profile->Port = settings.value("Port").value<int>();
        profile->Mount = settings.value("Mount").value<QString>();
        profile->Focuser = settings.value("Focuser").value<QString>();
        if(profile->Name == selectedProfile)
            m_profile = profile;
        settings.endGroup();
        m_profiles.append(profile);
    }
}

IndiClient::~IndiClient()
{
   this->Disconnect();
}


bool IndiClient::Connect(QString server, int port)
{
    this->setServer(server.toStdString().c_str(), port);
    return this->connectServer();
}

bool IndiClient::Disconnect()
{
    return this->disconnectServer();
}

bool IndiClient::Connected()
{
    return this->isServerConnected();
}

Profile* IndiClient::GetProfile(QString name)
{
    for (int i = 0; i < m_profiles.size(); ++i)
    {
        if(m_profiles.at(i)->Name == name)
            return m_profiles.at(i);
    }
    return 0;
}

void IndiClient::AddProfile(Profile *profile)
{
    m_profiles.append(profile);
    QSettings settings("Stroblhofwarte", "TargetClient");
    settings.beginGroup("setup");
    settings.setValue("Profile", profile->Name);
    m_profile = profile;
    settings.endGroup();
    UpdateProfile();
}

void IndiClient::UpdateProfile()
{
    QSettings settings("Stroblhofwarte", "TargetClient");
    settings.beginGroup("profileNames");
    for (int i = 0; i < m_profiles.size(); ++i)
        settings.setValue(m_profiles.at(i)->Name,0);
    settings.endGroup();
    for (int i = 0; i < m_profiles.size(); ++i)
    {
        settings.beginGroup("profile/" + m_profiles.at(i)->Name);
        settings.setValue("Host", m_profiles.at(i)->Host);
        settings.setValue("Port", m_profiles.at(i)->Port);
        settings.setValue("Mount", m_profiles.at(i)->Mount);
        settings.setValue("Focuser", m_profiles.at(i)->Focuser);
        settings.endGroup();
    }
    settings.sync();
}

bool IndiClient::RemoveProfile(QString name)
{
    Profile *profile = 0;
    int idx;
    for (int i = 0; i < m_profiles.size(); ++i)
    {
        if(m_profiles.at(i)->Name == name)
        {
            profile = m_profiles.at(i);
            idx = i;
            break;
        }
    }
    if(profile == 0) return false;

    QSettings settings("Stroblhofwarte", "TargetClient");
    settings.beginGroup("profileNames");
    settings.remove(profile->Name);
    settings.endGroup();
    settings.beginGroup("profile/" + profile->Name);
    settings.remove("Host");
    settings.remove("Port");
    settings.remove("Mount");
    settings.remove("Focuser");
    settings.endGroup();
    m_profiles.removeAt(idx);
    return true;
}

QList<Profile*> IndiClient::GetAllProfile()
{
    return m_profiles;
}

bool IndiClient::ActivateProfile(QString name)
{
    for (int i = 0; i < m_profiles.size(); ++i)
    {
        if(m_profiles.at(i)->Name == name)
        {
            m_profile = m_profiles.at(i);
            return true;
        }
    }
    return false;
}

Profile *IndiClient::ActiveProfile()
{
    return m_profile;
}

void IndiClient::newDevice(INDI::BaseDevice *dp)
{
    if(m_profile == 0) return;
    if (!strcmp(dp->getDeviceName(), m_profile->Mount.toStdString().c_str()))
        m_mount_dp = dp;
    if (!strcmp(dp->getDeviceName(), m_profile->Focuser.toStdString().c_str()))
        m_focuser_dp = dp;
}

void IndiClient::newProperty(INDI::Property *property)
{
    uint16_t driverInterface;
    QString driverVersion;
    if (!strcmp(property->getDeviceName(), m_profile->Mount.toStdString().c_str()) && !strcmp(property->getName(), "CONNECTION"))
    {
        connectDevice(m_profile->Mount.toStdString().c_str());
        return;
    }
    if (!strcmp(property->getDeviceName(), m_profile->Focuser.toStdString().c_str()) && !strcmp(property->getName(), "CONNECTION"))
    {
        connectDevice(m_profile->Focuser.toStdString().c_str());
        return;
    }
    if (!strcmp(property->getName(), "DRIVER_INFO"))
        {
            ITextVectorProperty *tvp = property->getText();
            if (tvp)
            {
                IText *tp = IUFindText(tvp, "DRIVER_INTERFACE");
                if (tp)
                {
                    driverInterface = static_cast<uint32_t>(atoi(tp->text));
                    if(driverInterface & INDI::BaseDevice::DRIVER_INTERFACE::TELESCOPE_INTERFACE)
                    {
                        m_mounts.append(QString(property->getDeviceName()));
                        emit newTelecope(property->getDeviceName());
                    }
                    if(driverInterface & INDI::BaseDevice::DRIVER_INTERFACE::FOCUSER_INTERFACE)
                    {
                        m_focuser.append(QString(property->getDeviceName()));
                        emit newFocuser(property->getDeviceName());
                    }
                }

                tp = IUFindText(tvp, "DRIVER_VERSION");
                if (tp)
                {
                    driverVersion = QString(tp->text);
                }
            }
        }
}

QList<QString> IndiClient::Mounts()
{
    std::vector<INDI::BaseDevice *> devices;
    QList<QString> mounts;
    if(this->getDevices(devices, INDI::BaseDevice::DRIVER_INTERFACE::TELESCOPE_INTERFACE))
    {
        for (INDI::BaseDevice *device : devices)
            QString name = device->getDeviceName();

    }
}


void IndiClient::MoveNorth()
{
    StopMove();
    ISwitchVectorProperty *ns_switch = NULL;
    if(m_mount_dp == 0) return;
    ns_switch = m_mount_dp->getSwitch("TELESCOPE_MOTION_NS");
    if (ns_switch == NULL)
        return;
    ISwitch *motion = IUFindSwitch(ns_switch, "MOTION_NORTH");
    motion->s = ISS_ON;
    sendNewSwitch(ns_switch);
}

void IndiClient::MoveSouth()
{
    StopMove();
    ISwitchVectorProperty *ns_switch = NULL;
    if(m_mount_dp == 0) return;
    ns_switch = m_mount_dp->getSwitch("TELESCOPE_MOTION_NS");
    if (ns_switch == NULL)
        return;
    ISwitch *motion = IUFindSwitch(ns_switch, "MOTION_SOUTH");
    motion->s = ISS_ON;
    sendNewSwitch(ns_switch);
}

void IndiClient::MoveWest()
{
    StopMove();
    ISwitchVectorProperty *we_switch = NULL;
    if(m_mount_dp == 0) return;
    we_switch = m_mount_dp->getSwitch("TELESCOPE_MOTION_WE");
    if (we_switch == NULL)
        return;
    ISwitch *motion = IUFindSwitch(we_switch, "MOTION_WEST");
    motion->s = ISS_ON;
    sendNewSwitch(we_switch);
}

void IndiClient::MoveEast()
{
    StopMove();
    ISwitchVectorProperty *we_switch = NULL;
    if(m_mount_dp == 0) return;
    we_switch = m_mount_dp->getSwitch("TELESCOPE_MOTION_WE");
    if (we_switch == NULL)
        return;
    ISwitch *motion = IUFindSwitch(we_switch, "MOTION_EAST");
    motion->s = ISS_ON;
    sendNewSwitch(we_switch);
}

void IndiClient::StopMove()
{
    ISwitchVectorProperty *ns_switch = NULL;
    ISwitchVectorProperty *we_switch = NULL;
    if(m_mount_dp == 0) return;
    ns_switch = m_mount_dp->getSwitch("TELESCOPE_MOTION_NS");
    if (ns_switch != NULL)
    {
        ISwitch *motion = IUFindSwitch(ns_switch, "MOTION_NORTH");
        motion->s = ISS_OFF;
        sendNewSwitch(ns_switch);
        motion = IUFindSwitch(ns_switch, "MOTION_SOUTH");
        motion->s = ISS_OFF;
       sendNewSwitch(ns_switch);
    }
    we_switch = m_mount_dp->getSwitch("TELESCOPE_MOTION_WE");
    if (we_switch != NULL)
    {
        ISwitch *motion = IUFindSwitch(we_switch, "MOTION_WEST");
        motion->s = ISS_OFF;
        sendNewSwitch(we_switch);
        motion = IUFindSwitch(we_switch, "MOTION_EAST");
        motion->s = ISS_OFF;
       sendNewSwitch(we_switch);
    }
}

void IndiClient::SlewGuide()
{
    ISwitchVectorProperty *slew_switch = NULL;
    if(m_mount_dp == 0) return;
    slew_switch = m_mount_dp->getSwitch("TELESCOPE_SLEW_RATE");
    if (slew_switch == NULL)
        return;
    IUResetSwitch(slew_switch);
    if(slew_switch->nsp > 0)
    {
        slew_switch->sp[0].s = ISS_ON;
        sendNewSwitch(slew_switch);
    }
}

void IndiClient::SlewCenter()
{
    ISwitchVectorProperty *slew_switch = NULL;
    if(m_mount_dp == 0) return;
    slew_switch = m_mount_dp->getSwitch("TELESCOPE_SLEW_RATE");
    if (slew_switch == NULL)
        return;
    IUResetSwitch(slew_switch);
    if(slew_switch->nsp > 1)
    {
        slew_switch->sp[1].s = ISS_ON;
        sendNewSwitch(slew_switch);
    }
}

void IndiClient::SlewFind()
{
    ISwitchVectorProperty *slew_switch = NULL;
    if(m_mount_dp == 0) return;
    slew_switch = m_mount_dp->getSwitch("TELESCOPE_SLEW_RATE");
    if (slew_switch == NULL)
        return;
    IUResetSwitch(slew_switch);
    if(slew_switch->nsp > 2)
    {
        slew_switch->sp[2].s = ISS_ON;
        sendNewSwitch(slew_switch);
    }
}

void IndiClient::SlewMax()
{
    ISwitchVectorProperty *slew_switch = NULL;
    if(m_mount_dp == 0) return;
    slew_switch = m_mount_dp->getSwitch("TELESCOPE_SLEW_RATE");
    if (slew_switch == NULL)
        return;
    IUResetSwitch(slew_switch);
    if(slew_switch->nsp > 3)
    {
        slew_switch->sp[3].s = ISS_ON;
        sendNewSwitch(slew_switch);
    }
}

void IndiClient::FocusIn()
{
    ISwitchVectorProperty *focus_switch = NULL;
    if(m_focuser_dp == 0) return;
    focus_switch = m_focuser_dp->getSwitch("FOCUS_MOTION");
    if (focus_switch == NULL)
        return;
    ISwitch *motion = IUFindSwitch(focus_switch, "FOCUS_INWARD");
    motion->s = ISS_ON;
    sendNewSwitch(focus_switch);
}

void IndiClient::FocusOut()
{
    ISwitchVectorProperty *focus_switch = NULL;
    if(m_focuser_dp == 0) return;
    focus_switch = m_focuser_dp->getSwitch("FOCUS_MOTION");
    if (focus_switch == NULL)
        return;
    ISwitch *motion = IUFindSwitch(focus_switch, "FOCUS_OUTWARD");
    motion->s = ISS_ON;
    sendNewSwitch(focus_switch);
}

void IndiClient::FocusStop()
{
    ISwitchVectorProperty *focus_switch = NULL;
    if(m_focuser_dp == 0) return;
    focus_switch = m_focuser_dp->getSwitch("FOCUS_MOTION");
    if (focus_switch == NULL)
        return;
    ISwitch *motion = IUFindSwitch(focus_switch, "FOCUS_INWARD");
    motion->s = ISS_OFF;
    sendNewSwitch(focus_switch);
    motion = IUFindSwitch(focus_switch, "FOCUS_OUTWARD");
    motion->s = ISS_OFF;
    sendNewSwitch(focus_switch);
}

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

#ifndef INDICLIENT_H
#define INDICLIENT_H

#include <QObject>
#include <QList>
#include <QString>
#include <QSettings>
#include <baseclient.h>
#include <basedevice.h>
#include <profile.h>

class IndiClient : public QObject, public INDI::BaseClient
{
    Q_OBJECT

public:
    IndiClient();
    ~IndiClient();

    bool Connect(QString server, int port);
    bool Disconnect();
    bool Connected();
    QList<QString> Mounts();
    QList<QString> Focuser();

    QList<Profile*> GetAllProfile();
    Profile* GetProfile(QString name);
    void AddProfile(Profile *profile);
    void UpdateProfile();
    bool ActivateProfile(QString name);
    bool RemoveProfile(QString name);
    Profile *ActiveProfile();

    void MoveNorth();
    void MoveSouth();
    void MoveWest();
    void MoveEast();
    void StopMove();

    void SlewGuide();
    void SlewCenter();
    void SlewFind();
    void SlewMax();

    void FocusIn();
    void FocusOut();
    void FocusStop();

signals:
    void newTelecope(QString name);
    void newFocuser(QString name);

protected:
    virtual void newDevice(INDI::BaseDevice *dp);
    virtual void removeDevice(INDI::BaseDevice *dp) {}
    virtual void newProperty(INDI::Property *property);
    virtual void removeProperty(INDI::Property *property) {}
    virtual void newBLOB(IBLOB *bp) {}
    virtual void newSwitch(ISwitchVectorProperty *svp) {}
    virtual void newNumber(INumberVectorProperty *nvp) {}
    virtual void newMessage(INDI::BaseDevice *dp, int messageID) {}
    virtual void newText(ITextVectorProperty *tvp) {}
    virtual void newLight(ILightVectorProperty *lvp) {}
    virtual void serverConnected() {}
    virtual void serverDisconnected(int exit_code) {}

private:
        QList<QString> m_mounts;
        QList<QString> m_focuser;
        QList<Profile*> m_profiles;
        Profile *m_profile;

        INDI::BaseDevice *m_mount_dp;
        INDI::BaseDevice *m_focuser_dp;
};

#endif // INDICLIENT_H

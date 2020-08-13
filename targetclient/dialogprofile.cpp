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
#include "dialogprofile.h"
#include "ui_dialogprofile.h"

DialogProfile::DialogProfile(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogProfile)
{
    ui->setupUi(this);
    m_indiClient = new IndiClient();
    QObject::connect(m_indiClient, &IndiClient::newTelecope, this, &DialogProfile::newTelecope);
    QObject::connect(m_indiClient, &IndiClient::newFocuser, this, &DialogProfile::newFocuser);
    QList<Profile*> profiles = m_indiClient->GetAllProfile();
    for (int i = 0; i < profiles.size(); ++i)
    {
        ui->profileSelection->addItem(profiles.at(i)->Name);
    }
    Profile *selected = m_indiClient->ActiveProfile();
    if(selected != 0)
        SetProfile(selected->Name);
    else
        SetProfile(ui->profileSelection->currentText());
}

DialogProfile::~DialogProfile()
{
    delete ui;
}

bool DialogProfile::SetProfile(QString name)
{
    QList<Profile*> profiles = m_indiClient->GetAllProfile();
    ui->mount->clear();
    ui->focuser->clear();
    for (int i = 0; i < profiles.size(); ++i)
    {
        if(profiles.at(i)->Name == name)
        {
            ui->profileSelection->setCurrentIndex(i);
            ui->server->setText(profiles.at(i)->Host);
            ui->port->setText(QString::number(profiles.at(i)->Port));
            ui->mount->addItem(profiles.at(i)->Mount);
            ui->focuser->addItem(profiles.at(i)->Focuser);
            return true;
        }
    }
    return false;
}

void DialogProfile::newTelecope(QString name)
{
    int idx = ui->mount->findText(name);
    if(idx == -1)
        ui->mount->addItem(name);
}

void DialogProfile::newFocuser(QString name)
{
    int idx = ui->focuser->findText(name);
    if(idx == -1)
        ui->focuser->addItem(name);
}

void DialogProfile::ClearForm()
{
    ui->server->setText("");
    ui->port->setText("");
    ui->mount->clear();
    ui->focuser->clear();
}

void DialogProfile::on_testAndFill_clicked()
{
    if(m_indiClient->Connect(ui->server->text().toStdString().c_str(), ui->port->text().toInt()))
        ui->info->setText("Connected");
    else
        ui->info->setText("NOT CONNECTED!");
}

void DialogProfile::on_pbNew_clicked()
{
    QString name = ui->profileSelection->currentText();
    ClearForm();
    if(m_indiClient->GetProfile(name) == 0)
    {
        // Profile does not exist. Create a new one
        Profile *profile = new Profile();
        profile->Name = name;
        ui->server->setText("192.168.0.1");
        m_indiClient->AddProfile(profile);
    }
}

void DialogProfile::on_server_editingFinished()
{
    Profile *profile = m_indiClient->ActiveProfile();
    if(profile == 0) return;
    profile->Host = ui->server->text();
    m_indiClient->UpdateProfile();
}

void DialogProfile::on_port_editingFinished()
{
    Profile *profile = m_indiClient->ActiveProfile();
    if(profile == 0) return;
    profile->Port = ui->port->text().toInt();
    m_indiClient->UpdateProfile();
}

void DialogProfile::on_mount_activated(const QString &arg1)
{
    Profile *profile = m_indiClient->ActiveProfile();
    if(profile == 0) return;
    profile->Mount = ui->mount->currentText();
    m_indiClient->UpdateProfile();
}

void DialogProfile::on_focuser_activated(const QString &arg1)
{
    Profile *profile = m_indiClient->ActiveProfile();
    if(profile == 0) return;
    profile->Focuser = ui->focuser->currentText();
    m_indiClient->UpdateProfile();
}

void DialogProfile::on_profileSelection_activated(const QString &arg1)
{
    ClearForm();
    SetProfile(arg1);
    m_indiClient->ActivateProfile(arg1);
}

void DialogProfile::on_pbDel_clicked()
{
    Profile *profile = m_indiClient->ActiveProfile();
    if(profile == 0) return;
    if(m_indiClient->RemoveProfile(profile->Name))
    {
        ui->profileSelection->clear();
        QList<Profile*> profiles = m_indiClient->GetAllProfile();
        for (int i = 0; i < profiles.size(); ++i)
        {
            ui->profileSelection->addItem(profiles.at(i)->Name);
        }
        ClearForm();
        SetProfile(ui->profileSelection->currentText());
    }
}

void DialogProfile::on_done_clicked()
{
    Profile *profile = m_indiClient->ActiveProfile();
    if(profile == 0) return;
    profile->Host = ui->server->text();
    profile->Port = ui->port->text().toInt();
    profile->Mount = ui->mount->currentText();
    profile->Focuser = ui->focuser->currentText();
    m_indiClient->UpdateProfile();
    delete m_indiClient;
    hide();
}

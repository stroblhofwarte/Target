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

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_indiClient = new IndiClient();
    // Setup reconnection timer:
    m_reconnectionTimer = new QTimer(this);
    connect(m_reconnectionTimer, SIGNAL(timeout()), this, SLOT(ReconnectionTimerSlot()));
    m_reconnectionTimer->start(10000);

    QList<Profile*> profiles = m_indiClient->GetAllProfile();
    Profile *activeProfile = m_indiClient->ActiveProfile();
    int idx = -1;
    for (int i = 0; i < profiles.size(); ++i)
    {
        ui->activeProfile->addItem(profiles.at(i)->Name);
        if(activeProfile != 0)
        {
            if(profiles.at(i)->Name == activeProfile->Name)
            {
                idx = i;
            }
        }
    }
    if(idx != -1)
    {
        m_indiClient->ActivateProfile(activeProfile->Name);
        ui->activeProfile->setCurrentIndex(idx);
        ui->host->setText(activeProfile->Host + ":" + QString::number(activeProfile->Port));
        ui->mount->setText(activeProfile->Mount);
        ui->focuser->setText(activeProfile->Focuser);
    }
    else
    {
        if(ui->activeProfile->count() > 0)
        {
            ui->activeProfile->setCurrentIndex(0);
            m_indiClient->ActivateProfile(ui->activeProfile->currentText());
            activeProfile = m_indiClient->ActiveProfile();
            ui->host->setText(activeProfile->Host + ":" + QString::number(activeProfile->Port));
            ui->mount->setText(activeProfile->Mount);
            ui->focuser->setText(activeProfile->Focuser);
        }
    }

    // Setup handbox receiver
    m_receiver = new HandBoxReceiver();
    connect(m_receiver, &HandBoxReceiver::Command, this, &MainWindow::onHandboxComdReceived);
    connect(m_receiver, &HandBoxReceiver::Connected, this, &MainWindow::onConnected);
    connect(m_receiver, &HandBoxReceiver::Connected, this, &MainWindow::onConnected);
    connect(m_receiver, &HandBoxReceiver::Connected, this, &MainWindow::onConnected);
    QSettings settings("Stroblhofwarte", "TargetClient");
    settings.beginGroup("setup");
    QString ip = settings.value("Handbox").value<QString>();
    settings.endGroup();
    ui->handbox->setText(ip);
    m_receiver->SetHandboxIp(ip);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ReconnectionTimerSlot()
{
    if(m_indiClient->Connected())
    {
        ui->indiState->setText("Connected");
    }
    else
    {
        Profile *profile = m_indiClient->ActiveProfile();
        if(profile == 0) return;
        m_indiClient->Connect(profile->Host, profile->Port);
        if(m_indiClient->Connected())
        {
            ui->indiState->setText("Connected");
        }
        else
            ui->indiState->setText("Not connected");
    }
    if(!m_receiver->IsConnected())
    {
        ui->handboxstate->setText("Disconnected");
        m_receiver->Reconnect();
    }
    else
    {
        ui->handboxstate->setText("Connected");
    }
}

void MainWindow::on_profileEditor_clicked()
{
    DialogProfile *dlg = new DialogProfile();
    dlg->show();
}

void MainWindow::on_activeProfile_activated(const QString &arg1)
{
    m_indiClient->Disconnect();
    m_indiClient->ActivateProfile(arg1);
    Profile *activeProfile = m_indiClient->ActiveProfile();
    if(activeProfile == 0) return;
    ui->mount->setText(activeProfile->Mount);
    ui->focuser->setText(activeProfile->Focuser);
    ui->host->setText(activeProfile->Host + ":" + QString::number(activeProfile->Port));
    ui->indiState->setText("Not connected");
    QSettings settings("Stroblhofwarte", "TargetClient");
    settings.beginGroup("setup");
    settings.setValue("Profile", activeProfile->Name);
    settings.endGroup();
}

void MainWindow::on_north_pressed()
{
    m_indiClient->MoveNorth();
}

void MainWindow::on_north_released()
{
    m_indiClient->StopMove();
}

void MainWindow::on_west_pressed()
{
    m_indiClient->MoveWest();
}

void MainWindow::on_west_released()
{
    m_indiClient->StopMove();
}

void MainWindow::on_east_pressed()
{
    m_indiClient->MoveEast();
}

void MainWindow::on_east_released()
{
    m_indiClient->StopMove();
}

void MainWindow::on_south_pressed()
{
    m_indiClient->MoveSouth();
}

void MainWindow::on_south_released()
{
    m_indiClient->StopMove();
}

void MainWindow::on_guide_clicked()
{
    m_indiClient->SlewGuide();
    ui->slew->setText("Guide");
}

void MainWindow::on_center_clicked()
{
    m_indiClient->SlewCenter();
    ui->slew->setText("Center");
}

void MainWindow::on_find_clicked()
{
    m_indiClient->SlewFind();
    ui->slew->setText("Find");
}

void MainWindow::on_max_clicked()
{
    m_indiClient->SlewMax();
    ui->slew->setText("Max");
}


void MainWindow::onHandboxComdReceived(HandBoxReceiver::COMMAND cmd)
{
    // Mount movements
    if(cmd == HandBoxReceiver::MoveNorth)
    {
        m_indiClient->MoveNorth();
    }
    if(cmd == HandBoxReceiver::MoveSouth)
    {
        m_indiClient->MoveSouth();
    }
    if(cmd == HandBoxReceiver::MoveEast)
    {
        m_indiClient->MoveEast();
    }
    if(cmd == HandBoxReceiver::MoveWest)
    {
        m_indiClient->MoveWest();
    }
    if(cmd == HandBoxReceiver::MoveStop)
    {
        m_indiClient->StopMove();
    }
    if(cmd == HandBoxReceiver::Focus1In)
    {
        m_indiClient->FocusIn();
    }
    if(cmd == HandBoxReceiver::Focus1Out)
    {
        m_indiClient->FocusOut();
    }
    if(cmd == HandBoxReceiver::FocusStop)
    {
        m_indiClient->FocusStop();
    }
    if(cmd == HandBoxReceiver::SlewGuide)
    {
        m_indiClient->SlewGuide();

        m_receiver->SetGuideLED(true);
        m_receiver->SetCenterLED(false);
        m_receiver->SetFindLED(false);

        ui->slew->setText("GUIDE");
    }
    if(cmd == HandBoxReceiver::SlewCenter)
    {
        m_indiClient->SlewCenter();

        m_receiver->SetGuideLED(false);
        m_receiver->SetCenterLED(true);
        m_receiver->SetFindLED(false);

        ui->slew->setText("CENTER");
    }
    if(cmd == HandBoxReceiver::SlewFind)
    {
        m_indiClient->SlewFind();

        m_receiver->SetGuideLED(false);
        m_receiver->SetCenterLED(false);
        m_receiver->SetFindLED(true);

        ui->slew->setText("FIND");
    }
    if(cmd == HandBoxReceiver::SlewMax)
    {
        m_indiClient->SlewMax();

        m_receiver->SetGuideLED(false);
        m_receiver->SetCenterLED(false);
        m_receiver->SetFindLED(false);

        ui->slew->setText("MAX");
    }
}

void MainWindow::onConnected()
{
    ui->handboxstate->setText("Connected");
}

void MainWindow::onDisconnected()
{
    ui->handboxstate->setText("Disconnected");
}

void MainWindow::onError()
{
    ui->handboxstate->setText("Error");
}

void MainWindow::on_handbox_returnPressed()
{
    m_receiver->SetHandboxIp(ui->handbox->text());
    QSettings settings("Stroblhofwarte", "TargetClient");
    settings.beginGroup("setup");
    settings.setValue("Handbox", ui->handbox->text());
    settings.endGroup();
}

void MainWindow::on_handboxReconnect_clicked()
{
    m_receiver->Reconnect();
}

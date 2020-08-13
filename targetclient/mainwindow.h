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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include "dialogprofile.h"
#include "indiclient.h"
#include "profile.h"
#include "handboxreceiver.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_profileEditor_clicked();
    void ReconnectionTimerSlot();
    void on_activeProfile_activated(const QString &arg1);
    void on_north_pressed();
    void on_north_released();
    void on_west_pressed();
    void on_west_released();
    void on_east_pressed();
    void on_east_released();
    void on_south_pressed();
    void on_south_released();
    void on_guide_clicked();
    void on_center_clicked();
    void on_find_clicked();
    void on_max_clicked();

    void onHandboxComdReceived(HandBoxReceiver::COMMAND cmd);
    void onConnected();
    void onDisconnected();
    void onError();

    void on_handbox_returnPressed();

    void on_handboxReconnect_clicked();

private:
    Ui::MainWindow *ui;
    IndiClient *m_indiClient;
    QTimer *m_reconnectionTimer;
    HandBoxReceiver *m_receiver;
    QString m_handbox;
};

#endif // MAINWINDOW_H

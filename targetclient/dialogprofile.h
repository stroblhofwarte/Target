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

#ifndef DIALOGPROFILE_H
#define DIALOGPROFILE_H

#include <QDialog>
#include "indiclient.h"

namespace Ui {
class DialogProfile;
}

class DialogProfile : public QDialog
{
    Q_OBJECT

public:
    explicit DialogProfile(QWidget *parent = 0);
    ~DialogProfile();

public slots:
    void newTelecope(QString name);
    void newFocuser(QString name);

private slots:
    void on_testAndFill_clicked();

    void on_pbNew_clicked();

    void on_server_editingFinished();

    void on_port_editingFinished();

    void on_mount_activated(const QString &arg1);

    void on_focuser_activated(const QString &arg1);

    void on_profileSelection_activated(const QString &arg1);

    void on_pbDel_clicked();

    void on_done_clicked();

private:
    bool SetProfile(QString name);
    void ClearForm();

    Ui::DialogProfile *ui;
    IndiClient *m_indiClient;
};

#endif // DIALOGPROFILE_H

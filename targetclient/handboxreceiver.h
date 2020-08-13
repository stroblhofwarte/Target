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

#ifndef HANDBOXRECEIVER_H
#define HANDBOXRECEIVER_H

#include <QObject>
#include <QTcpSocket>
#include <QNetworkDatagram>

typedef struct
{
    char id[24];
    unsigned char cmd;
} _KEY_DATA;

class HandBoxReceiver : public QObject
{
    Q_OBJECT
public:
    enum COMMAND
    {
         MoveNorth = 1,
         MoveSouth = 2,
         MoveEast = 3,
         MoveWest = 4,
         MoveStop = 5,
         Focus1In = 6,
         Focus1Out = 7,
         Focus2In  = 8,
         Focus2Out = 9,
         FocusStop = 10,
         SlewGuide = 11,
         SlewCenter = 12,
         SlewFind = 13,
         SlewMax = 14,
         Ping = 15,
         LEDGuideOn = 16,
         LEDGuideOff = 17,
         LEDCenterOn = 18,
         LEDCenterOff = 19,
         LEDFindOn = 20,
         LEDFindOff = 21
    };

    explicit HandBoxReceiver(QObject *parent = nullptr);
    void SetHandboxIp(QString ip);
    bool IsConnected();
    void Reconnect();
    void SetGuideLED(bool state);
    void SetCenterLED(bool state);
    void SetFindLED(bool state);

signals:
    void Command(COMMAND cmd);
    void NewHandbox(QString id);
    void Connected();
    void Disconnected();
    void Error();
public slots:
    void readPendingDatagrams();
protected slots:
    void TcpConnected();
    void TcpDisconnected();
    void TcpError(QTcpSocket::SocketError err);
private:
    QTcpSocket *m_socket;
    void ProcessData();
    QByteArray m_readBuffer;
    QString m_ip;
};

#endif // HANDBOXRECEIVER_H

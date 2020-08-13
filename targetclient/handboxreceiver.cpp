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

#include "handboxreceiver.h"

HandBoxReceiver::HandBoxReceiver(QObject *parent) : QObject(parent)
{
    m_socket = new QTcpSocket();
    m_socket->setSocketOption(QAbstractSocket::KeepAliveOption,1);
    connect(m_socket, &QTcpSocket::connected, this, &HandBoxReceiver::TcpConnected);
    connect(m_socket, &QTcpSocket::readyRead, this, &HandBoxReceiver::readPendingDatagrams);
    connect(m_socket, &QTcpSocket::disconnected, this, &HandBoxReceiver::TcpDisconnected);
    connect(m_socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &HandBoxReceiver::TcpError);
}

void HandBoxReceiver::readPendingDatagrams()
{
    m_readBuffer.append(m_socket->readAll());
    ProcessData();
}

void HandBoxReceiver::ProcessData()
{
    // First remove all ping results:
    m_readBuffer.replace((char)Ping, "");
    while(m_readBuffer.size() >= sizeof(_KEY_DATA))
    {
          QByteArray oneStruct = m_readBuffer.left(sizeof(_KEY_DATA));
          m_readBuffer.remove(0,sizeof(_KEY_DATA));
          _KEY_DATA *data = reinterpret_cast<_KEY_DATA*>(oneStruct.data());
          emit Command((COMMAND)data->cmd);
    }
}

void HandBoxReceiver::TcpConnected()
{
    emit Connected();
}

void HandBoxReceiver::TcpDisconnected()
{
    emit Disconnected();
}

void HandBoxReceiver::TcpError(QTcpSocket::SocketError err)
{
    emit Error();
}

void HandBoxReceiver::SetHandboxIp(QString ip)
{
    m_ip = ip;
    m_socket->reset();
    m_socket->connectToHost(ip, 5665);
}

bool HandBoxReceiver::IsConnected()
{
    const char data = (const char)Ping;
    m_socket->write(&data, 1);
    m_socket->flush();
    // Wait max. 500ms for an aswer
    if(m_socket->waitForReadyRead(500))
    {
        QByteArray result = m_socket->readAll();
        return true;
    }
    else
    {
        return false;
    }
}


void HandBoxReceiver::Reconnect()
{
    m_socket->reset();
    m_socket->connectToHost(m_ip, 5665);
}


void HandBoxReceiver::SetGuideLED(bool state)
{
    if(state)
    {
        const char data = COMMAND::LEDGuideOn;
        m_socket->write(&data, 1);
    }
    else
    {
        const char data = COMMAND::LEDGuideOff;
        m_socket->write(&data, 1);
    }
}

void HandBoxReceiver::SetCenterLED(bool state)
{
    if(state)
    {
        const char data = COMMAND::LEDCenterOn;
        m_socket->write(&data, 1);
    }
    else
    {
        const char data = COMMAND::LEDCenterOff;
        m_socket->write(&data, 1);
    }
}

void HandBoxReceiver::SetFindLED(bool state)
{
    if(state)
    {
        const char data = COMMAND::LEDFindOn;
        m_socket->write(&data, 1);
    }
    else
    {
        const char data = COMMAND::LEDFindOff;
        m_socket->write(&data, 1);
    }
}

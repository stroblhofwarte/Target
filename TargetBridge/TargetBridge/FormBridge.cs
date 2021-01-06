using ASCOM.DeviceInterface;
using ASCOM.DriverAccess;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace TargetBridge
{
    public partial class FormBridge : Form
    {
        #region Constants

        private readonly int _targetPort = 5665;
        private readonly Color _colorConnect = Color.FromArgb(118, 192, 118);
        private readonly Color _colorDisconnect = Color.FromArgb(162,109,109);
        private readonly Color _colorConnectionInit = Color.FromArgb(154, 146, 90);

        public enum enumConnectionState
        {
            idle,
            setup,
            connected,
            error
        }

        public enum enumGuideRate
        {
            guide,
            center,
            find,
            slew
        }

        enum enumCOMMAND
        {
            NoCommand = 0,
            MoveNorth = 1,
            MoveSouth = 2,
            MoveEast = 3,
            MoveWest = 4,
            MoveStop = 5,
            Focus1In = 6,
            Focus1Out = 7,
            Focus2In = 8,
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

        enum enumDirection
        {
            N,
            S,
            E,
            W
        }

        #endregion

        #region Properties

        private TcpClient _client;
        private bool _connected = false;
        private IPAddress _ipAddress;
        private IAsyncResult _connectionTask;
        private enumConnectionState _connectionState = enumConnectionState.idle;
        private bool _receiveThreadRunning = false;
        private byte[] _buffer;
        private int writeBufferSize = 25;
        private int readBufferSize = 25;
        private enumGuideRate _guiderate = enumGuideRate.slew;
        private int _receiveRetry = 0;
       

        private string _focuserId = "";
        private bool _focuserReady = false;
        private Focuser _focuser;
        private bool _focuserNotUseable = false;

        private string _telescopeId = "";
        private bool _telescopeReady = false;
        private Telescope _telescope;
        private double _guideRate;
        private double _centerRate;
        private double _findRate;
        private double _slewRate;
        private double _currentSpeedDeg_Sec;

        #endregion
        public FormBridge()
        {
            InitializeComponent();
            _connectionTask = null;
            _client = new TcpClient();
            _buffer = new byte[readBufferSize];
            textBoxServerAddress.Text = (string)Properties.Settings.Default["TargetIp"];
            _focuserId = (string)Properties.Settings.Default["FocuserId"];
            labelFocuser.Text = _focuserId;
            _focuserReady = ConnectFocuser(_focuserId);
            _telescopeId = (string)Properties.Settings.Default["TelescopeId"];
            labelTelescope.Text = _telescopeId;

            textBoxSetup1.Text = (string)Properties.Settings.Default["FocuserSetup1"];
            textBoxSetup2.Text = (string)Properties.Settings.Default["FocuserSetup2"];
            textBoxSetup3.Text = (string)Properties.Settings.Default["FocuserSetup3"];
            textBoxSetup4.Text = (string)Properties.Settings.Default["FocuserSetup4"];
        }

        private void buttonConnect_Click(object sender, EventArgs e)
        {
            ConnectToTarget(textBoxServerAddress.Text);
        }

        private bool ConnectFocuser(string id)
        {
            try
            {
                if (_focuserNotUseable) return false;
                _focuser = new Focuser(id);
                _focuser.Connected = true;
                if (!_focuser.Connected)
                {
                    labelFocuserPosition.Text = "not connected";
                    return false;
                }

                // Check if the focuser supports the "Halt" function:
                try
                {
                    _focuser.Halt();
                }
                catch (Exception ex)
                {
                    labelFocuserPosition.Text = "Focuser must support Halt!";
                    _focuser.Connected = false;
                    _focuserNotUseable = true;
                    return false;
                }

                _focuser.Link = true;
                labelFocuserPosition.Text = _focuser.Position.ToString();
                return true;
            } catch (Exception ex)
            {
                return false;
            }
        }

        private bool ProcessFocuser()
        {
            // Handle focuser
            if (!_focuserReady)
                _focuserReady = ConnectFocuser(_focuserId);
            if (_focuserReady)
            {
                labelFocuserPosition.Text = _focuser.Position.ToString();
            }
            return true;
        }

        private bool FocuserInOperation()
        {
            if (!_focuserReady) return false;
            if(_focuser.Absolute)
            {
                // This focuser is an absolute position device
                _focuser.Move(0);
            }
            else
            {
                _focuser.Move(-_focuser.MaxIncrement);
            }
            return true;
        }


        private bool FocuserOutOperation()
        {
            if (!_focuserReady) return false;
            if (_focuser.Absolute)
            {
                // This focuser is an absolute position device
                _focuser.Move(_focuser.MaxStep-1);
            }
            else
            {
                _focuser.Move(_focuser.MaxIncrement);
            }
            return true;
        }

        private bool FocuserHaltOperation()
        {
            if (!_focuserReady) return false;
            _focuser.Halt();
            return true;
        }

        #region ASCOM Telescope

        private bool ConnectTelescope(string id)
        {
            try
            {
                _telescope = new Telescope(id);
                _telescope.Connected = true;
                if (!_telescope.Connected)
                {

                    return false;
                }
                _guideRate = _telescope.GuideRateRightAscension;
                _centerRate = _guideRate * 4;
                _findRate = _guideRate * 64;
                _slewRate = 2.0;
                _currentSpeedDeg_Sec = _guideRate;
                _guiderate = enumGuideRate.guide;
                return true;
            } catch (Exception ex)
            {
                return false;
            }
        }

        private bool ProcessTelescope()
        {
            // Handle focuser
            if (!_telescopeReady)
                _telescopeReady = ConnectTelescope(_telescopeId);
            if(_telescopeReady)
            {
                labelRa.Text = _telescope.RightAscension.ToString();
                labelDec.Text = _telescope.Declination.ToString();
            }
            return true;
        }

        private bool MoveTelescope(enumDirection dir)
        {
            if(dir == enumDirection.N)
            {
                _telescope.MoveAxis(TelescopeAxes.axisSecondary, _currentSpeedDeg_Sec);
            }
            if (dir == enumDirection.S)
            {
                _telescope.MoveAxis(TelescopeAxes.axisSecondary, -_currentSpeedDeg_Sec);
            }
            if (dir == enumDirection.E)
            {
                _telescope.MoveAxis(TelescopeAxes.axisPrimary, -_currentSpeedDeg_Sec);
            }
            if (dir == enumDirection.W)
            {
                _telescope.MoveAxis(TelescopeAxes.axisPrimary, _currentSpeedDeg_Sec);
            }
            return true;
        }

        private bool StopTelescope()
        {
            _telescope.AbortSlew();
            return true;
        }

        #endregion

        private void ConnectToTarget(string address)
        {
            IPAddress localIpAddress;
            try
            {
                if (IPAddress.TryParse(address, out localIpAddress))
                {
                    if (localIpAddress != _ipAddress)
                    {
                        // Got a new valid IP address
                        _ipAddress = localIpAddress;
                        if (_client.Connected)
                        {
                            StopReceiveThread();
                            _client.Close();
                        }
                        _client = new TcpClient();
                        _connectionState = enumConnectionState.idle;
                        _connectionTask = _client.BeginConnect(_ipAddress, _targetPort, new AsyncCallback(ConnectCallback), _client);
                        _connectionState = enumConnectionState.setup;
                    }
                }
            }
            catch(Exception ex)
            {
                _connectionState = enumConnectionState.error;
            }
        }

        private void timerConnectionWatchdog_Tick(object sender, EventArgs e)
        {
            if(_connectionState == enumConnectionState.connected)
                SendCommand((int)enumCOMMAND.Ping);
            if(_connectionState == enumConnectionState.error || _connectionState == enumConnectionState.idle)
            {
                _ipAddress = null;
                ConnectToTarget(textBoxServerAddress.Text);
            }
            if (_connectionState == enumConnectionState.connected)
                buttonConnect.BackColor = _colorConnect;
            else if (_connectionState == enumConnectionState.idle)
                buttonConnect.BackColor = _colorDisconnect;
            else if (_connectionState == enumConnectionState.setup)
                buttonConnect.BackColor = _colorConnectionInit;
            else if (_connectionState == enumConnectionState.error)
                buttonConnect.BackColor = _colorDisconnect;

            ProcessFocuser();
            ProcessTelescope();
        }

        private void StartReceiveThread()
        {
            Thread t = new Thread(new ThreadStart(ListenForPackets));
            _receiveThreadRunning = true;
            t.Start();
        }

        private void StopReceiveThread()
        {
            _receiveThreadRunning = false;
        }

        private void HandleCommands(byte cmd)
        {
            if(cmd == (int)enumCOMMAND.SlewCenter)
            {
                _guiderate = enumGuideRate.center;
                _currentSpeedDeg_Sec = _centerRate;
            }
            if (cmd == (int)enumCOMMAND.SlewFind)
            {
                _guiderate = enumGuideRate.find;
                _currentSpeedDeg_Sec = _findRate;
            }
            if (cmd == (int)enumCOMMAND.SlewGuide)
            {
                _guiderate = enumGuideRate.guide;
                _currentSpeedDeg_Sec = _guideRate;
            }
            if (cmd == (int)enumCOMMAND.SlewMax)
            {
                _guiderate = enumGuideRate.slew;
                _currentSpeedDeg_Sec = _slewRate;
            }

            // Focuser
            if(cmd == (int)enumCOMMAND.Focus1In)
            {
                FocuserInOperation();
            }
            if (cmd == (int)enumCOMMAND.Focus1Out)
            {
                FocuserOutOperation();
            }
            if (cmd == (int)enumCOMMAND.FocusStop)
            {
                FocuserHaltOperation();
            }

            // Teleskop
            if(cmd == (int)enumCOMMAND.MoveNorth)
            {
                if(checkBoxFocuserOnly.Checked)
                {
                    FocuserInOperation();
                }
                else
                {
                    MoveTelescope(enumDirection.N);
                }
            }
            if (cmd == (int)enumCOMMAND.MoveSouth)
            {
                if (checkBoxFocuserOnly.Checked)
                {
                    FocuserOutOperation();
                }
                else
                {
                    MoveTelescope(enumDirection.S);
                }
            }
            if (cmd == (int)enumCOMMAND.MoveWest)
            {
                MoveTelescope(enumDirection.W);
            }
            if (cmd == (int)enumCOMMAND.MoveEast)
            {
                MoveTelescope(enumDirection.E);
            }

            if (cmd == (int)enumCOMMAND.MoveStop)
            {
                if (checkBoxFocuserOnly.Checked)
                {
                    FocuserHaltOperation();
                }
                else
                {
                    StopTelescope();
                }
            }
        }

        private void responseWithGuideRate(enumGuideRate rate)
        {
            if(rate == enumGuideRate.guide)
            {
                SendCommand((int)enumCOMMAND.LEDGuideOn);
                SendCommand((int)enumCOMMAND.LEDCenterOff);
                SendCommand((int)enumCOMMAND.LEDFindOff);
            }
            if (rate == enumGuideRate.center)
            {
                SendCommand((int)enumCOMMAND.LEDGuideOff);
                SendCommand((int)enumCOMMAND.LEDCenterOn);
                SendCommand((int)enumCOMMAND.LEDFindOff);
            }
            if (rate == enumGuideRate.find)
            {
                SendCommand((int)enumCOMMAND.LEDGuideOff);
                SendCommand((int)enumCOMMAND.LEDCenterOff);
                SendCommand((int)enumCOMMAND.LEDFindOn);
            }
            if (rate == enumGuideRate.slew)
            {
                SendCommand((int)enumCOMMAND.LEDGuideOff);
                SendCommand((int)enumCOMMAND.LEDCenterOff);
                SendCommand((int)enumCOMMAND.LEDFindOff);
            }
        }

        private byte[] PrepareCommand(int cmd)
        {
            byte[] buf = new byte[1];
            buf[0] = (byte)cmd;
            return buf;
        }

        private void SendCommand(int cmd)
        {
            _client.GetStream().Write(PrepareCommand(cmd), 0, 1);
            _client.GetStream().Flush();
        }
        private void ListenForPackets()
        {
            int bytesRead;

            while (_receiveThreadRunning)
            {
                bytesRead = 0;
                try

                {
                    //Blocks until a message is received from the server
                    bytesRead = _client.GetStream().Read(_buffer, 0, readBufferSize);
                    string result = System.Text.Encoding.UTF8.GetString(_buffer);
                }
                catch (Exception ex)
                {
                    //A socket error has occurred
                    //Console.WriteLine("A socket error has occurred with the client socket " + tcpClient.ToString());
                    _receiveRetry++;
                    if(_receiveRetry < 3)
                        continue;
                    _connectionState = enumConnectionState.error;
                    _receiveRetry = 0;
                    break;
                }

                _receiveRetry = 0;
                if (bytesRead == 0)
                {
                    //The server has disconnected
                    _connectionState = enumConnectionState.error;
                    break;
                }

                HandleCommands(_buffer[24]);
                _buffer[0] = 0;
                responseWithGuideRate(_guiderate);

                Thread.Sleep(15);
            }
        }

        private void ConnectCallback(IAsyncResult ar)
        {
            _connectionTask = null;
            TcpClient cl = ar.AsyncState as TcpClient;
            if(cl != null)
            {
                if (cl.Connected)
                {
                    _connectionState = enumConnectionState.connected;
                    _client.NoDelay = true;
                    _client.ReceiveTimeout = 3000;
                    SendCommand((int)enumCOMMAND.NoCommand);
                    StartReceiveThread();
                    return;
                }
            }
            _connectionState = enumConnectionState.error;
        }

        private void textBoxServerAddress_TextChanged(object sender, EventArgs e)
        {
            Properties.Settings.Default["TargetIp"] = textBoxServerAddress.Text;
            Properties.Settings.Default.Save();
        }

        private void FormBridge_FormClosing(object sender, FormClosingEventArgs e)
        {
            StopReceiveThread();
        }

        private void buttonChooseFocuser_Click(object sender, EventArgs e)
        {
            _focuserId = Focuser.Choose(_focuserId);
            Properties.Settings.Default["FocuserId"] = _focuserId;
            Properties.Settings.Default.Save();
            labelFocuser.Text = _focuserId;
            _focuserReady = ConnectFocuser(_focuserId);
            
        }

        private void buttonFocuserSetup_Click(object sender, EventArgs e)
        {
            if (_focuserReady)
                _focuser.SetupDialog();
            else
            {
                _focuser = new Focuser(_focuserId);
                _focuser.Connected = true;
                if (_focuser.Connected)
                {
                    _focuser.SetupDialog();
                }
                _focuser.Connected = false;
            }
        }

        private void buttonTelescopeChooser_Click(object sender, EventArgs e)
        {
            _telescopeId = Telescope.Choose(_telescopeId);
            Properties.Settings.Default["TelescopeId"] = _telescopeId;
            Properties.Settings.Default.Save();
            labelTelescope.Text = _telescopeId;
            _telescopeReady = ConnectTelescope(_telescopeId);
        }

        private void buttonTelescopeSetup_Click(object sender, EventArgs e)
        {
            if (_telescopeReady)
            {
                _telescope.Connected = false;
                _telescope.SetupDialog();
                _telescope.Connected = true;
            }
            else
            {
                _telescope = new Telescope(_telescopeId);
                _telescope.Connected = true;
                if (_telescope.Connected)
                {
                    _telescope.SetupDialog();
                }
                _telescope.Connected = false;
            }
        }

        private void textBoxSetup1_TextChanged(object sender, EventArgs e)
        {
            Properties.Settings.Default["FocuserSetup1"] = textBoxSetup1.Text;
            Properties.Settings.Default.Save();
        }

        private void textBoxSetup2_TextChanged(object sender, EventArgs e)
        {
            Properties.Settings.Default["FocuserSetup2"] = textBoxSetup2.Text;
            Properties.Settings.Default.Save();
        }

        private void textBoxSetup3_TextChanged(object sender, EventArgs e)
        {
            Properties.Settings.Default["FocuserSetup3"] = textBoxSetup3.Text;
            Properties.Settings.Default.Save();
        }

        private void textBoxSetup4_TextChanged(object sender, EventArgs e)
        {
            Properties.Settings.Default["FocuserSetup4"] = textBoxSetup4.Text;
            Properties.Settings.Default.Save();
        }

        private void textBoxPos1_TextChanged(object sender, EventArgs e)
        {
            Properties.Settings.Default["FocuserPosition1"] = textBoxPos1.Text;
            Properties.Settings.Default.Save();
        }

        private void textBoxPos2_TextChanged(object sender, EventArgs e)
        {
            Properties.Settings.Default["FocuserPosition2"] = textBoxPos2.Text;
            Properties.Settings.Default.Save();
        }

        private void textBoxPos3_TextChanged(object sender, EventArgs e)
        {
            Properties.Settings.Default["FocuserPosition3"] = textBoxPos3.Text;
            Properties.Settings.Default.Save();
        }

        private void textBoxPos4_TextChanged(object sender, EventArgs e)
        {
            Properties.Settings.Default["FocuserPosition4"] = textBoxPos4.Text;
            Properties.Settings.Default.Save();
        }

        private void buttonMove1_Click(object sender, EventArgs e)
        {
            if(_focuserReady)
            {
                if(_focuser.Absolute)
                {
                    DialogResult dialogResult = MessageBox.Show("Are you sure to move the focuser to Pos 1?", "Focuser Operation", MessageBoxButtons.YesNo);
                    if (dialogResult == DialogResult.Yes)  _focuser.Move(int.Parse(textBoxPos1.Text));
                }
            }
        }

        private void buttonMove2_Click(object sender, EventArgs e)
        {
            if (_focuserReady)
            {
                if (_focuser.Absolute)
                {
                    DialogResult dialogResult = MessageBox.Show("Are you sure to move the focuser to Pos 2?", "Focuser Operation", MessageBoxButtons.YesNo);
                    if (dialogResult == DialogResult.Yes) _focuser.Move(int.Parse(textBoxPos2.Text));
                }
            }
        }

        private void buttonMove3_Click(object sender, EventArgs e)
        {
            if (_focuserReady)
            {
                if (_focuser.Absolute)
                {
                    DialogResult dialogResult = MessageBox.Show("Are you sure to move the focuser to Pos 3?", "Focuser Operation", MessageBoxButtons.YesNo);
                    if (dialogResult == DialogResult.Yes) _focuser.Move(int.Parse(textBoxPos3.Text));
                }
            }
        }

        private void buttonMove4_Click(object sender, EventArgs e)
        {
            if (_focuserReady)
            {
                if (_focuser.Absolute)
                {
                    DialogResult dialogResult = MessageBox.Show("Are you sure to move the focuser to Pos 4?", "Focuser Operation", MessageBoxButtons.YesNo);
                    if (dialogResult == DialogResult.Yes) _focuser.Move(int.Parse(textBoxPos4.Text));
                }
            }
        }
    }
}

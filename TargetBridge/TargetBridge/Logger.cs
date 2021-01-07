using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TargetBridge
{
    public class Logger
    {
        #region Constants

        private readonly string _logfile = "./TargetBridge.log";

        #endregion

        #region Properties

        private StreamWriter _stream;
        private object _lock;

        #endregion

        public Logger()
        {
            _lock = new object();

            if (File.Exists(_logfile))
            {
                if (File.Exists(_logfile + ".1"))
                    File.Delete(_logfile + ".1");
                File.Move(_logfile, _logfile + ".1");
            }
            _stream = new StreamWriter(_logfile);
        }

        public void Log(string message)
        {
            lock (_lock)
            {
                _stream.WriteLine("{0} {1} :{2}", DateTime.Now.ToLongTimeString(), DateTime.Now.ToLongDateString(), message);
                _stream.Flush();
            }
        }

        public void Close()
        {
            _stream.Close();
        }
    }
}

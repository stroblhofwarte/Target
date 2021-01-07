/*
 *  Copyright 2021, Othmar Ehrhardt <othmar.ehrhardt@t-online.de>, 
 *                  https://astro.stroblhof-oberrohrbach.de
 * 
 *  This file is part of the TargetBridge project.
 *
 *  TargetBridge is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.

 *  TargetBridge is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with TargetBridge.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

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

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HttpServer
{
    interface ILogger
    {
        void LogHeader(string head, object value);
        void LogMessage(string msg, params object[] args);
        void CompleteLog();
        void ClearLog();
        void CompleteLog(object msg);
    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HttpServer
{
    class Logger : ILogger
    {
        private string CurrentLog = "";
        private readonly Func<object, string> Decoder;
        private readonly Action<string> Output;

        public Logger(Func<object, string> decoder = null, Action<string> output = null)
        {
            Decoder = decoder ?? new Func<object, string>((obj) => obj?.ToString());
            Output = output ?? new Action<string>((str) => Console.WriteLine(str));
        }

        public void LogHeader(string head, object value)
        {
            CurrentLog += $"{head}: {Decoder(value)}{Environment.NewLine}";
        }

        public void LogHeader(string head, string value, params object[] args)
        {
            var args_str = args?.Select((obj) => Decoder(obj)).ToArray();
            CurrentLog += $"{head}: {string.Format(value, args_str)}{Environment.NewLine}";
        }

        public void LogMessage(string msg, params object[] args)
        {
            var args_str = args?.Select((obj) => Decoder(obj)).ToArray();
            CurrentLog += string.Format(msg, args_str) + Environment.NewLine;
        }

        public void CompleteLog()
        {
            Console.WriteLine(CurrentLog);
            CurrentLog = "";
        }

        public void ClearLog()
        {
            CurrentLog = "";
        }

        public void CompleteLog(object msg)
        {
            Console.WriteLine(Decoder(msg));
        }
    }
}

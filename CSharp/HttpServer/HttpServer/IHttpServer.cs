using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HttpServer
{
    interface IHttpServer
    {
        Task RunAsync(string[] args);
        string AbsolutePath(string route);
    }
}

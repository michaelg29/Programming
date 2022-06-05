using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;

namespace HttpServer
{
    enum Version
    {
        V1,
        V2
    }

    class Program
    {
        public static async Task<bool> Shutdown(IHttpServer server, HttpListenerContext ctx)
        {
            if (ctx.Request.HttpMethod == "POST")
            {
                await HttpServerV2.WriteFile(ctx.Response, server.AbsolutePath("post_goodbye.html"));
            }
            else
            {
                await HttpServerV2.WriteFile(ctx.Response, server.AbsolutePath("goodbye.html"));
            }

            return false;
        }

        public static async Task Main(string[] args)
        {
            string hostDir = "view";
            string hostUrl = "http://localhost:8080/";
            Version version = Version.V2;

            for (int i = 0; i < args.Length; i++)
            {
                if (i < args.Length - 1)
                {
                    // valued argument
                    string value = args[i + 1];
                    if (args[i] == "-d")
                    {
                        hostDir = value;
                    }
                    else if (args[i] == "-u")
                    {
                        hostUrl = value;
                    }
                    else if (args[i] == "-v")
                    {
                        if (Enum.TryParse(value, out Version tryVersion))
                        {
                            version = tryVersion;
                        }
                    }
                }
                else
                {
                    // switch
                }
            }

            Console.WriteLine($"Running {version.ToString()}, listening on {hostUrl}, content from {hostDir}");

            IHttpServer server;
            ILogger logger = new Logger();
            switch (version)
            {
                case Version.V2:
                    server = new HttpServerV2(hostDir, hostUrl, logger);
                    ((HttpServerV2)server).RegisterRoute("/shutdown", Shutdown);
                    break;
                default:
                    server = new HttpServerV1(hostDir, hostUrl);
                    break;
            }

            await server.RunAsync(args);
        }
    }
}

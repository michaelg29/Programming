using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Http;
using System.Text;
using System.Threading.Tasks;

namespace HttpServer
{
    enum Version
    {
        V1,
        V2,
        V3
    }

    class Program
    {
        public static async Task<bool> Shutdown(HttpServerV2 server)
        {
            return false;
        }

        public static async Task<bool> Index(HttpServerV2 server)
        {
            await server.SendFileFormattedAsync("index.html", "random number", "");

            return true;
        }

        public static async Task<bool> ShutdownV3(HttpServerV3 server)
        {
            return false;
        }

        public static async Task<bool> ShutdownV3Get(HttpServerV3 server)
        {
            await server.SendFileFormattedAsync("index.html", "Cannot get shutdown", "");

            return true;
        }

        public static async Task<bool> IndexV3(HttpServerV3 server)
        {
            await server.SendFileFormattedAsync("index.html", server.Request.HttpMethod, "disabled");

            return true;
        }

        public static async Task<bool> IndexPatchGet(HttpServerV3 server)
        {
            Console.WriteLine("Index post get");

            await server.SendFileFormattedAsync("index.html", "PatchGet", "disabled");

            return true;
        }

        public static async Task<bool> IndexPostPut(HttpServerV3 server)
        {
            Console.WriteLine("Index post put");

            await server.SendFileFormattedAsync("index.html", "PostPut", "disabled");

            return true;
        }

        public static async Task Main(string[] args)
        {
            string hostDir = "view";
            string hostUrl = "http://+:8080/";

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
                }
            }

            Console.WriteLine($"Listening on {hostUrl}, content from {hostDir}");

            StaticFileServer server = new StaticFileServer(hostDir, hostUrl);
            
            await server.RunAsync(args);
        }

        //public static async Task Main(string[] args)
        //{
        //    string hostDir = "view";
        //    string hostUrl = "http://+:8080/";
        //    Version version = Version.V3;

        //    for (int i = 0; i < args.Length; i++)
        //    {
        //        if (i < args.Length - 1)
        //        {
        //            // valued argument
        //            string value = args[i + 1];
        //            if (args[i] == "-d")
        //            {
        //                hostDir = value;
        //            }
        //            else if (args[i] == "-u")
        //            {
        //                hostUrl = value;
        //            }
        //            else if (args[i] == "-v")
        //            {
        //                if (Enum.TryParse(value, out Version tryVersion))
        //                {
        //                    version = tryVersion;
        //                }
        //            }
        //        }
        //        else
        //        {
        //            // switch
        //        }
        //    }

        //    Console.WriteLine($"Running {version}, listening on {hostUrl}, content from {hostDir}");

        //    IHttpServer server;
        //    ILogger logger = new Logger();
        //    switch (version)
        //    {
        //        case Version.V3:
        //            server = new HttpServerV3(hostDir, hostUrl, logger);
        //            ((HttpServerV3)server).RegisterRoute("shutdown", ShutdownV3, HttpMethod.Post);
        //            ((HttpServerV3)server).RegisterRoute("shutdown", ShutdownV3Get);
        //            ((HttpServerV3)server).RegisterRoute("index", IndexV3);
        //            ((HttpServerV3)server).RegisterRoute("testPatchGet", IndexPatchGet, new List<HttpMethod> { new HttpMethod("Patch"), HttpMethod.Get });
        //            ((HttpServerV3)server).RegisterRoute("testPostPut", IndexPostPut, new List<HttpMethod> { HttpMethod.Post, HttpMethod.Put });
        //            break;
        //        case Version.V2:
        //            server = new HttpServerV2(hostDir, hostUrl, logger);
        //            ((HttpServerV2)server).RegisterRoute("/shutdown", Shutdown);
        //            ((HttpServerV2)server).RegisterRoute("/index", Index);
        //            break;
        //        default:
        //            server = new HttpServerV1(hostDir, hostUrl, logger);
        //            break;
        //    }

        //    await server.RunAsync(args);
        //}
    }
}

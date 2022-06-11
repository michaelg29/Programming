// Filename:  HttpServer.cs        
// Author:    Benjamin N. Summerton <define-private-public>        
// License:   Unlicense (http://unlicense.org/)

using System;
using System.IO;
using System.Text;
using System.Net;
using System.Threading.Tasks;
using System.Collections.Generic;
using System.Linq;

namespace HttpServer
{
    class HttpServerV1 : IHttpServer
    {
        private ILogger logger;
        private HttpListener listener;
        private string hostUrl;
        private int requestCount = 0;

        private string hostDir = "";
        public string GetHostDir => hostDir + (hostDir.EndsWith("/") ? "" : "/");
        public string AbsolutePath(string path) => $"{GetHostDir}{path}";

        private string errorPath = "error.html";
        private IDictionary<string, string> routes = new Dictionary<string, string>
        {
            { "/", "index.html" },
            { "/index", "index.html" }
        };

        public HttpServerV1(string hostDir, string hostUrl, ILogger logger)
        {
            this.hostDir = hostDir;
            this.hostUrl = hostUrl;

            this.logger = logger;
        }

        private string Path(string route)
        {
            if (routes.ContainsKey(route))
            {
                return $"{hostDir}/{routes[route]}";
            }

            return null;
        }

        private int FetchFile(string route, out string contents, out string format)
        {
            int code = 200;
            string path = Path(route);
            if (string.IsNullOrEmpty(path))
            {
                // determine if file exists
                if (File.Exists($"{hostDir}/{route}"))
                {
                    contents = File.ReadAllText($"{hostDir}/{route}");
                    format = "text";
                }
                else
                {
                    // return error file
                    contents = File.ReadAllText($"{hostDir}/{errorPath}");
                    code = 404;
                    format = "text/html";
                }
            }
            else
            {
                contents = File.ReadAllText(path);
                format = "text/html";
            }

            return code;
        }

        private async Task HandleIncomingConnections()
        {
            bool running = true;
            Random rnd = new Random(DateTime.Now.Ticks.GetHashCode());

            // While a user hasn't visited the `shutdown` url, keep on handling requests
            while (running)
            {
                // Will wait here until we hear from a connection
                HttpListenerContext ctx = await listener.GetContextAsync();
                requestCount++;

                // Print out some info about the request
                logger.LogMessage("Request {0}", requestCount);
                logger.LogHeader("Url", ctx.Request.Url);
                logger.LogHeader("Method", ctx.Request.HttpMethod);
                logger.LogHeader("Host", ctx.Request.UserHostName);
                logger.LogHeader("Agent", ctx.Request.UserAgent);
                logger.CompleteLog();

                // validate path
                string path = ctx.Request.Url.AbsolutePath;
                if (path.Contains(".."))
                {
                    path = "";
                }

                // If `shutdown` url requested w/ POST, then shutdown the server after serving the page
                if ((ctx.Request.HttpMethod == "POST") && (ctx.Request.Url.AbsolutePath == "/shutdown"))
                {
                    logger.CompleteLog("Shutdown requested");
                    running = false;
                }
                string disableSubmit = !running ? "disabled" : "";

                // get contents
                int code = FetchFile(path, out string pageData, out string contentType);
                try
                {
                    string formatted = string.Format(pageData, rnd.Next().ToString(), disableSubmit);
                    pageData = formatted;
                }
                catch
                {

                }
                byte[] data = Encoding.UTF8.GetBytes(pageData);

                // write response info
                ctx.Response.StatusCode = code;
                ctx.Response.ContentType = contentType;
                ctx.Response.ContentEncoding = Encoding.UTF8;
                ctx.Response.ContentLength64 = data.LongLength;

                // write to and close response
                await ctx.Response.OutputStream.WriteAsync(data, 0, data.Length);
                ctx.Response.Close();
            }
        }

        public async Task RunAsync(string[] args)
        {
            // Create a Http server and start listening for incoming connections
            listener = new HttpListener();
            listener.Prefixes.Add(hostUrl);
            listener.Start();
            logger.CompleteLog($"Listening on {hostUrl}");

            // Handle requests
            await HandleIncomingConnections();

            // Close the listener
            listener.Close();
        }
    }
}
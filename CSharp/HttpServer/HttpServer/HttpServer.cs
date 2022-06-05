// Filename:  HttpServer.cs        
// Author:    Benjamin N. Summerton <define-private-public>        
// License:   Unlicense (http://unlicense.org/)

using System;
using System.IO;
using System.Text;
using System.Net;
using System.Threading.Tasks;
using System.Collections.Generic;

namespace HttpServer
{
    class HttpServer
    {
        public static ILogger logger = new Logger();
        public static HttpListener listener;
        public static string url = "http://localhost:8000/";
        public static int pageViews = 0;
        public static int requestCount = 0;

        private static string dir = "view";
        private static string errorPath = "error.html";
        private static IDictionary<string, string> routes = new Dictionary<string, string>
        {
            { "/", "index.html" },
            { "/index", "index.html" }
        };

        public static string Path(string route)
        {
            if (routes.ContainsKey(route))
            {
                return $"{dir}/{routes[route]}";
            }

            return null;
        }

        public static int FetchFile(string route, out string contents, params string[] formatters)
        {
            int code = 200;
            string path = Path(route);
            if (string.IsNullOrEmpty(path))
            {
                // determine if file exists
                if (File.Exists($"{dir}/{route}"))
                {
                    contents = File.ReadAllText($"{dir}/{route}");
                    contents = string.Format(contents, formatters);
                }
                else
                {
                    // return error file
                    contents = File.ReadAllText($"{dir}/{errorPath}");
                    code = 404;
                }
            }
            else
            {
                contents = File.ReadAllText(path);
                contents = string.Format(contents, formatters);
            }

            return code;
        }

        public static async Task HandleIncomingConnections()
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

                // If `shutdown` url requested w/ POST, then shutdown the server after serving the page
                if ((ctx.Request.HttpMethod == "POST") && (ctx.Request.Url.AbsolutePath == "/shutdown"))
                {
                    logger.CompleteLog("Shutdown requested");
                    running = false;
                }
                string disableSubmit = !running ? "disabled" : "";

                // Make sure we don't increment the page views counter if `favicon.ico` is requested
                if (ctx.Request.Url.AbsolutePath != "/favicon.ico")
                    pageViews += 1;

                // get contents
                int code = FetchFile(ctx.Request.Url.AbsolutePath, out string pageData, rnd.Next().ToString(), disableSubmit);
                byte[] data = Encoding.UTF8.GetBytes(pageData);

                // write response info
                ctx.Response.StatusCode = code;
                ctx.Response.ContentType = "text/html";
                ctx.Response.ContentEncoding = Encoding.UTF8;
                ctx.Response.ContentLength64 = data.LongLength;

                // write to and close response
                await ctx.Response.OutputStream.WriteAsync(data, 0, data.Length);
                ctx.Response.Close();
            }
        }

        public static async Task Main(string[] args)
        {
            // Create a Http server and start listening for incoming connections
            listener = new HttpListener();
            listener.Prefixes.Add(url);
            listener.Start();
            Console.WriteLine("Listening on {0}", url);

            // Handle requests
            await HandleIncomingConnections();

            // Close the listener
            listener.Close();
        }
    }
}
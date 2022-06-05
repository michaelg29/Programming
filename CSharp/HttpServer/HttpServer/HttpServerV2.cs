using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;

namespace HttpServer
{
    class HttpServerV2 : IHttpServer
    {
        private ILogger logger;
        private HttpListener listener;

        private string hostUrl;
        private string hostDir;

        public string GetHostDir => hostDir + (hostDir.EndsWith("/") ? "" : "/");
        public string AbsolutePath(string path) => $"{GetHostDir}{path}";

        private string errorPath = "error.html";

        private IDictionary<string, Func<IHttpServer, HttpListenerContext, Task<bool>>> routes;

        public HttpServerV2(string hostDir, string hostUrl, ILogger logger)
        {
            this.hostDir = hostDir;
            this.hostUrl = hostUrl;
            this.routes = new Dictionary<string, Func<IHttpServer, HttpListenerContext, Task<bool>>>();

            this.logger = logger;
        }

        public void RegisterRoute(string route, Func<IHttpServer, HttpListenerContext, Task<bool>> action)
        {
            routes[route] = action;
        }

        private async Task<bool> ProcessRoute(HttpListenerContext ctx)
        {
            // try find route in dictionary
            string route = ctx.Request.Url.AbsolutePath;
            if (routes.ContainsKey(route))
            {
                return await routes[route](this, ctx);
            }

            // look for physical file
            if (File.Exists($"{GetHostDir}{route}"))
            {
                string contentType = GetMimeType(route);

                // write file
                await WriteFile(ctx.Response, AbsolutePath(route), HttpStatusCode.OK, contentType);
                return true;
            }

            // return error
            await WriteFile(ctx.Response, AbsolutePath(errorPath), HttpStatusCode.NotFound);
            return true;
        }

        public async Task RunAsync(string[] args)
        {
            // Create a Http server and start listening for incoming connections
            listener = new HttpListener();
            listener.Prefixes.Add(hostUrl);
            listener.Start();
            logger.CompleteLog($"Listening on {hostUrl}");

            // Handle requests
            bool running = true;
            while (running)
            {
                HttpListenerContext ctx = await listener.GetContextAsync();

                // log the request
                logger.LogMessage("New Request ========");
                logger.LogHeader("Url", ctx.Request.Url);
                logger.LogHeader("Method", ctx.Request.HttpMethod);
                logger.LogHeader("Host", ctx.Request.UserHostName);
                logger.LogHeader("Agent", ctx.Request.UserAgent);
                logger.CompleteLog();

                running = await ProcessRoute(ctx);

                // log the response
                logger.LogMessage("Response ========");
                logger.LogHeader("Code", ctx.Response.StatusCode);
                logger.LogHeader("Type", ctx.Response.ContentType);
                logger.LogHeader("Length", ctx.Response.ContentLength64);
                logger.CompleteLog();

                ctx.Response.Close();
            }

            // Close the listener
            listener.Close();
        }

        public static async Task WriteString(HttpListenerResponse response, string content, HttpStatusCode statusCode = HttpStatusCode.OK, string contentType = "text/html")
        {
            await WriteContent(response, Encoding.UTF8.GetBytes(content), statusCode, contentType);
        }

        public static async Task WriteFile(HttpListenerResponse response, string filePath, HttpStatusCode statusCode = HttpStatusCode.OK, string contentType = "text/html")
        {
            await WriteContent(response, File.ReadAllBytes(filePath), statusCode, contentType);
        }

        public static async Task WriteContent(HttpListenerResponse response, byte[] data, HttpStatusCode statusCode = HttpStatusCode.OK, string contentType = "text/html")
        {
            response.StatusCode = (int)statusCode;
            response.ContentType = contentType;
            response.ContentEncoding = Encoding.UTF8;
            response.ContentLength64 = data.LongLength;

            await response.OutputStream.WriteAsync(data, 0, data.Length);
        }

        public static string GetMimeType(string route)
        {
            // find extension
            string ext = null;
            int lastDot = route.LastIndexOf('.');
            int nextSlash = route.IndexOf('/', lastDot);
            if (nextSlash == -1 && lastDot != -1)
            {
                nextSlash = route.Length;
                ext = route.Substring(lastDot + 1);
            }
            else if (lastDot != -1)
            {
                ext = route.Substring(lastDot + 1, nextSlash - lastDot);
            }

            switch (ext)
            {
                // audio
                case "mp3":
                    return "audio/mpeg";

                // image
                case "jpg":
                case "jpeg":
                    return "image/jpeg";
                case "png":
                    return "image/png";
                case "tiff":
                    return "image/tiff";
                case "ico":
                    return "image/x-icon";

                // text
                case "htm":
                case "html":
                    return "text/html";
                case "css":
                    return "text/css";
                case "txt":
                    return "text/plain";
                case "json":
                    return "application/json";
                case "xml":
                    return "application/xml";

                // video
                case "mp4":
                    return "video/mp4";
                case "mpeg":
                    return "video/mpeg";
                case "webm":
                    return "video/webm";

                default:
                    return "text";
            }
        }
    }
}

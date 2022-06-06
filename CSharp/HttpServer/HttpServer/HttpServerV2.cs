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

        public string AbsolutePath(string path)
        {
            return path.StartsWith("/") ? $"{hostDir}{path}" : $"{hostDir}/{path}";
        }

        private string notFoundPath = "notFound.html";
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
            if (File.Exists($"{hostDir}{route}"))
            {
                string contentType = GetMimeType(route);

                // write file
                try
                {
                    await WriteFile(ctx.Response, AbsolutePath(route), HttpStatusCode.OK, contentType);
                    return true;
                }
                catch (HttpListenerException e)
                {
                    // return error
                    await WriteFile(ctx.Response, AbsolutePath(errorPath), HttpStatusCode.InternalServerError);
                    logger.CompleteLog($"HTTP exception: {e.Message}");
                    return true;
                }
                catch (Exception e)
                {
                    // return error
                    await WriteFile(ctx.Response, AbsolutePath(errorPath), HttpStatusCode.InternalServerError);
                    logger.CompleteLog($"Generic exception: {e.Message}");
                    return true;
                }
            }

            // return not found
            await WriteFile(ctx.Response, AbsolutePath(notFoundPath), HttpStatusCode.NotFound);
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
            response.OutputStream.Flush();

            Stream input = new FileStream(filePath, FileMode.Open);

            //Adding permanent http response headers
            response.ContentType = contentType;
            response.ContentLength64 = input.Length;
            response.AddHeader("Date", DateTime.Now.ToString("r"));

            byte[] buffer = new byte[1024 * 16];
            int nbytes;
            while ((nbytes = input.Read(buffer, 0, buffer.Length)) > 0)
                response.OutputStream.Write(buffer, 0, nbytes);
            input.Close();

            response.StatusCode = (int)statusCode;
            response.OutputStream.Flush();
        }

        public static async Task WriteContent(HttpListenerResponse response, byte[] data, HttpStatusCode statusCode = HttpStatusCode.OK, string contentType = "text/html")
        {
            response.StatusCode = (int)statusCode;
            response.ContentType = contentType;
            response.ContentEncoding = Encoding.UTF8;
            response.ContentLength64 = data.LongLength;

            await response.OutputStream.WriteAsync(data, 0, data.Length);
        }

        private static readonly IDictionary<string, string> mimeTypes = new Dictionary<string, string> 
        {
            { ".asf", "video/x-ms-asf" },
            { ".asx", "video/x-ms-asf" },
            { ".avi", "video/x-msvideo" },
            { ".bin", "application/octet-stream" },
            { ".cco", "application/x-cocoa" },
            { ".crt", "application/x-x509-ca-cert" },
            { ".css", "text/css" },
            { ".deb", "application/octet-stream" },
            { ".der", "application/x-x509-ca-cert" },
            { ".dll", "application/octet-stream" },
            { ".dmg", "application/octet-stream" },
            { ".ear", "application/java-archive" },
            { ".eot", "application/octet-stream" },
            { ".exe", "application/octet-stream" },
            { ".flv", "video/x-flv" },
            { ".gif", "image/gif" },
            { ".hqx", "application/mac-binhex40" },
            { ".htc", "text/x-component" },
            { ".htm", "text/html" },
            { ".html", "text/html" },
            { ".ico", "image/x-icon" },
            { ".img", "application/octet-stream" },
            { ".iso", "application/octet-stream" },
            { ".jar", "application/java-archive" },
            { ".jardiff", "application/x-java-archive-diff" },
            { ".jng", "image/x-jng" },
            { ".jnlp", "application/x-java-jnlp-file" },
            { ".jpeg", "image/jpeg" },
            { ".jpg", "image/jpeg" },
            { ".js", "application/x-javascript" },
            { ".mml", "text/mathml" },
            { ".mng", "video/x-mng" },
            { ".mov", "video/quicktime" },
            { ".mp3", "audio/mpeg" },
            { ".mpeg", "video/mpeg" },
            { ".mpg", "video/mpeg" },
            { ".msi", "application/octet-stream" },
            { ".msm", "application/octet-stream" },
            { ".msp", "application/octet-stream" },
            { ".pdb", "application/x-pilot" },
            { ".pdf", "application/pdf" },
            { ".pem", "application/x-x509-ca-cert" },
            { ".pl", "application/x-perl" },
            { ".pm", "application/x-perl" },
            { ".png", "image/png" },
            { ".prc", "application/x-pilot" },
            { ".ra", "audio/x-realaudio" },
            { ".rar", "application/x-rar-compressed" },
            { ".rpm", "application/x-redhat-package-manager" },
            { ".rss", "text/xml" },
            { ".run", "application/x-makeself" },
            { ".sea", "application/x-sea" },
            { ".shtml", "text/html" },
            { ".sit", "application/x-stuffit" },
            { ".swf", "application/x-shockwave-flash" },
            { ".tcl", "application/x-tcl" },
            { ".tk", "application/x-tcl" },
            { ".txt", "text/plain" },
            { ".war", "application/java-archive" },
            { ".wbmp", "image/vnd.wap.wbmp" },
            { ".wmv", "video/x-ms-wmv" },
            { ".xml", "text/xml" },
            { ".xpi", "application/x-xpinstall" },
            { ".zip", "application/zip" }
        };

        public static string GetMimeType(string route)
        {
            // find extension
            string ext = null;
            int lastDot = route.LastIndexOf('.');
            int nextSlash = route.IndexOf('/', lastDot);
            if (nextSlash == -1 && lastDot != -1)
            {
                nextSlash = route.Length;
                ext = route.Substring(lastDot);
            }
            else if (lastDot != -1)
            {
                ext = route.Substring(lastDot, nextSlash - lastDot);
            }

            if (mimeTypes.ContainsKey(ext))
            {
                return mimeTypes[ext];
            }
            return "text";
        }
    }
}

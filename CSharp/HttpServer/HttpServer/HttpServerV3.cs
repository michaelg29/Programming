using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.Http;
using System.Text;
using System.Threading.Tasks;

namespace HttpServer
{
    class HttpServerV3 : IHttpServer
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

        private bool running = false;

        private IDictionary<string, Func<HttpServerV3, Task<bool>>> routes;

        private HttpListenerContext ctx;

        public HttpListenerRequest Request
        {
            get => ctx.Request;
        }

        public HttpStatusCode ResponseCode
        {
            set
            {
                if (ctx != null)
                {
                    ctx.Response.StatusCode = (int)value;
                }
            }
        }

        public long ContentLength
        {
            set
            {
                if (ctx != null)
                {
                    ctx.Response.ContentLength64 = value;
                }
            }
        }

        public string ContentType
        {
            set
            {
                if (ctx != null)
                {
                    ctx.Response.ContentType = value;
                }
            }
        }

        public string FileExt
        {
            set
            {
                if (ctx != null)
                {
                    ContentType = GetMimeType(value);
                }
            }
        }

        public HttpServerV3(string hostDir, string hostUrl, ILogger logger)
        {
            this.hostDir = hostDir;
            this.hostUrl = hostUrl;
            this.routes = new Dictionary<string, Func<HttpServerV3, Task<bool>>>();

            this.logger = logger;
        }

        public void RegisterRoute(string route, Func<HttpServerV3, Task<bool>> action)
        {
            RegisterRoute(route, action, new List<HttpMethod>());
        }

        public void RegisterRoute(string route, Func<HttpServerV3, Task<bool>> action, HttpMethod method)
        {
            RegisterRoute(route, action, new List<HttpMethod> { method });
        }

        public void RegisterRoute(string route, Func<HttpServerV3, Task<bool>> action, IEnumerable<HttpMethod> methods)
        {
            if (!route.StartsWith("/"))
            {
                route = "/" + route;
            }

            if (methods != null && methods.Count() > 0)
            {
                foreach (HttpMethod method in methods)
                {
                    routes[$"{method.Method} {route}"] = action;
                }
            }
            else
            {
                routes[route] = action;
            }
        }

        private bool GetAction(string route, out Func<HttpServerV3, Task<bool>> action, string method = null)
        {
            if (!route.StartsWith("/"))
            {
                route = "/" + route;
            }

            if (method != null && routes.ContainsKey($"{method} {route}"))
            {
                action = routes[$"{method} {route}"];
                return true;
            }
            else if (routes.ContainsKey(route))
            {
                action = routes[route];
                return true;
            }

            action = null;
            return false;
        }

        private async Task<bool> ProcessRoute(HttpListenerContext ctx)
        {
            // try find route in dictionary
            string route = ctx.Request.Url.AbsolutePath;
            if (GetAction(route, out var action, ctx.Request.HttpMethod))
            {
                return await action(this);
            }

            // look for physical file
            if (File.Exists(AbsolutePath(route)))
            {
                // write file
                try
                {
                    this.ResponseCode = HttpStatusCode.OK;
                    await SendFileAsync(AbsolutePath(route));
                    return true;
                }
                catch (HttpListenerException e)
                {
                    // return error
                    this.ResponseCode = HttpStatusCode.InternalServerError;
                    logger.CompleteLog($"HTTP exception: {e.Message}");
                    return true;
                }
                catch (Exception e)
                {
                    // return error
                    this.ResponseCode = HttpStatusCode.InternalServerError;
                    //await SendFileAsync(AbsolutePath(errorPath));
                    logger.CompleteLog($"Generic exception: {e.Message}");
                    return true;
                }
            }

            // return not found
            this.ResponseCode = HttpStatusCode.NotFound;
            await SendFileAsync(AbsolutePath(notFoundPath));
            return true;
        }

        private async void ProcessContextAsync(IAsyncResult result)
        {
            ctx = this.listener.EndGetContext(result);

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
            ctx = null;

            if (running)
            {
                // listen for next request
                this.listener.BeginGetContext(new AsyncCallback(ProcessContextAsync), this.listener);
            }
            else
            {
                logger.CompleteLog("Shutdown requested, press <ENTER> to continue...");
            }
        }

        public async Task RunAsync(string[] args)
        {
            // Create a Http server and start listening for incoming connections
            listener = new HttpListener();
            listener.Prefixes.Add(hostUrl);
            listener.Start();
            logger.CompleteLog($"Listening on {hostUrl}");

            IAsyncResult ar = listener.BeginGetContext(new AsyncCallback(ProcessContextAsync), this.listener);

            running = true;

            // Handle console input
            while (running)
            {
                string cmd = Console.ReadLine();

                if (!running)
                {
                    break;
                }

                switch (cmd)
                {
                    case "quit":
                        running = false;
                        break;
                    default:
                        logger.CompleteLog("Invalid command");
                        break;
                }
            }

            listener.Abort();
            listener.Close();
        }

        public async Task SendFileAsync(string filePath)
        {
            // open stream
            Stream input = new FileStream(filePath, FileMode.Open);

            try
            {
                // set header values
                this.FileExt = filePath;
                this.ContentLength = input.Length;
                ctx.Response.ContentEncoding = Encoding.UTF8;
                //ctx.Response.SendChunked = true;

                // read in blocks
                byte[] buffer = new byte[1024 * 64];
                int nbytes;
                while ((nbytes = input.Read(buffer, 0, buffer.Length)) > 0)
                {
                    ctx.Response.OutputStream.Write(buffer, 0, nbytes);
                    ctx.Response.OutputStream.Flush();
                }
                input.Close();
            }
            catch
            {
                input.Close();
                throw;
            }

            // flush
            ctx.Response.OutputStream.Flush();
        }

        public async Task SendFileFormattedAsync(string filePath, params string[] args)
        {
            ctx.Response.OutputStream.Flush();

            string content = File.ReadAllText(AbsolutePath(filePath));
            try
            {
                string formatted = string.Format(content, args);
                content = formatted;
            }
            catch (Exception e)
            {
                logger.CompleteLog($"Could not format file: {e.Message}");
            }

            this.FileExt = filePath;
            this.ContentLength = content.Length;
            ctx.Response.ContentEncoding = Encoding.UTF8;

            byte[] buffer = Encoding.UTF8.GetBytes(content);
            await ctx.Response.OutputStream.WriteAsync(buffer, 0, content.Length);

            // flush
            ctx.Response.OutputStream.Flush();
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
            { ".wasm", "application/wasm" },
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
            return "application/octet-stream";
        }
    }
}

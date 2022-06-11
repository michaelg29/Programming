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
    class StaticFileServer
    {
        private HttpListener listener;

        private string hostUrl;
        private string hostDir;

        public string AbsolutePath(string path)
        {
            return path.StartsWith("/") ? $"{hostDir}{path}" : $"{hostDir}/{path}";
        }

        private string notFoundPath = "notFound.html";
        private string errorPath = "error.html";
        private string dirPath = "directory.html";

        private bool running = false;

        public StaticFileServer(string hostDir, string hostUrl)
        {
            this.hostDir = hostDir;
            this.hostUrl = hostUrl;
        }

        private async Task<bool> ProcessRequest(HttpListenerContext ctx)
        {
            string route = ctx.Request.Url.AbsolutePath;
            if (route == "/shutdown")
            {
                return false;
            }

            string absoluteRoute = AbsolutePath(route);

            // look for physical file
            if (File.Exists(absoluteRoute))
            {
                // write file
                try
                {
                    ctx.Response.StatusCode = (int)HttpStatusCode.OK;
                    await SendFileAsync(ctx.Response, absoluteRoute);
                    return true;
                }
                catch (Exception e)
                {
                    // return error
                    ctx.Response.StatusCode = (int)HttpStatusCode.InternalServerError;
                    Console.WriteLine($"{e.GetType()}: {e.Message}");
                    return true;
                }
            }

            // look for directory
            if (Directory.Exists(absoluteRoute))
            {
                // generate list
                string directoryList = "";
                string fileList = "";

                if (!route.EndsWith("/"))
                {
                    route += "/";
                }

                directoryList = string.Join("\n", Directory.GetDirectories(absoluteRoute)
                    .Select(s => {
                        int idx = Math.Max(s.LastIndexOf('/'), s.LastIndexOf('\\'));
                        string relPath = s.Substring(idx + 1);
                        return $"\t\t<ul><a href=\"{route}{relPath}\">{relPath}</a></ul>";
                    }));

                fileList = string.Join("\n", Directory.GetFiles(absoluteRoute)
                    .Select(s => {
                        int idx = Math.Max(s.LastIndexOf('/'), s.LastIndexOf('\\'));
                        string relPath = s.Substring(idx + 1);
                        return $"\t\t<ul><a href=\"{route}{relPath}\">{relPath}</a></ul>";
                    }));

                // write file
                try
                {
                    ctx.Response.StatusCode = (int)HttpStatusCode.OK;
                    await SendFileFormattedAsync(ctx.Response, dirPath, route, directoryList, fileList);
                    return true;
                }
                catch (Exception e)
                {
                    // return error
                    ctx.Response.StatusCode = (int)HttpStatusCode.InternalServerError;
                    Console.WriteLine($"{e.GetType()}: {e.Message}");
                    return true;
                }
            }

            // return not found
            ctx.Response.StatusCode = (int)HttpStatusCode.NotFound;
            await SendFileAsync(ctx.Response, AbsolutePath(notFoundPath));
            return true;
        }

        public async Task RunAsync(string[] args)
        {
            // Create a Http server and start listening for incoming connections
            listener = new HttpListener();
            listener.Prefixes.Add(hostUrl);
            listener.Start();
            Console.WriteLine($"Listening on {hostUrl}");

            running = true;

            // Handle console input
            while (running)
            {
                HttpListenerContext ctx = await listener.GetContextAsync();

                // log the request
                Console.WriteLine("New Request ========");
                Console.WriteLine("Url: {0}", ctx.Request.Url);
                Console.WriteLine("Method: {0}", ctx.Request.HttpMethod);
                Console.WriteLine("Host: {0}", ctx.Request.UserHostName);
                Console.WriteLine("Agent: {0}", ctx.Request.UserAgent);

                running = await ProcessRequest(ctx);

                // log the response
                Console.WriteLine("Response ========");
                Console.WriteLine("Code: {0}", ctx.Response.StatusCode);
                Console.WriteLine("Type: {0}", ctx.Response.ContentType);
                Console.WriteLine("Length: {0}", ctx.Response.ContentLength64);

                ctx.Response.Close();
            }

            listener.Close();
        }

        public async Task SendFileAsync(HttpListenerResponse response, string filePath)
        {
            // open stream
            Stream input = new FileStream(filePath, FileMode.Open);

            try
            {
                // set header values
                response.ContentType = GetMimeType(filePath);
                response.ContentLength64 = input.Length;
                response.ContentEncoding = Encoding.UTF8;

                // read in blocks
                byte[] buffer = new byte[1024 * 64];
                int nbytes;
                while ((nbytes = input.Read(buffer, 0, buffer.Length)) > 0)
                {
                    response.OutputStream.Write(buffer, 0, nbytes);
                    response.OutputStream.Flush();
                }
                input.Close();
            }
            catch
            {
                input.Close();
                throw;
            }

            // flush
            response.OutputStream.Flush();
        }

        public async Task SendFileFormattedAsync(HttpListenerResponse response, string filePath, params string[] args)
        {
            response.OutputStream.Flush();

            string content = File.ReadAllText(AbsolutePath(filePath));
            try
            {
                string formatted = string.Format(content, args);
                content = formatted;
            }
            catch (Exception e)
            {
                Console.WriteLine($"Could not format file: {e.Message}");
            }

            // set header values
            response.ContentType = GetMimeType(filePath);
            response.ContentLength64 = content.Length;
            response.ContentEncoding = Encoding.UTF8;

            byte[] buffer = Encoding.UTF8.GetBytes(content);
            await response.OutputStream.WriteAsync(buffer, 0, content.Length);

            // flush
            response.OutputStream.Flush();
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
            string ext = Path.GetExtension(route);

            if (mimeTypes.ContainsKey(ext))
            {
                return mimeTypes[ext];
            }
            return "application/octet-stream";
        }
    }
}
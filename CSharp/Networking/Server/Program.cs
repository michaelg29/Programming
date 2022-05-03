using System;
using System.Collections.Generic;
using System.Threading.Tasks;
using System.Net;
using System.Net.Sockets;
using System.Text;

class Client
{
    Socket socket { get; set; }
    string name { get; set; }

    public Client(Socket socket)
    {
        this.socket = socket;
    }
}

class Server
{
    string ip { get; }
    int port { get; }
    public Boolean isRunning { get; private set; }

    public delegate void Message(string msg);
    public Message msgHandler;

    IPHostEntry host;
    IPAddress ipAddress;
    IPEndPoint localEndPoint;
    Socket listener;
    List<Client> clients;
    List<Task> tasks;

    CancellationTokenSource cts = new CancellationTokenSource();

    public Server(string ip, int port, Message msgHandler)
    {
        this.ip = ip;
        this.port = port;
        this.isRunning = false;
        this.clients = new List<Client>();
        this.msgHandler = msgHandler;
    }

    public void Init()
    {
        this.host = Dns.GetHostEntry(this.ip);
        this.ipAddress = host.AddressList[0];
        this.localEndPoint = new IPEndPoint(ipAddress, this.port);
    }

    public string SendCommand(string cmd)
    {
        if (cmd.Equals("/quit"))
        {
            Cleanup();
            return "Server stopped";
        }

        return "Invalid Command";
    }

    private async void AcceptClient(IAsyncResult ar)
    {
        
    }

    public async Task Run()
    {
        tasks = new List<Task>();
        try
        {
            listener = new Socket(ipAddress.AddressFamily, SocketType.Stream, ProtocolType.Tcp);
            listener.Bind(localEndPoint);
            listener.Listen(10);
            this.isRunning = true;

            this.msgHandler($"Waiting on {ip}:{port}");

            // add listener task
            tasks.Add(listener.AcceptAsync());

            while (tasks.Any())
            {
                Task finishedTask = await Task.WhenAny(tasks);
                tasks.Remove(finishedTask);

                if (finishedTask.GetType().GetGenericArguments()[0].Equals())
                {
                    // receive task
                }
            }

            Socket handler = await listener.AcceptAsync();
            Console.WriteLine("Accepted");

            string data = null;
            byte[] bytes = null;

            while (true)
            {
                bytes = new byte[1024];
                int bytesRec = await handler.ReceiveAsync(bytes, SocketFlags.None);
                data += Encoding.ASCII.GetString(bytes, 0, Math.Min(bytesRec, 1024));
                if (bytesRec == 0 || data.IndexOf("<EOF>") > -1)
                {
                    break;
                }

                handler.Send(bytes, bytesRec, SocketFlags.None);
            }

            Console.WriteLine($"Received: {data}");

            handler.Shutdown(SocketShutdown.Both);
            handler.Close();
        }
        catch (Exception e)
        {
            throw;
        }
    }

    public void Cleanup()
    {
        cts.Cancel();
        foreach (Task t in tasks)
        {
            t.Dispose();
        }
        listener.Close();

    }
}

class Program
{
    public static void MsgHandler(string msg)
    {
        Console.WriteLine(msg);
    }

    public static async Task Main(string[] args)
    {
        Console.WriteLine("Hello, world!");

        try
        {
            Server s = new Server("localhost", 5500, MsgHandler);
            s.Init();
            s.Run();

            while (s.isRunning)
            {
                string cmd = Console.ReadLine();
                string res = s.SendCommand(cmd);
                Console.WriteLine(res);
            }
        }
        catch (Exception e)
        {
            Console.WriteLine(e.ToString());
        }
    }
}
using System;
using System.Collections.Generic;
using System.Threading.Tasks;
using System.Net;
using System.Net.Sockets;
using System.Text;

class Client
{
    const int BUF_SIZE = 1024;

    Socket socket { get; set; }
    string name { get; set; }
    Task<int> recvTask;
    int recvBytes;
    byte[] recvBuffer;
    
    public bool isActive { get; private set; }

    public Client(Socket socket)
    {
        this.socket = socket;
        this.isActive = true;
    }

    public async Task<Client> ReceiveAsync(CancellationToken token)
    {
        if (this.isActive)
        {
            recvBuffer = new byte[BUF_SIZE];
            recvBytes = await socket.ReceiveAsync(recvBuffer, SocketFlags.None, token);

            if (recvBytes <= 0)
            {
                Shutdown();
            }
        }

        return this;
    }

    public string GetBuffer()
    {
        if (recvBytes > 0)
        {
            // decode array
            string ret = Encoding.ASCII.GetString(recvBuffer, 0, recvBytes);
            // clear array
            Array.Clear(recvBuffer, 0, recvBytes);
            recvBytes = 0;
            recvBuffer = null;
            return ret;
        }
        
        return null;
    }

    public void Shutdown()
    {
        this.isActive = false;
        socket.Shutdown(SocketShutdown.Both);
        socket.Close();
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
            tasks.Add(listener.AcceptAsync(cts.Token).AsTask());

            while (tasks.Any())
            {
                Task finishedTask = await Task.WhenAny(tasks);
                tasks.Remove(finishedTask);

                if (finishedTask.GetType().GetGenericArguments()[0].Equals(typeof(Client)))
                {
                    // receive task
                    Client recvClient = await (Task<Client>)finishedTask;
                    string data = recvClient.GetBuffer();
                    if (!recvClient.isActive)
                    {
                        clients.Remove(recvClient);
                        this.msgHandler($"Client disconnected");
                        continue;
                    }
                    if (data == null)
                    {
                        continue;
                    }
                    this.msgHandler($"Client sent: {data}");
                    // listen to next message
                    tasks.Add(recvClient.ReceiveAsync(cts.Token));
                }
                else
                {
                    // accept socket
                    Socket newSocket = await (Task<Socket>)finishedTask;
                    Client newClient = new Client(newSocket);
                    clients.Add(newClient);
                    this.msgHandler($"Accepted client from {newSocket.LocalEndPoint.ToString()}");
                    // wait for next client
                    tasks.Add(listener.AcceptAsync(cts.Token).AsTask());
                    // listen to client
                    tasks.Add(newClient.ReceiveAsync(cts.Token));
                }
            }
        }
        catch (Exception e)
        {
            throw;
        }
    }

    public void Cleanup()
    {
        cts.Cancel();
        listener.Close();
        this.isRunning = false;

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
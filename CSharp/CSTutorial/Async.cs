using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Net.Http;
using System.Threading;
using System.Threading.Tasks;

namespace CSTutorial
{
    public class User
    {
        public int id;
        public string name;
    }
    public class Async
    {
        /*
         * async function substitutions
         * 
         * Task.Wait, Task.Result => await; retrieving background result
         * Task.WaitAny => await Task.WhenAny
         * Task.WaitAll => await Task.WhenAll
         * Thread.Sleep => await Task.Delay
         */

        private static readonly HttpClient httpClient = new HttpClient();
        public static void run()
        {
            // delay
            wait(); // will output result after 2 seconds

            // CPU-bound execution
            // calls test
            // "run After" prints before "All done"
            test();
            Console.WriteLine("run After");
            Thread.Sleep(1000); // ensure await call completes

            // IO-bound execution
            //request();
            //Console.WriteLine("Sending request");
            //Thread.Sleep(5000); // ensure await call completes

            //Console.WriteLine("Sending second request");
            //request2();
            //Thread.Sleep(5000); // ensure await call completes

            // multiple request
            multipleRequest();
            Thread.Sleep(1000); // ensure await call completes
        }

        public static async void wait()
        {
            await waitAndApologizeAsync();
            Console.WriteLine($"Today is {DateTime.Now:D}");
            Console.WriteLine($"The current time is {DateTime.Now.TimeOfDay:t}");
            Console.WriteLine("The current temperature is 76 degrees.");
        }

        public static async Task waitAndApologizeAsync()
        {
            await Task.Delay(2000);
            Console.WriteLine("Sorry for the delay");
        }

        public static async void test()
        {
            await Task.Run(() => test2());
            Console.WriteLine("All done");
        }

        public static void test2()
        {
            for (int i = 0; i < 100; i++)
            {
                Console.Write(i.ToString());
            }
        }

        public static async void request()
        {
            var stringData = await httpClient.GetStringAsync("http://127.0.0.1:5500/Other/codes.md");
            Console.WriteLine(stringData);
        }

        public static async void request2()
        {
            var req = httpClient.GetStringAsync("http://127.0.0.1:5500/Other/codes.md");
            Console.WriteLine("Requested again");
            var stringData = await req;
            Console.WriteLine(stringData);
        }

        public static async void multipleRequest()
        {
            IEnumerable<User> users = await getUsers();
            foreach (User u in users)
            {
                Console.WriteLine($"Found {u.name} with {u.id}");
            }
        }

        public static async Task<IEnumerable<User>> getUsers()
        {
            int[] userIds = { 0, 1, 2, 3, 4 };
            var getUserTasks = new List<Task<User>>();
            foreach (int userId in userIds)
            {
                getUserTasks.Add(getUserAsync(userId));
            }
            return await Task.WhenAll(getUserTasks);
        }

        public static async Task<User> getUserAsync(int id)
        {
            User ret = new User();
            ret.id = id;
            ret.name = $"Juipiter{id}";
            return ret;
        }
    }

    public class AsyncCancelEx
    {
        static readonly CancellationTokenSource s_cts = new CancellationTokenSource();

        static readonly HttpClient s_client = new HttpClient
        {
            MaxResponseContentBufferSize = 1_000_000
        };

        static readonly IEnumerable<string> s_urlList = new string[]
        {
            "https://docs.microsoft.com",
            "https://docs.microsoft.com/aspnet/core",
            "https://docs.microsoft.com/azure",
            "https://docs.microsoft.com/azure/devops",
            "https://docs.microsoft.com/dotnet",
            "https://docs.microsoft.com/dynamics365",
            "https://docs.microsoft.com/education",
            "https://docs.microsoft.com/enterprise-mobility-security",
            "https://docs.microsoft.com/gaming",
            "https://docs.microsoft.com/graph",
            "https://docs.microsoft.com/microsoft-365",
            "https://docs.microsoft.com/office",
            "https://docs.microsoft.com/powershell",
            "https://docs.microsoft.com/sql",
            "https://docs.microsoft.com/surface",
            "https://docs.microsoft.com/system-center",
            "https://docs.microsoft.com/visualstudio",
            "https://docs.microsoft.com/windows",
            "https://docs.microsoft.com/xamarin"
        };
    
        static async Task Main_()
        {
            Console.WriteLine("Select E to cancel by pressing ENTER or T to cancel by timeout");
            ConsoleKey k = Console.ReadKey().Key;
            while (!(k == ConsoleKey.E || k == ConsoleKey.T))
            {
                Console.WriteLine("Nope");
                k = Console.ReadKey().Key;
            }

            if (k == ConsoleKey.E)
            {
                Console.WriteLine("Press ENTER to cancel");

                Task cancelTask = Task.Run(() =>
                {
                    while (Console.ReadKey().Key != ConsoleKey.Enter)
                    {
                        Console.WriteLine("Press ENTER to cancel");
                    }

                    Console.WriteLine("Cancelling");
                    //s_cts.Cancel(); // cancel
                });

                Task sumPagesSizesTask = SumPageSizesAsync();

                // stop after first task to finish
                await Task.WhenAny(new[] { cancelTask, sumPagesSizesTask });
                Console.WriteLine("Done");
            }
            else
            {
                int t = 3500;
                Console.WriteLine($"Requested, will abort after {t} ms");

                try
                {
                    s_cts.CancelAfter(t);
                    await SumPageSizesAsync();
                }
                catch (OperationCanceledException)
                {
                    Console.WriteLine("Timed out");
                }
                finally
                {
                    s_cts.Dispose();
                }
            }

            Console.WriteLine("Done");
        }

        static async Task SumPageSizesAsync()
        {
            var stopwatch = Stopwatch.StartNew();

            int total = 0;

            // process sequentially
            //foreach (string url in s_urlList)
            //{
            //    int len = await ProcessUrlAsync(url, s_client, s_cts.Token);
            //    total += len;
            //}

            // process as they finish
            IEnumerable<Task<int>> query =
                from url in s_urlList
                select ProcessUrlAsync(url, s_client, s_cts.Token);
            List<Task<int>> tasks = query.ToList();
            while (tasks.Any())
            {
                Task<int> finishedTask = await Task.WhenAny(tasks);
                tasks.Remove(finishedTask);
                total += await finishedTask;
            }

            stopwatch.Stop();
            Console.WriteLine($"Retrieved {total:#,#} bytes in {stopwatch.Elapsed} seconds");
        }

        static async Task<int> ProcessUrlAsync(string url, HttpClient client, CancellationToken token)
        {
            // pass cancellation token to tasks to allow us to cancel them
            HttpResponseMessage res = await client.GetAsync(url, token);
            byte[] content = await res.Content.ReadAsByteArrayAsync(token);
            Console.WriteLine($"{url,-60} {content.Length,10:#,#}");

            return content.Length;
        }
    }
}

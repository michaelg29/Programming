using System;
using System.Collections.Generic;
using System.Collections.Concurrent;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CSTutorial
{
    public class AsyncQueue
    {
        static async Task ExecuteAllTogether()
        {
            // build list
            List<Task<int>> tasksToPerform = new List<Task<int>>();
            for (int i = 2; i <= 30; i++)
            {
                int j = i;
                tasksToPerform.Add(BuildTask(j));
            }

            // process as they finish
            while (tasksToPerform.Count > 0)
            {
                var task = await Task.WhenAny(tasksToPerform);
                tasksToPerform.Remove(task);
                var res = await task;
                Trace($"Task {res} finished waiting; Size of task queue: {tasksToPerform.Count}");
            }

        }

        static async Task _Main()
        {
            // build list
            List<Task<Task<int>>> tasksToPerform = new List<Task<Task<int>>>();
            for (int i = 2; i <= 30; i++)
            {
                int j = i;
                tasksToPerform.Add(new Task<Task<int>>(() => BuildTask(j)));
            }

            const int QUEUE_SIZE = 5;
            List<Task<Task<int>>> queue = new List<Task<Task<int>>>();
            // populate queue
            int taskIdx = 0;
            for (int n = Math.Min(QUEUE_SIZE, tasksToPerform.Count); taskIdx < n; taskIdx++)
            {
                queue.Add(tasksToPerform[taskIdx]);
                tasksToPerform[taskIdx].Start();
            }

            while (queue.Count > 0)
            {
                // get completed task
                var task = await Task.WhenAny(queue);
                var key = await await task;

                queue.Remove(task);

                if (taskIdx < tasksToPerform.Count)
                {
                    queue.Add(tasksToPerform[taskIdx]);
                    tasksToPerform[taskIdx].Start();
                    taskIdx++;
                }
                Trace($"Task {key} finished waiting; Size of task queue: {queue.Count}, Current task index: {taskIdx}");
            }
        }

        private static void Trace(string message)
        {
            var timestamp = DateTime.Now;
            Console.WriteLine($"[{timestamp:hh:mm:ss.fff}]: {message}");
        }

        static async Task<int> BuildTask(int i)
        {
            Trace($"starting to wait for {i} seconds");
            await Task.Delay(i * 1000);
            Trace($"Finished counter {i}");
            return i;
        }
    }
}

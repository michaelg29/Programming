using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CSTutorial
{
    class ProgressBar
    {
        static void Main(string[] args)
        {
            int counter = 0;
            int total = 10;
            Console.WriteLine($"Current progress: {counter}/{total}, {(float)counter / (float)total * 100}%");

            while (counter < total)
            {
                Thread.Sleep(1000);
                counter++;
                Console.WriteLine($"\rCurrent progress: {counter}/{total}, {(float)counter / (float)total * 100:#.#0}%");
            }

            Console.WriteLine();
        }
    }
}

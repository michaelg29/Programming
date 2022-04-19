using System;

namespace CSTutorial
{
    public class Collections
    {
        public static void run()
        {
            // 1D declarations
            int[] arr1 = new int[5];
            int[] arr2 = new int[] { 1, 3, 5, 7, 9 };
            int[] arr3 = { 1, 2, 3, 4, 5, 6 };
            int n = arr3.Length;

            // 2D
            int[,] arr2d1 = new int[2, 3];
            int[,] arr2d2 = { { 1, 2, 3 }, { 4, 5, 6 } };
            int dim = arr2d1.Rank; // 2
            foreach (int i in arr2d2)
            {
                Console.Write("{0} ", i);
            }
            Console.WriteLine();

            // 3D
            int[,,] arr3d = new int[4, 2, 3]; // 4 x 2 x 3
            int[,,] arr3da = new int[2, 2, 3] { { { 1, 2, 3 }, { 4, 5, 6 } },
                                       { { 7, 8, 9 }, { 10, 11, 12 } } };
            int val = arr3da[1, 0, 1];
            int len2 = arr3da.GetLength(1); // 2

            // jagged array: array of arrays
            int[][] jaggedArray = new int[6][];
            jaggedArray[0] = new int[4] { 1, 2, 3, 4 };
            jaggedArray[1] = new int[5];
            int[][] jaggedArray2 = new int[][]
            {
                new int[] { 1, 2, 3, 4 },
                new int[] { 0, 2 }
            };
            jaggedArray2[0][1] = 77;
            int[][,] jaggedArray4 = new int[2][,]
            {
                new int[,] { { 1, 3 }, { 5, 7 } },
                new int[,] { { 0, 2 }, { 4, 6 } }
            };

            // implicitly typed
            var a = new[] { "hello", null };
            // object initializers
            var contacts = new[]
            {
                new
                {
                    Name = "John Doe",
                    PhoneNumbers = new[] { "123-456-7890" }
                },
                new
                {
                    Name = "Jane Doe",
                    PhoneNumbers = new[] { "987-654-3210", "125-678-9012" }
                }
            };
            for (int i = 0, j = contacts.Length; i < j; i++)
            {
                Console.Write("Contact {0} at: ", contacts[i].Name);
                foreach (string num in contacts[i].PhoneNumbers)
                {
                    Console.Write("{0}, ", num);
                }
                Console.WriteLine();
            }
        }
    }
}

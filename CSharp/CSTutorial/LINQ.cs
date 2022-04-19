using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Linq;

namespace CSTutorial
{
    class Customer
    {
        public string Name { get; set; }
        public string City { get; set; }
    }

    class Person
    {
        public string First { get; set; }
        public string Last { get; set; }
        public int ID { get; set; }
        public string City { get; set; }
    }

    class Student : Person
    {
        public string Street { get; set; }
        public List<int> Scores;
    }

    class Teacher : Person
    {

    }

    public class LINQ
    {
        public static void run()
        {
            // data source
            int[] numbers = new int[7] { 0, 1, 2, 3, 4, 5, 6 };
            // query creation
            var numQuery = (
                from num in numbers
                where (num % 2) == 0
                select num).ToList();
            // query execution
            int numEvens = numQuery.Count();
            Console.WriteLine(numEvens);

            // generics
            IEnumerable<string> intQuery =
                from num in numbers
                where (num % 2) == 0
                select num.ToString();

            // ordering
            var descendingList = (
                from num in numbers
                orderby num descending
                select num.ToString()).ToArray();
            Console.WriteLine(string.Join(' ', descendingList));

            // data source 2
            Customer[] customers =
            {
                new Customer{ Name = "Michael", City = "Hellsinki" },
                new Customer{ Name = "John", City = "London" },
                new Customer{ Name = "Jane", City = "London" },
                new Customer{ Name = "Jill", City = "Ottowa" },
                new Customer{ Name = "Brendan", City = "Paris" },
                new Customer{ Name = "Catherine", City = "Paris" }
            };
            Customer[] distributors =
            {
                new Customer{ Name = "Michael", City = "Hellsinki" },
                new Customer{ Name = "John", City = "London" },
                new Customer{ Name = "Jane", City = "London" },
                new Customer{ Name = "Jill", City = "Ottowa" },
                new Customer{ Name = "Brendan", City = "Paris" },
                new Customer{ Name = "Catherine", City = "Paris" }
            };

            // ordering
            var custQuery1 =
                from cust in customers
                where cust.City == "London"
                orderby cust.Name ascending
                select cust;
            Console.WriteLine(String.Join(' ', custQuery1));

            // grouping
            IEnumerable<IGrouping<string, Customer>> custQuery2 =
                from cust in customers
                orderby cust.Name ascending
                group cust by cust.City into custGroup
                where custGroup.Count() > 0
                orderby custGroup.Key
                select custGroup;
            foreach (var customerGroup in custQuery2)
            {
                Console.WriteLine(customerGroup.Key);
                foreach (Customer customer in customerGroup)
                {
                    Console.WriteLine($"\t{customer.Name}");
                }
            }

            // joins
            var innerJoinQuery =
                from cust in customers
                join dist in distributors on cust.City equals dist.City
                select new { CustomerName = cust.Name, DistributorName = dist.Name, City = cust.City};

            foreach (var join in innerJoinQuery)
            {
                Console.WriteLine($"{join.CustomerName} with {join.DistributorName} in {join.City}");
            }

            // data transformations
            List<Student> students = new List<Student>()
            {
                new Student { First="Svetlana",
                    Last="Omelchenko",
                    ID=111,
                    Street="123 Main Street",
                    City="Seattle",
                    Scores= new List<int> { 97, 92, 81, 60 } },
                new Student { First="Claire",
                    Last="O’Donnell",
                    ID=112,
                    Street="124 Main Street",
                    City="Redmond",
                    Scores= new List<int> { 75, 84, 91, 39 } },
                new Student { First="Sven",
                    Last="Mortensen",
                    ID=113,
                    Street="125 Main Street",
                    City="Lake City",
                    Scores= new List<int> { 88, 94, 65, 91 } },
            };
            List<Teacher> teachers = new List<Teacher>()
            {
                new Teacher { First="Ann", Last="Beebe", ID=945, City="Seattle" },
                new Teacher { First="Alex", Last="Robinson", ID=956, City="Redmond" },
                new Teacher { First="Michiyo", Last="Sato", ID=972, City="Tacoma" }
            };
            // query
            var pplInSeattle = (
                from student in students
                where student.City == "Seattle"
                select student.Last)
                .Concat(from teacher in teachers
                        where teacher.City == "Seattle"
                        select teacher.Last);
            Console.WriteLine($"Seattle residents: {string.Join(", ", pplInSeattle)}");
            // select subset
            var query = from student in students
                        select new { Name = student.Last, City = student.City };
            // transform to XML
            var xmlEl = new XElement("Root",
                from student in students
                let scores = string.Join(",", student.Scores)
                select new XElement("student",
                    new XElement("First", student.First),
                    new XElement("Last", student.Last),
                    new XElement("Scores", scores)
                )
            );
            Console.WriteLine(xmlEl);

            // operations
            double[] radii = { 1, 2, 3 };
            IEnumerable<string> output =
                radii.Select(r => $"Area of C({r}) = {r * r * Math.PI:F2}");
            foreach (string str in output)
            {
                Console.WriteLine(str);
            }

        }
    }
}

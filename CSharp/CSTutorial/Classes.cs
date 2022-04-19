using System;
using System.Diagnostics;
using System.Threading;

using CSTutorial.Classes;

namespace CSTutorial
{
    public class ClassTest
    {
        public static void run()
        {
            int param = 5;
            Derived d = new Derived(param);
            d.DoWork(param); // Base.DoWork(double)
            ((Base)d).DoWork(param); // override Base.DoWork(int)

            Base b = new Base(5);
            d = new Derived(5);
            Console.WriteLine($"{b}, {d}");

            MyStopwatch sw = new MyStopwatch();
            sw.start();
            for (int i = 0; i < 0; i++)
            {
                Console.WriteLine(sw.Display);
                Thread.Sleep(1000);
            }

            var item = new SaleItem
            {
                Name = "Shoes",
                Price = 19.95m
            };
            Console.WriteLine($"{item.Name} sells for {item.Price:C2}");
        }
    }

    namespace Classes
    {
        class Base
        {
            public const int Months = 12; // no static keyword
            public int x { get; set; }

            public Base(int x)
            {
                this.x = x;
            }

            public virtual void DoWork(int param)
            {
                Console.WriteLine($"Base.DoWork({param})");
            }

            public override string ToString()
            {
                return $"Base.{x}";
            }
        }

        class Derived : Base
        {
            public Derived(int x)
                : base(x)
            { }

            public override void DoWork(int param)
            {
                Console.WriteLine($"Override Base.DoWork({param})");
            }

            public void DoWork(double param)
            {
                // 0 is required in format
                // # is optional in format
                Console.WriteLine($"Derived.DoWork({param:0.#0})");
            }

            public override string ToString()
            {
                return $"Derived.{x}";
            }
        }

        class TimePeriod
        {
            private long _seconds;

            public long Seconds {
                get => _seconds % 60;
                set => _seconds = value;
            }

            public long Minutes
            {
                get { return _seconds / 60; }
                set
                {
                    if (value < 0)
                    {
                        throw new ArgumentOutOfRangeException(
                            $"{nameof(value)} must be positive");
                    }
                    _seconds = (_seconds % 60) + (value * 3600);
                }
            }
        }

        class MyStopwatch
        {
            private long _startTime;

            public void start()
            {
                _startTime = Stopwatch.GetTimestamp();
            }

            public TimePeriod Elapsed
            {
                get
                {
                    TimePeriod ret = new TimePeriod();
                    ret.Seconds = (Stopwatch.GetTimestamp() - _startTime) / Stopwatch.Frequency;
                    return ret;
                }
            }

            public string Display => $"{Elapsed.Minutes:0}:{Elapsed.Seconds:00}";
        }

        class SaleItem
        {
            public string Name { get; set; }
            public decimal Price { get; set; } = 0.0m;
        }

        // override property
        abstract class Shape
        {
            public abstract double Area { get; set; }
        }

        class Square : Shape
        {
            public double side;

            public Square(double s) => side = s;

            public override double Area
            {
                get => side * side;
                set => side = Math.Sqrt(value);
            }
        }

        class Cube : Shape
        {
            public double side;

            public Cube(double s) => side = s;

            public override double Area
            { 
                get => 6 * side * side;
                set => side = Math.Sqrt(value / 6);
            }
        }
    }
}

using System;
using System.Diagnostics;

namespace Lab5
{
    internal class Program
    {
        static void InitialFunc()
        {
            int[,] a = new int[10,10];
            int res = 0;

            for (int i = 0; i < 10; i++)
            {
                for (int j = 0; j < 10; j++)
                {
                    a[j,i]++;
                }
            }
        }
        
        static void OptimizedFunc()
        {
            int[,] a = new int[10,10];
            
            for (int i = 0; i < 10; i++)
            {
                for (int j = 0; j < 10; j++)
                {
                    a[i,j]++;
                }
            }
        }
        
        public static void Main(string[] args)
        {
            Stopwatch stopwatch1 = new Stopwatch();
            Stopwatch stopwatch2 = new Stopwatch();
            
            stopwatch1.Start();
            InitialFunc();
            stopwatch1.Stop();
            
            stopwatch2.Start();
            OptimizedFunc();
            stopwatch2.Stop();
           
            Console.WriteLine("Потрачено тактов на выполнение изначальной функции: " + stopwatch1.ElapsedTicks);
            Console.WriteLine("Потрачено тактов на выполнение оптимизированной функции: " + stopwatch2.ElapsedTicks);
        }
    }
}
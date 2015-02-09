using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Testbed.CSharpConsole
{
    class Program
    {
        static void Main(string[] args)
        {
            // declare an integer 
            var x = 5;
            // declare an array 
            int[] kushal = new int[x];
            // this ought to give an error 
            kushal[5] = 3;
        }
    }
}

using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Testbed.CSharpConsole
{
    class Program
    {
        static readonly string Filename = "..\\..\\input.txt";
        static string GetFileContents(string Filename)
        {
            string line;
            using (StreamReader reader = new StreamReader(Filename))
            {
                line = reader.ReadLine();
            }
            return line;
        }
        static void Main(string[] args)
        {
            string input = GetFileContents(Filename);
        }
    }
}

using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Lexer.SharpPascalCompiler
{
    class Program
    {
        private static readonly string strInputFileLocation = "..\\..\\input.txt";
        static void Main(string[] args)
        {
            try
            {
                using (StreamReader sr = new StreamReader(strInputFileLocation))
                {
                    String line = sr.ReadLine();
                    Console.WriteLine(line);
                }
            }
            catch (Exception e)
            {
                Console.WriteLine("The file could not be read:");
                Console.WriteLine(e.Message);
            }
        }
    }
}

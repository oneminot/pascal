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
        static UInt16 GetFileContents(string Filename)
        {
            string line;
            using (StreamReader reader = new StreamReader(Filename))
            {
                line = reader.ReadLine();
            }
            try
            {
                UInt16 request = Convert.ToUInt16(line);
                return request;
            }
            catch (OverflowException e)
            {
                Console.WriteLine("Overflow exception");
                Console.WriteLine(e.ToString());
                return 0;
            }
            catch (FormatException e)
            {
                Console.WriteLine("Overflow exception");
                Console.WriteLine(e.ToString());
                return 0;
            }

        }
        static void Main(string[] args)
        {
            UInt16 input = GetFileContents(Filename);
        }
    }
}

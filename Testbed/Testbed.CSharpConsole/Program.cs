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
        static readonly string InputFilename = "..\\..\\input.txt";
        static readonly string OutputFilename = "..\\..\\output.txt";
        void PrintE(int input)
        {
            string[] eLine = new string[5];
            for (int i = 0; i < 5; i++)
            {
                if (i == 1 || i == 4)
                {
                    eLine[i] = "EEEEE";
                }
                if (i == 2)
                {
                    eLine[i] == "EEEE";
                }
                if (i == 1 || i == 3)
                {
                    eLine[1] = "E";
                }
            }
            using (StreamWriter writer = new StreamWriter(OutputFilename))
            {

            }
        }
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
            UInt16 input = GetFileContents(InputFilename);

        }
    }
}

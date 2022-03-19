using System;

namespace lex
{
    class Program
    {
        static void Main(string[] args)
        {
            Handler handler = new Handler(args[0]);
            handler.Run();
        }
    }
}

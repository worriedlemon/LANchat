using System;
using System.Collections.Generic;

namespace LANchat
{
    using Flag = System.UInt32;

    public static class ArgumentProcessor
    {
        public delegate void Debug(string message);

        public static Debug VerboseDebugger { get; private set; } = null;

        public enum PossibleOptions
        {
            VERSION = 'v',
            VERBOSE = 'V',
            NO_GUI = 'n',
            HELP = 'h',
            OTHER = '\0'
        }

        private static readonly Dictionary<string, PossibleOptions> FullOptionNames = new Dictionary<string, PossibleOptions>()
        {
            { "version", PossibleOptions.VERSION },
            { "verbose", PossibleOptions.VERBOSE },
            { "no_gui", PossibleOptions.NO_GUI },
            { "help", PossibleOptions.HELP }
        };

        private static void PrintHelp()
        {
            Console.WriteLine("Usage: LANchat [option]\n\n" +
                "Possible options:\n" +
                "\t-v (--version)\t\tPrint application version" +
                "\t-V (--verbose)\t\tRun application in verbose mode" +
                "\t-n (--no_gui)\t\tRun application in console mode (also sets verbose mode) - obsolete" +
                "\t-h (--help)\t\tPrint this message");
        }

        public static bool ProcessArguments(string[] args)
        {
            Flag options = 0; 
            foreach (string arg in args)
            {
                PossibleOptions option;
                if (arg.StartsWith("--"))
                {
                    try
                    {
                        option = FullOptionNames[arg.Substring(2)];
                    }
                    catch (KeyNotFoundException)
                    {
                        option = PossibleOptions.OTHER;
                    }
                }
                else if (arg.StartsWith("-") || arg.StartsWith("/"))
                {
                    if (arg.Length == 2)
                    {
                        option = (PossibleOptions)arg[1];
                    }
                    else option = PossibleOptions.OTHER;
                }
                else option = PossibleOptions.OTHER;

                switch (option)
                {
                    case PossibleOptions.VERSION:
                        Console.WriteLine($"{GlobalDefs.ApplicationTitle} ver. {GlobalDefs.ApplicationVersion}");
                        options |= 1;
                        break;
                    case PossibleOptions.NO_GUI:
                        //GlobalDefs.ConsoleMode = true;
                        Console.WriteLine("Console mode is not implemented yet");
                        options |= 1;
                        break;
                    case PossibleOptions.VERBOSE:
                        GlobalDefs.VerboseMode = true;
                        VerboseDebugger = Console.WriteLine;
                        options |= 2;
                        break;
                    case PossibleOptions.HELP:
                        PrintHelp();
                        options |= 1;
                        break;
                    case PossibleOptions.OTHER:
                        Console.WriteLine($"Unrecognized option {arg} skipped.");
                        break;
                }
            }
            return options == 1;
        }
    }
}

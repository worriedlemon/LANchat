namespace LANchat
{
    public static class GlobalDefs
    {
        public static string ApplicationTitle { get; set; } = "LANchat";
        public static string ApplicationVersion { get; set; } = "1.0";

        public static bool VerboseMode { get; set; } = false;

        [System.Obsolete("This field is not used, changing values does nothing")]
        public static bool ConsoleMode { get; set; } = false;

        public const ushort DefaultPort = 51492;
    }
}

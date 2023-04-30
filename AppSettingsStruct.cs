namespace LANchat
{
    [System.Serializable]
    public class AppSettings : Connection.ILoadable
    {
        public string Username;
        public ushort Port;
        public bool IsDefaultPort
        {
            get
            {
                return Port == GlobalDefs.DefaultPort;
            }
        }

        public AppSettings() { }

        public static AppSettings DefaultSettings() => new AppSettings()
        {
            Username = System.Environment.UserName,
            Port = GlobalDefs.DefaultPort
        };

        public override string ToString() => $"User '{Username}' on port {Port}";
    }
}
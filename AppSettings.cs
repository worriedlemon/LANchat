using System.Text.Json.Serialization;

namespace LANchat
{
    [System.Serializable]
    public class AppSettings : Connection.ILoadable
    {
        public string Username;
        public ushort Port;

        [JsonIgnore]
        public bool IsDefaultPort
        {
            get
            {
                return Port == GlobalDefs.DefaultPort;
            }
        }

        public AppSettings() { }

        [JsonConstructor]
        private AppSettings(string username, ushort port)
        {
            Username = username;
            Port = port;
        }

        public static AppSettings DefaultSettings() => new AppSettings()
        {
            Username = System.Environment.UserName,
            Port = GlobalDefs.DefaultPort
        };

        public override string ToString() => $"User '{Username}' on port {Port}";
    }
}
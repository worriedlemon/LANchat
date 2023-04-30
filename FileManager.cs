using System.IO;
using System.Xml.Serialization;

namespace LANchat
{
    public static class FileManager
    {
        public static void SaveToXML<T>(T obj, string path) where T : class, Connection.ILoadable
        {
            XmlSerializer xml = new XmlSerializer(typeof(T));
            xml.Serialize(File.CreateText(path), obj);
        }

        public static T LoadFromXML<T>(string path) where T : class, Connection.ILoadable
        {
            XmlSerializer xml = new XmlSerializer(typeof(T));
            return xml.Deserialize(File.OpenText(path)) as T;
        }
    }
}

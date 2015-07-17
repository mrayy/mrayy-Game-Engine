using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

namespace KMDSessionCreator.Data
{
    public class KMDSessions
    {
        List<Faculty> _Faculties=new List<Faculty>();
        List<Session> _Sessions = new List<Session>();


        public List<Faculty> Faculties
        {
            get
            {
                return _Faculties;
            }
        }
        public List<Session> Sessions
        {
            get
            {
                return _Sessions;
            }
        }

        public Faculty GetFaculty(string name)
        {
            foreach(var v in Faculties)
            {
                if (v.Name == name)
                    return v;
            }
            return null;
        }

        public void Clear()
        {
            _Faculties.Clear();
            _Sessions.Clear();
        }

        bool LoadUsers(string path)
        {
            
            XmlReaderSettings settings = new XmlReaderSettings();
            settings.IgnoreWhitespace = true;
            settings.ConformanceLevel = ConformanceLevel.Fragment;
            settings.IgnoreComments = true;
            XmlReader reader = XmlReader.Create(path, settings);
            if (reader == null)
                return false;
            while (reader.Read())
            {

                switch (reader.NodeType)
                {
                    case XmlNodeType.Element:
                        if (reader.Name == "User")
                        {
                            Data.Faculty f = new Faculty();
                            f.Load(reader);
                            Faculties.Add(f);
                        }
                        break;
                }
            }

            reader.Close();
            return true;
        }
        void LoadSessions(XmlReader reader)
        {
            while (reader.MoveToNextAttribute())
            {
                if (reader.Name == "Users")
                {
                    LoadUsers(Path.GetDirectoryName(reader.BaseURI)+"\\"+reader.Value);
                }
            }
            while (reader.Read())
            {

                switch (reader.NodeType)
                {
                    case XmlNodeType.Element:
                        if (reader.Name == "Session")
                        {
                            Session s = new Session();
                            s.Load(reader);
                            Sessions.Add(s);
                        }
                        break;
                    case XmlNodeType.Text:
                        break;

                }
            }

        }
        public bool LoadXML(string path)
        {
            XmlReaderSettings settings = new XmlReaderSettings();
            settings.IgnoreWhitespace = true;
            settings.ConformanceLevel = ConformanceLevel.Fragment;
            settings.IgnoreComments = true;
            XmlReader reader = XmlReader.Create(path, settings);
            if (reader == null)
                return false;
            
            while (reader.Read())
            {

                switch(reader.NodeType)
                {
                case XmlNodeType.Element:
                        if (reader.Name == "Sessions")
                        {
                            LoadSessions(reader);
                        }
                    break;
                case XmlNodeType.Text:
                    break;

                }
            }
            reader.Close();
            return true;
        }

        bool ExportUsersXML(string path)
        {
            XmlWriter xmlWriter = XmlWriter.Create(path, new XmlWriterSettings
            {
                Indent = true
            });
            xmlWriter.WriteStartElement("Users");
            foreach (var s in Faculties)
            {
                s.Export(xmlWriter);
            }
            xmlWriter.WriteEndElement();
            xmlWriter.Close();
            return true;

        }
        public bool ExportXML(string path)
        {
            XmlWriter xmlWriter = XmlWriter.Create(path, new XmlWriterSettings
            {
                Indent = true
            });

            string users=Path.GetDirectoryName(path)+"\\"+Path.GetFileNameWithoutExtension(path)+"_Users.xml";

            xmlWriter.WriteStartElement("Sessions");
            xmlWriter.WriteAttributeString("Users",Path.GetFileName(users));
            foreach (var s in Sessions)
            {
                s.Export(xmlWriter);
            }
            xmlWriter.WriteEndElement();
            xmlWriter.Close();

            ExportUsersXML(users);
            return true;
        }
    }
}

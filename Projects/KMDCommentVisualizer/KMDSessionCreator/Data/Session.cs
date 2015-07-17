using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

namespace KMDSessionCreator.Data
{
    public class Session
    {

        public string Name = "";
        public string theme = "";
        public bool IsSession=true;

        public DateTime StartTime=new DateTime();
        public DateTime EndTime = new DateTime();

        public Color BackgroundColor=new Color();
        public string Picture = "";
        public string Description = "";



        public List<Faculty> Advisors = new List<Faculty>();
        public List<Faculty> Committee = new List<Faculty>();
        public List<SubProject> Subprojects = new List<SubProject>();


        public override string ToString()
        {
            return Name;
        }

        public bool Load(XmlReader reader)
        {
            Name = reader.GetAttribute("SessionName");
            theme = reader.GetAttribute("Theme");
            string startTm = reader.GetAttribute("SessionStartTime");
            string endTm   = reader.GetAttribute("SessionEndTime");

            StartTime = DateTime.Parse(startTm);
            EndTime = DateTime.Parse(endTm);
            string[] color = reader.GetAttribute("Color").Split(",".ToCharArray());
            BackgroundColor = Color.FromArgb(int.Parse(color[0]), int.Parse(color[1]), int.Parse(color[2]));
            Picture = reader.GetAttribute("Picture");

            bool done = false;
            bool description = false;
            while (!done && reader.Read())
            {
                switch (reader.NodeType)
                {
                    case XmlNodeType.Element:
                        switch (reader.Name)
                        {
                            case "Description":
                                description = true;
                                break;
                            case "Adviser":
                                Faculty f= Data.AppShared.Instance.Sessions.GetFaculty(reader.GetAttribute("Name"));
                                if(f==null)
                                    break;
                                Advisors.Add(f);
                                break;
                            case "Committee":
                                Faculty c = Data.AppShared.Instance.Sessions.GetFaculty(reader.GetAttribute("Name"));
                                if(c==null)
                                    break;
                                Committee.Add(c);
                                break;
                            case "SubProject":
                                SubProject p = new SubProject();
                                p.Load(reader);
                                Subprojects.Add(p);
                                break;
                        }
                        break;
                    case XmlNodeType.Text:
                        if(description)
                        {
                            Description = reader.Value.Trim();
                        }
                        break;
                    case XmlNodeType.EndElement:
                         if(reader.Name=="Session")
                            done = true;
                        break;
                }
            }
            
            return true;
        }

        public bool Export(XmlWriter xmlWriter)
        {

            xmlWriter.WriteStartElement("Session");
            xmlWriter.WriteAttributeString("SessionName", Name);
            xmlWriter.WriteAttributeString("Theme", theme);
            xmlWriter.WriteAttributeString("SessionStartTime", StartTime.ToShortTimeString());
            xmlWriter.WriteAttributeString("SessionEndTime", EndTime.ToShortTimeString());
            xmlWriter.WriteAttributeString("Color", String.Format("{0},{1},{2}", BackgroundColor.R, BackgroundColor.G, BackgroundColor.B));
            xmlWriter.WriteAttributeString("Picture", Picture);
            xmlWriter.WriteAttributeString("IsSession", IsSession.ToString());

            xmlWriter.WriteStartElement("Description");
            xmlWriter.WriteString(Description);
            xmlWriter.WriteEndElement();

            foreach(var v in Advisors)
            {
                v.ExportAttr("Adviser",xmlWriter);
            }
            foreach(var v in Committee)
            {
                v.ExportAttr("Committee",xmlWriter);
            }
            foreach(var v in Subprojects)
            {
                v.Export(xmlWriter);
            }

            xmlWriter.WriteEndElement();
            return true;
        }
    }
}

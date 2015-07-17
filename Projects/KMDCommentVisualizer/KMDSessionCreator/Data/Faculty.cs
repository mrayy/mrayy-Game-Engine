using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

namespace KMDSessionCreator.Data
{
    public class Faculty
    {
        public string Name = "";
        public string Title = "";
        public string ImageURL = "";

        public override string ToString()
        {
            return Name;
        }
        public bool ExportAttr(string eName, XmlWriter xmlWriter)
        {
            xmlWriter.WriteStartElement(eName);
            xmlWriter.WriteAttributeString("Name", Name);
            xmlWriter.WriteEndElement();
            return true;
        }
        public bool Export( XmlWriter xmlWriter)
        {

            xmlWriter.WriteStartElement("User");
            xmlWriter.WriteAttributeString("Name", Name);
            xmlWriter.WriteAttributeString("Title", Title);
            xmlWriter.WriteAttributeString("ImageURL", ImageURL);
            xmlWriter.WriteEndElement();

            return true;
        }

        public bool Load(XmlReader reader)
        {
            Name = reader.GetAttribute("Name");
            Title = reader.GetAttribute("Title");
            ImageURL= reader.GetAttribute("ImageURL");
            return true;
        }
    }
}

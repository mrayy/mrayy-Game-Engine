using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

namespace KMDSessionCreator.Data
{
    public class SubProject
    {
        public string Name="";
        public string Title = "";
        public int Length=0;

        public override string ToString()
        {
            return Name;
        }
        public bool Export(XmlWriter xmlWriter)
        {
            xmlWriter.WriteStartElement("SubProject");
            xmlWriter.WriteAttributeString("Name", Name);
            xmlWriter.WriteAttributeString("Title", Title);
            xmlWriter.WriteAttributeString("Length", Length.ToString());
            xmlWriter.WriteEndElement();


            return true;
        }
        public bool Load(XmlReader reader)
        {
            Name = reader.GetAttribute("Name");
            Title = reader.GetAttribute("Title");
            Length = Int16.Parse(reader.GetAttribute("Length"));
            return true;
        }
    }
}

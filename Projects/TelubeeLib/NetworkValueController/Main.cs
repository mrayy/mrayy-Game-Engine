using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Xml;

namespace NetworkValueController
{
    public partial class Main : Form
    {
        public sealed class StringWriterWithEncoding : StringWriter
        {
            private readonly Encoding encoding;

            public StringWriterWithEncoding(Encoding encoding)
            {
                this.encoding = encoding;
            }

            public override Encoding Encoding
            {
                get { return encoding; }
            }
        }
        UdpUser client;

        ValueGroupCtl values;
        bool isConnected=false;
        public Main()
        {
            InitializeComponent();
            grpPos.Visible = false;


        }

        void RequestScheme()
        {
            string data = "RequestScheme#";
            client.Send(data); 

        }
        
        void XmlLoadValue(XmlElement reader,ValueGroupCtl parent)
        {

            parent.CreateValue(reader.GetAttribute("Name"), reader.GetAttribute("Type"), reader.GetAttribute("Value"));
        }
        void XmlLoadGroup(XmlElement reader, ValueGroupCtl parent)
        {
            ValueGroupCtl g=new ValueGroupCtl();
            if(parent==null)
            {
                Controls.Add(g);
                values = g;
                values.Left = grpPos.Left;
                values.Top = grpPos.Top;
                values.Width = grpPos.Width;
                values.Height = Bottom - values.Top;
                values.SetName("Values");
                values.OnChanged += values_OnChanged;
            }
            else
            {
                parent.AddGroup(g);
            }
//             while (reader.MoveToNextAttribute())
//             {
//             }
            g.SetName(reader.GetAttribute("Name"));
            XmlNode e;
            e = reader.FirstChild;
            while(e!=null)
            {
                if (e.NodeType == XmlNodeType.Element)
                {
                    if (e.Name == "ValueGroup")
                    {
                        XmlLoadGroup(e as XmlElement, g);
                    }
                    else if (e.Name == "Value")
                    {
                        XmlLoadValue(e as XmlElement, g);
                    }
                }
                e = e.NextSibling;
            }

        }
        void ParseScheme(string scheme)
        {
            if(values!=null)
            {
                Controls.Remove(values);
                values = null;
            }
            using (var r = new StringReader(scheme))
            {
                XmlDocument doc = new XmlDocument();
                doc.Load(r);
                XmlElement e = doc.DocumentElement;
                if (e!=null)
                {
                    XmlLoadGroup(e,null);
                }
            }

            values.UpdateValuesPosition();
        }

        void DataArrived(string data,System.Net.IPEndPoint ep)
        {
            textBox1.Text = data;
            ParseScheme(data);
        }

        void values_OnChanged(object sender, EventArgs e)
        {
            SendData();
        }

        void Connect()
        {
            if (isConnected)
            {
                isConnected = false;
                client.Client.Close();
            }
            client = UdpUser.ConnectTo(txtIP.Text, int.Parse(txtPort.Text));
            isConnected = true;
            Task.Factory.StartNew(async () =>
            {
                while (isConnected)
                {
                    try
                    {
                        var received = await client.Receive();
                        this.BeginInvoke((MethodInvoker)delegate
                        {
                            DataArrived(received.Message, received.Sender);
                        });
                    }catch
                    {

                    }
                }
            });

            RequestScheme();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Connect();
        }

        void SendData()
        {
            string data;
            data = "Data#";
            using (var sw = new StringWriterWithEncoding(Encoding.UTF8))
            {
                using (var xw = XmlWriter.Create(sw, new XmlWriterSettings() { Indent = true,WriteEndDocumentOnClose=true,CheckCharacters=true,OmitXmlDeclaration=true, Encoding = Encoding.UTF8 }))
                {
                    // Build Xml with xw.

                    values.Export(xw);
                }
                data+= sw.ToString();
            }
            data += "\n";
            try
            {
                client.Send(data);

            }catch
            {

            }
            //textBox1.Text = data;
            //client.Send(data);
        }

    }
}

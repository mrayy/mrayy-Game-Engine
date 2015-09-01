using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Xml;

namespace NetworkValueController
{
    public partial class ValueControllerCtl : UserControl
    {
        public event EventHandler OnChanged;

        string name;
        string value;
        public ValueControllerCtl()
        {
            InitializeComponent();
        }

        void CreateController(string type,string value)
        {
            Control c=null;
            this.value = value;
            valLbl.Text = value;
            type = type.ToLower();
            if (type == "string")
            {
                TextBox tb = new TextBox();
                tb.TextChanged += c_txtChanged;
                tb.Text = value;
                c = tb;
            }
            else if (type == "float" || type == "int")
            {
                TrackBar tb = new TrackBar();
                tb.Minimum = 0;
                tb.Maximum = 100;
                try
                {
                    tb.Value = (int)(float.Parse(value) * 100);
                }catch
                { }
                tb.ValueChanged += c_tbChanged;
                c = tb;
            }
            else if (type == "vector2di" || type == "vector2df")
            {
                Vector2Ctl tb = new Vector2Ctl();

                tb.Value = value;
                tb.OnChanged += tb_OnChanged;
                c = tb;
            }
            else
                return;

            this.Controls.Add(c);
            c.Left = vlbl.Left;
            c.Top = vlbl.Top;
            c.Width = vlbl.Width;
            c.Height = vlbl.Height;
        }

        void tb_OnChanged(object sender, EventArgs e)
        {
            value = (sender as Vector2Ctl).Value;
            valLbl.Text = value;
            if (OnChanged != null)
                OnChanged(this, e);
        }

        void c_txtChanged(object sender, EventArgs e)
        {
            value = (sender as TextBox).Text;
            valLbl.Text = value;
            if (OnChanged != null)
                OnChanged(this, e);
        }
        void c_tbChanged(object sender, EventArgs e)
        {
            value = ((float)(sender as TrackBar).Value/100.0f).ToString();
            valLbl.Text = value;
            if (OnChanged != null)
                OnChanged(this, e);
        }


        public void SetName(string name)
        {
            this.name = name;
            namelbl.Text = name;
        }
        public void SetType(string type,string value)
        {
            CreateController(type,value);
        }

        public bool Export(XmlWriter xmlWriter)
        {

            xmlWriter.WriteStartElement("Value");
            xmlWriter.WriteAttributeString("Name", namelbl.Text);
            xmlWriter.WriteAttributeString("Value", value);
            xmlWriter.WriteEndElement();

            return true;
        }
    }
}

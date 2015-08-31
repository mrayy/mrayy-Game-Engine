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
    public partial class ValueGroupCtl : UserControl
    {
        public event EventHandler OnChanged;
        List<ValueControllerCtl> values = new List<ValueControllerCtl>();
        List<ValueGroupCtl> groups = new List<ValueGroupCtl>();

        public ValueGroupCtl()
        {
            InitializeComponent();
            Anchor = AnchorStyles.Left | AnchorStyles.Right | AnchorStyles.Top;
        }

        public void SetName(string name)
        {
            vgrp.Text = name;
        }
        public void AddValue(ValueControllerCtl v)
        {
            values.Add(v);
            vgrp.Controls.Add(v);
            v.OnChanged += c_OnChanged;
            UpdateValuesPosition();
        }
        public void AddGroup(ValueGroupCtl v)
        {
            groups.Add(v);
            vgrp.Controls.Add(v);
            v.OnChanged += c_OnChanged;
            UpdateValuesPosition();
        }
        public ValueControllerCtl CreateValue(string name, string type,string value)
        {
            ValueControllerCtl c = new ValueControllerCtl();
            c.SetType(type, value);
            c.SetName(name);
            AddValue(c);
            return c;
        }

        void c_OnChanged(object sender, EventArgs e)
        {
            if (OnChanged != null)
                OnChanged(sender, e);
        }
        public ValueGroupCtl CreateValueGroup(string name)
        {
            ValueGroupCtl c = new ValueGroupCtl();
            c.SetName(name);

            AddGroup(c);
            return c;
        }

        public void UpdateValuesPosition()
        {
            int y = 20;
            int height = 30;
            for(int i=0;i<values.Count;++i)
            {
              //  values[i].Width = vgrp.Width - 10;
              //  values[i].Height = height;
                values[i].Left = 5;
                values[i].Top = y;
                y += values[i].Height;
            }
            for (int i = 0; i < groups.Count; ++i)
            {
                groups[i].Width = vgrp.Width - 10;
                groups[i].Left = 5;
                groups[i].Top = y;

                groups[i].UpdateValuesPosition();
                y += groups[i].Height;
            }
            this.Height = y;
            this.Update();

        }

        public bool Export(XmlWriter xmlWriter)
        {

            xmlWriter.WriteStartElement("ValueGroup");
            xmlWriter.WriteAttributeString("Name", vgrp.Text);
            for (int i = 0; i < values.Count; ++i)
                values[i].Export(xmlWriter);
            for (int i = 0; i < groups.Count; ++i)
                groups[i].Export(xmlWriter);
            xmlWriter.WriteEndElement();

            return true;
        }
    }
}

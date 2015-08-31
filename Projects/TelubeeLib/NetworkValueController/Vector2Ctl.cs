using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace NetworkValueController
{
    public partial class Vector2Ctl : UserControl
    {
        public event EventHandler OnChanged;

        public int X
        {
            set
            {
                XVal.Value = value;
            }
            get
            {
                return XVal.Value;
            }
        }
        public int Y
        {
            set
            {
                YVal.Value = value;
            }
            get
            {
                return YVal.Value;
            }
        }

        public string Value
        {
            set
            {
                string[] vals=value.Split(",".ToCharArray());
                if (vals.Length == 0)
                    return;
                if(vals.Length==1)
                {
                    X = Y = (int)(float.Parse(vals[0]));
                }
                else
                {
                    X = (int)(float.Parse(vals[0]) );
                    Y = (int)(float.Parse(vals[1]) );
                }
            }
            get
            {
                return (X * 100.0f).ToString() + "," + (Y * 100.0f).ToString();
            }
        }
        public Vector2Ctl()
        {
            InitializeComponent();
        }

        private void XVal_ValueChanged(object sender, EventArgs e)
        {
            if(OnChanged!=null)
                OnChanged(this, e);
        }

        private void YVal_ValueChanged(object sender, EventArgs e)
        {
            if (OnChanged != null)
                OnChanged(this, e);
        }


    }
}

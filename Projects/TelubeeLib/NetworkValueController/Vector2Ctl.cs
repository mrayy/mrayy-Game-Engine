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
                try
                {
                    XVal.Value = value;
                }catch
                {

                }
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
                try
                {
                    YVal.Value = value;
                }
                catch
                {

                }
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
                    X = Y = (int)(float.Parse(vals[0]) * 100);
                }
                else
                {
                    X = (int)(float.Parse(vals[0]) * 100);
                    Y = (int)(float.Parse(vals[1]) * 100);
                }
            }
            get
            {
                return (X / 100.0f).ToString() + "," + (Y / 100.0f).ToString();
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

        private void Vector2Ctl_Resize(object sender, EventArgs e)
        {
            XVal.Width = Width / 2;
            YVal.Width = Width / 2;
            YVal.Left = Width / 2;

        }


    }
}

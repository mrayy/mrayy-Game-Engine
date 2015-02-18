using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Threading;

namespace AIRuleDesigner
{
    public partial class SplashScreen : Form
    {
        ManualResetEvent _event = new ManualResetEvent(false);
        public SplashScreen()
        {
            InitializeComponent();
            Thread th = new Thread(new ThreadStart(this.runScreen));
            th.SetApartmentState(ApartmentState.STA);
            th.IsBackground = true;
            th.Start();
            this._event.WaitOne();
        }

        public void runScreen()
        {
            base.Width = 0;
            base.Height = 0;
            base.FormBorderStyle = FormBorderStyle.None;
            base.ShowInTaskbar = false;
            try
            {
                Application.Run(this);
            }
            catch (System.Exception e)
            {

            }
        }

        private void SplashScreen_Load(object sender, EventArgs e)
        {
            //_event.Set();
           // _event = null;
        }
    }
}
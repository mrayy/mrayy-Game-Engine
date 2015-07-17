using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace KMDSessionCreator
{
    public partial class Main : Form
    {
        public Main()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Data.Session s= new Data.Session();
            SessionForm f= (new SessionForm());
            f.FormClosed += SessionForm_FormClosed;
            f.ShowSession(s);
        }

        void SessionForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            SessionForm f=sender as SessionForm;
            Data.AppShared.Instance.Sessions.Sessions.Add(f.Session);
            lstSessions.Items.Add(f.Session);
        }

        private void Main_Load(object sender, EventArgs e)
        {
            Data.AppShared.Instance.Sessions = new Data.KMDSessions();
        }


        void _Fill(Data.KMDSessions s)
        {
            lstSessions.Items.Clear();
            Data.AppShared.Instance.Sessions = s;
            foreach(var c in s.Sessions)
            {
                lstSessions.Items.Add(c);
            }
        }

        private void button5_Click(object sender, EventArgs e)
        {
            if(MessageBox.Show("Start new sessions?","New Session",MessageBoxButtons.OKCancel)==System.Windows.Forms.DialogResult.OK)
                _Fill(new Data.KMDSessions());

        }

        private void button2_Click(object sender, EventArgs e)
        {
            if (lstSessions.SelectedIndex == -1)
                return;
            Data.AppShared.Instance.Sessions.Sessions.RemoveAt(lstSessions.SelectedIndex);
            lstSessions.Items.RemoveAt(lstSessions.SelectedIndex);
        }

        private void button4_Click(object sender, EventArgs e)
        {
            SaveFileDialog f = new SaveFileDialog();
            f.Filter = "XML Files | *.xml";
            if(f.ShowDialog()==System.Windows.Forms.DialogResult.OK)
            {
                Data.AppShared.Instance.Sessions.ExportXML(f.FileName);
            }
        }

        private void button3_Click(object sender, EventArgs e)
        {

            if (MessageBox.Show("Remove current data?", "New Session", MessageBoxButtons.OKCancel) == System.Windows.Forms.DialogResult.OK)
            {
                Data.AppShared.Instance.Sessions.Clear();
                OpenFileDialog f = new OpenFileDialog();
                f.Filter = "XML Files | *.xml";
                if (f.ShowDialog() == System.Windows.Forms.DialogResult.OK)
                {
                    Data.AppShared.Instance.Sessions.LoadXML(f.FileName);
                }
                _Fill(Data.AppShared.Instance.Sessions);
            }
        }

        private void lstSessions_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void lstSessions_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            if(lstSessions.SelectedItem==null)
                return;
            SessionForm f = (new SessionForm());
            f.ShowSession(lstSessions.SelectedItem as Data.Session);
        }

        private void button6_Click(object sender, EventArgs e)
        {
            if (lstSessions.SelectedIndex <= 0)
                return;
            int idx = lstSessions.SelectedIndex;
            Data.Session s= Data.AppShared.Instance.Sessions.Sessions[idx];
            Data.AppShared.Instance.Sessions.Sessions.RemoveAt(idx);
            Data.AppShared.Instance.Sessions.Sessions.Insert(idx - 1, s);
            _Fill(Data.AppShared.Instance.Sessions);
            lstSessions.SelectedIndex = idx - 1;

        }

        private void button7_Click(object sender, EventArgs e)
        {

            if (lstSessions.SelectedIndex ==-1 ||  lstSessions.SelectedIndex ==lstSessions.Items.Count-1)
                return;
            int idx = lstSessions.SelectedIndex;
            Data.Session s = Data.AppShared.Instance.Sessions.Sessions[idx];
            Data.AppShared.Instance.Sessions.Sessions.RemoveAt(idx);
            Data.AppShared.Instance.Sessions.Sessions.Insert(idx + 1, s);
            _Fill(Data.AppShared.Instance.Sessions);
            lstSessions.SelectedIndex = idx + 1;
        }
    }
}

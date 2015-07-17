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
    public partial class SessionForm : Form
    {
        Data.Session _session;
        public Data.Session Session
        {
            get { return _session; }
        }

        public SessionForm()
        {
            InitializeComponent();
        }

        void _FillFrom(Data.Session s)
        {
            _session = s;
            txtsession.Text = _session.Name;
            txtTheme.Text = _session.theme;
            chkIsSession.Checked = _session.IsSession;
            txtStartTime.Text = _session.StartTime.ToShortTimeString();
            txtEndTime.Text = _session.EndTime.ToShortTimeString();

            txtR.Text = _session.BackgroundColor.R.ToString();
            txtG.Text = _session.BackgroundColor.G.ToString();
            txtB.Text = _session.BackgroundColor.B.ToString();
            txtPicture.Text = _session.Picture;
            txtDesc.Text = _session.Description;

            foreach(var p in _session.Subprojects)
            {
                lstSubProj.Items.Add(p);
            }

            foreach (var p in _session.Committee)
            {
                lstComte.Items.Add(p);
            }

            foreach (var p in _session.Advisors)
            {
                lstAdvisors.Items.Add(p);
            }
        }

        void _RemoveAdvisor()
        {
            if (lstAdvisors.SelectedIndex == -1)
                return;
            _session.Advisors.RemoveAt(lstAdvisors.SelectedIndex);
            lstAdvisors.Items.RemoveAt(lstAdvisors.SelectedIndex);
        }
        void _RemoveCommittee()
        {
            if (lstComte.SelectedIndex == -1)
                return;
            _session.Committee.RemoveAt(lstComte.SelectedIndex);
            lstComte.Items.RemoveAt(lstComte.SelectedIndex);
        }
        void _RemoveSubProject()
        {
            if (lstSubProj.SelectedIndex == -1)
                return;
            _session.Subprojects.RemoveAt(lstSubProj.SelectedIndex);
            lstSubProj.Items.RemoveAt(lstSubProj.SelectedIndex);
        }
        void _AddAdvisor()
        {

            FacultyForm f = new FacultyForm();
            if (f.Show() == System.Windows.Forms.DialogResult.OK)
            {
                _session.Advisors.Add(f.Faculty);
                lstAdvisors.Items.Add(f.Faculty);
            }
        }
        void _AddCommittee()
        {

            FacultyForm f = new FacultyForm();
            if (f.Show() == System.Windows.Forms.DialogResult.OK)
            {
                _session.Committee.Add(f.Faculty);
                lstComte.Items.Add(f.Faculty);
            }
        }

        void _AddSubProject()
        {
            SubProjectForm f = new SubProjectForm();
            if(f.ShowForm(new Data.SubProject())==System.Windows.Forms.DialogResult.OK)
            {
                _session.Subprojects.Add(f.SubProject);
                lstSubProj.Items.Add(f.SubProject);
            }
        }

        public void ShowSession(Data.Session s)
        {
            _FillFrom(s);
            Show();
        }

        void _RefreshBGColor()
        {
            try
            {
                _session.BackgroundColor = Color.FromArgb(int.Parse(txtR.Text), int.Parse(txtG.Text), int.Parse(txtB.Text));

                lblClr.BackColor = _session.BackgroundColor;
            }catch (Exception e)
            {

            }
        }

        private void txtR_TextChanged(object sender, EventArgs e)
        {
            _RefreshBGColor();
        }


        private void button6_Click(object sender, EventArgs e)
        {
            _AddSubProject();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            _AddAdvisor();

        }

        private void button4_Click(object sender, EventArgs e)
        {
            _AddCommittee();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            _RemoveAdvisor();
        }

        private void button3_Click(object sender, EventArgs e)
        {
            _RemoveCommittee();
        }

        private void button5_Click(object sender, EventArgs e)
        {
            _RemoveSubProject();
        }

        private void SessionForm_VisibleChanged(object sender, EventArgs e)
        {
        }

        private void SessionForm_FormClosing(object sender, FormClosingEventArgs e)
        {

            _session.Name=txtsession.Text;
            _session.theme=txtTheme.Text;
            _session.IsSession=chkIsSession.Checked;
            DateTime.TryParse(txtStartTime.Text, out _session.StartTime);
            DateTime.TryParse(txtEndTime.Text, out _session.EndTime);

            _session.Picture=txtPicture.Text;
            _session.Description=txtDesc.Text;
        }

        private void lblClr_Click(object sender, EventArgs e)
        {

            ColorSelectorForm f = new ColorSelectorForm();
            if ((f).ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                txtR.Text = f.SelectedColor.R.ToString();
                txtG.Text = f.SelectedColor.G.ToString();
                txtB.Text = f.SelectedColor.B.ToString();
                _session.BackgroundColor = f.SelectedColor;
            }
        }

        private void lstSubProj_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void lstSubProj_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            if (lstSubProj.SelectedItem == null)
                return;
            SubProjectForm f = new SubProjectForm();
            if (f.ShowForm(lstSubProj.SelectedItem as Data.SubProject) == System.Windows.Forms.DialogResult.OK)
            {
            }
        }
    }
}

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
    public partial class FacultyForm : Form
    {

        public Data.Faculty Faculty;

        public FacultyForm()
        {
            InitializeComponent();
        }



        private void lstFac_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (lstFac.SelectedItem == null)
            {
                grpProp.Visible = false;
                return;
            }
            else
            {
                grpProp.Visible = true;
            }
            Faculty = lstFac.SelectedItem as Data.Faculty;
            _FillFaculty(Faculty);
        }

        void _Fill()
        {
            lstFac.Items.Clear();
            foreach (var f in Data.AppShared.Instance.Sessions.Faculties)
            {
                lstFac.Items.Add(f);
            }
        }

        void _FillFaculty(Data.Faculty f)
        {
            txtName.Text = f.Name;
            txtTitle.Text = f.Title;
            txtImg.Text = f.ImageURL;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Data.Faculty f = new Data.Faculty();
            lstFac.Items.Add(f);
            Data.AppShared.Instance.Sessions.Faculties.Add(f);
            lstFac.SelectedIndex = lstFac.Items.Count - 1;
        }

        private void button2_Click(object sender, EventArgs e)
        {
            if (lstFac.SelectedIndex != -1)
            {
                lstFac.Items.Remove(lstFac.SelectedIndex);
                if (lstFac.Items.Count > 0)
                    lstFac.SelectedIndex = 0;
                else
                {
                    Faculty = null;
                }
            }
        }

        public DialogResult Show()
        {
            _Fill();
            return ShowDialog();
        }

        private void txtName_TextChanged(object sender, EventArgs e)
        {
            Faculty.Name = txtName.Text;
        }

        private void txtTitle_TextChanged(object sender, EventArgs e)
        {
            Faculty.Title = txtTitle.Text;

        }

        private void txtImg_TextChanged(object sender, EventArgs e)
        {
            Faculty.ImageURL = txtImg.Text;
        }

        private void txtName_Leave(object sender, EventArgs e)
        {
            lstFac.Items[lstFac.SelectedIndex] = Faculty;
        }

        private void lstFac_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            DialogResult = System.Windows.Forms.DialogResult.OK;
            Close();
        }
    }
}

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
    public partial class SubProjectForm : Form
    {

        public Data.SubProject SubProject;

        public SubProjectForm()
        {
            InitializeComponent();
        }

        private void SubProjectForm_Load(object sender, EventArgs e)
        {

        }

        void _Fill()
        {
            txtName.Text = SubProject.Name;
            txtTitle.Text = SubProject.Title;
            txtLength.Text = SubProject.Length.ToString();
        }

        public DialogResult ShowForm(Data.SubProject p)
        {
            SubProject = p;
            _Fill();
            DialogResult = System.Windows.Forms.DialogResult.Cancel;
            return ShowDialog();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            DialogResult = System.Windows.Forms.DialogResult.OK;
            SubProject.Name = txtName.Text;
            SubProject.Title = txtTitle.Text;
            int.TryParse(txtLength.Text, out SubProject.Length);

            Close();
        }
    }
}

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Xml;

namespace GUISkinExporter
{
    public partial class MainForm : Form
    {
        RegionRenderer m_renderer = new RegionRenderer();
        GUITheme m_theme = new GUITheme();
        GUIProfile m_currentProfile;
        string imageFileName;
        string ThemeFileNameTxt;
        Point mouseLastPos;
        public MainForm()
        {
            InitializeComponent();
        }

        private void button3_Click(object sender, EventArgs e)
        {
            DialogResult res= openFileDialog1.ShowDialog();
            if (res == DialogResult.OK)
            {
                imageFileName = openFileDialog1.SafeFileName;
                m_renderer.baseImage = Image.FromFile(openFileDialog1.FileName);
            }
        }

        private void Form1_Load(object sender, EventArgs e)
        {
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (m_currentProfile==null)
            {
                MessageBox.Show("Select or create a profile first");
                return;
            }
            GUIRegion r = m_currentProfile.CreateRegion(m_renderer);
            m_currentProfile.SelectedRegion=r;
            propertyGrid1.SelectedObject = r;
            m_renderer.drawRegions(panel1.CreateGraphics(), panel1.DisplayRectangle, m_currentProfile);
        }

        private void panel1_Paint(object sender, PaintEventArgs e)
        {
            if (m_currentProfile == null)
            {
                return;
            }
            m_renderer.drawRegions(panel1.CreateGraphics(), panel1.DisplayRectangle, m_currentProfile);
        }

        private void panel1_Resize(object sender, EventArgs e)
        {
            if (m_currentProfile == null)
            {
                return;
            }
            m_renderer.drawRegions(panel1.CreateGraphics(), panel1.DisplayRectangle, m_currentProfile);
        }

        private void panel1_MouseMove(object sender, MouseEventArgs e)
        {
            if (m_currentProfile == null)
            {
                return;
            }
            m_renderer.drawRegions(panel1.CreateGraphics(), panel1.DisplayRectangle, m_currentProfile);
            m_renderer.performMove(e.X, e.Y, m_currentProfile);

            if (e.Button == MouseButtons.Right)
            {
                int dy = (int)((e.Y - mouseLastPos.Y)* 0.5f);
                m_renderer.Scale += dy*0.2f ;
                scaleFactorTxt.Text = m_renderer.Scale.ToString();
                m_renderer.drawRegions( panel1.CreateGraphics(), panel1.DisplayRectangle, m_currentProfile);
            }

            if (e.Button==MouseButtons.Middle)
            {
                int dx = e.X - mouseLastPos.X;
                int dy = e.Y - mouseLastPos.Y;
                Point p = new Point(m_renderer.Translate.X + (int)(2.0f * m_renderer.Scale) * dx,
                    m_renderer.Translate.Y + (int)(2.0f * m_renderer.Scale) * dy);
                m_renderer.Translate = p;
                m_renderer.drawRegions( panel1.CreateGraphics(), panel1.DisplayRectangle, m_currentProfile);
            }
            mouseLastPos.X = e.X;
            mouseLastPos.Y = e.Y;
        }

        private void panel1_MouseDown(object sender, MouseEventArgs e)
        {
            if (m_currentProfile == null)
            {
                return;
            }
            m_renderer.performClick(e.X, e.Y, e.Button, m_currentProfile);
            propertyGrid1.SelectedObject = m_currentProfile.SelectedRegion;
        }

        private void panel1_MouseUp(object sender, MouseEventArgs e)
        {
            if (m_currentProfile == null)
            {
                return;
            }
            m_renderer.performMouseUp(e.X, e.Y, e.Button, m_currentProfile);
        }

        private void button5_Click(object sender, EventArgs e)
        {
            if (m_currentProfile == null)
            {
                return;
            }
            m_renderer.Scale += 0.1f;
            scaleFactorTxt.Text = m_renderer.Scale.ToString();
            m_renderer.drawRegions( panel1.CreateGraphics(), panel1.DisplayRectangle, m_currentProfile);
        }

        private void button4_Click(object sender, EventArgs e)
        {
            m_renderer.Scale -= 0.1f;
            scaleFactorTxt.Text = m_renderer.Scale.ToString();
            if (m_currentProfile == null)
            {
                return;
            }
            m_renderer.drawRegions( panel1.CreateGraphics(), panel1.DisplayRectangle, m_currentProfile);
        }

        private void scaleFactorTxt_TextChanged(object sender, EventArgs e)
        {
            m_renderer.Scale = (float)Convert.ToDouble(scaleFactorTxt.Text);
            if (m_currentProfile == null)
            {
                return;
            }
            scaleFactorTxt.Text = m_renderer.Scale.ToString();
            m_renderer.drawRegions( panel1.CreateGraphics(), panel1.DisplayRectangle, m_currentProfile);
        }

        private void button6_Click(object sender, EventArgs e)
        {
            Point p = new Point(m_renderer.Translate.X - (int)(10.0f * m_renderer.Scale), m_renderer.Translate.Y);
            m_renderer.Translate = p;
            if (m_currentProfile == null)
            {
                return;
            }
            m_renderer.drawRegions( panel1.CreateGraphics(), panel1.DisplayRectangle, m_currentProfile);
        }

        private void button7_Click(object sender, EventArgs e)
        {
            Point p = new Point(m_renderer.Translate.X + (int)(10.0f * m_renderer.Scale), m_renderer.Translate.Y);
            m_renderer.Translate = p;
            if (m_currentProfile == null)
            {
                return;
            }
            m_renderer.drawRegions( panel1.CreateGraphics(), panel1.DisplayRectangle, m_currentProfile);
        }

        private void button8_Click(object sender, EventArgs e)
        {
            Point p = new Point(m_renderer.Translate.X, m_renderer.Translate.Y + (int)(10.0f * m_renderer.Scale));
            m_renderer.Translate = p;
            if (m_currentProfile == null)
            {
                return;
            }
            m_renderer.drawRegions( panel1.CreateGraphics(), panel1.DisplayRectangle, m_currentProfile);
        }

        private void button9_Click(object sender, EventArgs e)
        {
            Point p = new Point(m_renderer.Translate.X, m_renderer.Translate.Y - (int)(10.0f * m_renderer.Scale));
            m_renderer.Translate = p;
            if (m_currentProfile == null)
            {
                return;
            }
            m_renderer.drawRegions( panel1.CreateGraphics(), panel1.DisplayRectangle, m_currentProfile);
        }

        private void button10_Click(object sender, EventArgs e)
        {
            Point p = new Point(0, 0);
            m_renderer.Translate = p;
            m_renderer.Scale = 1;
            if (m_currentProfile == null)
            {
                return;
            }
            m_renderer.drawRegions( panel1.CreateGraphics(), panel1.DisplayRectangle, m_currentProfile);
        }

        void export(string name)
        {

            if (m_renderer.baseImage == null)
            {
                MessageBox.Show("Image Not Set");
                return;
            }
            XmlTextWriter writer = new XmlTextWriter(name, Encoding.ASCII);
            writer.Formatting = Formatting.Indented;
            writer.WriteStartElement("GUITheme");
            writer.WriteAttributeString("Name", ThemeNameTxt.Text);
            writer.WriteAttributeString("source", imageFileName);
            float invW = 1.0f / (float)m_renderer.baseImage.Width;
            float invH = 1.0f / (float)m_renderer.baseImage.Height;
            writer.WriteAttributeString("Width", m_renderer.baseImage.Width.ToString());
            writer.WriteAttributeString("Height", m_renderer.baseImage.Height.ToString());

            for (int j = 0; j < m_theme.Profiles.Count; j++)
            {
                GUIProfile p = m_theme.Profiles[j];
                writer.WriteStartElement("Profile");
                writer.WriteAttributeString("Name", p.Name);

                List<GUIRegion> regions = p.Regions;
                for (int i = 0; i < regions.Count; i++)
                {
                    RectangleF r = regions[i].Rect;
                    writer.WriteStartElement("Region");
                    writer.WriteAttributeString("Name", regions[i].Name);
                    writer.WriteAttributeString("ID", regions[i].ID.ToString());
                    writer.WriteAttributeString("X", r.X.ToString());
                    writer.WriteAttributeString("Y", r.Y.ToString());
                    writer.WriteAttributeString("Width", r.Width.ToString());
                    writer.WriteAttributeString("Height", r.Height.ToString());

                    float u = invW * (float)r.X;
                    float v = invH * (float)r.Y;
                    float du = invW * (float)r.Width;
                    float dv = invH * (float)r.Height;
                    writer.WriteAttributeString("U", u.ToString());
                    writer.WriteAttributeString("V", v.ToString());
                    writer.WriteAttributeString("dU", du.ToString());
                    writer.WriteAttributeString("dV", dv.ToString());
                    writer.WriteEndElement();
                }
                writer.WriteEndElement();
            }

            writer.WriteEndElement();
            writer.Close();
        
        }
        private void button11_Click(object sender, EventArgs e)
        {
            DialogResult res = saveFileDialog1.ShowDialog();
            if (res == DialogResult.OK)
            {
                ThemeFileNameTxt = saveFileDialog1.FileName;
                export(saveFileDialog1.FileName);
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            propertyGrid1.SelectedObject = null;
            if (m_currentProfile==null)
            {
                return;
            }
            m_currentProfile.RemoveSelectedRegion();
            m_renderer.drawRegions( panel1.CreateGraphics(), panel1.DisplayRectangle, m_currentProfile);
        }

        void import(string name)
        {

            m_currentProfile = null;
            m_theme.ClearProfiles();

            bool readingImage = false;
            bool readingProfile = false;
            bool readingRegion = false;

            XmlReaderSettings settings = new XmlReaderSettings();
            settings.IgnoreWhitespace = true;
            settings.ConformanceLevel = ConformanceLevel.Fragment;
            settings.IgnoreComments = true;
            XmlReader reader = XmlReader.Create(name, settings);
            GUIRegion r = null;
            GUIProfile p = null;
            while (reader.Read())
            {

                if (reader.IsStartElement())
                {
                    string n = reader.Name;
                    if (!reader.IsEmptyElement)
                    {
                        string v = reader.Value;
                    }

                    if (n == "GUITheme")
                    {
                        readingImage = true;
                        readingRegion = false;
                        readingProfile = false;
                    }

                    if (n == "Profile")
                    {
                        readingProfile = true;
                        readingImage = false;
                        readingRegion = false;
                    }
                    if (n == "Region")
                    {
                        readingRegion = true;
                        readingImage = false;
                        readingProfile = false;
                    }
                }
                if (reader.HasAttributes)
                {
                    reader.MoveToFirstAttribute();
                    do
                    {
                        string an = reader.Name;

                        if (readingImage)
                        {
                            switch (an)
                            {
                                case "source":
                                    m_renderer.baseImage = Image.FromFile(reader.Value);
                                    imageFileName = reader.Value;
                                    break;
                                case "Name":
                                    ThemeNameTxt.Text = reader.Value;
                                    break;
                            }
                        }
                        else if (readingProfile)
                        {
                            switch (an)
                            {
                                case "Name":
                                    p = m_theme.AddProfile(reader.Value);
                                    break;
                            }
                        }
                        else if (readingRegion)
                        {
                            if (p == null)
                            {
                                throw new Exception("Error while parsing xml file, there is no Profile for the region ..");
                            }
                            switch (an)
                            {
                                case "Name":
                                    {
                                        r = p.CreateRegion(m_renderer);
                                        r.Name = reader.Value;
                                    }
                                    break;
                                case "ID":
                                    {
                                        if(r==null)
                                            r = p.CreateRegion(m_renderer);

                                        r.ID = reader.ReadContentAsInt();
                                    } break;
                                case "X":
                                    {
                                        if (r == null)
                                        {
                                            throw new Exception("Error parsing xml document - Name expected");
                                        }
                                        Rectangle rect = r.Rect;
                                        rect.X = reader.ReadContentAsInt();
                                        r.Rect = rect;
                                    } break;
                                case "Y":
                                    {
                                        if (r == null)
                                        {
                                            throw new Exception("Error parsing xml document - Name expected");
                                        }
                                        Rectangle rect = r.Rect;
                                        rect.Y = reader.ReadContentAsInt();
                                        r.Rect = rect;
                                    } break;
                                case "Width":
                                    {
                                        if (r == null)
                                        {
                                            throw new Exception("Error parsing xml document - Name expected");
                                        }
                                        Rectangle rect = r.Rect;
                                        rect.Width = reader.ReadContentAsInt();
                                        r.Rect = rect;
                                    } break;
                                case "Height":
                                    {
                                        if (r == null)
                                        {
                                            throw new Exception("Error parsing xml document - Name expected");
                                        }
                                        Rectangle rect = r.Rect;
                                        rect.Height = reader.ReadContentAsInt();
                                        r.Rect = rect;
                                    } break;
                            }
                        }
                    }
                    while (reader.MoveToNextAttribute());
                }
                reader.MoveToElement();
            }
            reader.Close();

            refreshProfile();
            ThemeFileNameTxt = name;
        }
        private void button12_Click(object sender, EventArgs e)
        {
            DialogResult res = XMLFileDialog.ShowDialog();
            if (res == DialogResult.OK)
            {
                import(XMLFileDialog.FileName);
            }
        }

        private void panel1_Scroll(object sender, ScrollEventArgs e)
        {

        }

        private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            MessageBox.Show("This Utility been created by : Mohamad Yamen Saraiji", "About", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }
        private void refreshProfile()
        {
            ProfileList.Items.Clear();
            for (int i = 0; i < m_theme.Profiles.Count; i++)
            {
                ProfileList.Items.Add(m_theme.Profiles[i].Name);
                if (m_currentProfile != null && m_currentProfile.Name == m_theme.Profiles[i].Name)
                {
                    ProfileList.SelectedIndex = i;
                }
            }
        }

        private void button3_Click_1(object sender, EventArgs e)
        {
            if (ProfileNameTxt.Text == "") return;
            if (m_theme.findProfile(ProfileNameTxt.Text) != null)
            {
                MessageBox.Show("Profile Name is already Exist!");
                return;
            }
            m_currentProfile = m_theme.AddProfile(ProfileNameTxt.Text);

            refreshProfile();
            this.Refresh();
        }

        private void ProfileList_SelectedIndexChanged(object sender, EventArgs e)
        {
            m_currentProfile = m_theme.findProfile(ProfileList.SelectedItem.ToString());
            this.Refresh();
        }

        private void button11_Click_1(object sender, EventArgs e)
        {
            if (m_currentProfile!=null)
            {
                m_theme.RemoveProfile(m_currentProfile.Name);
                m_currentProfile = m_theme.getFirst();
                this.Refresh();
            }
        }

        private void propertyGrid1_PropertyValueChanged(object s, PropertyValueChangedEventArgs e)
        {
            this.panel1_Paint(null, null);
        }

        private void button12_Click_1(object sender, EventArgs e)
        {
            if (m_currentProfile!=null)
            {
                m_currentProfile.Name = ProfileNameTxt.Text;
                this.refreshProfile();
            }
        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            m_renderer.ShowNames = checkBox1.Checked;
            this.panel1_Paint(null, null);
        }

        void duplicateSelected()
        {
            if (m_currentProfile != null && m_currentProfile.SelectedRegion != null)
            {
                GUIRegion s = m_currentProfile.SelectedRegion;
                GUIRegion r = m_currentProfile.CreateRegion(m_renderer);
                r.Rect = s.Rect;
                r.m_regionRect.X += 3;
                r.m_regionRect.Y += 3;
                r.Name = s.Name;
                r.ID = m_currentProfile.Regions.Count-1;
                m_currentProfile.SelectedRegion=r;
            }
        }
        private void button13_Click(object sender, EventArgs e)
        {
            duplicateSelected();
        }

        private void menuStrip1_KeyDown(object sender, KeyEventArgs e)
        {
            if (m_currentProfile != null && m_currentProfile.SelectedRegion != null)
            {
                m_currentProfile.RemoveSelectedRegion();
            }

        }


        private void duplicateRegionToolStripMenuItem_Click(object sender, EventArgs e)
        {
            duplicateSelected();
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            DialogResult res= MessageBox.Show("Do yo want save the changes?", "Exit", MessageBoxButtons.YesNoCancel);
            switch (res)
            {
                case DialogResult.Cancel:
                    e.Cancel = true;
                    break;
                case DialogResult.Yes:
                    if (ThemeFileNameTxt == "")
                    {

                        res = saveFileDialog1.ShowDialog();
                        if (res == DialogResult.OK)
                            export(saveFileDialog1.FileName);
                    }else
                        export(ThemeFileNameTxt);
                    break;
                case DialogResult.No:
                    break;
            }
        }


    }
}
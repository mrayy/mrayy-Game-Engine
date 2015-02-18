using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace ImageSetGenerator
{
    public partial class Divider : Form
    {
        string m_basePath = "";
        string m_filePath = "";
        public Divider()
        {
            InitializeComponent();
            AllowDrop = true;
            DragEnter += new DragEventHandler(Divider_DragEnter);
            DragDrop += new DragEventHandler(Divider_DragDrop);
        }

        void Divider_DragDrop(object sender, DragEventArgs e)
        {
            string[]files=(string[])e.Data.GetData(DataFormats.FileDrop);
            m_filePath=m_basePath = files[0];
            string dir = "";
            int i=m_basePath.LastIndexOf('\\');
            if (i != -1)
            {
                dir = m_basePath.Substring(0, i+1);
                dir += "Splits\\";
                m_basePath = dir + m_basePath.Substring(i + 1);
            }

            i = m_basePath.LastIndexOf('.'); ;
            if (i != -1)
            {
                m_basePath = m_basePath.Substring(0, i);
            }
            pictureBox1.Image = Image.FromFile(files[0]);
        }

        void Divider_DragEnter(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent(DataFormats.FileDrop))
                e.Effect = DragDropEffects.Copy;
            
        }

        private void pictureBox1_DragDrop(object sender, DragEventArgs e)
        {
            
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (m_filePath == "")
            {
                return;
            }

            pictureBox1.Image = Image.FromFile(m_filePath);


            Size sz= pictureBox1.Image.Size;

            int cols;
            int rows;
            if (!int.TryParse(rowsTxt.Text, out rows))
                return;
            if (!int.TryParse(colsTxt.Text, out cols))
                return;

            float w = sz.Width / cols;
            float h = sz.Height / rows;

            Bitmap bmp = new Bitmap((int)w, (int)h, 
                System.Drawing.Imaging.PixelFormat.Format32bppArgb);


            Graphics g = Graphics.FromImage(bmp);
            g.CompositingMode = System.Drawing.Drawing2D.CompositingMode.SourceCopy;
            g.CompositingQuality = System.Drawing.Drawing2D.CompositingQuality.HighQuality;

            int i = 0;
            for (int y = 0; y < rows; ++y)
            {
                for (int x = 0; x < cols; ++x)
                {
                    g.Clear(Color.FromArgb(0, 0, 0, 0));
                    g.DrawImage(pictureBox1.Image, new Rectangle(0, 0, (int)w, (int)h), new Rectangle((int)(x * w), (int)(y * h), (int)w, (int)h), GraphicsUnit.Pixel);

                    string path = m_basePath + i.ToString()+".png";
                    bmp.Save(path, System.Drawing.Imaging.ImageFormat.Png);
                    ++i;
                }
            }
            g.Dispose();
        }

        private void button2_Click(object sender, EventArgs e)
        {

            if (pictureBox1.Image == null)
            {
                return;
            }

            Size sz = pictureBox1.Image.Size;

            int cols;
            int rows;
            if (!int.TryParse(rowsTxt.Text, out rows))
                return;
            if (!int.TryParse(colsTxt.Text, out cols))
                return;

            float w = sz.Width / cols;
            float h = sz.Height / rows;

            Graphics g= pictureBox1.CreateGraphics();

            Pen p=new Pen(Color.Black,1);

            for (int y = 0; y < rows; ++y)
            {
                g.DrawLine(p, 0, y*h, sz.Width, y*h);
            }
            for (int x = 0; x < cols; ++x)
            {
                g.DrawLine(p, x*w, 0, x*w, sz.Height);
            }

            g.Dispose();
        }
    }
}
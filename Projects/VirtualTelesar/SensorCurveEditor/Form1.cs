using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Xml;
using System.IO;

namespace SensorCurveEditor
{
    public partial class CurveEditorForm : Form
    {
        public CurveEditorForm()
        {
            InitializeComponent();
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void numMinIn_ValueChanged(object sender, EventArgs e)
        {
            if (numMaxIn.Value < numMinIn.Value)
                numMaxIn.Value = numMinIn.Value;

            curveEditor.MinX = (float)numMinIn.Value;
            curveEditor.MaxX = (float)numMaxIn.Value;
        }

        private void numMaxIn_ValueChanged(object sender, EventArgs e)
        {
            if (numMinIn.Value > numMaxIn.Value)
                numMinIn.Value = numMaxIn.Value;
            curveEditor.MinX = (float)numMinIn.Value;
            curveEditor.MaxX = (float)numMaxIn.Value;

        }

        private void numMinOut_ValueChanged(object sender, EventArgs e)
        {
            if (numMaxOut.Value < numMinOut.Value)
                numMaxOut.Value = numMinOut.Value;
            curveEditor.MinY = (float)numMinOut.Value;
            curveEditor.MaxY = (float)numMaxOut.Value;

        }

        private void numMaxOut_ValueChanged(object sender, EventArgs e)
        {
            if (numMinOut.Value > numMaxOut.Value)
                numMinOut.Value = numMaxOut.Value;
            curveEditor.MinY = (float)numMinOut.Value;
            curveEditor.MaxY = (float)numMaxOut.Value;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Export();
        }


        float[] TakeSamples()
        {
            float[] res = new float[curveEditor.LevelValue.Length];
            for (int i = 0; i < curveEditor.LevelValue.Length;++i )
            {
                res[i] = curveEditor.LevelValue[i] / 255.0f;
            }
            return res;
            
        }

        void Import()
        {
        }
        void Export()
        {

            SaveFileDialog dlg = new SaveFileDialog();
            if (dlg.ShowDialog() == DialogResult.OK)
            {
                Export(dlg.FileName);
            }
        }
        void Export2(string path)
        {
            float[] samples = TakeSamples();

            StreamWriter wrtr = new StreamWriter(File.OpenWrite(path));
            for (int j = 0; j < samples.Length; j+=25)
            {
                wrtr.WriteLine((samples[j]*curveEditor.MaxY).ToString());
            }
            wrtr.Close();

        }
        void Export(string path)
        {
            float[] samples = TakeSamples();

            XmlTextWriter writer = new XmlTextWriter(path, Encoding.ASCII);
            writer.Formatting = Formatting.Indented;
            writer.WriteStartElement("Modulator");
            writer.WriteAttributeString("Name", txtName.Text);
            writer.WriteAttributeString("InMin", numMinIn.Value.ToString());
            writer.WriteAttributeString("InMax", numMaxIn.Value.ToString());
            writer.WriteAttributeString("OutMin", numMinOut.Value.ToString());
            writer.WriteAttributeString("OutMax", numMaxOut.Value.ToString());
            for (int j = 0; j < samples.Length; j++)
            {
                writer.WriteStartElement("Key");
                writer.WriteAttributeString("Value", samples[j].ToString());
                writer.WriteEndElement();
            }
            writer.WriteEndElement();
            writer.Close();
        }

        private void CurveEditorForm_Load(object sender, EventArgs e)
        {

        }
    }
}

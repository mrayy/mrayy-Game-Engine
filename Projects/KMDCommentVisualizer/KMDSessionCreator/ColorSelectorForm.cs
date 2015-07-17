using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace KMDSessionCreator
{
    public partial class ColorSelectorForm : Form
    {

        class ColorInfo
        {
            public string Name;
            public Color color;

            public override string ToString()
            {
                return Name+" : "+color.ToString();
            }
        }
        List<ColorInfo> colors = new List<ColorInfo>();

        ColorInfo _selectedColor;
        public Color SelectedColor
        {
            get { return _selectedColor.color; }
        }

        public ColorSelectorForm()
        {
            InitializeComponent();
            foreach (var prop in typeof(System.Drawing.Color).GetProperties(BindingFlags.Public | BindingFlags.Static))
            {
                if (prop.PropertyType == typeof(System.Drawing.Color))
                {
                    AddColor(prop.Name,(System.Drawing.Color)prop.GetValue(null));
                }
            }
        }


        void AddColor(string name,Color c)
        {
            ColorInfo clr = new ColorInfo();
            clr.Name = name;
            clr.color = c;

            colors.Add(clr);
            lstColor.Items.Add(clr);
        }

        private void lstColor_SelectedIndexChanged(object sender, EventArgs e)
        {
            _selectedColor=(lstColor.SelectedItem as ColorInfo);
            lblClr.BackColor = _selectedColor.color;
        }

        private void lstColor_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            this.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.Close();
        }
    }
}

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using mrayGraphLibNet;
using mrayGraphLibNet.Drawing;
using AIRuleDesigner.Shapes;
using System.Threading;

namespace AIRuleDesigner
{
    public partial class Main : Form
    {
        public Main()
        {
            InitializeComponent();
           // new SplashScreen();
        }


        private void timer1_Tick(object sender, EventArgs e)
        {
            drawCanvas1.drawObjects();
        }

        private void Main_Load(object sender, EventArgs e)
        {

        }

        private void drawCanvas1_MouseMove(object sender, MouseEventArgs e)
        {
            drawCanvas1.onMouseMove(e.X, e.Y,e.Button);
        }

        private void drawCanvas1_MouseDown(object sender, MouseEventArgs e)
        {
            drawCanvas1.onMouseDown(e.X, e.Y,e.Button);
            propertyGrid1.SelectedObject = drawCanvas1.selectedShape;
            
        }

        private void drawCanvas1_Load(object sender, EventArgs e)
        {

        }

        private void toolStripSplitButton1_ButtonClick(object sender, EventArgs e)
        {

        }

        private void boxToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Shape b = new BoxShape(drawCanvas1, new Vector2d(0, 0), new Vector2d(100, 50), null);
            drawCanvas1.addShape(b);
        }
        private void ellipseToolStripMenuItem_Click(object sender, EventArgs e)
        {

            Shape b = new CompareOperator(drawCanvas1, new Vector2d(0, 0),EOperatorType.Equal, null);
            drawCanvas1.addShape(b);
        }
        private void circleToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Shape b = new ConditionList(drawCanvas1, ConditionCollectionType.AND, new Vector2d(0, 0), new Vector2d(100, 50), null);
            drawCanvas1.addShape(b);
        }
        private void button1_Click(object sender, EventArgs e)
        {
            if (drawCanvas1.selectedShape != null)
            {
                drawCanvas1.selectedShape.addInputSocket();
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            if (drawCanvas1.selectedShape != null)
            {
                drawCanvas1.selectedShape.addOutputSocket();
            }
        }



    }
}
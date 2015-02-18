

using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Windows.Forms;


namespace GUISkinExporter
{
    public class RegionRenderer
    {
        float m_scale = 1;
        Point lastMousePos=new Point();
        Point m_translate = new Point();
        PointF dmove = new PointF();
        Point lastRegionPos = new Point();
       
        public Image baseImage;

        bool mousePressed=false;

        bool m_showNames = true;
        Rectangle m_lastRect=new Rectangle();

        public float Scale
        {
            set
            {
                if (value <= 0.5)
                {
                    m_scale = 0.5f;
                    return;
                }
                m_scale = value;
            }
            get
            {
                return m_scale;
            }
        }
        public Point Translate
        {
            set
            {
                m_translate = value;
            }
            get
            {
                return m_translate;
            }
        }
        public bool ShowNames
        {
            set
            {
                m_showNames = value;
            }
            get
            {
                return m_showNames;
            }
        }

        public void performMouseUp(int X, int Y, MouseButtons buttons, GUIProfile p)
        {
            mousePressed = false;

        }

        public bool performClick(int X, int Y, MouseButtons buttons, GUIProfile p)
        {
            X -= m_translate.X;
            Y -= m_translate.Y;



            if (buttons == MouseButtons.Left)
            {
                mousePressed = true;
            }
            if (buttons == MouseButtons.Right)
            {
                p.SelectedRegion=null;
            }
            if (mousePressed)
            {
                lastMousePos.X = X;
                lastMousePos.Y = Y;
                dmove.X = 0;
                dmove.Y = 0;
            }

            float tmpX = X;
            float tmpY = Y;
            tmpX /= Scale;
            tmpY /= Scale;
            X = (int)tmpX;
            Y = (int)tmpY;

            Point pt = new Point(X, Y);

            GUIRegion lastSelected = p.SelectedRegion;
            GUIRegion selected = null;
            p.SelectedRegion=null;
            int i;
            for (i = 0; i < p.Regions.Count; i++)
            {
                if (p.Regions[i].OnMouseDown(pt, this))
                {
                    selected = p.Regions[i];
                }
            }
            if (selected != lastSelected)
                p.SelectedRegion=selected;
            else
                p.SelectedRegion = lastSelected;
            if (p.SelectedRegion != null)
            {
                lastRegionPos.X = p.SelectedRegion.RegionRect.X;
                lastRegionPos.Y = p.SelectedRegion.RegionRect.Y;
                return true;
            }
            return false;
        }

        public void performMove(int X, int Y,GUIProfile rc)
        {
            X -= m_translate.X;
            Y -= m_translate.Y;

            float dx = ((X - lastMousePos.X)/Scale);
            float dy = ((Y - lastMousePos.Y) / Scale);
            /*
            if (baseImage != null && mousePressed)
            {
                double invW = 1.0 / baseImage.Size.Width;
                double invH = 1.0 / baseImage.Size.Height;
                float rx = (float)Math.IEEERemainder((double)dx, invW);
                float ry= (float)Math.IEEERemainder((double)dy, invH);
            }
            */
            if (rc.SelectedRegion != null && mousePressed)
            {

                if (!rc.SelectedRegion.ScaleRegion((int)dx, (int)dy, this))
                {
                    rc.SelectedRegion.m_regionRect.X = lastRegionPos.X + (int)dx;
                    rc.SelectedRegion.m_regionRect.Y = lastRegionPos.Y + (int)dy;
                }
            }
        }


        public void drawRegions(Graphics graph, Rectangle displayRect, GUIProfile rc)
        {
            BufferedGraphics myBuffer;
            BufferedGraphicsContext context = BufferedGraphicsManager.Current;
            myBuffer = context.Allocate(graph, displayRect);
            Graphics G = myBuffer.Graphics;
            G.TranslateTransform(m_translate.X, m_translate.Y);
            G.ScaleTransform(Scale, Scale);
            if (baseImage != null)
            {
                G.CompositingQuality = System.Drawing.Drawing2D.CompositingQuality.HighSpeed;
                G.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.HighSpeed;
                G.Clear(Color.Pink);
                Rectangle r = new Rectangle(displayRect.X, displayRect.Y, baseImage.Width, baseImage.Height);
                G.DrawImage(baseImage, r);
                m_lastRect = r;
            }
            /* */
            graph.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.AntiAlias;


            for (int i = 0; i < rc.Regions.Count; i++)
            {
                rc.Regions[i].Paint(G,this);
            }
            myBuffer.Render(graph);
            myBuffer.Dispose();/**/
            G.Dispose();
        }
    }
}

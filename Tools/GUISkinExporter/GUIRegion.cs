using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;

namespace GUISkinExporter
{
    public class GUIRegion
    {
        string m_name;
        int m_id;
        public int ID
        {
            set
            {
                m_id = value;
            }
            get
            {
                return m_id;
            }
        }
        public string Name
        {
            set
            {
                m_name = value;
            }
            get
            {
                return m_name;
            }
        }
        
        ////

        GUIProfile m_profile;
        public GUIProfile Profile
        {
            get
            {
                return m_profile;
            }
        }

        public Rectangle m_regionRect = new Rectangle();

        public Rectangle RegionRect
        {
            get
            {
                return m_regionRect;
            }
        }

        bool m_scaleRectSelect = false;
        Point m_lastSize = new Point();
        public Rectangle Rect
        {
            set
            {
                m_regionRect = value;
            }
            get
            {
                return m_regionRect;
            }
        }



        public GUIRegion(GUIProfile p)
        {
            m_profile = p;
            m_regionRect.Width = 20;
            m_regionRect.Height = 20;
        }

        public bool Selected
        {
            get
            {
                return m_profile.SelectedRegion == this;
            }
        }


        public bool OnMouseDown(Point p, RegionRenderer rr)
        {
            RectangleF r = new RectangleF();
            r.Width = 10 / rr.Scale;
            r.Height = 10 / rr.Scale;
            if (r.Width < 2) r.Width = 2;
            if (r.Height < 2) r.Height = 2;
            r.X = m_regionRect.X + m_regionRect.Width - r.Width / 2;
            r.Y = m_regionRect.Y + m_regionRect.Height - r.Height / 2;
            if (r.Contains(p))
            {
                m_scaleRectSelect = true;
                m_lastSize.X = m_regionRect.Width;
                m_lastSize.Y = m_regionRect.Height;
                return true;
            }
            m_scaleRectSelect = false;
            return m_regionRect.Contains(p);
        }
        public bool ScaleRegion(float dx, float dy, RegionRenderer rr)
        {
            if (m_scaleRectSelect)
            {
                m_regionRect.Width = m_lastSize.X + (int)dx;
                m_regionRect.Height = m_lastSize.Y + (int)dy;
                return true;
            }
            return false;
        }
        public void Paint(Graphics g, RegionRenderer rr)
        {
            Pen p;
            g.DrawString(ID.ToString(), new Font(FontFamily.GenericSansSerif, 12 / rr.Scale), Brushes.Red, m_regionRect.X, m_regionRect.Y);
            if (rr.ShowNames)
            {
                g.DrawString(Name, new Font(FontFamily.GenericSansSerif, 16 / rr.Scale), Brushes.Blue, m_regionRect.X, m_regionRect.Y + 15 / rr.Scale);
            }
            if (Selected)
            {
                p = new Pen(Brushes.Blue, 4 / rr.Scale);
            }
            else
                p = new Pen(Brushes.Gray, 4 / rr.Scale);

            g.DrawRectangle(p, m_regionRect.X, m_regionRect.Y, m_regionRect.Width, m_regionRect.Height);
            if (Selected)
            {
                RectangleF r = new RectangleF();
                r.Width = 10 / rr.Scale;
                r.Height = 10 / rr.Scale;
                if (r.Width < 2) r.Width = 2;
                if (r.Height < 2) r.Height = 2;
                r.X = m_regionRect.X + m_regionRect.Width - r.Width / 2;
                r.Y = m_regionRect.Y + m_regionRect.Height - r.Height / 2;
                g.DrawRectangle(p, r.X, r.Y, r.Width, r.Height);
                r.Width -= 2;
                r.Height -= 2;
                r.X += 1;
                r.Y += 1;
                g.FillRectangle(Brushes.White, r);
            }
        }

    }
}

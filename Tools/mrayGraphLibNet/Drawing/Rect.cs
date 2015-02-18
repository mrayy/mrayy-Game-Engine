using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;

namespace mrayGraphLibNet.Drawing
{
    public class Rect
    {
        float m_x;
        float m_y;
        float m_w;
        float m_h;

        public float X
        {
            get { return m_x; }
            set { m_x = value; }
        }
        public float Y
        {
            get { return m_y; }
            set { m_y = value; }
        }
        public float Width
        {
            get { return m_w; }
            set { m_w = value; }
        }

        public float Height
        {
            get { return m_h; }
            set { m_h = value; }
        }

        public override string ToString()
        {
            return X+","+Y+","+Width+","+Height;
        }

        public Vector2d getSize()
        {
            return new Vector2d(Width, Height);
        }
        public Vector2d getCenter()
        {
            return new Vector2d(X + (Width) / 2, Y + (Height) / 2);
        }
        public bool inside(float X, float Y)
        {
            return X > this.X && Y > this.Y && X < this.X + this.Width && Y < this.Y + this.Height;
        }

        public bool inside(Vector2d p)
        {
            return inside(p.X,p.Y);
        }

        public RectangleF getRectangle()
        {
            return new RectangleF(X,Y,Width,Height);
        }
    }
}

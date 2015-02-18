using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Windows.Forms;
using YLScsDrawing.Geometry;

namespace YLScsDrawing.Controls
{
    public class ImageCurve : UserControl
    {

        public ImageCurve()
        {
            // Set the value of the double-buffering style bits to true.
            this.SetStyle(ControlStyles.AllPaintingInWmPaint |
              ControlStyles.UserPaint | ControlStyles.ResizeRedraw |
              ControlStyles.UserPaint | ControlStyles.DoubleBuffer, true);
        }

        List<Point> keyPt = new List<Point>();
        byte[] level = new byte[256];

        float _minX = 0;
        float _minY = 0;
        float _maxX = 1;
        float _maxY = 1;
        public float MinX { set { _minX = value; Invalidate(); } get { return _minX; } }
        public float MinY { set { _minY = value; Invalidate(); } get { return _minY; } }
        public float MaxX { set { _maxX = value; Invalidate(); } get { return _maxX; } }
        public float MaxY { set { _maxY = value; Invalidate(); } get { return _maxY; } }

        public bool DrawGrid { set; get; }
        public bool DrawControlPoints { set; get; }

        public event ImageLevelChangedEventHandler ImageLevelChanged;

        protected virtual void OnLevelChanged(ImageLevelEventArgs e)
        {
            if (ImageLevelChanged != null) // Make sure there are methods to execute.
                ImageLevelChanged(this, e); // Raise the event.
        }

        public byte[] LevelValue
        {
            get { getImageLevel(); return level; }
        }

        private void getImageLevel()
        {
            Point[] pts = new Point[keyPt.Count];
            for (int i = 0; i < pts.Length; i++)
            {
                pts[i].X = keyPt[i].X * 255 / this.Width;
                pts[i].Y = 255 - keyPt[i].Y * 255 / this.Height;
            }

            for (int i = 0; i < pts[0].X; i++)
                level[i] = (byte)pts[0].Y;
            for (int i = pts[pts.Length - 1].X; i < 256; i++)
                level[i] = (byte)pts[pts.Length - 1].Y;

            YLScsDrawing.Geometry.Spline sp = new YLScsDrawing.Geometry.Spline();
            sp.DataPoint = pts;
            sp.Precision = 1.0;
            Point[] spt=sp.SplinePoint;
            for (int i = 0; i < spt.Length; i++)
            {
                int n = spt[i].Y;
                if (n < 0) n = 0;
                if (n > 255) n = 255;
                level[pts[0].X + i] = (byte)n;
            }
        }

        int ww, hh;
        protected override void OnLoad(EventArgs e)
        {
            base.OnLoad(e);
            ww = this.Width;
            hh = this.Height;
            keyPt.Clear();
            keyPt.Add(new Point(0,hh));
            keyPt.Add(new Point(ww,0));

            Invalidate();
        }

        protected override void OnResize(EventArgs e)
        {
            base.OnResize(e);
            for (int i = 0; i < keyPt.Count; i++)
            {
                keyPt[i] = new Point(keyPt[i].X * this.Width / ww, keyPt[i].Y * this.Height / hh);
            }
            ww = this.Width;
            hh = this.Height;

            Invalidate();
        }

        int moveflag;
        bool drag = false;
        protected override void OnMouseDown(MouseEventArgs e)
        {
            base.OnMouseDown(e);
            for (int i = 1; i < keyPt.Count; i++)
            {
                if (e.X > keyPt[i-1].X+20 && e.Y > 0 && e.X < keyPt[i].X-20 && e.Y < this.Height)
                {
                    keyPt.Insert(i, e.Location);
                    drag = true;
                    moveflag = i;
                    this.Cursor = Cursors.Hand;
                    Invalidate();
                }
            }

            Rectangle r = new Rectangle(e.X - 20, e.Y - 20, 40, 40);
            for (int i = 0; i < keyPt.Count; i++)
            {
                if (r.Contains(keyPt[i]))
                {
                    drag = true;
                    moveflag = i;
                }
            }
        }

        protected override void OnMouseMove(MouseEventArgs e)
        {
            base.OnMouseMove(e);

            //mouse cursor
            bool handCursor = false;
            for (int i = 0; i < keyPt.Count; i++)
            {
                Rectangle r = new Rectangle(keyPt[i].X - 2, keyPt[i].Y - 2, 4, 4);
                if (r.Contains(e.Location))
                    handCursor = true;
            }
            if (handCursor) this.Cursor = Cursors.Hand;
            else this.Cursor = Cursors.Default;

            // move the picked point
            if (this.ClientRectangle.Contains(e.Location))
            {
                if (drag && moveflag > 0 && moveflag < keyPt.Count - 1)
                {
                    if (e.X > keyPt[moveflag - 1].X + 20 && e.X < keyPt[moveflag + 1].X - 20)
                    {
                        keyPt[moveflag] = e.Location;
                    }
                    else
                    {
                        keyPt.RemoveAt(moveflag);
                        drag = false;
                    }
                }

                if (drag && moveflag == 0 && e.X < keyPt[1].X - 20)
                        keyPt[0] = e.Location;
                
                if (drag && moveflag == keyPt.Count - 1&& e.X > keyPt[keyPt.Count - 2].X +20)
                        keyPt[moveflag] = e.Location;
                
                Invalidate();
            }
        }

        protected override void OnMouseUp(MouseEventArgs e)
        {
            base.OnMouseUp(e);
            if (drag)
            {
                drag = false;
                getImageLevel();
                OnLevelChanged(new ImageLevelEventArgs(level));
            }
        }


        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);

            Graphics g = e.Graphics;

            if (DrawGrid)
            {
                int n = 4;
                int n2 = n * 2;
                // draw ruler
                for (int i = 0; i < n2; ++i)
                {
                    int x = (i * Width) / n2;
                    int y = (i * Height) / n2;
                    g.DrawLine(new Pen(Color.LightGray), new Point(x, Height), new Point(x, 0));
                    g.DrawLine(new Pen(Color.LightGray), new Point(0, y), new Point(Width, y));
                }
                // draw ruler
                for (int i = 0; i < n; ++i)
                {
                    int x = (i * Width) / n;
                    int y = (i * Height) / n;
                    g.DrawLine(new Pen(Color.Gray), new Point(x, Height), new Point(x, 0));
                    g.DrawLine(new Pen(Color.Gray), new Point(0, y), new Point(Width, y));
                }


                    //draw mid line
                g.DrawLine(new Pen(new HatchBrush(HatchStyle.Percent50, Color.Black, Color.Transparent)), new Point(0, Height), new Point(Width, 0));
            }
            // draw curve
            g.DrawLine(new Pen(Color.Black), new Point(0, keyPt[0].Y), keyPt[0]);
            g.DrawLine(new Pen(Color.Black), new Point(this.Width, keyPt[keyPt.Count - 1].Y), keyPt[keyPt.Count - 1]);
            
            YLScsDrawing.Geometry.Spline spline = new YLScsDrawing.Geometry.Spline();
            spline.ListDataPoint = keyPt;
            spline.Precision = 5;
            Point[] splinePt=spline.SplinePoint;
            g.DrawLines(new Pen(Color.Black), splinePt);
            g.DrawLine(new Pen(Color.Black), keyPt[keyPt.Count - 1], splinePt[splinePt.Length - 1]);

            foreach (Point pt in keyPt)
            {
                Point[] pts = new Point[]{new Point(pt.X,pt.Y-3),new Point(pt.X-3,pt.Y),
                    new Point(pt.X,pt.Y+3),new Point(pt.X+3,pt.Y)};
                g.FillPolygon(new SolidBrush(Color.Red), pts);
            }
            if (DrawControlPoints && spline.ControlPoints != null)
            {
                for (int i = 1; i < spline.ControlPoints.Length; ++i)
                {
                    g.DrawLine(new Pen(Brushes.Gray, 1), (int)spline.ControlPoints[i - 1].X, (int)spline.ControlPoints[i - 1].Y, (int)spline.ControlPoints[i].X, (int)spline.ControlPoints[i].Y);
                }
                for (int i = 0; i < spline.ControlPoints.Length; ++i)
                {
                    Vector v=spline.ControlPoints[i];
                    g.FillRectangle(Brushes.Green, (int)v.X - 2, (int)v.Y - 2, 4, 4);
                }
            }
            Font f = new Font("Times New Roman", 12);
            SizeF strSize;
            strSize = g.MeasureString(MinY.ToString(), f);
            g.DrawString(MinY.ToString(), f, Brushes.Black, new PointF(0, Height-strSize.Height-20));
            strSize = g.MeasureString(MinX.ToString(), f);
            g.DrawString(MinX.ToString(), f, Brushes.Black, new PointF(20, Height - strSize.Height));

            g.DrawString(MaxY.ToString(), f,Brushes.Black,new PointF(0,0));
            strSize = g.MeasureString(MaxX.ToString(), f);
            g.DrawString(MaxX.ToString(), f, Brushes.Black, new PointF(Width - strSize.Width, Height-strSize.Height));
        }

    }

    /// <summary>
    /// For image curve control
    /// </summary>

    public class ImageLevelEventArgs : EventArgs
    {
        private byte[] levelValue;

        public ImageLevelEventArgs(byte[] LevelValue)
        {
            levelValue = LevelValue;
        }

        public byte[] LevelValue
        {
            get { return levelValue; }
        }
    }

    //Declare a delegate 
    public delegate void ImageLevelChangedEventHandler(object sender, ImageLevelEventArgs e);

}


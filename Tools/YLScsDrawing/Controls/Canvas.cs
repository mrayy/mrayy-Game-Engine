using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Windows.Forms;

namespace YLScsDrawing.Controls
{
    public class Canvas : UserControl
    {
        public Canvas()
            : base()
        {
            // Set the value of the double-buffering style bits to true.
            this.SetStyle(ControlStyles.AllPaintingInWmPaint |
              ControlStyles.UserPaint | ControlStyles.ResizeRedraw |
              ControlStyles.UserPaint | ControlStyles.DoubleBuffer, true);
        }

        Rectangle originalCanvas = new Rectangle(0, 0, 400, 600);
        public Size CanvasSize
        {
            set { originalCanvas.Size = value; setup(); }
            get { return originalCanvas.Size; }
        }

        Color canvasBackColor = Color.Black;
        public Color CanvasBackColor
        {
            set { canvasBackColor = value; Invalidate(); }
            get { return canvasBackColor; }
        }

        float zoomFactor = 1f;
        public float ZoomFactor
        {
            get { return zoomFactor; }
            set
            {
                zoomFactor = Math.Max(0.001f, value); // if =0, tranform matrix will be thrown exception
                setup();
            }
        }

        Bitmap pictureItem;
        public Bitmap CanvasImage
        {
            set { pictureItem = value; Invalidate(); }
            get { return pictureItem; }
        }

        Point imageLocation = new Point();
        public Point ImageLocation
        {
            set { imageLocation = value; }
            get { return imageLocation; }
        }

        Rectangle zoomedCanvas = new Rectangle();
        Rectangle visibleCanvas = new Rectangle();
        Matrix mxCanvasToControl, mxControlToCanvas; // transform matrix

        private void setup()
        {
            // setup zoomed canvas Rectangle
            zoomedCanvas.Width = (int)((float)originalCanvas.Width * zoomFactor);
            zoomedCanvas.Height = (int)((float)originalCanvas.Height * zoomFactor);
            this.AutoScrollMinSize = zoomedCanvas.Size;
            Point canvasLoc = new Point();
            if (zoomedCanvas.Width < this.ClientRectangle.Width)
                canvasLoc.X = (this.ClientRectangle.Width - zoomedCanvas.Width) / 2;
            else canvasLoc.X = AutoScrollPosition.X;
            if (zoomedCanvas.Height < this.ClientRectangle.Height)
                canvasLoc.Y = (this.ClientRectangle.Height - zoomedCanvas.Height) / 2;
            else canvasLoc.Y = AutoScrollPosition.Y;
            zoomedCanvas.Location = canvasLoc;

            // setup transform matrix
            mxCanvasToControl = new Matrix();
            mxCanvasToControl.Scale(zoomFactor, zoomFactor);
            mxCanvasToControl.Translate(canvasLoc.X, canvasLoc.Y, MatrixOrder.Append);

            mxControlToCanvas = mxCanvasToControl.Clone();
            mxControlToCanvas.Invert();

            visibleCanvas = this.ClientRectangle;
            visibleCanvas.Intersect(zoomedCanvas);

            Invalidate();
        }

        protected override void OnScroll(ScrollEventArgs se)
        {
            base.OnScroll(se);
            setup();
        }

        protected override void OnMouseWheel(MouseEventArgs e)
        {
            base.OnMouseWheel(e);
            if (e.Delta > 0) this.ZoomFactor += 0.1f;
            if (e.Delta < 0) this.ZoomFactor -= 0.1f;
        }

        protected override void OnResize(EventArgs e)
        {
            base.OnResize(e);
            setup();
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            Graphics g = e.Graphics;
            g.FillRectangle(new SolidBrush(canvasBackColor), visibleCanvas);
            g.Transform = mxCanvasToControl;

            if (pictureItem != null)
            {
                g.DrawImage(pictureItem, imageLocation);
            }
        }
    }
}

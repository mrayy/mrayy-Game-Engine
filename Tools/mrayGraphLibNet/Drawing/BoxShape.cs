using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Windows.Forms;

namespace mrayGraphLibNet.Drawing
{
    public class BoxShape:Shape
    {
        public BoxShape(DrawCanvas canvas,Vector2d pos, Vector2d size, Shape parent)
            : base(parent,canvas)
        {
            Position = pos;
            Size = size;
            calcBoundingBox();
        }
        public override void draw(Graphics G)
        {
            calcBoundingBox();
            Rect absPos = getAbsBoundingBox();
            if (Fill)
                G.FillRectangle(new SolidBrush(FillColor), absPos.X, absPos.Y, absPos.Width, absPos.Height);
            G.DrawRectangle(new Pen(getLineDrawColor(),getLineDrawWidth()), absPos.X, absPos.Y, absPos.Width, absPos.Height);

            String txt = getTextInBoundingBox(-30);
            SizeF txtSize = G.MeasureString(txt, Common.mainFont);
            PointF txtPos = new PointF();
            txtPos.X = absPos.X + (absPos.Width - txtSize.Width) / 2;
            txtPos.Y = absPos.Y + absPos.Height * 0.5f - txtSize.Height * 0.5f;

            G.DrawString(txt, Common.mainFont, Brushes.Black, txtPos);
            base.draw(G);
        }
        public override bool onMouseMove(int X, int Y, MouseButtons button)
        {
            return base.onMouseMove(X, Y, button);
        }
        public override bool onMouseDown(int X, int Y, MouseButtons button)
        {
            return base.onMouseDown(X, Y, button);
        }

        public override void calcBoundingBox()
        {
            m_BoundingBox.X = Position.X;
            m_BoundingBox.Y = Position.Y;
            m_BoundingBox.Width = Size.X;
            m_BoundingBox.Height = Size.Y;
        }
        public override SocketShape createSocketType(Vector2d pos, ESocketType type)
        {
            return new IOSocket(canvas, pos, this, type);
        }
    }
}

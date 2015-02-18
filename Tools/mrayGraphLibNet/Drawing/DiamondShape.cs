using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Windows.Forms;

namespace mrayGraphLibNet.Drawing
{
    public class DiamondShape:Shape
    {
        public DiamondShape(DrawCanvas canvas,Vector2d pos, Vector2d size, Shape parent)
            : base(parent,canvas)
        {
            Position = pos;
            Size = size;
            calcBoundingBox();
            addInputSocket();
            addOutputSocket();
            addOutputSocket();
        }
        public override void draw(Graphics G)
        {
            calcBoundingBox();
            Rect absPos = getAbsBoundingBox();
            PointF[] points=new PointF[5];
            points[0].X=absPos.X;
            points[0].Y=absPos.Y+absPos.Height/2;

            points[1].X=absPos.X+absPos.Width/2;
            points[1].Y=absPos.Y+absPos.Height;

            points[2].X=absPos.X+absPos.Width;
            points[2].Y=absPos.Y+absPos.Height/2;
            
            points[3].X=absPos.X+absPos.Width/2;
            points[3].Y = absPos.Y;

            points[4].X = absPos.X;
            points[4].Y = absPos.Y + absPos.Height / 2;

            if (Fill)
                G.FillPolygon(new SolidBrush(FillColor), points);
            G.DrawLines(new Pen(getLineDrawColor(),getLineDrawWidth()), points);

            String txt = getTextInBoundingBox(0);
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
        public override SocketShape addOutputSocket()
        {
            Vector2d v = new Vector2d();
            if (m_outsockets.Count == 0)
            {
                v.X = Size.X;
                v.Y = Size.Y / 2;
            }
            else if (m_outsockets.Count == 1)
            {
                v.X = Size.X /2;
                v.Y = Size.Y;
            }
            else
                return null;
            SocketShape s = createSocketType( v,  ESocketType.Output);
            m_outsockets.Add(s);
            s.Parent = this;
            return s;
        }
        public override SocketShape addInputSocket()
        {
            if (m_insockets.Count != 0)
                return null;

            Vector2d v = new Vector2d();
            v.X = 0;
            v.Y = Size.Y / 2;
            SocketShape s = createSocketType( v, ESocketType.Input);
            m_insockets.Add(s);
            s.Parent = this;
            return s;
        }
        public override bool canAddInputMoreSockets()
        {
            return m_insockets.Count == 0;
        }
        public override bool canAddOutputMoreSockets()
        {
            return m_outsockets.Count < 2;
        }
        public override SocketShape createSocketType(Vector2d pos, ESocketType type)
        {
            return new IOSocket(canvas, pos, this, type);
        }
    }
}

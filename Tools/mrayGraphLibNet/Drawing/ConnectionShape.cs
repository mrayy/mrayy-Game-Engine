using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Windows.Forms;

namespace mrayGraphLibNet.Drawing
{
    public class ConnectionShape:Shape
    {
        SocketShape m_s1;
        SocketShape m_s2;
        Rect CircleRect = new Rect();
        Vector2d midPoint = new Vector2d();
        bool selected=false;

        public ConnectionShape(DrawCanvas canvas,SocketShape s1, SocketShape s2)
            : base(null,canvas)
        {
            m_s1 = s1;
            m_s2 = s2;
            m_s1.addConnectionLine(this);
            m_s2.addConnectionLine(this);
           //calcMidPoint();
            midPoint.X = (m_s1.getAbsPosition().X + m_s2.getAbsPosition().X) / 2;
            midPoint.Y = (m_s1.getAbsPosition().Y + m_s2.getAbsPosition().Y) / 2;
        }

        void calcMidPoint()
        {
            Vector2d v1, v2, c;
            c = (m_s1.getAbsPosition() + m_s2.getAbsPosition()) * 0.5f;

            v1 = m_s2.getAbsPosition() - m_s1.getAbsPosition();

            float lineLen = v1.length();

            v1.Normalize();


            v2 = c - m_s1.getAbsPosition();
            float z = lineLen;

            float len = (float)Math.Sqrt((double)(v2.X * v2.X + v2.Y * v2.Y + z * z));
            v2.X /= len;
            v2.Y /= len;
            z /= len;

            midPoint.X =c.X+ v1.Y * z * (lineLen * 0.3f);
            midPoint.Y = c.Y + -v1.X * z * (lineLen * 0.3f);

        }
        public override void draw(Graphics G)
        {

            Pen drawPen = new Pen(getLineDrawColor(), 2);
            PointF[] points = new PointF[3];

            Vector2d v1, v2;
            Vector2d absPos1 = m_s1.getAbsPosition();
            Vector2d absPos2 = m_s2.getAbsPosition();

            Vector2d va = midPoint - absPos1;
            Vector2d vb = midPoint - absPos2;

            va.Normalize();
            vb.Normalize();

            v1 = absPos1;
            v2 = absPos2;

            points[0] = v1.getPoint();
            points[1] = midPoint.getPoint();
            points[2] = v2.getPoint();

            Vector2d c=new Vector2d();
            c.X = (absPos1.X + absPos2.X) * 0.5f;
            c.Y = (absPos1.Y + absPos2.Y) * 0.5f;
            Color C;
            C = Color.FromArgb(0, 0, 0);
            G.DrawCurve(new Pen(C, 4), points, 0.4f);
            //G.DrawLines(new Pen(C, 4), points);
            C = Color.FromArgb(255, 255, 255);
            G.DrawCurve(new Pen(C, 2), points, 0.4f);
            //G.DrawLines(new Pen(C, 2), points);

            float radius = 4;
            CircleRect.X = midPoint.X - radius;
            CircleRect.Y = midPoint.Y - radius;
            CircleRect.Width = (int)(2 * radius);
            CircleRect.Height = (int)(2 * radius);
            G.DrawEllipse(drawPen, CircleRect.getRectangle());

            float angle = (float)Math.Atan2((double)(midPoint.Y - v2.Y),
                                (double)(midPoint.X - v2.X)) * 180 / 3.141562f;


            System.Drawing.Drawing2D.Matrix tmp = G.Transform;
            G.TranslateTransform(v2.X, v2.Y);
            G.RotateTransform(angle);
            PointF[] points2 = new PointF[3];
            points2[0] = new PointF(  10,  - 5);
            points2[1] = new PointF(0, 0);
            points2[2] = new PointF(10, +5);
            G.DrawLines(drawPen, points2);
            G.Transform = tmp;

/*

            Pen drawPen = new Pen(getLineDrawColor(), getLineDrawWidth());
            PointF[] points1 = new PointF[7];
            Vector2d absPos1 = m_s1.getAbsPosition();
            Vector2d absPos2 = m_s2.getAbsPosition();


            Vector2d va = midPoint.translate - A.translate;
            Vector2d vb = midPoint.translate - B.translate;

            points1[0] = new PointF(absPos1.X, absPos1.Y);
            points1[1] = new PointF(absPos1.X+5, absPos1.Y);
            points1[2] = new PointF((int)((absPos1.X + absPos2.X) * 0.5),
                (int)(absPos1.Y));
            points1[3] = new PointF((int)((absPos1.X + absPos2.X) * 0.5),
                (absPos2.Y + absPos1.Y) / 2);
            points1[4] = new PointF((int)((absPos1.X + absPos2.X) * 0.5),
                absPos2.Y);
            points1[5] = new PointF(absPos2.X-5, absPos2.Y);
            points1[6] = new PointF(absPos2.X, absPos2.Y);
            G.DrawBeziers(drawPen, points1);
            //G.DrawBezier(drawPen, points1[0], points1[1], points1[2], points1[3]);
            //G.DrawLines(drawPen, points1);
            PointF midPoint = new PointF();
            midPoint.X = points1[3].X;
            midPoint.Y = points1[3].Y;
//             midPoint.X = points1[2].X;
//             midPoint.Y = points1[2].Y;

            float angle = (float)Math.Atan2((double)(points1[5].Y - points1[6].Y),
                                (double)(points1[5].X - points1[6].X)) * 180 / 3.141562f;

            System.Drawing.Drawing2D.Matrix tmp = G.Transform;
            G.TranslateTransform(absPos2.X, absPos2.Y);
            G.RotateTransform(angle);
            PointF[] points2 = new PointF[3];
            points2[0] = new PointF(  10,  - 5);
            points2[1] = new PointF(0, 0);
            points2[2] = new PointF(  10,  + 5);
            G.DrawLines(drawPen, points2);
            G.Transform = tmp;

            
            float radius=4;
            CircleRect.X = midPoint.X - radius;
            CircleRect.Y = midPoint.Y - radius;
            CircleRect.Width = (int)(2 * radius);
            CircleRect.Height = (int)(2 * radius);
            G.DrawEllipse(drawPen, CircleRect.getRectangle());
            */
            calcBoundingBox();

        }
        public override bool onMouseMove(int X, int Y, MouseButtons button)
        {
            if (selected && button == System.Windows.Forms.MouseButtons.Left)
            {
                midPoint.X = X;
                midPoint.Y = Y;
            }
            return CircleRect.inside(X, Y);
        }
        public override bool onMouseDown(int X, int Y, MouseButtons button)
        {
            if(CircleRect.inside(X, Y)){

                selected = true;
                return true;
            }
            return false;
        }

        public override void calcBoundingBox()
        {
            m_BoundingBox = CircleRect;
        }
    }
}

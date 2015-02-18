using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Windows.Forms;

namespace mrayGraphLibNet.Drawing
{
    public enum ESocketType
    {
        Input,
        Output
    }
    public class SocketShape:Shape
    {

        private ESocketType m_socketType;

        public ESocketType SocketType
        {
            get { return m_socketType; }
        }
        public SocketShape(DrawCanvas canvas, Vector2d pos, Shape parent, ESocketType type)
            : base(parent,canvas)
        {
            Position = pos;
            Size = new Vector2d(10, 5);
            m_socketType = type;
            calcBoundingBox();
        }
        public virtual bool Accept(SocketShape other)
        {
            return m_connections.Count==0;
        }
        public override bool isSocket()
        {
            return true;
        }
        public bool isFull()
        {
            return m_connections.Count > 0;
        }
        public override void draw(Graphics G)
        {
            Rect absPos = getAbsBoundingBox();
            Color fillColor;
            if (canvas.m_socket != null)
            {
                if (Accept(canvas.m_socket) && canvas.m_socket.Accept(this))
                    fillColor = Common.socketAcceptColor;
                else
                    fillColor = Common.socketNotAcceptColor;
            }else
            if (isFull())
                fillColor = Common.socketFullColor;
            else if (SocketType == ESocketType.Input)
                fillColor = Common.inputSocketColor;
            else
                fillColor = Common.outputSocketColor;
            if (Fill)
                G.FillRectangle(new SolidBrush(fillColor), absPos.X, absPos.Y, absPos.Width, absPos.Height);
            G.DrawRectangle(new Pen(getLineDrawColor(), getLineDrawWidth()), absPos.X, absPos.Y, absPos.Width, absPos.Height);
        }
        public override bool onMouseMove(int X, int Y, MouseButtons button)
        {
            return BoundingBox.inside(X, Y);
        }
        public override bool onMouseDown(int X, int Y, MouseButtons button)
        {
            return false;
        }

        public override void calcBoundingBox()
        {
            m_BoundingBox.X = Position.X - (int)(Size.X * 0.5);
            m_BoundingBox.Y = Position.Y - (int)(Size.Y * 0.5);
            m_BoundingBox.Width = Size.X;
            m_BoundingBox.Height = Size.Y;
        }
    }
}

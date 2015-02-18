using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using System.Drawing.Drawing2D;

namespace mrayGraphLibNet.Drawing
{
    public partial class DrawCanvas : UserControl
    {

        float m_Scale = 1;

        Vector2d m_Translate = new Vector2d();
        List<Shape> m_shapeList = new List<Shape>();

        public SocketShape m_socket;

        Vector2d m_currMousePos = new Vector2d();

        public Shape selectedShape;
        public Shape hoverShape;

        public Vector2d Translate
        {
            get { return m_Translate; }
            set { m_Translate = value; }
        }
        public float Scale
        {
            get { return m_Scale; }
            set {
                if (value <= 0.1) m_Scale = 0.1f;
                else m_Scale = value; 
            }
        }

        Shape getShapeFromPoint(Vector2d p)
        {
            for (int i = 0; i < m_shapeList.Count;i++ )
            {
                Shape s = m_shapeList[i].getShapeFromPoint(p);
                if (s != null)
                    return s;
            }
            return null;
        }

        BufferedGraphics myBuffer;
        public DrawCanvas()
        {
            InitializeComponent();
            BufferedGraphicsContext context = BufferedGraphicsManager.Current;
            myBuffer = context.Allocate(this.CreateGraphics(), this.DisplayRectangle);
        }
        public Graphics getGraphics()
        {
            return this.CreateGraphics();
        }
        public bool onMouseMove(int X, int Y, MouseButtons button)
        {
            m_currMousePos.X = X;
            m_currMousePos.Y = Y;
            if (selectedShape != null)
                selectedShape.onMouseMove(X, Y, button);
            Shape s = getShapeFromPoint(new Vector2d(X, Y));
            if (s != null)
            {
                hoverShape = s;
                return true;
            }
            hoverShape = null;
            return false;
        }
        public bool onMouseDown(int X, int Y, MouseButtons button)
        {
            SocketShape tmpSock = m_socket;
            m_socket = null;
            Shape s = getShapeFromPoint(new Vector2d(X, Y));
            if (s != null)
            {
                if (button == MouseButtons.Left)
                {
                    selectedShape = s;
                    selectedShape.onMouseDown(X, Y, button);
                    if (selectedShape.isSocket())
                    {
                        m_socket = (SocketShape)selectedShape;
                        if (tmpSock !=null && tmpSock.Accept(m_socket)&&
                            m_socket.Accept(tmpSock))
                        {
                            SocketShape first = m_socket;
                            ConnectionShape c = null;
                            if (tmpSock.SocketType == ESocketType.Output)
                            {
                                first = tmpSock;
                                c = ShapeManager.addConnection(this, tmpSock, m_socket);
                            }
                            else
                                c = ShapeManager.addConnection(this, m_socket, tmpSock);
                            if (c != null)
                            {
                                addShape(c);
                            }
                            m_socket = null;
                        }
                    }
                    return true;
                }
            }
            selectedShape = null;
            return false;
        }

        public void addShape(Shape s)
        {
            if (s == null) return;
            m_shapeList.Add(s);
        }
        public void removeShape(Shape s)
        {
            m_shapeList.Remove(s);
            if (selectedShape == s)
                selectedShape = null;
            if (hoverShape == s)
                hoverShape = null;
        }

        public void drawObjects()
        {
            Graphics G = myBuffer.Graphics;

            G.Clear(BackColor);
            G.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.HighQuality;

            G.ResetTransform();
            G.ScaleTransform(Scale,Scale);
            G.TranslateTransform(Translate.X, Translate.Y);

            for (int i = 0; i < m_shapeList.Count; i++)
            {
                m_shapeList[i].draw(G);
            }
            if (m_socket != null)
            {
                G.DrawLine(new Pen(Color.Black,2), m_socket.getAbsPosition().getPoint(), m_currMousePos.getPoint());
            }

            myBuffer.Render(this.CreateGraphics());
        }

        public void onResize()
        {
            myBuffer.Dispose();
            BufferedGraphicsContext context = BufferedGraphicsManager.Current;
            myBuffer = context.Allocate(this.CreateGraphics(), this.DisplayRectangle);
        }

        private void DrawCanvas_Load(object sender, EventArgs e)
        {

        }

        private void DrawCanvas_Resize(object sender, EventArgs e)
        {
            onResize();
        }
    }
}

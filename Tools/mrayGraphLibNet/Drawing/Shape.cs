using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Windows.Forms;

namespace mrayGraphLibNet.Drawing
{
    public abstract class Shape
    {
        protected DrawCanvas canvas = null;

        protected Vector2d m_position = new Vector2d();
        protected Vector2d m_Size = new Vector2d();
        protected Rect m_BoundingBox = new Rect();
        protected bool m_fill = true;

        protected Color m_fillColor = Color.Wheat;
        protected Color m_lineColor = Color.Black;
        protected Color m_textColor = Color.Black;

        protected List<ConnectionShape> m_connections = new List<ConnectionShape>();

        protected List<SocketShape> m_insockets = new List<SocketShape>();
        protected List<SocketShape> m_outsockets = new List<SocketShape>();

        protected bool selected = false;

        protected Vector2d mousePos = new Vector2d();
        Shape m_parent;

        protected List<Shape> m_children=new List<Shape>();

        protected string m_name;
        protected string m_text;

        public Color TextColor
        {
            get { return m_textColor; }
            set { m_textColor = value; }
        }

        public string Text
        {
            get { return m_text; }
            set { 
                m_text = value;
                textChanged();
            }
        }


        public string Name
        {
            get { return m_name; }
            set { m_name = value; }
        }

        public Shape Parent
        {
            get { return m_parent; }
            set { m_parent = value; }
        }

        public Color FillColor
        {
            get { return m_fillColor; }
            set { m_fillColor = value; }
        }
        public Color LineColor
        {
            get { return m_lineColor; }
            set { m_lineColor = value; }
        }

        public bool Fill
        {
            get { return m_fill; }
            set { m_fill = value; }
        }

        public Rect BoundingBox
        {
            get { return m_BoundingBox; }
        }
        public Vector2d Position
        {
            get { return m_position; }
            set
            {
                m_position = value;
                calcBoundingBox();
            }
        }
        public Vector2d Size
        {
            get { return m_Size; }
            set { 
                m_Size = value;
                calcBoundingBox();
            }
        }

        public virtual SocketShape createSocketType(Vector2d pos, ESocketType type)
        {
            return new SocketShape(canvas, pos, this, type);
        }

        protected String getTextInBoundingBox(int offset)
        {
            Graphics G = canvas.getGraphics();
            if (G == null)
                return Text;
            String tmp="";
            int len = Text.Length;
            float txtSize = 0;
            int i = 0;
            for (; i < len; i++)
            {
                float sz= G.MeasureString(Text[i].ToString(), Common.mainFont).Width;
                if (txtSize + sz > BoundingBox.Width - offset)
                    break;
                tmp += Text[i];
                txtSize += sz;
            }
            if (i != len)
                tmp += "..";
            return tmp;
        }

        protected virtual void textChanged()
        {
            /*
            Graphics G= canvas.getGraphics();
            if (G == null)
                return;
            SizeF txtSize = G.MeasureString(Text, Common.mainFont);

            if (BoundingBox.Width < txtSize.Width + 10)
                BoundingBox.Width = txtSize.Width + 10;*/
        }
        public Shape(Shape parent, DrawCanvas canvas)
        {
            this.canvas = canvas;
            this.Parent = parent;
            Text = "Test";
        }
        public virtual bool isSocket()
        {
            return false;
        }
        public void addShape(Shape s)
        {
            m_children.Add(s);
            s.Parent = this;
        }
        public void addConnectionLine(ConnectionShape l)
        {
            m_connections.Add(l);
        }
        public int getConnectionsCount()
        {
            return m_connections.Count;
        }
        public virtual void draw(Graphics G)
        {
            for (int i = 0; i < m_children.Count;i++ )
            {
                m_children[i].draw(G);
            }
            for (int i = 0; i < m_insockets.Count; i++)
            {
                m_insockets[i].draw(G);
            }
            for (int i = 0; i < m_outsockets.Count; i++)
            {
                m_outsockets[i].draw(G);
            }
        }

        public virtual SocketShape addInputSocket()
        {
            if (!canAddInputMoreSockets())
                return null;
            Vector2d v=new Vector2d();
            v.X = 0;
            v.Y = 2;
            if (m_insockets.Count > 0)
            {
                v.Y += m_insockets[m_insockets.Count - 1].Position.Y + m_insockets[m_insockets.Count - 1].Size.Y + 2;
            }
            SocketShape s = createSocketType(v,  ESocketType.Input);
            m_insockets.Add(s);
            s.Parent = this;
            return s;
        }
        public virtual SocketShape addOutputSocket()
        {
            if (!canAddOutputMoreSockets())
                return null;
            Vector2d v = new Vector2d();
            v.X = BoundingBox.Width;
            v.Y = 2;
            if (m_outsockets.Count > 0)
            {
                v.Y += m_outsockets[m_outsockets.Count - 1].Position.Y + m_outsockets[m_outsockets.Count - 1].Size.Y + 2;
            }
            SocketShape s = createSocketType( v,  ESocketType.Output);
            m_outsockets.Add(s);
            s.Parent = this;
            return s;
        }

        public virtual bool onMouseMove(int X, int Y, MouseButtons button)
        {
            if (selected && button == System.Windows.Forms.MouseButtons.Left)
            {
                Position.X = X - mousePos.X;
                Position.Y = Y - mousePos.Y;
                calcBoundingBox();
            }
            return BoundingBox.inside(X, Y);
        }
        public virtual bool onMouseDown(int X, int Y, MouseButtons button)
        {
            if (BoundingBox.inside(X, Y))
            {
                selected = true;
                mousePos.X = X - Position.X;
                mousePos.Y = Y - Position.Y;
                return true;
            }
            return false;
        }

        public abstract void calcBoundingBox();

        public virtual Vector2d getAbsPosition()
        {
            if (Parent!=null)
            {
                Vector2d p=Parent.getAbsPosition();
                p.X += Position.X;
                p.Y += Position.Y;
                return p;
            }
            return new Vector2d(Position.X,Position.Y);
        }
        public virtual Rect getAbsBoundingBox()
        {
            if (Parent != null)
            {
                Vector2d p = Parent.getAbsPosition();
                Rect r = new Rect();
                r.X =p.X+ BoundingBox.X;
                r.Y = p.Y + BoundingBox.Y;
                r.Width = BoundingBox.Width;
                r.Height = BoundingBox.Height;
                return r;
            }
            return BoundingBox;
        }

        public Color getLineDrawColor()
        {
            if (canvas!=null)
            {
                if (canvas.selectedShape==this) return Common.selectColor;
                if (canvas.hoverShape == this) return Common.hoverColor;
            }
            return LineColor;
        }
        public int getLineDrawWidth()
        {
            if (canvas != null)
            {
                if (canvas.selectedShape == this) return Common.selectLineWidth;
                if (canvas.hoverShape == this) return Common.hoverLineWidth;
            }
            return Common.normalLineWidth;
        }


        public virtual Shape getShapeFromPoint(Vector2d p)
        {
            if (getAbsBoundingBox().inside(p))
                return this;
            for (int i = 0; i < m_children.Count; i++)
            {
                Shape s=m_children[i].getShapeFromPoint(p);
                if (s!=null)
                    return s;
            }
            for (int i = 0; i < m_insockets.Count; i++)
            {
                Shape s = m_insockets[i].getShapeFromPoint(p);
                if (s != null)
                    return s;
            }
            for (int i = 0; i < m_outsockets.Count; i++)
            {
                Shape s = m_outsockets[i].getShapeFromPoint(p);
                if (s != null)
                    return s;
            }
            return null;
        }

        public virtual bool canAddInputMoreSockets()
        {
            return true;
        }
        public virtual bool canAddOutputMoreSockets()
        {
            return true;
        }
    }
}

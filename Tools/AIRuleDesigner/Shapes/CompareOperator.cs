using System;
using System.Collections.Generic;
using System.Text;
using mrayGraphLibNet.Drawing;

namespace AIRuleDesigner.Shapes
{
    public enum EOperatorType
    {
        Equal,
        Less,
        LessEq,
        Greater,
        GreaterEq
    }
    public class CompareOperator:CircleShape
    {
        EOperatorType m_optype;

        public EOperatorType OPType
        {
            get { return m_optype; }
            set { 
                m_optype = value;
                textChanged();
            }
        }
        public CompareOperator(DrawCanvas canvas, Vector2d pos,EOperatorType type, Shape parent):
            base(canvas,pos,15,parent)
        {
            m_optype = type;
            addInputSocket();
            addInputSocket();
            addOutputSocket();
        }
        public override bool canAddInputMoreSockets()
        {
            return m_insockets.Count<2;
        }
        public override bool canAddOutputMoreSockets()
        {
            return m_outsockets.Count == 0;
        }
        public override SocketShape addInputSocket()
        {
            if (!canAddInputMoreSockets())
                return null;
            Vector2d v = new Vector2d();
            v.X = 0;
            if(m_insockets.Count==0)
                v.Y = Size.Y / 2;
            else
                v.Y = -Size.Y / 2;
            SocketShape s = new IOSocket(canvas,v,this, ESocketType.Input);
            m_insockets.Add(s);
            s.Parent = this;
            return s;
        }
        protected override void textChanged()
        {
            switch (m_optype)
            {
                case EOperatorType.Equal: m_text = "="; break;
                case EOperatorType.Less: m_text = "<"; break;
                case EOperatorType.LessEq: m_text = "<="; break;
                case EOperatorType.Greater: m_text = ">"; break;
                case EOperatorType.GreaterEq: m_text = ">="; break;
            }
        }
        public override SocketShape createSocketType(Vector2d pos, ESocketType type)
        {
            return new OperatorSocket(canvas, pos, this, type);
        }
    }
}

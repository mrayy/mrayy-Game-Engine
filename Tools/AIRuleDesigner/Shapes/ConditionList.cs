using System;
using System.Collections.Generic;
using System.Text;
using mrayGraphLibNet.Drawing;
using System.Windows.Forms;

namespace AIRuleDesigner.Shapes
{
    public enum ConditionCollectionType
    {
        OR,
        AND
    }
    public class ConditionList:BoxShape
    {
        private ConditionCollectionType m_Type;

        public ConditionCollectionType Type
        {
            get { return m_Type; }
            set { 
                m_Type = value; 
                textChanged();
                }
        }
        public ConditionList(DrawCanvas canvas,ConditionCollectionType type,Vector2d pos, Vector2d size, Shape parent)
            : base(canvas,pos,size, parent)
        {
            Type = type;
        }
        protected override void textChanged()
        {
            switch (m_Type)
            {
                case ConditionCollectionType.AND: m_text = "AND"; break;
                case ConditionCollectionType.OR: m_text = "OR"; break;
            }
        }

        public override bool canAddInputMoreSockets()
        {
            return true;
        }
        public override bool canAddOutputMoreSockets()
        {
            return m_outsockets.Count ==0;
        }
        public override SocketShape createSocketType(Vector2d pos, ESocketType type)
        {
            return new OperatorSocket(canvas, pos, this, type);
        }
    }
}

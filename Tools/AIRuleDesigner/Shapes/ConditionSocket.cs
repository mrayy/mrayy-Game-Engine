using System;
using System.Collections.Generic;
using System.Text;
using mrayGraphLibNet.Drawing;

namespace AIRuleDesigner.Shapes
{
    public class ConditionSocket:SocketShape
    {
        public ConditionSocket(DrawCanvas canvas, Vector2d pos, Shape parent, ESocketType type)
            : base(canvas,pos,parent,type)
        {
        }
        public override bool Accept(SocketShape other)
        {
            try
            {
                ConditionSocket s = (ConditionSocket)other;
                if (s == null)
                    return false;
                if (s.SocketType != SocketType)
                    return base.Accept(other);
                return false;
            }
            catch (Exception e)
            {
                return false;
            }
        }
    }
}

using System;
using System.Collections.Generic;
using System.Text;
using mrayGraphLibNet.Drawing;

namespace AIRuleDesigner.Shapes
{
    public class OperatorSocket : SocketShape
    {
        public OperatorSocket(DrawCanvas canvas, Vector2d pos, Shape parent, ESocketType type)
            : base(canvas, pos, parent, type)
        {
        }
        public override bool Accept(SocketShape other)
        {
            try
            {
                OperatorSocket s = (OperatorSocket)other;
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

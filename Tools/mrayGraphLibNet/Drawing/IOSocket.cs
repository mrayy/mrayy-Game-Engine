using System;
using System.Collections.Generic;
using System.Text;

namespace mrayGraphLibNet.Drawing
{
    public class IOSocket:SocketShape
    {

        public override bool Accept(SocketShape other)
        {
            try
            {
                IOSocket s = (IOSocket)other;
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
        public IOSocket(DrawCanvas canvas, Vector2d pos, Shape parent, ESocketType type)
            : base(canvas, pos, parent,type)
        {
        }
    
    }
}

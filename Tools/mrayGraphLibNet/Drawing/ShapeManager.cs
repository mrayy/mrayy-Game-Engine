using System;
using System.Collections.Generic;
using System.Text;

namespace mrayGraphLibNet.Drawing
{
    public class ShapeManager
    {
        public static ConnectionShape addConnection(DrawCanvas canvas,SocketShape s1, SocketShape s2)
        {
            if (s1.SocketType != ESocketType.Output)
                return null;
            if (s2.SocketType != ESocketType.Input)
                return null;

            if (s2.getConnectionsCount() != 0)
                return null;
            ConnectionShape c = new ConnectionShape(canvas,s1, s2);
            return c;
        }
    }
}

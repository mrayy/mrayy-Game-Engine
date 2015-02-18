using System;
using System.Collections.Generic;
using System.Text;

namespace mrayGraphLibNet.Drawing
{
    public class CircleShape:EllipseShape
    {
        private float m_Radius;
        

        public float Radius
        {
          get { return m_Radius; }
          set { 
              m_Radius = value;
              base.Size = new Vector2d(2*m_Radius,2*m_Radius);
          }
        }
        public CircleShape(DrawCanvas canvas, Vector2d pos, float radius, Shape parent):
            base(canvas,pos,new Vector2d(radius*2,radius*2),parent)
        {
            m_Radius = radius;
        }
    }
}

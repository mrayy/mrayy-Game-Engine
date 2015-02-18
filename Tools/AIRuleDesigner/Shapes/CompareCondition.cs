using System;
using System.Collections.Generic;
using System.Text;
using mrayGraphLibNet.Drawing;

namespace AIRuleDesigner.Shapes
{
    public class CompareCondition:BoxShape
    {
        public CompareCondition(DrawCanvas canvas, Vector2d pos, Vector2d size, Shape parent)
            : base(canvas,pos,size, parent)
        {
        }
        public override bool canAddInputMoreSockets()
        {
            return m_insockets.Count < 3;
        }
        public override bool canAddOutputMoreSockets()
        {
            return m_outsockets.Count == 0;
        }
    }
}

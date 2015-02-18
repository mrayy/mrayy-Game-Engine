using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;

namespace mrayGraphLibNet.Drawing
{
    public class Vector2d
    {
        public float X;
        public float Y;


        public Vector2d()
        {
            X = Y = 0;
        }
        public Vector2d(float x, float y)
        {
            X = x;
            Y = y;
        }
        public override string ToString()
        {
            return X + "," + Y;
        }

        public float length()
        {
            return (float)Math.Sqrt((double)(X * X + Y * Y));
        }
        public float Normalize()
        {
            float len = length();
            if (len > 0)
            {
                X /= len;
                Y /= len;
            }
            return len;
        }
        public PointF getPoint()
        {
            return new PointF(X, Y);
        }
        public static Vector2d operator +(Vector2d a, Vector2d b)
        {
            return new Vector2d(a.X + b.X, a.Y + b.Y);
        }
        public static Vector2d operator -(Vector2d a, Vector2d b)
        {
            return new Vector2d(a.X - b.X, a.Y - b.Y);
        }
        public static Vector2d operator *(Vector2d a, float f)
        {
            return new Vector2d(a.X * f, a.Y * f);
        }
        public static Vector2d operator +(Vector2d a, float f)
        {
            return new Vector2d(a.X + f, a.Y + f);
        }
        public static Vector2d operator -(Vector2d a, float f)
        {
            return new Vector2d(a.X - f, a.Y - f);
        }
    }
}

using System;
using System.Collections.Generic;
using System.Drawing;

namespace YLScsDrawing.Geometry
{
    public class Spline
    {
        private Vector[] dataPoint;
        private Vector[] controlPoint;
        private List<Vector> splinePoint = new List<Vector>();
        private double precision = 0.1;
        private bool isXcalibrated=true;
       
        public double Precision
        {
            set { precision = value; }
            get { return precision; }
        }

        public bool IsXaxisCalibrated
        {
            set { isXcalibrated = value; }
        }

        public Vector[] ControlPoints { get { return controlPoint; } }

        public PointF[] DataPointF
        {
            set
            {
                int n = value.Length;
                dataPoint = new Vector[n];
                for (int i = 0; i < n; i++)
                    dataPoint[i] = new Vector(value[i]);
            }
        }

        public Point[] DataPoint
        {
            set
            {
                int n = value.Length;
                dataPoint = new Vector[n];
                for (int i = 0; i < n; i++)
                    dataPoint[i] = new Vector(value[i]);
            }
        }

        public List<PointF> ListDataPointF
        {
            set
            {
                int n = value.Count;
                dataPoint = new Vector[n];
                for (int i = 0; i < n; i++)
                    dataPoint[i] = new Vector(value[i]);
            }
        }

        public List<Point> ListDataPoint
        {
            set
            {
                int n = value.Count;
                dataPoint = new Vector[n];
                for (int i = 0; i < n; i++)
                    dataPoint[i] = new Vector(value[i]);
            }
        }

        public Point[] SplinePoint
        {
            get
            {
                getSplinePoints();
                Point[] pts = new Point[splinePoint.Count];
                for (int i = 0; i < pts.Length; i++)
                    pts[i] = splinePoint[i].ToPoint();
                return pts;
            }
        }

        public PointF[] SplinePointF
        {
            get
            {
                getSplinePoints();
                PointF[] pts = new PointF[splinePoint.Count];
                for (int i = 0; i < pts.Length; i++)
                    pts[i] = splinePoint[i].ToPointF();
                return pts;
            }
        }

        private void getSplinePoints()
        {
            splinePoint.Clear();
            if (dataPoint.Length == 1) splinePoint.Add(dataPoint[0]);

            if (dataPoint.Length == 2)
            {
                int n = 1;
                if(isXcalibrated)
                    n=(int)((dataPoint[1].X - dataPoint[0].X) / precision);
                else n = (int)((dataPoint[1].Y - dataPoint[0].Y) / precision);
                if (n == 0) n = 1;
                if (n < 0) n = -n;
                for (int j = 0; j < n; j++)
                {
                    double t = (double)j / (double)n;

                    splinePoint.Add((1 - t) * dataPoint[0] + t * dataPoint[1]);
                }
            }

            if (dataPoint.Length > 2)
            {
                getControlPoints();

                //draw bezier curves using Bernstein Polynomials
                for (int i = 0; i < controlPoint.Length - 1; i++)
                {
                    Vector b1 = controlPoint[i] * 2.0 / 3.0 + controlPoint[i + 1] / 3.0;
                    Vector b2 = controlPoint[i] / 3.0 + controlPoint[i + 1] * 2.0 / 3.0;

                    int n = 1;
                    if(isXcalibrated)
                        n=(int)((dataPoint[i + 1].X - dataPoint[i].X) / precision);
                    else n = (int)((dataPoint[i + 1].Y - dataPoint[i].Y) / precision);
                    if (n == 0) n = 1;
                    if (n < 0) n = -n;
                    for (int j = 0; j < n; j++ )
                    {
                        double t = (double)j / (double)n;
                        Vector v = (1 - t) * (1 - t) * (1 - t) * dataPoint[i] + 3 * (1 - t) * (1 - t) * t * b1 +
                            3 * (1 - t) * t * t * b2 + t * t * t * dataPoint[i + 1];
                        splinePoint.Add(v);
                    }
                }
            }
        }

        private void getControlPoints()
        {
            if (dataPoint != null && dataPoint.Length == 3)
            {
                controlPoint = new Vector[3];
                controlPoint[0] = dataPoint[0];
                controlPoint[1] =(6 * dataPoint[1] - dataPoint[0] - dataPoint[2])/4;
                controlPoint[2] = dataPoint[2];
            }

            if (dataPoint!=null && dataPoint.Length> 3)
            {
                int n = dataPoint.Length;
                controlPoint = new Vector[n];
                double[] diag = new double[n]; // tridiagonal matrix a(i , i)
                double[] sub = new double[n]; // tridiagonal matrix a(i , i-1)
                double[] sup = new double[n]; // tridiagonal matrix a(i , i+1)

                for (int i = 0; i < n; i++)
                {
                    controlPoint[i] = dataPoint[i];
                    diag[i] = 4;
                    sub[i] = 1;
                    sup[i] = 1;
                }

                controlPoint[1] = 6 * controlPoint[1] - controlPoint[0];
                controlPoint[n - 2] = 6 * controlPoint[n - 2] - controlPoint[n - 1];

                for (int i = 2; i < n - 2; i++)
                {
                    controlPoint[i] = 6 * controlPoint[i];
                }

                // Gaussian elimination fron row 1 to n-2
                for (int i = 2; i < n - 1; i++)
                {
                    sub[i] = sub[i] / diag[i - 1];
                    diag[i] = diag[i] - sub[i] * sup[i - 1];
                    controlPoint[i] = controlPoint[i] - sub[i] * controlPoint[i - 1];
                }

                controlPoint[n - 2] = controlPoint[n - 2] / diag[n - 2];

                for (int i = n - 3; i >0; i--)
                {
                    controlPoint[i] = (controlPoint[i] - sup[i] * controlPoint[i + 1]) / diag[i];
                }
            }
        }
    }
}

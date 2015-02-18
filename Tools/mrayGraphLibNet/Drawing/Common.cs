using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;

namespace mrayGraphLibNet.Drawing
{
    public class Common
    {
        public static Font mainFont = new Font(new FontFamily("Tahoma"), 10);
        public static Color hoverColor = Color.Brown;
        public static Color selectColor = Color.Blue;

        public static Color inputSocketColor = Color.BurlyWood;
        public static Color outputSocketColor = Color.Red;
        public static Color socketFullColor = Color.Green;
        public static Color socketAcceptColor = Color.LightGreen;
        public static Color socketNotAcceptColor = Color.DarkRed;

        public static int normalLineWidth = 1;
        public static int selectLineWidth = 2;
        public static int hoverLineWidth = 2;
    }
}

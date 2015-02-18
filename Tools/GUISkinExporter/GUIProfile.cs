using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;

namespace GUISkinExporter
{
    public class GUIProfile
    {
        string m_name;
        string m_imageSet;

        public string Name
        {
            set
            {
                m_name = value;
            }
            get
            {
                return m_name;
            }
        }
        public string ImageSet
        {
            set
            {
                m_imageSet = value;
            }
            get
            {
                return m_imageSet;
            }
        }

        List<GUIRegion> m_regions = new List<GUIRegion>();

        public List<GUIRegion> Regions
        {
            get
            {
                return m_regions;
            }
        }

        GUIRegion m_selected = null;

        public GUIRegion SelectedRegion
        {
            set
            {
                if(m_regions.Contains(value))
                    m_selected = value;
            }
            get
            {
                return m_selected;
            }
        }

        public GUIRegion CreateRegion(RegionRenderer rr)
        {
            GUIRegion region = new GUIRegion(this);

            Rectangle rec = region.Rect;
            rec.X = (int)(-rr.Translate.X / rr.Scale);
            rec.Y = (int)(-rr.Translate.Y / rr.Scale);
            region.Rect = rec;
            m_regions.Add(region);
            return region;
        }

        public void RemoveSelectedRegion()
        {
            if (SelectedRegion != null)
            {
                m_regions.Remove(SelectedRegion);
                SelectedRegion = null;
            }
        }

        public void ClearRegions()
        {
            m_regions.Clear();
        }
    }
}

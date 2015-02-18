using System;
using System.Collections.Generic;
using System.Text;

namespace GUISkinExporter
{
    class GUITheme
    {
        string m_name;
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

        List<GUIProfile> m_profiles = new List<GUIProfile>();

        public List<GUIProfile> Profiles
        {
            get
            {
                return m_profiles;
            }
        }

        public GUIProfile AddProfile(string name)
        {
            GUIProfile p = new GUIProfile();
            p.Name = name;
            m_profiles.Add(p);
            return p;
        }
        public void ClearProfiles()
        {
            for (int i = 0; i < m_profiles.Count; i++)
            {
                m_profiles[i].ClearRegions();
            }
        }

        public void RemoveProfile(GUIProfile p)
        {
            m_profiles.Remove(p);
        }
        public void RemoveProfile(string p)
        {
            for (int i = 0; i < m_profiles.Count; i++)
            {
                if (m_profiles[i].Name == p)
                {
                    m_profiles.RemoveAt(i);
                    return;
                }
            }
        }
        public GUIProfile getFirst()
        {
            if (m_profiles.Count > 0)
                return m_profiles[0];
            return null;
        }
        public GUIProfile findProfile(string name)
        {
            for (int i = 0; i < m_profiles.Count; i++)
            {
                if (m_profiles[i].Name == name)
                {
                    return m_profiles[i];
                }
            }
            return null;
        }
    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace KMDSessionCreator.Data
{
    public class AppShared
    {

        static AppShared _instance = new AppShared();

        public Data.KMDSessions Sessions;

        public static AppShared Instance
        {
            get
            {
                return _instance;
            }
        }
    }
}

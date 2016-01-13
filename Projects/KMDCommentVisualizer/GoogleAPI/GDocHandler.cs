using Google.GData.Client;
using Google.GData.Spreadsheets;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace GoogleAPI
{

    public class RowComment
    {
        public int id;
        public string publisher;
        public string project;
        public string text;
        public DateTime timestamp;
    }

    public class GDocHandler
    {
        string uid;
        string pwd;
        string URI;
        Thread updateThread;

        int m_lastID=0;

        public int TimeColumn = 0;
        public int PublisherColumn = 1;
        public int ProjectColumn = 2;
        public int TextColumn = 3;
        public int IDColumn = 4;

        public delegate void DataArrivedDelegate(GDocHandler h,List<RowComment> comments);
        public event DataArrivedDelegate OnDataArrived;


        void ThreadHandler()
        {
            while(updateThread.IsAlive)
            {
                try
                {
                    _updateData();
                }catch(Exception e)
                {
                }
                Thread.Sleep(5000);
            }
        }


        public void Reset()
        {
            m_lastID = 0;
        }

        void _updateData()
        {

            ListQuery query = new ListQuery(URI);
            
            query.OrderByColumn = "";
            query.Reverse = false;
            //query.SpreadsheetQuery = "id>" + m_lastID;

            // Set the view with the new feed
            ListFeed feed = GoogleUser.Instance.Service.Query(query);
            if (feed != null)
            {
                List<RowComment> comments = new List<RowComment>();
                AtomEntryCollection entries = feed.Entries;

                for (int i = m_lastID; i < entries.Count; i++)
                {
                    ListEntry entry = entries[i] as ListEntry;
                    ListEntry.CustomElementCollection elements = entry.Elements;

                    int id = 0;
                   // int.TryParse(elements[IDColumn].Value,out id);
                   // if (id < m_lastID)
                   //     continue;
                    id = i;
                    RowComment c = new RowComment();
                    c.id=id;
                    c.publisher = elements[PublisherColumn].Value.Trim();
                    if (c.publisher == "")
                        c.publisher = "Student";
                    c.project = elements[ProjectColumn].Value.Trim();
                    c.text = elements[TextColumn].Value.Trim();
                    DateTime.TryParse(elements[TimeColumn].Value,out  c.timestamp );

                    comments.Add(c);
                   // m_lastID = Math.Max(m_lastID, c.id);
                }
                if (comments.Count == 0)
                    return;
                m_lastID = entries.Count ;
                if (OnDataArrived != null)
                    OnDataArrived(this,comments);
            }
        }

        public GDocHandler()
        {
            updateThread = new Thread(new ThreadStart(ThreadHandler));
        }

        public void Init(string uid, string pwd, string URI)
        {
            this.URI = URI;
            updateThread.Start();
        }
        public void Init(string URI)
        {
            this.URI = URI;

            updateThread.Start();
        }

        public void Close()
        {
            updateThread.Abort();
        }
    }
}

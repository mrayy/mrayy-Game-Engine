﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using Google.GData.Client;
using Google.GData.Spreadsheets;
using System.Configuration;


namespace GoogleAPI
{
    public partial class Main : Form
    {
        GDocHandler gdocHandlerStd;
        GDocHandler gdocHandlerFac;

        int m_id;

        DBHandler dbHandler;

        public Main()
        {
            InitializeComponent();

            listListView.Columns.Add("ID", 80, HorizontalAlignment.Center);
            listListView.Columns.Add("Publisher", 80, HorizontalAlignment.Center);
            listListView.Columns.Add("Project", 80, HorizontalAlignment.Center);
            listListView.Columns.Add("Text", 80, HorizontalAlignment.Center);
            listListView.Columns.Add("Timestamp", 80, HorizontalAlignment.Center);

            dbHandler = new DBHandler();
            dbHandler.ConnectDB(ConfigurationManager.AppSettings["DBHost"],
                ConfigurationManager.AppSettings["DBTarget"],
                ConfigurationManager.AppSettings["DBTable"],
                ConfigurationManager.AppSettings["DBUser"],
                ConfigurationManager.AppSettings["DBPWD"]);

            gdocHandlerStd = new GDocHandler();
            gdocHandlerFac = new GDocHandler();
            string url = ConfigurationManager.AppSettings["DocURL"];
            string url2 = ConfigurationManager.AppSettings["DocURL2"];
            gdocHandlerStd.Init(@"mrayyamen@gmail.com", @"H0wL0vely!2", url);
            gdocHandlerFac.Init(@"mrayyamen@gmail.com", @"H0wL0vely!2", url2);

            gdocHandlerStd.OnDataArrived += gdocHandler_OnDataArrived;
            gdocHandlerFac.OnDataArrived += gdocHandler_OnDataArrived;

            gdocHandlerStd.TimeColumn = 0;
            gdocHandlerStd.ProjectColumn = 1;
            gdocHandlerStd.TextColumn = 2;
            gdocHandlerStd.PublisherColumn = 3;


            gdocHandlerFac.TimeColumn = 0;
            gdocHandlerFac.PublisherColumn = 1;
            gdocHandlerFac.ProjectColumn = 2;
            gdocHandlerFac.TextColumn = 3;

            m_id = 0;

            this.Resize += Main_Resize;
        }

        void Main_Resize(object sender, EventArgs e)
        {
            if(WindowState==FormWindowState.Minimized)
            {
                _notification.Visible = true;
                _notification.ShowBalloonTip(1, "GoogleDoc", "GoogleDoc fetcher is hidden",ToolTipIcon.Info);
                this.ShowInTaskbar = false;
                //Hide();
            }else if(WindowState==FormWindowState.Normal)
            {
                _notification.Visible = false;
                this.ShowInTaskbar = true;
            }
        }

        delegate void updateListDataDel(List<RowComment> comments);

        void UpdateListData(List<RowComment> comments)
        {


            foreach (RowComment c in comments)
            {
                ListViewItem item = new ListViewItem();
                item.Text = c.id.ToString();
                item.SubItems.Add(c.publisher);
                item.SubItems.Add(c.project);
                item.SubItems.Add(c.text);
                item.SubItems.Add(c.timestamp.ToString());

                listListView.Items.Add(item);
            }
        }

        void gdocHandler_OnDataArrived(GDocHandler h, List<RowComment> comments)
        {
            foreach (RowComment c in comments)
            {
                c.id = m_id;
                m_id++;
            }
            this.Invoke(new updateListDataDel(UpdateListData), comments);
            dbHandler.AddComments(comments);
        }

        void Init(string user,string pwd,string URI)
        {
            


        }

        void ConnectDB()
        {

        }
        /// <summary>
        /// Sets the list view on the List tab
        /// </summary>
        /// <param name="feed">The feed providing the data</param>
        void SetListListView(ListFeed feed)
        {
           

        }
        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            dbHandler.InitDBTables();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            dbHandler.ClearDBTables();
            gdocHandlerFac.Reset();
            gdocHandlerStd.Reset();
        }

        private void button3_Click(object sender, EventArgs e)
        {
            (new DocFinder()).Show();
        }

        private void Main_FormClosing(object sender, FormClosingEventArgs e)
        {
            gdocHandlerFac.Close();
            gdocHandlerStd.Close();
            dbHandler.CloseConnection();
        }

        private void _notification_Click(object sender, EventArgs e)
        {
        }

        private void showToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.WindowState = FormWindowState.Normal;

        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void _notification_MouseClick(object sender, MouseEventArgs e)
        {
            if(e.Button==System.Windows.Forms.MouseButtons.Left)
                this.WindowState = FormWindowState.Normal;

        }
    }
}

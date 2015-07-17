using Google.GData.Client;
using Google.GData.Spreadsheets;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace GoogleAPI
{
    public partial class DocFinder : Form
    {
        public DocFinder()
        {
            InitializeComponent();
        }

        private void DocFinder_Load(object sender, EventArgs e)
        {

            SpreadsheetQuery query = new SpreadsheetQuery();
            query.Title = "";
            setSpreadsheetListView(GoogleUser.Instance.Service.Query(query));

        }

        void setSpreadsheetListView(SpreadsheetFeed feed)
        {
            // Clear out the old list
            this.listView1.Items.Clear();

            AtomEntryCollection entries = feed.Entries;
            for (int i = 0; i < entries.Count; i++)
            {
                // Get the worksheets feed URI
                AtomLink worksheetsLink = entries[i].Links.FindService(GDataSpreadsheetsNameTable.WorksheetRel,
                                                                       AtomLink.ATOM_TYPE);

                // Create an item that will display the title and hide the worksheets feed URI
                ListViewItem item = new ListViewItem(new string[2] { entries[i].Title.Text, worksheetsLink.HRef.Content });

                this.listView1.Items.Add(item);
            }
        }

        private void listView1_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (listView1.SelectedItems.Count > 0)
            {
                // Get the worksheet feed from the selected entry
                WorksheetQuery query = new WorksheetQuery(listView1.SelectedItems[0].SubItems[1].Text);
                textBox1.Text = listView1.SelectedItems[0].SubItems[1].Text;
                SetWorksheetListView(GoogleUser.Instance.Service.Query(query));
            }

        }
        void SetWorksheetListView(WorksheetFeed feed)
        {
            this.listView2.Items.Clear();

            AtomEntryCollection entries = feed.Entries;
            for (int i = 0; i < entries.Count; i++)
            {
                // Get the cells feed URI
                AtomLink cellsLink = entries[i].Links.FindService(GDataSpreadsheetsNameTable.CellRel, AtomLink.ATOM_TYPE);

                // Get the list feed URI
                AtomLink listLink = entries[i].Links.FindService(GDataSpreadsheetsNameTable.ListRel, AtomLink.ATOM_TYPE);

                // Create an item that will display the title and hide the cells and list feed URIs
                ListViewItem item = new ListViewItem(new string[3]{ entries[i].Title.Text + " (Rows:"
                                                     + ((WorksheetEntry)entries[i]).RowCount.Count
                                                     + ", Cols:" + ((WorksheetEntry)entries[i]).ColCount.Count + ")",
                                                     cellsLink.HRef.Content, listLink.HRef.Content});

                this.listView2.Items.Add(item);
            }
        }

        private void listView2_SelectedIndexChanged(object sender, EventArgs e)
        {

            if (listView2.SelectedItems.Count > 0)
            {
                textBox1.Text = listView2.SelectedItems[0].SubItems[2].Text;

                CellQuery query = new CellQuery(listView2.SelectedItems[0].SubItems[1].Text);
                SetCellListView(GoogleUser.Instance.Service.Query(query));
            }
        }


        /// <summary>
        /// Sets the list view on the Cells tab
        /// </summary>
        /// <param name="feed">The feed providing the data</param>
        void SetCellListView(CellFeed feed)
        {
            // Clear out all the old information
            this.listView3.Items.Clear();
            this.listView3.Columns.Clear();

            AtomEntryCollection entries = feed.Entries;

            // Add in the column headers, as many as the column count asks
            // The number of rows, we only care to go as far as the data goes
            this.listView3.Columns.Add("", 20, HorizontalAlignment.Left);
            for (int i = 1; i < feed.ColCount.Count; i++)
            {
                this.listView3.Columns.Add(i.ToString(), 80, HorizontalAlignment.Center);
            }

            int currentRow = 1;
            int currentCol = 1;

            ListViewItem item = new ListViewItem();
            item.Text = 1.ToString();

            for (int i = 0; i < entries.Count; i++)
            {
                CellEntry entry = entries[i] as CellEntry;
                if (entry != null)
                {

                    // Add the current row, since we are starting 
                    // a new row in data from the feed
                    while (entry.Cell.Row > currentRow)
                    {
                        this.listView3.Items.Add(item);
                        item = new ListViewItem();
                        item.Text = (currentRow + 1).ToString();
                        currentRow++;
                        currentCol = 1;
                    }

                    // Add blank entries where there is no data for the column
                    while (entry.Cell.Column > currentCol)
                    {
                        item.SubItems.Add("");
                        currentCol++;
                    }

                    // Add the current data entry
                    item.SubItems.Add(entry.Cell.Value);
                    currentCol++;
                }
            }
        }
    }
}

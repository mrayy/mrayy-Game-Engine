using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MySql.Data.MySqlClient;
using System.Windows.Forms;

namespace GoogleAPI
{
    public class DBHandler
    {
        private MySqlConnection sqlConnection;
        private string m_server, m_db, m_uid, m_pwd;

        string CommentsTB = "CommentsTB";

        int m_lastID=0;

        public DBHandler()
        {

        }

        public bool ConnectDB(string server,string db,string table,string uid,string pwd)
        {
            m_server = server;
            m_db = db;
            m_uid = uid;
            m_pwd = pwd;
            CommentsTB = table;

            string connectionStr = "SERVER=" + server + "; DATABASE=" + db + "; UID=" + uid + "; PASSWORD=" + pwd+";";
            MySqlConnectionStringBuilder connBuilder=new MySqlConnectionStringBuilder();
            connBuilder.Server = server;
            connBuilder.Database = db;
            connBuilder.UserID = uid;
            connBuilder.Password = pwd;

            sqlConnection = new MySqlConnection(connBuilder.ConnectionString);

            m_lastID=FetchLastID();
            return true;
        }

        public bool OpenConnection()
        {

            try
            {
                sqlConnection.Open();
                return true;
            }
            catch (MySqlException ex)
            {
                switch (ex.Number)
                {
                    case 0:
                        MessageBox.Show("Cannot connect to server.  Contact administrator:"+ ex.Message);
                        break;

                    case 1045:
                        MessageBox.Show("Invalid username/password, please try again:" + ex.Message);
                        break;
                    default:
                        MessageBox.Show(ex.Message);
                        break;
                }
                return false;
            }
        }

        public bool CloseConnection()
        {
            try
            {
                sqlConnection.Close();
                return true;
            }
            catch (MySqlException ex)
            {
               MessageBox.Show(ex.Message);
                return false;
            }
        }

        int FetchLastID()
        {
            string query = "select id from " + CommentsTB + " ORDER BY id DESC LIMIT 1;";
            int id = -1;
            if (this.OpenConnection() == true)
            {
                MySqlCommand cmd = new MySqlCommand(query, sqlConnection);

                try
                {
                    MySqlDataReader rdr= cmd.ExecuteReader();
                    if (rdr.Read())
                    {
                        id=int.Parse(rdr["id"].ToString());
                    }
                }
                catch (MySqlException ex)
                {
                    MessageBox.Show(ex.Message);
                }
                this.CloseConnection();
            }
            return id;
        }

        public void InitDBTables()
        {
            string query = "create table " + CommentsTB + " ( id INT NOT NULL, Publisher VARCHAR(50), Project VARCHAR(100),Text NVARCHAR(500), Time TIMESTAMP, PRIMARY KEY(id) );";

            if (this.OpenConnection() == true)
            {
                MySqlCommand cmd = new MySqlCommand(query, sqlConnection);

                try
                {
                    cmd.ExecuteNonQuery();
                }catch(MySqlException ex)
                {
                    MessageBox.Show(ex.Message);
                }
                this.CloseConnection();
            }
        }

        public void ClearDBTables()
        {
            string query = "delete from " + CommentsTB ;

            if (this.OpenConnection() == true)
            {
                MySqlCommand cmd = new MySqlCommand(query, sqlConnection);

                try
                {
                    cmd.ExecuteNonQuery();
                }
                catch (MySqlException ex)
                {
                    MessageBox.Show(ex.Message);
                }
                this.CloseConnection();
            }
        }
        public void AddComments(List<RowComment> comments)
        {

            string query = "insert into "+CommentsTB+" (id,Publisher,Project,Text,Time) Values " ;

            bool need = false;
            int count = 0;
            List<string> vals = new List<string>();
            foreach(RowComment c in comments)
            {
                if (c.id <= m_lastID)
                    continue;
                if (need)
                    query += ",";
                need = true;
                c.text=c.text.Replace("\'","- ");
                if(c.text.Length>500)
                    c.text=c.text.Substring(0, 500);
                query += "(" + c.id.ToString() + ", '" + c.publisher + "','" + c.project + "',@V"+count.ToString()+",'" + c.timestamp.ToString("yyyy-MM-dd HH:mm:ss") + "')";
                vals.Add(c.text);
                m_lastID = Math.Max(m_lastID, c.id);
                count++;
            }
            if (count==0)
                return;

            if (this.OpenConnection() == true)
            {
                MySqlCommand cmd = new MySqlCommand(query, sqlConnection);
                //add values now
                for (int i = 0; i < count;++i)
                {
                    cmd.Parameters.AddWithValue("@V" + i.ToString(), vals[i]);
                }
                try
                {
                    cmd.ExecuteNonQuery();
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message);
                }
                this.CloseConnection();
            }
        }
    }
}

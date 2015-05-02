using Google.Apis.Services;
using Google.Apis.YouTube.v3;
using Google.Apis.YouTube.v3.Data;
using QRCoder;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.Drawing;
using System.Windows.Forms;
using System.Xml;
namespace YoutubeIDGrabber
{
    public partial class Form1 : Form
    {
        private Process m_process;
        private QRCodeGenerator qrGenerator = new QRCodeGenerator();
        private string clientID = "";
        private string developerKey = "AI39si4mPuDr0HYtr2PBsTVbe0sYNZEmw6pGOv3sVqliTh9lJgqVV4d1FqPiWxBUhYUltzbOga4KAv214mPmbNoPO_Uk";
        public Form1()
        {
            this.InitializeComponent();
            this.Start();
        }
        private void Start()
        {
            if (this.m_process != null && !this.m_process.HasExited)
            {
                this.m_process.Kill();
            }
            YouTubeService youTubeService = new YouTubeService(new Google.Apis.Services.BaseClientService.Initializer
            {
                ApiKey = "AIzaSyARcfpmuOwkTh7qV2mqTIX88OCTicxsIkA",
                ApplicationName = base.GetType().ToString()
            });
            PlaylistItemsResource.ListRequest listRequest = youTubeService.PlaylistItems.List("snippet");
            listRequest.PlaylistId = "UUCltJX1vyLHvY4ELpTN58GA";
            listRequest.MaxResults = new long?(50L);
            List<Video> list = new List<Video>();
            PlaylistItemListResponse playlistItemListResponse;
            for (string pageToken = ""; pageToken != null; pageToken = playlistItemListResponse.NextPageToken)
            {
                listRequest.PageToken = pageToken;
                playlistItemListResponse = listRequest.Execute();
                if (playlistItemListResponse.Items.Count == 0)
                {
                    break;
                }
                VideosResource.ListRequest listRequest2 = youTubeService.Videos.List("snippet");
                foreach (PlaylistItem current in playlistItemListResponse.Items)
                {
                    if (current.Id != null && !(current.Id == "") && current.Snippet.PublishedAt.Value.Year >= 2014)
                    {
                        listRequest2.Id = current.Snippet.ResourceId.VideoId;
                        VideoListResponse videoListResponse = listRequest2.Execute();
                        if (videoListResponse.Items.Count != 0)
                        {
                            list.Add(videoListResponse.Items[0]);
                        }
                    }
                }
            }
            this.ExportResults(list);
            this.printVideoFeed(list);
            try
            {
                this.m_process = Process.Start("KMDChannelVisualizer.exe");
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }
        private void printVideoFeed(List<Video> feed)
        {
            this.listBox1.Items.Clear();
            foreach (Video current in feed)
            {
                this.printVideoEntry(current);
            }
        }
        private void printVideoEntry(Video video)
        {
            this.listBox1.Items.Add(video.Snippet.Title);
        }
        private void ExportResults(List<Video> feed)
        {
            XmlWriter xmlWriter = XmlWriter.Create("Feed.xml", new XmlWriterSettings
            {
                Indent = true
            });
            xmlWriter.WriteStartElement("Feed");
            foreach (Video current in feed)
            {
                if (current.Id != null && !(current.Id == ""))
                {
                    xmlWriter.WriteStartElement("Video");
                    string plainText = "https://www.youtube.com/watch?v=" + current.Id;
                    QRCodeGenerator.QRCode qRCode = this.qrGenerator.CreateQrCode(plainText, QRCodeGenerator.ECCLevel.H);
                    new Bitmap(qRCode.GetGraphic(10), new Size(256, 256)).Save("QR\\" + current.Id + ".png");
                    xmlWriter.WriteAttributeString("ID", current.Id);
                    xmlWriter.WriteAttributeString("Title", current.Snippet.Title);
                    xmlWriter.WriteAttributeString("PublishAt", current.Snippet.PublishedAt.ToString());
                    xmlWriter.WriteElementString("Description", current.Snippet.Description);
                    xmlWriter.WriteElementString("Thumbnail", current.Snippet.Thumbnails.Default.Url);
                    xmlWriter.WriteEndElement();
                }
            }
            xmlWriter.WriteEndElement();
            xmlWriter.Close();
        }
        private void button1_Click(object sender, EventArgs e)
        {
            this.Start();
        }
    }
}

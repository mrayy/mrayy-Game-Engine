namespace KMDSessionCreator
{
    partial class SessionForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.label1 = new System.Windows.Forms.Label();
            this.txtsession = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.txtTheme = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.chkIsSession = new System.Windows.Forms.CheckBox();
            this.label6 = new System.Windows.Forms.Label();
            this.txtDesc = new System.Windows.Forms.TextBox();
            this.label7 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.lstAdvisors = new System.Windows.Forms.ListBox();
            this.button1 = new System.Windows.Forms.Button();
            this.button2 = new System.Windows.Forms.Button();
            this.button3 = new System.Windows.Forms.Button();
            this.button4 = new System.Windows.Forms.Button();
            this.lstComte = new System.Windows.Forms.ListBox();
            this.label9 = new System.Windows.Forms.Label();
            this.lstSubProj = new System.Windows.Forms.ListBox();
            this.label10 = new System.Windows.Forms.Label();
            this.button5 = new System.Windows.Forms.Button();
            this.button6 = new System.Windows.Forms.Button();
            this.txtPicture = new System.Windows.Forms.TextBox();
            this.txtStartTime = new System.Windows.Forms.TextBox();
            this.txtEndTime = new System.Windows.Forms.TextBox();
            this.txtR = new System.Windows.Forms.TextBox();
            this.txtB = new System.Windows.Forms.TextBox();
            this.txtG = new System.Windows.Forms.TextBox();
            this.lblClr = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(13, 36);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(80, 12);
            this.label1.TabIndex = 0;
            this.label1.Text = "Session Name:";
            // 
            // txtsession
            // 
            this.txtsession.Location = new System.Drawing.Point(99, 33);
            this.txtsession.Name = "txtsession";
            this.txtsession.Size = new System.Drawing.Size(372, 19);
            this.txtsession.TabIndex = 0;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(13, 142);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(105, 12);
            this.label2.TabIndex = 2;
            this.label2.Text = "Session Start Time:";
            // 
            // txtTheme
            // 
            this.txtTheme.Location = new System.Drawing.Point(99, 58);
            this.txtTheme.Name = "txtTheme";
            this.txtTheme.Size = new System.Drawing.Size(372, 19);
            this.txtTheme.TabIndex = 1;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(13, 61);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(41, 12);
            this.label3.TabIndex = 3;
            this.label3.Text = "Theme:";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(12, 215);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(98, 12);
            this.label4.TabIndex = 5;
            this.label4.Text = "Background Color:";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(14, 252);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(43, 12);
            this.label5.TabIndex = 6;
            this.label5.Text = "Picture:";
            // 
            // chkIsSession
            // 
            this.chkIsSession.AutoSize = true;
            this.chkIsSession.Location = new System.Drawing.Point(99, 83);
            this.chkIsSession.Name = "chkIsSession";
            this.chkIsSession.Size = new System.Drawing.Size(77, 16);
            this.chkIsSession.TabIndex = 2;
            this.chkIsSession.Text = "Is Session";
            this.chkIsSession.UseVisualStyleBackColor = true;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(13, 167);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(99, 12);
            this.label6.TabIndex = 8;
            this.label6.Text = "Session End Time:";
            // 
            // txtDesc
            // 
            this.txtDesc.Location = new System.Drawing.Point(99, 299);
            this.txtDesc.Multiline = true;
            this.txtDesc.Name = "txtDesc";
            this.txtDesc.Size = new System.Drawing.Size(372, 90);
            this.txtDesc.TabIndex = 9;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(13, 302);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(63, 12);
            this.label7.TabIndex = 9;
            this.label7.Text = "Description";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(514, 36);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(69, 12);
            this.label8.TabIndex = 11;
            this.label8.Text = "Advisor List:";
            // 
            // lstAdvisors
            // 
            this.lstAdvisors.FormattingEnabled = true;
            this.lstAdvisors.ItemHeight = 12;
            this.lstAdvisors.Location = new System.Drawing.Point(516, 61);
            this.lstAdvisors.Name = "lstAdvisors";
            this.lstAdvisors.Size = new System.Drawing.Size(197, 136);
            this.lstAdvisors.TabIndex = 13;
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(516, 204);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 23);
            this.button1.TabIndex = 14;
            this.button1.Text = "+";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(597, 204);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(41, 23);
            this.button2.TabIndex = 15;
            this.button2.Text = "-";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // button3
            // 
            this.button3.Location = new System.Drawing.Point(597, 429);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(41, 23);
            this.button3.TabIndex = 18;
            this.button3.Text = "-";
            this.button3.UseVisualStyleBackColor = true;
            this.button3.Click += new System.EventHandler(this.button3_Click);
            // 
            // button4
            // 
            this.button4.Location = new System.Drawing.Point(516, 429);
            this.button4.Name = "button4";
            this.button4.Size = new System.Drawing.Size(75, 23);
            this.button4.TabIndex = 17;
            this.button4.Text = "+";
            this.button4.UseVisualStyleBackColor = true;
            this.button4.Click += new System.EventHandler(this.button4_Click);
            // 
            // lstComte
            // 
            this.lstComte.FormattingEnabled = true;
            this.lstComte.ItemHeight = 12;
            this.lstComte.Location = new System.Drawing.Point(516, 299);
            this.lstComte.Name = "lstComte";
            this.lstComte.Size = new System.Drawing.Size(197, 124);
            this.lstComte.TabIndex = 16;
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(514, 274);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(85, 12);
            this.label9.TabIndex = 15;
            this.label9.Text = "Committee List:";
            // 
            // lstSubProj
            // 
            this.lstSubProj.FormattingEnabled = true;
            this.lstSubProj.ItemHeight = 12;
            this.lstSubProj.Location = new System.Drawing.Point(99, 399);
            this.lstSubProj.Name = "lstSubProj";
            this.lstSubProj.Size = new System.Drawing.Size(372, 124);
            this.lstSubProj.TabIndex = 10;
            this.lstSubProj.SelectedIndexChanged += new System.EventHandler(this.lstSubProj_SelectedIndexChanged);
            this.lstSubProj.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.lstSubProj_MouseDoubleClick);
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(12, 399);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(71, 12);
            this.label10.TabIndex = 20;
            this.label10.Text = "Sub projects:";
            // 
            // button5
            // 
            this.button5.Location = new System.Drawing.Point(180, 529);
            this.button5.Name = "button5";
            this.button5.Size = new System.Drawing.Size(41, 23);
            this.button5.TabIndex = 12;
            this.button5.Text = "-";
            this.button5.UseVisualStyleBackColor = true;
            this.button5.Click += new System.EventHandler(this.button5_Click);
            // 
            // button6
            // 
            this.button6.Location = new System.Drawing.Point(99, 529);
            this.button6.Name = "button6";
            this.button6.Size = new System.Drawing.Size(75, 23);
            this.button6.TabIndex = 11;
            this.button6.Text = "+";
            this.button6.UseVisualStyleBackColor = true;
            this.button6.Click += new System.EventHandler(this.button6_Click);
            // 
            // txtPicture
            // 
            this.txtPicture.Location = new System.Drawing.Point(123, 249);
            this.txtPicture.Name = "txtPicture";
            this.txtPicture.Size = new System.Drawing.Size(348, 19);
            this.txtPicture.TabIndex = 8;
            // 
            // txtStartTime
            // 
            this.txtStartTime.Location = new System.Drawing.Point(123, 139);
            this.txtStartTime.Name = "txtStartTime";
            this.txtStartTime.Size = new System.Drawing.Size(348, 19);
            this.txtStartTime.TabIndex = 3;
            // 
            // txtEndTime
            // 
            this.txtEndTime.Location = new System.Drawing.Point(123, 164);
            this.txtEndTime.Name = "txtEndTime";
            this.txtEndTime.Size = new System.Drawing.Size(348, 19);
            this.txtEndTime.TabIndex = 4;
            // 
            // txtR
            // 
            this.txtR.Location = new System.Drawing.Point(123, 215);
            this.txtR.Name = "txtR";
            this.txtR.Size = new System.Drawing.Size(51, 19);
            this.txtR.TabIndex = 5;
            this.txtR.TextChanged += new System.EventHandler(this.txtR_TextChanged);
            // 
            // txtB
            // 
            this.txtB.Location = new System.Drawing.Point(237, 215);
            this.txtB.Name = "txtB";
            this.txtB.Size = new System.Drawing.Size(51, 19);
            this.txtB.TabIndex = 7;
            this.txtB.TextChanged += new System.EventHandler(this.txtR_TextChanged);
            // 
            // txtG
            // 
            this.txtG.Location = new System.Drawing.Point(180, 215);
            this.txtG.Name = "txtG";
            this.txtG.Size = new System.Drawing.Size(51, 19);
            this.txtG.TabIndex = 6;
            this.txtG.TextChanged += new System.EventHandler(this.txtR_TextChanged);
            // 
            // lblClr
            // 
            this.lblClr.BackColor = System.Drawing.Color.White;
            this.lblClr.Location = new System.Drawing.Point(294, 204);
            this.lblClr.Name = "lblClr";
            this.lblClr.Size = new System.Drawing.Size(100, 34);
            this.lblClr.TabIndex = 29;
            this.lblClr.Click += new System.EventHandler(this.lblClr_Click);
            // 
            // SessionForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(737, 578);
            this.Controls.Add(this.lblClr);
            this.Controls.Add(this.txtG);
            this.Controls.Add(this.txtB);
            this.Controls.Add(this.txtR);
            this.Controls.Add(this.txtEndTime);
            this.Controls.Add(this.txtStartTime);
            this.Controls.Add(this.txtPicture);
            this.Controls.Add(this.button5);
            this.Controls.Add(this.button6);
            this.Controls.Add(this.label10);
            this.Controls.Add(this.lstSubProj);
            this.Controls.Add(this.button3);
            this.Controls.Add(this.button4);
            this.Controls.Add(this.lstComte);
            this.Controls.Add(this.label9);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.lstAdvisors);
            this.Controls.Add(this.label8);
            this.Controls.Add(this.txtDesc);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.chkIsSession);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.txtTheme);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.txtsession);
            this.Controls.Add(this.label1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "SessionForm";
            this.Text = "SessionForm";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.SessionForm_FormClosing);
            this.VisibleChanged += new System.EventHandler(this.SessionForm_VisibleChanged);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox txtsession;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox txtTheme;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.CheckBox chkIsSession;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.TextBox txtDesc;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.ListBox lstAdvisors;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.Button button4;
        private System.Windows.Forms.ListBox lstComte;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.ListBox lstSubProj;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.Button button5;
        private System.Windows.Forms.Button button6;
        private System.Windows.Forms.TextBox txtPicture;
        private System.Windows.Forms.TextBox txtStartTime;
        private System.Windows.Forms.TextBox txtEndTime;
        private System.Windows.Forms.TextBox txtR;
        private System.Windows.Forms.TextBox txtB;
        private System.Windows.Forms.TextBox txtG;
        private System.Windows.Forms.Label lblClr;
    }
}
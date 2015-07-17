namespace KMDSessionCreator
{
    partial class FacultyForm
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
            this.lstFac = new System.Windows.Forms.ListBox();
            this.label1 = new System.Windows.Forms.Label();
            this.button2 = new System.Windows.Forms.Button();
            this.button1 = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.txtName = new System.Windows.Forms.TextBox();
            this.txtTitle = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.txtImg = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.grpProp = new System.Windows.Forms.GroupBox();
            this.grpProp.SuspendLayout();
            this.SuspendLayout();
            // 
            // lstFac
            // 
            this.lstFac.FormattingEnabled = true;
            this.lstFac.ItemHeight = 12;
            this.lstFac.Location = new System.Drawing.Point(12, 38);
            this.lstFac.Name = "lstFac";
            this.lstFac.Size = new System.Drawing.Size(186, 340);
            this.lstFac.TabIndex = 0;
            this.lstFac.SelectedIndexChanged += new System.EventHandler(this.lstFac_SelectedIndexChanged);
            this.lstFac.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.lstFac_MouseDoubleClick);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(13, 20);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(68, 12);
            this.label1.TabIndex = 1;
            this.label1.Text = "Faculty List:";
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(96, 384);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(41, 23);
            this.button2.TabIndex = 2;
            this.button2.Text = "-";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(12, 384);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 23);
            this.button1.TabIndex = 1;
            this.button1.Text = "+";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(9, 28);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(36, 12);
            this.label2.TabIndex = 17;
            this.label2.Text = "Name:";
            // 
            // txtName
            // 
            this.txtName.Location = new System.Drawing.Point(51, 25);
            this.txtName.Name = "txtName";
            this.txtName.Size = new System.Drawing.Size(187, 19);
            this.txtName.TabIndex = 3;
            this.txtName.TextChanged += new System.EventHandler(this.txtName_TextChanged);
            this.txtName.Leave += new System.EventHandler(this.txtName_Leave);
            // 
            // txtTitle
            // 
            this.txtTitle.Location = new System.Drawing.Point(51, 50);
            this.txtTitle.Multiline = true;
            this.txtTitle.Name = "txtTitle";
            this.txtTitle.Size = new System.Drawing.Size(187, 89);
            this.txtTitle.TabIndex = 4;
            this.txtTitle.TextChanged += new System.EventHandler(this.txtTitle_TextChanged);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(9, 53);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(30, 12);
            this.label3.TabIndex = 19;
            this.label3.Text = "Title:";
            // 
            // txtImg
            // 
            this.txtImg.Location = new System.Drawing.Point(51, 145);
            this.txtImg.Name = "txtImg";
            this.txtImg.Size = new System.Drawing.Size(187, 19);
            this.txtImg.TabIndex = 5;
            this.txtImg.TextChanged += new System.EventHandler(this.txtImg_TextChanged);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(9, 148);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(37, 12);
            this.label4.TabIndex = 21;
            this.label4.Text = "Image:";
            // 
            // grpProp
            // 
            this.grpProp.Controls.Add(this.txtTitle);
            this.grpProp.Controls.Add(this.txtImg);
            this.grpProp.Controls.Add(this.label2);
            this.grpProp.Controls.Add(this.label4);
            this.grpProp.Controls.Add(this.txtName);
            this.grpProp.Controls.Add(this.label3);
            this.grpProp.Location = new System.Drawing.Point(222, 38);
            this.grpProp.Name = "grpProp";
            this.grpProp.Size = new System.Drawing.Size(244, 206);
            this.grpProp.TabIndex = 23;
            this.grpProp.TabStop = false;
            this.grpProp.Text = "Properties";
            this.grpProp.Visible = false;
            // 
            // FacultyForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(478, 437);
            this.Controls.Add(this.grpProp);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.lstFac);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "FacultyForm";
            this.Text = "Faculty List";
            this.grpProp.ResumeLayout(false);
            this.grpProp.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ListBox lstFac;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox txtName;
        private System.Windows.Forms.TextBox txtTitle;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox txtImg;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.GroupBox grpProp;
    }
}
namespace KMDSessionCreator
{
    partial class ColorSelectorForm
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
            this.lstColor = new System.Windows.Forms.ListBox();
            this.lblClr = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // lstColor
            // 
            this.lstColor.FormattingEnabled = true;
            this.lstColor.ItemHeight = 12;
            this.lstColor.Location = new System.Drawing.Point(12, 12);
            this.lstColor.Name = "lstColor";
            this.lstColor.Size = new System.Drawing.Size(250, 364);
            this.lstColor.TabIndex = 0;
            this.lstColor.SelectedIndexChanged += new System.EventHandler(this.lstColor_SelectedIndexChanged);
            this.lstColor.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.lstColor_MouseDoubleClick);
            // 
            // lblClr
            // 
            this.lblClr.Location = new System.Drawing.Point(12, 379);
            this.lblClr.Name = "lblClr";
            this.lblClr.Size = new System.Drawing.Size(250, 38);
            this.lblClr.TabIndex = 1;
            // 
            // ColorSelectorForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(274, 444);
            this.Controls.Add(this.lblClr);
            this.Controls.Add(this.lstColor);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "ColorSelectorForm";
            this.Text = "ColorSelector";
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ListBox lstColor;
        private System.Windows.Forms.Label lblClr;
    }
}
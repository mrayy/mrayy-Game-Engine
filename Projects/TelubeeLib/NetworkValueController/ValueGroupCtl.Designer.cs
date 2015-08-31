namespace NetworkValueController
{
    partial class ValueGroupCtl
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

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.vgrp = new System.Windows.Forms.GroupBox();
            this.SuspendLayout();
            // 
            // vgrp
            // 
            this.vgrp.Dock = System.Windows.Forms.DockStyle.Fill;
            this.vgrp.Location = new System.Drawing.Point(0, 0);
            this.vgrp.Name = "vgrp";
            this.vgrp.Size = new System.Drawing.Size(558, 509);
            this.vgrp.TabIndex = 0;
            this.vgrp.TabStop = false;
            this.vgrp.Text = "groupBox1";
            // 
            // ValueGroupCtl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.vgrp);
            this.Name = "ValueGroupCtl";
            this.Size = new System.Drawing.Size(558, 509);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox vgrp;
    }
}

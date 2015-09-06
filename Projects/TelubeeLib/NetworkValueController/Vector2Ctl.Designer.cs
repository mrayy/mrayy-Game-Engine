namespace NetworkValueController
{
    partial class Vector2Ctl
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
            this.XVal = new System.Windows.Forms.TrackBar();
            this.YVal = new System.Windows.Forms.TrackBar();
            ((System.ComponentModel.ISupportInitialize)(this.XVal)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.YVal)).BeginInit();
            this.SuspendLayout();
            // 
            // XVal
            // 
            this.XVal.Location = new System.Drawing.Point(3, 3);
            this.XVal.Maximum = 100;
            this.XVal.Minimum = -1;
            this.XVal.Name = "XVal";
            this.XVal.Size = new System.Drawing.Size(190, 45);
            this.XVal.TabIndex = 0;
            this.XVal.ValueChanged += new System.EventHandler(this.XVal_ValueChanged);
            // 
            // YVal
            // 
            this.YVal.Location = new System.Drawing.Point(179, 3);
            this.YVal.Maximum = 100;
            this.YVal.Minimum = -1;
            this.YVal.Name = "YVal";
            this.YVal.Size = new System.Drawing.Size(200, 45);
            this.YVal.TabIndex = 1;
            this.YVal.ValueChanged += new System.EventHandler(this.YVal_ValueChanged);
            // 
            // Vector2Ctl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.YVal);
            this.Controls.Add(this.XVal);
            this.Name = "Vector2Ctl";
            this.Size = new System.Drawing.Size(382, 38);
            this.Resize += new System.EventHandler(this.Vector2Ctl_Resize);
            ((System.ComponentModel.ISupportInitialize)(this.XVal)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.YVal)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TrackBar XVal;
        private System.Windows.Forms.TrackBar YVal;
    }
}

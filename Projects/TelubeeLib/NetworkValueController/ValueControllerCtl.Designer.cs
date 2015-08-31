namespace NetworkValueController
{
    partial class ValueControllerCtl
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
            this.vlbl = new System.Windows.Forms.Label();
            this.namelbl = new System.Windows.Forms.Label();
            this.valLbl = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // vlbl
            // 
            this.vlbl.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.vlbl.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.vlbl.Location = new System.Drawing.Point(241, 0);
            this.vlbl.Name = "vlbl";
            this.vlbl.Size = new System.Drawing.Size(221, 22);
            this.vlbl.TabIndex = 2;
            this.vlbl.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.vlbl.Visible = false;
            // 
            // namelbl
            // 
            this.namelbl.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.namelbl.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.namelbl.Location = new System.Drawing.Point(11, 0);
            this.namelbl.Name = "namelbl";
            this.namelbl.Size = new System.Drawing.Size(224, 22);
            this.namelbl.TabIndex = 1;
            this.namelbl.Text = "label2";
            this.namelbl.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // valLbl
            // 
            this.valLbl.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.valLbl.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.valLbl.Location = new System.Drawing.Point(468, 0);
            this.valLbl.Name = "valLbl";
            this.valLbl.Size = new System.Drawing.Size(100, 22);
            this.valLbl.TabIndex = 3;
            this.valLbl.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // ValueControllerCtl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.valLbl);
            this.Controls.Add(this.vlbl);
            this.Controls.Add(this.namelbl);
            this.Name = "ValueControllerCtl";
            this.Size = new System.Drawing.Size(596, 29);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Label vlbl;
        private System.Windows.Forms.Label namelbl;
        private System.Windows.Forms.Label valLbl;
    }
}

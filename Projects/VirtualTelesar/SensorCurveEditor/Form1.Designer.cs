namespace SensorCurveEditor
{
    partial class CurveEditorForm
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
            this.txtName = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.numMinIn = new System.Windows.Forms.NumericUpDown();
            this.numMaxIn = new System.Windows.Forms.NumericUpDown();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.numMinOut = new System.Windows.Forms.NumericUpDown();
            this.numMaxOut = new System.Windows.Forms.NumericUpDown();
            this.button1 = new System.Windows.Forms.Button();
            this.button2 = new System.Windows.Forms.Button();
            this.curveEditor = new YLScsDrawing.Controls.ImageCurve();
            this.label6 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.numMinIn)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numMaxIn)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numMinOut)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numMaxOut)).BeginInit();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(13, 87);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(69, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "Curve Name:";
            this.label1.Click += new System.EventHandler(this.label1_Click);
            // 
            // txtName
            // 
            this.txtName.Location = new System.Drawing.Point(88, 84);
            this.txtName.Name = "txtName";
            this.txtName.Size = new System.Drawing.Size(120, 20);
            this.txtName.TabIndex = 2;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(13, 139);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(69, 13);
            this.label2.TabIndex = 1;
            this.label2.Text = "Input Range:";
            this.label2.Click += new System.EventHandler(this.label1_Click);
            // 
            // numMinIn
            // 
            this.numMinIn.DecimalPlaces = 2;
            this.numMinIn.Increment = new decimal(new int[] {
            5,
            0,
            0,
            131072});
            this.numMinIn.Location = new System.Drawing.Point(88, 137);
            this.numMinIn.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.numMinIn.Minimum = new decimal(new int[] {
            1000,
            0,
            0,
            -2147483648});
            this.numMinIn.Name = "numMinIn";
            this.numMinIn.Size = new System.Drawing.Size(59, 20);
            this.numMinIn.TabIndex = 3;
            this.numMinIn.ValueChanged += new System.EventHandler(this.numMinIn_ValueChanged);
            // 
            // numMaxIn
            // 
            this.numMaxIn.DecimalPlaces = 2;
            this.numMaxIn.Increment = new decimal(new int[] {
            5,
            0,
            0,
            131072});
            this.numMaxIn.Location = new System.Drawing.Point(153, 137);
            this.numMaxIn.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.numMaxIn.Minimum = new decimal(new int[] {
            1000,
            0,
            0,
            -2147483648});
            this.numMaxIn.Name = "numMaxIn";
            this.numMaxIn.Size = new System.Drawing.Size(59, 20);
            this.numMaxIn.TabIndex = 3;
            this.numMaxIn.ValueChanged += new System.EventHandler(this.numMaxIn_ValueChanged);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(105, 121);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(24, 13);
            this.label3.TabIndex = 1;
            this.label3.Text = "Min";
            this.label3.Click += new System.EventHandler(this.label1_Click);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(167, 121);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(27, 13);
            this.label4.TabIndex = 1;
            this.label4.Text = "Max";
            this.label4.Click += new System.EventHandler(this.label1_Click);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(5, 165);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(77, 13);
            this.label5.TabIndex = 1;
            this.label5.Text = "Output Range:";
            this.label5.Click += new System.EventHandler(this.label1_Click);
            // 
            // numMinOut
            // 
            this.numMinOut.DecimalPlaces = 2;
            this.numMinOut.Increment = new decimal(new int[] {
            5,
            0,
            0,
            131072});
            this.numMinOut.Location = new System.Drawing.Point(88, 163);
            this.numMinOut.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.numMinOut.Minimum = new decimal(new int[] {
            1000,
            0,
            0,
            -2147483648});
            this.numMinOut.Name = "numMinOut";
            this.numMinOut.Size = new System.Drawing.Size(59, 20);
            this.numMinOut.TabIndex = 3;
            this.numMinOut.ValueChanged += new System.EventHandler(this.numMinOut_ValueChanged);
            // 
            // numMaxOut
            // 
            this.numMaxOut.DecimalPlaces = 2;
            this.numMaxOut.Increment = new decimal(new int[] {
            5,
            0,
            0,
            131072});
            this.numMaxOut.Location = new System.Drawing.Point(153, 163);
            this.numMaxOut.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.numMaxOut.Minimum = new decimal(new int[] {
            1000,
            0,
            0,
            -2147483648});
            this.numMaxOut.Name = "numMaxOut";
            this.numMaxOut.Size = new System.Drawing.Size(59, 20);
            this.numMaxOut.TabIndex = 3;
            this.numMaxOut.ValueChanged += new System.EventHandler(this.numMaxOut_ValueChanged);
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(52, 206);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 23);
            this.button1.TabIndex = 4;
            this.button1.Text = "Export";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(133, 206);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(75, 23);
            this.button2.TabIndex = 4;
            this.button2.Text = "Import";
            this.button2.UseVisualStyleBackColor = true;
            // 
            // curveEditor
            // 
            this.curveEditor.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.curveEditor.DrawControlPoints = false;
            this.curveEditor.DrawGrid = true;
            this.curveEditor.Location = new System.Drawing.Point(260, 24);
            this.curveEditor.MaxX = 1F;
            this.curveEditor.MaxY = 1F;
            this.curveEditor.MinX = 0F;
            this.curveEditor.MinY = 0F;
            this.curveEditor.Name = "curveEditor";
            this.curveEditor.Size = new System.Drawing.Size(248, 235);
            this.curveEditor.TabIndex = 0;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(219, 21);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(39, 13);
            this.label6.TabIndex = 5;
            this.label6.Text = "Output";
            this.label6.TextAlign = System.Drawing.ContentAlignment.TopRight;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(469, 262);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(31, 13);
            this.label7.TabIndex = 5;
            this.label7.Text = "Input";
            this.label7.TextAlign = System.Drawing.ContentAlignment.TopRight;
            // 
            // CurveEditorForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(520, 289);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.numMaxOut);
            this.Controls.Add(this.numMaxIn);
            this.Controls.Add(this.numMinOut);
            this.Controls.Add(this.numMinIn);
            this.Controls.Add(this.txtName);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.curveEditor);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Fixed3D;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "CurveEditorForm";
            this.Text = "Sensor Curve Editor";
            this.Load += new System.EventHandler(this.CurveEditorForm_Load);
            ((System.ComponentModel.ISupportInitialize)(this.numMinIn)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numMaxIn)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numMinOut)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numMaxOut)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private YLScsDrawing.Controls.ImageCurve curveEditor;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox txtName;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.NumericUpDown numMinIn;
        private System.Windows.Forms.NumericUpDown numMaxIn;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.NumericUpDown numMinOut;
        private System.Windows.Forms.NumericUpDown numMaxOut;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label7;
    }
}


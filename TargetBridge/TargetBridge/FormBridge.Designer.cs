
namespace TargetBridge
{
    partial class FormBridge
    {
        /// <summary>
        /// Erforderliche Designervariable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Verwendete Ressourcen bereinigen.
        /// </summary>
        /// <param name="disposing">True, wenn verwaltete Ressourcen gelöscht werden sollen; andernfalls False.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Vom Windows Form-Designer generierter Code

        /// <summary>
        /// Erforderliche Methode für die Designerunterstützung.
        /// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.textBoxServerAddress = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.buttonConnect = new System.Windows.Forms.Button();
            this.timerConnectionWatchdog = new System.Windows.Forms.Timer(this.components);
            this.buttonChooseFocuser = new System.Windows.Forms.Button();
            this.labelFocuser = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.labelFocuserPosition = new System.Windows.Forms.Label();
            this.buttonFocuserSetup = new System.Windows.Forms.Button();
            this.checkBoxFocuserOnly = new System.Windows.Forms.CheckBox();
            this.buttonTelescopeSetup = new System.Windows.Forms.Button();
            this.label6 = new System.Windows.Forms.Label();
            this.labelTelescope = new System.Windows.Forms.Label();
            this.buttonTelescopeChooser = new System.Windows.Forms.Button();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.labelRa = new System.Windows.Forms.Label();
            this.labelDec = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.textBoxSetup1 = new System.Windows.Forms.TextBox();
            this.textBoxPos1 = new System.Windows.Forms.TextBox();
            this.buttonMove1 = new System.Windows.Forms.Button();
            this.label8 = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.buttonMove2 = new System.Windows.Forms.Button();
            this.textBoxPos2 = new System.Windows.Forms.TextBox();
            this.textBoxSetup2 = new System.Windows.Forms.TextBox();
            this.label10 = new System.Windows.Forms.Label();
            this.buttonMove3 = new System.Windows.Forms.Button();
            this.textBoxPos3 = new System.Windows.Forms.TextBox();
            this.textBoxSetup3 = new System.Windows.Forms.TextBox();
            this.label11 = new System.Windows.Forms.Label();
            this.buttonMove4 = new System.Windows.Forms.Button();
            this.textBoxPos4 = new System.Windows.Forms.TextBox();
            this.textBoxSetup4 = new System.Windows.Forms.TextBox();
            this.label12 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // textBoxServerAddress
            // 
            this.textBoxServerAddress.Location = new System.Drawing.Point(197, 6);
            this.textBoxServerAddress.Name = "textBoxServerAddress";
            this.textBoxServerAddress.Size = new System.Drawing.Size(101, 20);
            this.textBoxServerAddress.TabIndex = 0;
            this.textBoxServerAddress.Text = "192.168.42.83";
            this.textBoxServerAddress.TextChanged += new System.EventHandler(this.textBoxServerAddress_TextChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(12, 9);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(179, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "Target(Astronomy) IP Address:";
            // 
            // buttonConnect
            // 
            this.buttonConnect.Location = new System.Drawing.Point(305, 4);
            this.buttonConnect.Name = "buttonConnect";
            this.buttonConnect.Size = new System.Drawing.Size(75, 23);
            this.buttonConnect.TabIndex = 2;
            this.buttonConnect.Text = "Connect";
            this.buttonConnect.UseVisualStyleBackColor = true;
            this.buttonConnect.Click += new System.EventHandler(this.buttonConnect_Click);
            // 
            // timerConnectionWatchdog
            // 
            this.timerConnectionWatchdog.Enabled = true;
            this.timerConnectionWatchdog.Interval = 2000;
            this.timerConnectionWatchdog.Tick += new System.EventHandler(this.timerConnectionWatchdog_Tick);
            // 
            // buttonChooseFocuser
            // 
            this.buttonChooseFocuser.Location = new System.Drawing.Point(99, 70);
            this.buttonChooseFocuser.Name = "buttonChooseFocuser";
            this.buttonChooseFocuser.Size = new System.Drawing.Size(32, 23);
            this.buttonChooseFocuser.TabIndex = 3;
            this.buttonChooseFocuser.Text = "...";
            this.buttonChooseFocuser.UseVisualStyleBackColor = true;
            this.buttonChooseFocuser.Click += new System.EventHandler(this.buttonChooseFocuser_Click);
            // 
            // labelFocuser
            // 
            this.labelFocuser.AutoSize = true;
            this.labelFocuser.Location = new System.Drawing.Point(149, 75);
            this.labelFocuser.Name = "labelFocuser";
            this.labelFocuser.Size = new System.Drawing.Size(125, 13);
            this.labelFocuser.TabIndex = 4;
            this.labelFocuser.Text = "<NOT SELECTED YET>";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label3.Location = new System.Drawing.Point(12, 54);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(103, 13);
            this.label3.TabIndex = 5;
            this.label3.Text = "ASCOM Focuser:";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(378, 75);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(84, 13);
            this.label2.TabIndex = 6;
            this.label2.Text = "Current Position:";
            // 
            // labelFocuserPosition
            // 
            this.labelFocuserPosition.AutoSize = true;
            this.labelFocuserPosition.Location = new System.Drawing.Point(469, 75);
            this.labelFocuserPosition.Name = "labelFocuserPosition";
            this.labelFocuserPosition.Size = new System.Drawing.Size(16, 13);
            this.labelFocuserPosition.TabIndex = 7;
            this.labelFocuserPosition.Text = "...";
            // 
            // buttonFocuserSetup
            // 
            this.buttonFocuserSetup.Location = new System.Drawing.Point(18, 70);
            this.buttonFocuserSetup.Name = "buttonFocuserSetup";
            this.buttonFocuserSetup.Size = new System.Drawing.Size(75, 23);
            this.buttonFocuserSetup.TabIndex = 8;
            this.buttonFocuserSetup.Text = "Setup";
            this.buttonFocuserSetup.UseVisualStyleBackColor = true;
            this.buttonFocuserSetup.Click += new System.EventHandler(this.buttonFocuserSetup_Click);
            // 
            // checkBoxFocuserOnly
            // 
            this.checkBoxFocuserOnly.AutoSize = true;
            this.checkBoxFocuserOnly.Location = new System.Drawing.Point(99, 111);
            this.checkBoxFocuserOnly.Name = "checkBoxFocuserOnly";
            this.checkBoxFocuserOnly.Size = new System.Drawing.Size(142, 17);
            this.checkBoxFocuserOnly.TabIndex = 9;
            this.checkBoxFocuserOnly.Text = "lock Mount, focuser only";
            this.checkBoxFocuserOnly.UseVisualStyleBackColor = true;
            // 
            // buttonTelescopeSetup
            // 
            this.buttonTelescopeSetup.Location = new System.Drawing.Point(18, 158);
            this.buttonTelescopeSetup.Name = "buttonTelescopeSetup";
            this.buttonTelescopeSetup.Size = new System.Drawing.Size(75, 23);
            this.buttonTelescopeSetup.TabIndex = 15;
            this.buttonTelescopeSetup.Text = "Setup";
            this.buttonTelescopeSetup.UseVisualStyleBackColor = true;
            this.buttonTelescopeSetup.Click += new System.EventHandler(this.buttonTelescopeSetup_Click);
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label6.Location = new System.Drawing.Point(12, 142);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(117, 13);
            this.label6.TabIndex = 12;
            this.label6.Text = "ASCOM Telescope:";
            // 
            // labelTelescope
            // 
            this.labelTelescope.AutoSize = true;
            this.labelTelescope.Location = new System.Drawing.Point(149, 163);
            this.labelTelescope.Name = "labelTelescope";
            this.labelTelescope.Size = new System.Drawing.Size(125, 13);
            this.labelTelescope.TabIndex = 11;
            this.labelTelescope.Text = "<NOT SELECTED YET>";
            // 
            // buttonTelescopeChooser
            // 
            this.buttonTelescopeChooser.Location = new System.Drawing.Point(99, 158);
            this.buttonTelescopeChooser.Name = "buttonTelescopeChooser";
            this.buttonTelescopeChooser.Size = new System.Drawing.Size(32, 23);
            this.buttonTelescopeChooser.TabIndex = 10;
            this.buttonTelescopeChooser.Text = "...";
            this.buttonTelescopeChooser.UseVisualStyleBackColor = true;
            this.buttonTelescopeChooser.Click += new System.EventHandler(this.buttonTelescopeChooser_Click);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(381, 162);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(25, 13);
            this.label4.TabIndex = 16;
            this.label4.Text = "RA:";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(381, 190);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(30, 13);
            this.label5.TabIndex = 17;
            this.label5.Text = "Dec:";
            // 
            // labelRa
            // 
            this.labelRa.AutoSize = true;
            this.labelRa.Location = new System.Drawing.Point(412, 162);
            this.labelRa.Name = "labelRa";
            this.labelRa.Size = new System.Drawing.Size(16, 13);
            this.labelRa.TabIndex = 18;
            this.labelRa.Text = "...";
            // 
            // labelDec
            // 
            this.labelDec.AutoSize = true;
            this.labelDec.Location = new System.Drawing.Point(412, 190);
            this.labelDec.Name = "labelDec";
            this.labelDec.Size = new System.Drawing.Size(16, 13);
            this.labelDec.TabIndex = 19;
            this.labelDec.Text = "...";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(57, 239);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(16, 13);
            this.label7.TabIndex = 20;
            this.label7.Text = "1:";
            // 
            // textBoxSetup1
            // 
            this.textBoxSetup1.Location = new System.Drawing.Point(80, 239);
            this.textBoxSetup1.Name = "textBoxSetup1";
            this.textBoxSetup1.Size = new System.Drawing.Size(194, 20);
            this.textBoxSetup1.TabIndex = 21;
            this.textBoxSetup1.TextChanged += new System.EventHandler(this.textBoxSetup1_TextChanged);
            // 
            // textBoxPos1
            // 
            this.textBoxPos1.Location = new System.Drawing.Point(280, 239);
            this.textBoxPos1.Name = "textBoxPos1";
            this.textBoxPos1.Size = new System.Drawing.Size(100, 20);
            this.textBoxPos1.TabIndex = 22;
            this.textBoxPos1.TextChanged += new System.EventHandler(this.textBoxPos1_TextChanged);
            // 
            // buttonMove1
            // 
            this.buttonMove1.Location = new System.Drawing.Point(387, 237);
            this.buttonMove1.Name = "buttonMove1";
            this.buttonMove1.Size = new System.Drawing.Size(75, 23);
            this.buttonMove1.TabIndex = 23;
            this.buttonMove1.Text = "move";
            this.buttonMove1.UseVisualStyleBackColor = true;
            this.buttonMove1.Click += new System.EventHandler(this.buttonMove1_Click);
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(80, 220);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(38, 13);
            this.label8.TabIndex = 24;
            this.label8.Text = "Name:";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(280, 219);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(96, 13);
            this.label9.TabIndex = 25;
            this.label9.Text = "Position (absolute):";
            // 
            // buttonMove2
            // 
            this.buttonMove2.Location = new System.Drawing.Point(387, 263);
            this.buttonMove2.Name = "buttonMove2";
            this.buttonMove2.Size = new System.Drawing.Size(75, 23);
            this.buttonMove2.TabIndex = 29;
            this.buttonMove2.Text = "move";
            this.buttonMove2.UseVisualStyleBackColor = true;
            this.buttonMove2.Click += new System.EventHandler(this.buttonMove2_Click);
            // 
            // textBoxPos2
            // 
            this.textBoxPos2.Location = new System.Drawing.Point(280, 265);
            this.textBoxPos2.Name = "textBoxPos2";
            this.textBoxPos2.Size = new System.Drawing.Size(100, 20);
            this.textBoxPos2.TabIndex = 28;
            this.textBoxPos2.TextChanged += new System.EventHandler(this.textBoxPos2_TextChanged);
            // 
            // textBoxSetup2
            // 
            this.textBoxSetup2.Location = new System.Drawing.Point(80, 265);
            this.textBoxSetup2.Name = "textBoxSetup2";
            this.textBoxSetup2.Size = new System.Drawing.Size(194, 20);
            this.textBoxSetup2.TabIndex = 27;
            this.textBoxSetup2.TextChanged += new System.EventHandler(this.textBoxSetup2_TextChanged);
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(57, 265);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(16, 13);
            this.label10.TabIndex = 26;
            this.label10.Text = "2:";
            // 
            // buttonMove3
            // 
            this.buttonMove3.Location = new System.Drawing.Point(387, 289);
            this.buttonMove3.Name = "buttonMove3";
            this.buttonMove3.Size = new System.Drawing.Size(75, 23);
            this.buttonMove3.TabIndex = 33;
            this.buttonMove3.Text = "move";
            this.buttonMove3.UseVisualStyleBackColor = true;
            this.buttonMove3.Click += new System.EventHandler(this.buttonMove3_Click);
            // 
            // textBoxPos3
            // 
            this.textBoxPos3.Location = new System.Drawing.Point(280, 291);
            this.textBoxPos3.Name = "textBoxPos3";
            this.textBoxPos3.Size = new System.Drawing.Size(100, 20);
            this.textBoxPos3.TabIndex = 32;
            this.textBoxPos3.TextChanged += new System.EventHandler(this.textBoxPos3_TextChanged);
            // 
            // textBoxSetup3
            // 
            this.textBoxSetup3.Location = new System.Drawing.Point(80, 291);
            this.textBoxSetup3.Name = "textBoxSetup3";
            this.textBoxSetup3.Size = new System.Drawing.Size(194, 20);
            this.textBoxSetup3.TabIndex = 31;
            this.textBoxSetup3.TextChanged += new System.EventHandler(this.textBoxSetup3_TextChanged);
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(57, 291);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(16, 13);
            this.label11.TabIndex = 30;
            this.label11.Text = "3:";
            // 
            // buttonMove4
            // 
            this.buttonMove4.Location = new System.Drawing.Point(387, 315);
            this.buttonMove4.Name = "buttonMove4";
            this.buttonMove4.Size = new System.Drawing.Size(75, 23);
            this.buttonMove4.TabIndex = 37;
            this.buttonMove4.Text = "move";
            this.buttonMove4.UseVisualStyleBackColor = true;
            this.buttonMove4.Click += new System.EventHandler(this.buttonMove4_Click);
            // 
            // textBoxPos4
            // 
            this.textBoxPos4.Location = new System.Drawing.Point(280, 317);
            this.textBoxPos4.Name = "textBoxPos4";
            this.textBoxPos4.Size = new System.Drawing.Size(100, 20);
            this.textBoxPos4.TabIndex = 36;
            this.textBoxPos4.TextChanged += new System.EventHandler(this.textBoxPos4_TextChanged);
            // 
            // textBoxSetup4
            // 
            this.textBoxSetup4.Location = new System.Drawing.Point(80, 317);
            this.textBoxSetup4.Name = "textBoxSetup4";
            this.textBoxSetup4.Size = new System.Drawing.Size(194, 20);
            this.textBoxSetup4.TabIndex = 35;
            this.textBoxSetup4.TextChanged += new System.EventHandler(this.textBoxSetup4_TextChanged);
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(57, 317);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(16, 13);
            this.label12.TabIndex = 34;
            this.label12.Text = "4:";
            // 
            // FormBridge
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(524, 381);
            this.Controls.Add(this.buttonMove4);
            this.Controls.Add(this.textBoxPos4);
            this.Controls.Add(this.textBoxSetup4);
            this.Controls.Add(this.label12);
            this.Controls.Add(this.buttonMove3);
            this.Controls.Add(this.textBoxPos3);
            this.Controls.Add(this.textBoxSetup3);
            this.Controls.Add(this.label11);
            this.Controls.Add(this.buttonMove2);
            this.Controls.Add(this.textBoxPos2);
            this.Controls.Add(this.textBoxSetup2);
            this.Controls.Add(this.label10);
            this.Controls.Add(this.label9);
            this.Controls.Add(this.label8);
            this.Controls.Add(this.buttonMove1);
            this.Controls.Add(this.textBoxPos1);
            this.Controls.Add(this.textBoxSetup1);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.labelDec);
            this.Controls.Add(this.labelRa);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.buttonTelescopeSetup);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.labelTelescope);
            this.Controls.Add(this.buttonTelescopeChooser);
            this.Controls.Add(this.checkBoxFocuserOnly);
            this.Controls.Add(this.buttonFocuserSetup);
            this.Controls.Add(this.labelFocuserPosition);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.labelFocuser);
            this.Controls.Add(this.buttonChooseFocuser);
            this.Controls.Add(this.buttonConnect);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.textBoxServerAddress);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "FormBridge";
            this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Hide;
            this.Text = "TargetBridge";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.FormBridge_FormClosing);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox textBoxServerAddress;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button buttonConnect;
        private System.Windows.Forms.Timer timerConnectionWatchdog;
        private System.Windows.Forms.Button buttonChooseFocuser;
        private System.Windows.Forms.Label labelFocuser;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label labelFocuserPosition;
        private System.Windows.Forms.Button buttonFocuserSetup;
        private System.Windows.Forms.CheckBox checkBoxFocuserOnly;
        private System.Windows.Forms.Button buttonTelescopeSetup;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label labelTelescope;
        private System.Windows.Forms.Button buttonTelescopeChooser;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label labelRa;
        private System.Windows.Forms.Label labelDec;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.TextBox textBoxSetup1;
        private System.Windows.Forms.TextBox textBoxPos1;
        private System.Windows.Forms.Button buttonMove1;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Button buttonMove2;
        private System.Windows.Forms.TextBox textBoxPos2;
        private System.Windows.Forms.TextBox textBoxSetup2;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.Button buttonMove3;
        private System.Windows.Forms.TextBox textBoxPos3;
        private System.Windows.Forms.TextBox textBoxSetup3;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.Button buttonMove4;
        private System.Windows.Forms.TextBox textBoxPos4;
        private System.Windows.Forms.TextBox textBoxSetup4;
        private System.Windows.Forms.Label label12;
    }
}


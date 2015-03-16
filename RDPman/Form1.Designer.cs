namespace RDPman
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;
        private System.Windows.Forms.MainMenu mainMenu1;

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
            System.Windows.Forms.Label label1;
            System.Windows.Forms.Label label2;
            System.Windows.Forms.Label label3;
            System.Windows.Forms.Label label4;
            System.Windows.Forms.Label label6;
            System.Windows.Forms.Label label7;
            System.Windows.Forms.Label label8;
            System.Windows.Forms.Label label9;
            System.Windows.Forms.Label label10;
            System.Windows.Forms.Label label11;
            System.Windows.Forms.Label label12;
            this.mainMenu1 = new System.Windows.Forms.MainMenu();
            this.mnuFile = new System.Windows.Forms.MenuItem();
            this.mnuExit = new System.Windows.Forms.MenuItem();
            this.mnuLoad = new System.Windows.Forms.MenuItem();
            this.mnuSave = new System.Windows.Forms.MenuItem();
            this.mnuStart = new System.Windows.Forms.MenuItem();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabHost = new System.Windows.Forms.TabPage();
            this.chkSavePassword = new System.Windows.Forms.CheckBox();
            this.txtDomain = new System.Windows.Forms.TextBox();
            this.txtPass = new System.Windows.Forms.TextBox();
            this.txtUser = new System.Windows.Forms.TextBox();
            this.txtPort = new System.Windows.Forms.TextBox();
            this.txtServer = new System.Windows.Forms.TextBox();
            this.tabScreen = new System.Windows.Forms.TabPage();
            this.chkFit2Screen = new System.Windows.Forms.CheckBox();
            this.chkFullscreen = new System.Windows.Forms.CheckBox();
            this.cboColorDepth = new System.Windows.Forms.ComboBox();
            this.txtHeight = new System.Windows.Forms.TextBox();
            this.txtWidth = new System.Windows.Forms.TextBox();
            this.tabProg = new System.Windows.Forms.TabPage();
            this.chkBarcodeReader = new System.Windows.Forms.CheckBox();
            this.btnBrowse = new System.Windows.Forms.Button();
            this.label5 = new System.Windows.Forms.Label();
            this.txtProgramLocation = new System.Windows.Forms.TextBox();
            this.inputPanel1 = new Microsoft.WindowsCE.Forms.InputPanel();
            this.chkExitAfterStart = new System.Windows.Forms.CheckBox();
            label1 = new System.Windows.Forms.Label();
            label2 = new System.Windows.Forms.Label();
            label3 = new System.Windows.Forms.Label();
            label4 = new System.Windows.Forms.Label();
            label6 = new System.Windows.Forms.Label();
            label7 = new System.Windows.Forms.Label();
            label8 = new System.Windows.Forms.Label();
            label9 = new System.Windows.Forms.Label();
            label10 = new System.Windows.Forms.Label();
            label11 = new System.Windows.Forms.Label();
            label12 = new System.Windows.Forms.Label();
            this.tabControl1.SuspendLayout();
            this.tabHost.SuspendLayout();
            this.tabScreen.SuspendLayout();
            this.tabProg.SuspendLayout();
            this.SuspendLayout();
            // 
            // label1
            // 
            label1.Font = new System.Drawing.Font("Tahoma", 9F, System.Drawing.FontStyle.Bold);
            label1.Location = new System.Drawing.Point(8, 7);
            label1.Name = "label1";
            label1.Size = new System.Drawing.Size(83, 17);
            label1.Text = "server:";
            label1.TextAlign = System.Drawing.ContentAlignment.TopRight;
            // 
            // label2
            // 
            label2.Font = new System.Drawing.Font("Tahoma", 9F, System.Drawing.FontStyle.Bold);
            label2.Location = new System.Drawing.Point(8, 40);
            label2.Name = "label2";
            label2.Size = new System.Drawing.Size(83, 17);
            label2.Text = "port:";
            label2.TextAlign = System.Drawing.ContentAlignment.TopRight;
            // 
            // label3
            // 
            label3.Font = new System.Drawing.Font("Tahoma", 9F, System.Drawing.FontStyle.Bold);
            label3.Location = new System.Drawing.Point(8, 75);
            label3.Name = "label3";
            label3.Size = new System.Drawing.Size(83, 17);
            label3.Text = "user:";
            label3.TextAlign = System.Drawing.ContentAlignment.TopRight;
            // 
            // label4
            // 
            label4.Font = new System.Drawing.Font("Tahoma", 9F, System.Drawing.FontStyle.Bold);
            label4.Location = new System.Drawing.Point(8, 108);
            label4.Name = "label4";
            label4.Size = new System.Drawing.Size(83, 17);
            label4.Text = "pass:";
            label4.TextAlign = System.Drawing.ContentAlignment.TopRight;
            // 
            // label6
            // 
            label6.Font = new System.Drawing.Font("Tahoma", 9F, System.Drawing.FontStyle.Bold);
            label6.Location = new System.Drawing.Point(8, 141);
            label6.Name = "label6";
            label6.Size = new System.Drawing.Size(83, 17);
            label6.Text = "color depth:";
            label6.TextAlign = System.Drawing.ContentAlignment.TopRight;
            // 
            // label7
            // 
            label7.Font = new System.Drawing.Font("Tahoma", 9F, System.Drawing.FontStyle.Bold);
            label7.Location = new System.Drawing.Point(8, 73);
            label7.Name = "label7";
            label7.Size = new System.Drawing.Size(83, 17);
            label7.Text = "height:";
            label7.TextAlign = System.Drawing.ContentAlignment.TopRight;
            // 
            // label8
            // 
            label8.Font = new System.Drawing.Font("Tahoma", 9F, System.Drawing.FontStyle.Bold);
            label8.Location = new System.Drawing.Point(8, 47);
            label8.Name = "label8";
            label8.Size = new System.Drawing.Size(83, 17);
            label8.Text = "width:";
            label8.TextAlign = System.Drawing.ContentAlignment.TopRight;
            // 
            // label9
            // 
            label9.Font = new System.Drawing.Font("Tahoma", 9F, System.Drawing.FontStyle.Bold);
            label9.Location = new System.Drawing.Point(8, 208);
            label9.Name = "label9";
            label9.Size = new System.Drawing.Size(83, 17);
            label9.Text = "fullscreen:";
            label9.TextAlign = System.Drawing.ContentAlignment.TopRight;
            // 
            // label10
            // 
            label10.Font = new System.Drawing.Font("Tahoma", 9F, System.Drawing.FontStyle.Bold);
            label10.Location = new System.Drawing.Point(7, 4);
            label10.Name = "label10";
            label10.Size = new System.Drawing.Size(83, 17);
            label10.Text = "Workarea:";
            // 
            // label11
            // 
            label11.Font = new System.Drawing.Font("Tahoma", 9F, System.Drawing.FontStyle.Bold);
            label11.Location = new System.Drawing.Point(8, 185);
            label11.Name = "label11";
            label11.Size = new System.Drawing.Size(83, 17);
            label11.Text = "Local:";
            // 
            // label12
            // 
            label12.Font = new System.Drawing.Font("Tahoma", 9F, System.Drawing.FontStyle.Bold);
            label12.Location = new System.Drawing.Point(8, 171);
            label12.Name = "label12";
            label12.Size = new System.Drawing.Size(83, 17);
            label12.Text = "domain:";
            label12.TextAlign = System.Drawing.ContentAlignment.TopRight;
            // 
            // mainMenu1
            // 
            this.mainMenu1.MenuItems.Add(this.mnuFile);
            this.mainMenu1.MenuItems.Add(this.mnuStart);
            // 
            // mnuFile
            // 
            this.mnuFile.MenuItems.Add(this.mnuExit);
            this.mnuFile.MenuItems.Add(this.mnuLoad);
            this.mnuFile.MenuItems.Add(this.mnuSave);
            this.mnuFile.Text = "File";
            // 
            // mnuExit
            // 
            this.mnuExit.Text = "Exit";
            this.mnuExit.Click += new System.EventHandler(this.mnuExit_Click);
            // 
            // mnuLoad
            // 
            this.mnuLoad.Text = "Load";
            this.mnuLoad.Click += new System.EventHandler(this.mnuLoad_Click);
            // 
            // mnuSave
            // 
            this.mnuSave.Text = "Save";
            this.mnuSave.Click += new System.EventHandler(this.mnuSave_Click);
            // 
            // mnuStart
            // 
            this.mnuStart.Text = "Start";
            this.mnuStart.Click += new System.EventHandler(this.mnuStart_Click);
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.tabHost);
            this.tabControl1.Controls.Add(this.tabScreen);
            this.tabControl1.Controls.Add(this.tabProg);
            this.tabControl1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControl1.Location = new System.Drawing.Point(0, 0);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(240, 268);
            this.tabControl1.TabIndex = 0;
            // 
            // tabHost
            // 
            this.tabHost.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(128)))), ((int)(((byte)(255)))), ((int)(((byte)(128)))));
            this.tabHost.Controls.Add(this.chkSavePassword);
            this.tabHost.Controls.Add(this.txtDomain);
            this.tabHost.Controls.Add(this.txtPass);
            this.tabHost.Controls.Add(this.txtUser);
            this.tabHost.Controls.Add(this.txtPort);
            this.tabHost.Controls.Add(this.txtServer);
            this.tabHost.Controls.Add(label12);
            this.tabHost.Controls.Add(label4);
            this.tabHost.Controls.Add(label3);
            this.tabHost.Controls.Add(label2);
            this.tabHost.Controls.Add(label1);
            this.tabHost.Location = new System.Drawing.Point(0, 0);
            this.tabHost.Name = "tabHost";
            this.tabHost.Size = new System.Drawing.Size(240, 245);
            this.tabHost.Text = "Host";
            // 
            // chkSavePassword
            // 
            this.chkSavePassword.Location = new System.Drawing.Point(111, 139);
            this.chkSavePassword.Name = "chkSavePassword";
            this.chkSavePassword.Size = new System.Drawing.Size(121, 20);
            this.chkSavePassword.TabIndex = 7;
            this.chkSavePassword.Text = "save password";
            this.chkSavePassword.CheckStateChanged += new System.EventHandler(this.chkSavePassword_CheckStateChanged);
            // 
            // txtDomain
            // 
            this.txtDomain.Location = new System.Drawing.Point(111, 171);
            this.txtDomain.Name = "txtDomain";
            this.txtDomain.Size = new System.Drawing.Size(122, 21);
            this.txtDomain.TabIndex = 5;
            // 
            // txtPass
            // 
            this.txtPass.Location = new System.Drawing.Point(111, 108);
            this.txtPass.Name = "txtPass";
            this.txtPass.PasswordChar = '*';
            this.txtPass.Size = new System.Drawing.Size(122, 21);
            this.txtPass.TabIndex = 5;
            // 
            // txtUser
            // 
            this.txtUser.Location = new System.Drawing.Point(111, 75);
            this.txtUser.Name = "txtUser";
            this.txtUser.Size = new System.Drawing.Size(122, 21);
            this.txtUser.TabIndex = 5;
            // 
            // txtPort
            // 
            this.txtPort.Location = new System.Drawing.Point(111, 40);
            this.txtPort.Name = "txtPort";
            this.txtPort.Size = new System.Drawing.Size(122, 21);
            this.txtPort.TabIndex = 5;
            // 
            // txtServer
            // 
            this.txtServer.Location = new System.Drawing.Point(111, 7);
            this.txtServer.Name = "txtServer";
            this.txtServer.Size = new System.Drawing.Size(122, 21);
            this.txtServer.TabIndex = 5;
            // 
            // tabScreen
            // 
            this.tabScreen.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(192)))), ((int)(((byte)(224)))), ((int)(((byte)(255)))));
            this.tabScreen.Controls.Add(this.chkFit2Screen);
            this.tabScreen.Controls.Add(this.chkFullscreen);
            this.tabScreen.Controls.Add(this.cboColorDepth);
            this.tabScreen.Controls.Add(this.txtHeight);
            this.tabScreen.Controls.Add(this.txtWidth);
            this.tabScreen.Controls.Add(label11);
            this.tabScreen.Controls.Add(label10);
            this.tabScreen.Controls.Add(label9);
            this.tabScreen.Controls.Add(label6);
            this.tabScreen.Controls.Add(label7);
            this.tabScreen.Controls.Add(label8);
            this.tabScreen.Location = new System.Drawing.Point(0, 0);
            this.tabScreen.Name = "tabScreen";
            this.tabScreen.Size = new System.Drawing.Size(232, 242);
            this.tabScreen.Text = "Screen";
            // 
            // chkFit2Screen
            // 
            this.chkFit2Screen.Location = new System.Drawing.Point(8, 23);
            this.chkFit2Screen.Name = "chkFit2Screen";
            this.chkFit2Screen.Size = new System.Drawing.Size(174, 18);
            this.chkFit2Screen.TabIndex = 18;
            this.chkFit2Screen.Text = "Fit to mobile Screen";
            this.chkFit2Screen.CheckStateChanged += new System.EventHandler(this.chkFit2Screen_CheckStateChanged);
            // 
            // chkFullscreen
            // 
            this.chkFullscreen.Location = new System.Drawing.Point(111, 206);
            this.chkFullscreen.Name = "chkFullscreen";
            this.chkFullscreen.Size = new System.Drawing.Size(121, 18);
            this.chkFullscreen.TabIndex = 18;
            this.chkFullscreen.Text = "Show Fullscreen";
            // 
            // cboColorDepth
            // 
            this.cboColorDepth.Items.Add("8");
            this.cboColorDepth.Items.Add("15");
            this.cboColorDepth.Items.Add("16");
            this.cboColorDepth.Location = new System.Drawing.Point(111, 141);
            this.cboColorDepth.Name = "cboColorDepth";
            this.cboColorDepth.Size = new System.Drawing.Size(121, 22);
            this.cboColorDepth.TabIndex = 17;
            // 
            // txtHeight
            // 
            this.txtHeight.Location = new System.Drawing.Point(111, 73);
            this.txtHeight.Name = "txtHeight";
            this.txtHeight.Size = new System.Drawing.Size(122, 21);
            this.txtHeight.TabIndex = 10;
            // 
            // txtWidth
            // 
            this.txtWidth.Location = new System.Drawing.Point(111, 47);
            this.txtWidth.Name = "txtWidth";
            this.txtWidth.Size = new System.Drawing.Size(122, 21);
            this.txtWidth.TabIndex = 9;
            // 
            // tabProg
            // 
            this.tabProg.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(192)))), ((int)(((byte)(255)))), ((int)(((byte)(255)))));
            this.tabProg.Controls.Add(this.chkExitAfterStart);
            this.tabProg.Controls.Add(this.chkBarcodeReader);
            this.tabProg.Controls.Add(this.btnBrowse);
            this.tabProg.Controls.Add(this.label5);
            this.tabProg.Controls.Add(this.txtProgramLocation);
            this.tabProg.Location = new System.Drawing.Point(0, 0);
            this.tabProg.Name = "tabProg";
            this.tabProg.Size = new System.Drawing.Size(232, 242);
            this.tabProg.Text = "Prog";
            // 
            // chkBarcodeReader
            // 
            this.chkBarcodeReader.Location = new System.Drawing.Point(7, 52);
            this.chkBarcodeReader.Name = "chkBarcodeReader";
            this.chkBarcodeReader.Size = new System.Drawing.Size(225, 21);
            this.chkBarcodeReader.TabIndex = 3;
            this.chkBarcodeReader.Text = "enable BarcodeReader support";
            // 
            // btnBrowse
            // 
            this.btnBrowse.Location = new System.Drawing.Point(206, 3);
            this.btnBrowse.Name = "btnBrowse";
            this.btnBrowse.Size = new System.Drawing.Size(26, 18);
            this.btnBrowse.TabIndex = 2;
            this.btnBrowse.Text = "...";
            this.btnBrowse.Click += new System.EventHandler(this.btnBrowse_Click);
            // 
            // label5
            // 
            this.label5.Font = new System.Drawing.Font("Tahoma", 9F, System.Drawing.FontStyle.Bold);
            this.label5.Location = new System.Drawing.Point(7, 4);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(153, 18);
            this.label5.Text = "Program Location:";
            // 
            // txtProgramLocation
            // 
            this.txtProgramLocation.Location = new System.Drawing.Point(7, 25);
            this.txtProgramLocation.Name = "txtProgramLocation";
            this.txtProgramLocation.Size = new System.Drawing.Size(226, 21);
            this.txtProgramLocation.TabIndex = 0;
            this.txtProgramLocation.Text = "\\Program Files\\rdesktopce\\rdesktopce.exe";
            this.txtProgramLocation.WordWrap = false;
            // 
            // inputPanel1
            // 
            this.inputPanel1.EnabledChanged += new System.EventHandler(this.inputPanel1_EnabledChanged);
            // 
            // chkExitAfterStart
            // 
            this.chkExitAfterStart.Location = new System.Drawing.Point(7, 122);
            this.chkExitAfterStart.Name = "chkExitAfterStart";
            this.chkExitAfterStart.Size = new System.Drawing.Size(225, 18);
            this.chkExitAfterStart.TabIndex = 5;
            this.chkExitAfterStart.Text = "Exit RDPman after Start";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(96F, 96F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.AutoScroll = true;
            this.ClientSize = new System.Drawing.Size(240, 268);
            this.Controls.Add(this.tabControl1);
            this.Menu = this.mainMenu1;
            this.MinimizeBox = false;
            this.Name = "Form1";
            this.Text = "RDPman";
            this.Closing += new System.ComponentModel.CancelEventHandler(this.Form1_Closing);
            this.tabControl1.ResumeLayout(false);
            this.tabHost.ResumeLayout(false);
            this.tabScreen.ResumeLayout(false);
            this.tabProg.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.MenuItem mnuFile;
        private System.Windows.Forms.MenuItem mnuExit;
        private System.Windows.Forms.MenuItem mnuLoad;
        private System.Windows.Forms.MenuItem mnuSave;
        private System.Windows.Forms.MenuItem mnuStart;
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabHost;
        private System.Windows.Forms.TabPage tabScreen;
        private System.Windows.Forms.TextBox txtPass;
        private System.Windows.Forms.TextBox txtUser;
        private System.Windows.Forms.TextBox txtPort;
        private System.Windows.Forms.TextBox txtServer;
        private System.Windows.Forms.TabPage tabProg;
        private System.Windows.Forms.Button btnBrowse;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox txtProgramLocation;
        private System.Windows.Forms.TextBox txtHeight;
        private System.Windows.Forms.TextBox txtWidth;
        private System.Windows.Forms.ComboBox cboColorDepth;
        private System.Windows.Forms.CheckBox chkFullscreen;
        private System.Windows.Forms.TextBox txtDomain;
        private System.Windows.Forms.CheckBox chkBarcodeReader;
        private System.Windows.Forms.CheckBox chkSavePassword;
        private Microsoft.WindowsCE.Forms.InputPanel inputPanel1;
        private System.Windows.Forms.CheckBox chkFit2Screen;
        private System.Windows.Forms.CheckBox chkExitAfterStart;
    }
}


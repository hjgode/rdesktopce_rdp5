/*
Copyright (c) 2008, Harry Jennerway
http://www.lime49.com/
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this list
  of conditions and the following disclaimer. 

* Redistributions in binary form must reproduce the above copyright notice, this 
  list of conditions and the following disclaimer in the documentation and/or other
  materials provided with the distribution. 

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/
namespace Lime49 {
    partial class OpenFileDialog {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;
        private System.Windows.Forms.MainMenu mnuMain;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing) {
            if(disposing && (components != null)) {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent() {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(OpenFileDialog));
            this.mnuMain = new System.Windows.Forms.MainMenu();
            this.itmCancel = new System.Windows.Forms.MenuItem();
            this.itmOk = new System.Windows.Forms.MenuItem();
            this.ddVisitedLocations = new System.Windows.Forms.ComboBox();
            this.lstFiles = new System.Windows.Forms.ListView();
            this.imlFileTypes = new System.Windows.Forms.ImageList();
            this.panel1 = new System.Windows.Forms.Panel();
            this.txtFileName = new System.Windows.Forms.TextBox();
            this.lblFileName = new System.Windows.Forms.Label();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // mnuMain
            // 
            this.mnuMain.MenuItems.Add(this.itmCancel);
            this.mnuMain.MenuItems.Add(this.itmOk);
            // 
            // itmCancel
            // 
            resources.ApplyResources(this.itmCancel, "itmCancel");
            // 
            // itmOk
            // 
            resources.ApplyResources(this.itmOk, "itmOk");
            // 
            // ddVisitedLocations
            // 
            resources.ApplyResources(this.ddVisitedLocations, "ddVisitedLocations");
            this.ddVisitedLocations.Name = "ddVisitedLocations";
            // 
            // lstFiles
            // 
            this.lstFiles.FullRowSelect = true;
            resources.ApplyResources(this.lstFiles, "lstFiles");
            this.lstFiles.Name = "lstFiles";
            this.lstFiles.SmallImageList = this.imlFileTypes;
            this.lstFiles.View = System.Windows.Forms.View.List;
            this.imlFileTypes.Images.Clear();
            this.imlFileTypes.Images.Add(((System.Drawing.Image)(resources.GetObject("resource"))));
            this.imlFileTypes.Images.Add(((System.Drawing.Image)(resources.GetObject("resource1"))));
            this.imlFileTypes.Images.Add(((System.Drawing.Image)(resources.GetObject("resource2"))));
            this.imlFileTypes.Images.Add(((System.Drawing.Image)(resources.GetObject("resource3"))));
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.txtFileName);
            this.panel1.Controls.Add(this.lblFileName);
            resources.ApplyResources(this.panel1, "panel1");
            this.panel1.Name = "panel1";
            // 
            // txtFileName
            // 
            resources.ApplyResources(this.txtFileName, "txtFileName");
            this.txtFileName.Name = "txtFileName";
            // 
            // lblFileName
            // 
            resources.ApplyResources(this.lblFileName, "lblFileName");
            this.lblFileName.Name = "lblFileName";
            // 
            // OpenFileDialog
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(96F, 96F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            resources.ApplyResources(this, "$this");
            this.ControlBox = false;
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.lstFiles);
            this.Controls.Add(this.ddVisitedLocations);
            this.Menu = this.mnuMain;
            this.MinimizeBox = false;
            this.Name = "OpenFileDialog";
            this.panel1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ComboBox ddVisitedLocations;
        private System.Windows.Forms.ListView lstFiles;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.TextBox txtFileName;
        private System.Windows.Forms.Label lblFileName;
        private System.Windows.Forms.ImageList imlFileTypes;
        private System.Windows.Forms.MenuItem itmCancel;
        private System.Windows.Forms.MenuItem itmOk;
    }
}
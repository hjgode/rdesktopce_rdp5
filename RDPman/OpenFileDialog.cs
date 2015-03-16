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
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Globalization;

namespace Lime49 {
    public partial class OpenFileDialog : Form {
        private string filter = "*",
                       selectedFileName = string.Empty,
                       currentDirectory = "\\";
        private bool changingDir = false;
        private enum FileIcon : int {
            File,
            Directory,
            StorageCard,
            Up
        }

        /// <summary>
        /// Gets or sets the filter used to show files.
        /// </summary>
        /// <value>The filter.</value>
        public string Filter {
            get { return filter; }
            set { filter = value; }
        }
        /// <summary>
        /// Gets or sets the full path (including directory) of theselected file.
        /// </summary>
        /// <value>The path of the selected file.</value>
        public string SelectedFile {
            get {
                return Path.Combine(currentDirectory, selectedFileName); 
            }
            set { 
                FileInfo file = new FileInfo(value);
                selectedFileName = file.Name;
                currentDirectory = file.DirectoryName;
                txtFileName.Text = value;
            }
            
        }
        /// <summary>
        /// Gets or sets the name of the selected file.
        /// </summary>
        /// <value>The name of the selected file.</value>
        public string SelectedFileName {
            get { return selectedFileName; }
            set { selectedFileName = value; }
        }
        /// <summary>
        /// Gets or sets the currently selected path.
        /// </summary>
        /// <value>The current path.</value>
        public string CurrentDirectory {
            get { return currentDirectory; }
            set { currentDirectory = value; }
        }

        /// <summary>
        /// Initializes a new <see cref="OpenFileDialog"/> to show the root of the file system.
        /// </summary>
        public OpenFileDialog() {
            InitializeComponent();
            this.itmCancel.Click += delegate(object sender, EventArgs e) {
                this.DialogResult = DialogResult.Cancel;
            };
            this.itmOk.Click += delegate(object sender, EventArgs e) {
                ProcessSelection();
            };
            this.ddVisitedLocations.SelectedValueChanged += delegate(object sender, EventArgs e) {
                if(!changingDir && ddVisitedLocations.SelectedItem != null)
                    ShowDirectory(ddVisitedLocations.SelectedItem.ToString());
            };
            this.lstFiles.ItemActivate += delegate(object sender, EventArgs e) { ProcessSelection(); };
            this.lstFiles.SelectedIndexChanged += delegate(object sender, EventArgs e) {
                if(lstFiles.SelectedIndices.Count > 0) {
                    txtFileName.Text = lstFiles.Items[lstFiles.SelectedIndices[0]].Text;
                }
            };
            this.txtFileName.KeyDown += delegate(object sender, KeyEventArgs e) {
                if((e.KeyCode == Keys.Return || e.KeyCode == Keys.Enter) && !string.IsNullOrEmpty(txtFileName.Text)) {
                    string text = txtFileName.Text.ToLower(CultureInfo.InvariantCulture);
                    for(int i = 0; i < lstFiles.Items.Count; i++) {
                        if(lstFiles.Items[i].Text == text) {
                            lstFiles.Items[i].Selected = true;
                            ProcessSelection();
                        }
                    }
                }
            };
            ShowDirectory(CurrentDirectory);
        }

        /// <summary>
        /// Initializes a new <see cref="OpenFileDialog"/> which shows the specified directory and file.
        /// </summary>
        /// <param name="currentPath">The current path.</param>
        /// <param name="selectedFile">The selected file.</param>
        public OpenFileDialog(string currentPath, string selectedFile)
            : this() {
            this.CurrentDirectory = currentPath;
            this.SelectedFile = selectedFile;
            this.txtFileName.Text = selectedFile;
        }

        /// <summary>
        /// Updates the file name text box with the current file name, and displays the current directory in the list view.
        /// </summary>
        /// <param name="directoryPath">The directory path to show.</param>
        public void ShowDirectory(string directoryPath) {
            changingDir = true;
            lstFiles.SuspendLayout();
            lstFiles.Items.Clear();
            currentDirectory = directoryPath;
            if(currentDirectory != Directory.GetDirectoryRoot(currentDirectory)) {
                ListViewItem itm = new ListViewItem("..");
                itm.Tag = new DirectoryInfo(currentDirectory).Parent.FullName;
                itm.ImageIndex = (int)FileIcon.Up;
                lstFiles.Items.Add(itm);
            }
            string[] directories = Directory.GetDirectories(currentDirectory);
            Array.Sort(directories, delegate(string item1, string item2) {
                return String.Compare(Path.GetFileName(item1.ToString()), Path.GetFileName(item2.ToString()), true, CultureInfo.InvariantCulture);
            });
            foreach(string dir in directories) {
                ListViewItem itm = new ListViewItem(Path.GetFileName(dir));
                itm.Tag = dir;
                FileInfo info = new FileInfo(dir);
                if((info.Attributes & (FileAttributes.Directory | FileAttributes.Temporary)) == (FileAttributes.Directory | FileAttributes.Temporary)) {
                    itm.ImageIndex = (int)FileIcon.StorageCard;
                } else {
                    itm.ImageIndex = (int)FileIcon.Directory;
                }
                lstFiles.Items.Add(itm);
            }
            string[] files = Directory.GetFiles(currentDirectory, filter);
            Array.Sort(files, delegate(string item1, string item2) {
                return String.Compare(Path.GetFileName(item1.ToString()), Path.GetFileName(item2.ToString()), true, CultureInfo.InvariantCulture);
            });
            foreach(string file in files) {
                ListViewItem itm = new ListViewItem(Path.GetFileName(file));
                itm.Tag = file;
                itm.ImageIndex = (int)FileIcon.File;
                lstFiles.Items.Add(itm);
            }
            UpdatePathComboBox();
            lstFiles.ResumeLayout();
            txtFileName.Text = string.Empty;
            changingDir = false;
        }

        /// <summary>
        /// Processes a selection in the list view and either displays a directory or closes the dialog..
        /// </summary>
        private void ProcessSelection() {
            ListViewItem selectedItem = null;
            if(lstFiles.SelectedIndices.Count > 0 && lstFiles.Focused) { // Item was selected in the file list
                selectedItem = lstFiles.Items[lstFiles.SelectedIndices[0]] as ListViewItem;
            } else { // Name was entered into the text box
                SelectedFileName = txtFileName.Text;
                for(int i = 0; i < lstFiles.Items.Count; i++) {
                    if(lstFiles.Items[i].Text == SelectedFileName) {
                        selectedItem = lstFiles.Items[i];
                    }
                }
            }
            if(selectedItem == null) {
                if(ValidateFile()) {
                    this.DialogResult = DialogResult.OK;
                }
            } else {
                switch(selectedItem.ImageIndex) {
                    case (int)FileIcon.File:
                        SelectedFileName = txtFileName.Text;
                        if(ValidateFile()) {
                            this.DialogResult = DialogResult.OK;
                        }
                        break;
                    case (int)FileIcon.Directory:
                    case (int)FileIcon.StorageCard:
                    case (int)FileIcon.Up:
                        ShowDirectory(selectedItem.Tag.ToString());
                        break;
                }
            }
        }

        /// <summary>
        /// Updates the path combo box to show all parents of the current directory.
        /// </summary>
        private void UpdatePathComboBox() {
            ddVisitedLocations.BeginUpdate();
            ddVisitedLocations.Items.Clear();
            string rootPath = Path.GetPathRoot(CurrentDirectory);
            List<string> directories = new List<string>();
            directories.Add(rootPath);
            if(CurrentDirectory != rootPath) {
                string[] parts = CurrentDirectory.Substring(1).Split(Path.DirectorySeparatorChar); // remove root "\"
                directories.Add(rootPath + parts[0]);
                if(parts.Length > 1) {
                    for(int i = parts.Length - 1; i > 0; i--) {
                        directories.Add(rootPath + String.Join(Path.DirectorySeparatorChar.ToString(), parts, 0, i + 1));
                    }
                }
            }
            directories.Sort();
            foreach(string dir in directories) {
                ddVisitedLocations.Items.Add(dir);
            }
            ddVisitedLocations.SelectedIndex = ddVisitedLocations.Items.Count - 1;
            ddVisitedLocations.EndUpdate();
        }

        /// <summary>
        /// Validates the selected file, more validation can be added if needed.
        /// </summary>
        /// <returns></returns>
        private bool ValidateFile() {
            if(String.IsNullOrEmpty(selectedFileName)) {
                txtFileName.SelectAll();
                txtFileName.Focus();
                return false;
            }
            return true;
        }

        private void inputPanel1_EnabledChanged(object sender, EventArgs e)
        {

        }
    }
}
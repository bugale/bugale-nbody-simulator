/*
    This is the Bugale N-Body Data Editor. A data editor for Bugale N-Body Simulator.
    Copyright (C) 2012  bugale
	
	You may contact me by email: bugalit@gmail.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 3 as published by
    the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
namespace BugaleNBodyDataEditor
{
    partial class MainForm
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            this.grd_settings = new System.Windows.Forms.PropertyGrid();
            this.splt_main = new System.Windows.Forms.SplitContainer();
            this.lbx_main = new System.Windows.Forms.ListBox();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.btn_newfile = new System.Windows.Forms.ToolStripMenuItem();
            this.openToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.btn_open_old = new System.Windows.Forms.ToolStripMenuItem();
            this.saveToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.btn_close = new System.Windows.Forms.ToolStripMenuItem();
            this.editToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.btn_new = new System.Windows.Forms.ToolStripMenuItem();
            this.btn_del = new System.Windows.Forms.ToolStripMenuItem();
            this.btn_up = new System.Windows.Forms.ToolStripMenuItem();
            this.btn_down = new System.Windows.Forms.ToolStripMenuItem();
            this.splt_main.Panel1.SuspendLayout();
            this.splt_main.Panel2.SuspendLayout();
            this.splt_main.SuspendLayout();
            this.menuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // grd_settings
            // 
            this.grd_settings.CausesValidation = false;
            this.grd_settings.Dock = System.Windows.Forms.DockStyle.Fill;
            this.grd_settings.Location = new System.Drawing.Point(0, 0);
            this.grd_settings.Name = "grd_settings";
            this.grd_settings.PropertySort = System.Windows.Forms.PropertySort.NoSort;
            this.grd_settings.Size = new System.Drawing.Size(396, 255);
            this.grd_settings.TabIndex = 0;
            this.grd_settings.ToolbarVisible = false;
            this.grd_settings.PropertyValueChanged += new System.Windows.Forms.PropertyValueChangedEventHandler(this.grd_settings_PropertyValueChanged);
            // 
            // splt_main
            // 
            this.splt_main.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splt_main.Location = new System.Drawing.Point(0, 24);
            this.splt_main.Name = "splt_main";
            this.splt_main.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splt_main.Panel1
            // 
            this.splt_main.Panel1.CausesValidation = false;
            this.splt_main.Panel1.Controls.Add(this.lbx_main);
            // 
            // splt_main.Panel2
            // 
            this.splt_main.Panel2.Controls.Add(this.grd_settings);
            this.splt_main.Size = new System.Drawing.Size(396, 506);
            this.splt_main.SplitterDistance = 247;
            this.splt_main.TabIndex = 1;
            // 
            // lbx_main
            // 
            this.lbx_main.Dock = System.Windows.Forms.DockStyle.Left;
            this.lbx_main.FormattingEnabled = true;
            this.lbx_main.Location = new System.Drawing.Point(0, 0);
            this.lbx_main.Name = "lbx_main";
            this.lbx_main.Size = new System.Drawing.Size(396, 247);
            this.lbx_main.TabIndex = 1;
            this.lbx_main.SelectedIndexChanged += new System.EventHandler(this.lbx_main_SelectedIndexChanged);
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.editToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(396, 24);
            this.menuStrip1.TabIndex = 2;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.btn_newfile,
            this.openToolStripMenuItem,
            this.btn_open_old,
            this.saveToolStripMenuItem,
            this.btn_close});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // btn_newfile
            // 
            this.btn_newfile.Name = "btn_newfile";
            this.btn_newfile.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.N)));
            this.btn_newfile.Size = new System.Drawing.Size(194, 22);
            this.btn_newfile.Text = "New";
            this.btn_newfile.Click += new System.EventHandler(this.btn_newfile_Click);
            // 
            // openToolStripMenuItem
            // 
            this.openToolStripMenuItem.Name = "openToolStripMenuItem";
            this.openToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.O)));
            this.openToolStripMenuItem.Size = new System.Drawing.Size(194, 22);
            this.openToolStripMenuItem.Text = "Open";
            this.openToolStripMenuItem.Click += new System.EventHandler(this.btn_open_Click);
            // 
            // btn_open_old
            // 
            this.btn_open_old.Name = "btn_open_old";
            this.btn_open_old.Size = new System.Drawing.Size(194, 22);
            this.btn_open_old.Text = "Open Old Format(.dat)";
            this.btn_open_old.Click += new System.EventHandler(this.btn_open_old_Click);
            // 
            // saveToolStripMenuItem
            // 
            this.saveToolStripMenuItem.Name = "saveToolStripMenuItem";
            this.saveToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.S)));
            this.saveToolStripMenuItem.Size = new System.Drawing.Size(194, 22);
            this.saveToolStripMenuItem.Text = "Save";
            this.saveToolStripMenuItem.Click += new System.EventHandler(this.btn_save_Click);
            // 
            // btn_close
            // 
            this.btn_close.Name = "btn_close";
            this.btn_close.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Alt | System.Windows.Forms.Keys.F4)));
            this.btn_close.Size = new System.Drawing.Size(194, 22);
            this.btn_close.Text = "Close";
            this.btn_close.Click += new System.EventHandler(this.btn_close_Click);
            // 
            // editToolStripMenuItem
            // 
            this.editToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.btn_new,
            this.btn_del,
            this.btn_up,
            this.btn_down});
            this.editToolStripMenuItem.Name = "editToolStripMenuItem";
            this.editToolStripMenuItem.Size = new System.Drawing.Size(39, 20);
            this.editToolStripMenuItem.Text = "Edit";
            // 
            // btn_new
            // 
            this.btn_new.Enabled = false;
            this.btn_new.Name = "btn_new";
            this.btn_new.ShortcutKeyDisplayString = "Ctrl++";
            this.btn_new.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Oemplus)));
            this.btn_new.Size = new System.Drawing.Size(203, 22);
            this.btn_new.Text = "New Body";
            this.btn_new.Click += new System.EventHandler(this.btn_new_Click);
            // 
            // btn_del
            // 
            this.btn_del.Enabled = false;
            this.btn_del.Name = "btn_del";
            this.btn_del.ShortcutKeys = System.Windows.Forms.Keys.Delete;
            this.btn_del.Size = new System.Drawing.Size(203, 22);
            this.btn_del.Text = "Remove Body";
            this.btn_del.Click += new System.EventHandler(this.btn_del_Click);
            // 
            // btn_up
            // 
            this.btn_up.Enabled = false;
            this.btn_up.Name = "btn_up";
            this.btn_up.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Up)));
            this.btn_up.Size = new System.Drawing.Size(203, 22);
            this.btn_up.Text = "Move Up";
            this.btn_up.Click += new System.EventHandler(this.btn_up_Click);
            // 
            // btn_down
            // 
            this.btn_down.Enabled = false;
            this.btn_down.Name = "btn_down";
            this.btn_down.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Down)));
            this.btn_down.Size = new System.Drawing.Size(203, 22);
            this.btn_down.Text = "Move Down";
            this.btn_down.Click += new System.EventHandler(this.btn_down_Click);
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(396, 530);
            this.Controls.Add(this.splt_main);
            this.Controls.Add(this.menuStrip1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.Name = "MainForm";
            this.Text = "Bugale N-Body Data Editor v1.0.0";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.MainForm_FormClosing);
            this.splt_main.Panel1.ResumeLayout(false);
            this.splt_main.Panel2.ResumeLayout(false);
            this.splt_main.ResumeLayout(false);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.PropertyGrid grd_settings;
        private System.Windows.Forms.SplitContainer splt_main;
        private System.Windows.Forms.ListBox lbx_main;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem btn_newfile;
        private System.Windows.Forms.ToolStripMenuItem openToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem btn_open_old;
        private System.Windows.Forms.ToolStripMenuItem saveToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem btn_close;
        private System.Windows.Forms.ToolStripMenuItem editToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem btn_new;
        private System.Windows.Forms.ToolStripMenuItem btn_del;
        private System.Windows.Forms.ToolStripMenuItem btn_up;
        private System.Windows.Forms.ToolStripMenuItem btn_down;
    }
}


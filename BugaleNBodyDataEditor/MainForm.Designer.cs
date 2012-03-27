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
            this.btn_save = new System.Windows.Forms.Button();
            this.btn_del = new System.Windows.Forms.Button();
            this.btn_new = new System.Windows.Forms.Button();
            this.btn_down = new System.Windows.Forms.Button();
            this.btn_up = new System.Windows.Forms.Button();
            this.lbx_main = new System.Windows.Forms.ListBox();
            this.splt_main.Panel1.SuspendLayout();
            this.splt_main.Panel2.SuspendLayout();
            this.splt_main.SuspendLayout();
            this.SuspendLayout();
            // 
            // grd_settings
            // 
            this.grd_settings.CausesValidation = false;
            this.grd_settings.Dock = System.Windows.Forms.DockStyle.Fill;
            this.grd_settings.Location = new System.Drawing.Point(0, 0);
            this.grd_settings.Name = "grd_settings";
            this.grd_settings.PropertySort = System.Windows.Forms.PropertySort.NoSort;
            this.grd_settings.Size = new System.Drawing.Size(396, 266);
            this.grd_settings.TabIndex = 0;
            this.grd_settings.ToolbarVisible = false;
            this.grd_settings.PropertyValueChanged += new System.Windows.Forms.PropertyValueChangedEventHandler(this.grd_settings_PropertyValueChanged);
            // 
            // splt_main
            // 
            this.splt_main.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splt_main.Location = new System.Drawing.Point(0, 0);
            this.splt_main.Name = "splt_main";
            this.splt_main.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splt_main.Panel1
            // 
            this.splt_main.Panel1.CausesValidation = false;
            this.splt_main.Panel1.Controls.Add(this.btn_save);
            this.splt_main.Panel1.Controls.Add(this.btn_del);
            this.splt_main.Panel1.Controls.Add(this.btn_new);
            this.splt_main.Panel1.Controls.Add(this.btn_down);
            this.splt_main.Panel1.Controls.Add(this.btn_up);
            this.splt_main.Panel1.Controls.Add(this.lbx_main);
            // 
            // splt_main.Panel2
            // 
            this.splt_main.Panel2.Controls.Add(this.grd_settings);
            this.splt_main.Size = new System.Drawing.Size(396, 530);
            this.splt_main.SplitterDistance = 260;
            this.splt_main.TabIndex = 1;
            // 
            // btn_save
            // 
            this.btn_save.Location = new System.Drawing.Point(325, 198);
            this.btn_save.Name = "btn_save";
            this.btn_save.Size = new System.Drawing.Size(59, 47);
            this.btn_save.TabIndex = 5;
            this.btn_save.Text = "Save";
            this.btn_save.UseVisualStyleBackColor = true;
            this.btn_save.Click += new System.EventHandler(this.btn_save_Click);
            // 
            // btn_del
            // 
            this.btn_del.Enabled = false;
            this.btn_del.Location = new System.Drawing.Point(325, 104);
            this.btn_del.Name = "btn_del";
            this.btn_del.Size = new System.Drawing.Size(59, 25);
            this.btn_del.TabIndex = 4;
            this.btn_del.Text = "Remove";
            this.btn_del.UseVisualStyleBackColor = true;
            this.btn_del.Click += new System.EventHandler(this.btn_del_Click);
            // 
            // btn_new
            // 
            this.btn_new.Location = new System.Drawing.Point(325, 135);
            this.btn_new.Name = "btn_new";
            this.btn_new.Size = new System.Drawing.Size(59, 25);
            this.btn_new.TabIndex = 3;
            this.btn_new.Text = "New";
            this.btn_new.UseVisualStyleBackColor = true;
            this.btn_new.Click += new System.EventHandler(this.btn_new_Click);
            // 
            // btn_down
            // 
            this.btn_down.Enabled = false;
            this.btn_down.Location = new System.Drawing.Point(325, 43);
            this.btn_down.Name = "btn_down";
            this.btn_down.Size = new System.Drawing.Size(59, 25);
            this.btn_down.TabIndex = 2;
            this.btn_down.Text = "Down";
            this.btn_down.UseVisualStyleBackColor = true;
            this.btn_down.Click += new System.EventHandler(this.btn_down_Click);
            // 
            // btn_up
            // 
            this.btn_up.Enabled = false;
            this.btn_up.Location = new System.Drawing.Point(325, 12);
            this.btn_up.Name = "btn_up";
            this.btn_up.Size = new System.Drawing.Size(59, 25);
            this.btn_up.TabIndex = 1;
            this.btn_up.Text = "Up";
            this.btn_up.UseVisualStyleBackColor = true;
            this.btn_up.Click += new System.EventHandler(this.btn_up_Click);
            // 
            // lbx_main
            // 
            this.lbx_main.Dock = System.Windows.Forms.DockStyle.Left;
            this.lbx_main.FormattingEnabled = true;
            this.lbx_main.Location = new System.Drawing.Point(0, 0);
            this.lbx_main.Name = "lbx_main";
            this.lbx_main.Size = new System.Drawing.Size(319, 260);
            this.lbx_main.TabIndex = 0;
            this.lbx_main.SelectedIndexChanged += new System.EventHandler(this.lbx_main_SelectedIndexChanged);
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(396, 530);
            this.Controls.Add(this.splt_main);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.Name = "MainForm";
            this.Text = "Bugale N-Body Data Editor Beta 0.2";
            this.Load += new System.EventHandler(this.MainForm_Load);
            this.splt_main.Panel1.ResumeLayout(false);
            this.splt_main.Panel2.ResumeLayout(false);
            this.splt_main.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.PropertyGrid grd_settings;
        private System.Windows.Forms.SplitContainer splt_main;
        private System.Windows.Forms.ListBox lbx_main;
        private System.Windows.Forms.Button btn_del;
        private System.Windows.Forms.Button btn_new;
        private System.Windows.Forms.Button btn_down;
        private System.Windows.Forms.Button btn_up;
        private System.Windows.Forms.Button btn_save;
    }
}


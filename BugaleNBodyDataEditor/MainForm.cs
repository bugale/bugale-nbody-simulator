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
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace BugaleNBodyDataEditor
{
    public partial class MainForm : Form
    {
        private MainSettings mainSettings;
        private List<Body3D> bodies;
        private bool changeGrid = true;
        private bool changed = false;
        public MainForm()
        {
            InitializeComponent();
        }

        private void RefreshList()
        {
            try
            {
                this.lbx_main.Items.Clear();
                this.lbx_main.Items.Add("Main Settings");
                for (int i = 0; i < this.bodies.Count; i++)
                    this.lbx_main.Items.Add("Body #" + i.ToString() + ", Name: " + this.bodies[i].Name);
            }
            catch (Exception err)
            {
                MessageBox.Show(err.ToString(), "Error!", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void lbx_main_SelectedIndexChanged(object sender, EventArgs e)
        {
            try
            {
                if (this.lbx_main.SelectedIndex == 0)
                {
                    this.btn_up.Enabled = false;
                    this.btn_down.Enabled = false;
                    this.btn_del.Enabled = false;
                    this.btn_new.Enabled = true;
                    if (this.changeGrid) this.grd_settings.SelectedObject = this.mainSettings;
                }
                else if (this.lbx_main.SelectedIndex == this.bodies.Count)
                {
                    this.btn_up.Enabled = this.bodies.Count != 1;
                    this.btn_down.Enabled = false;
                    this.btn_del.Enabled = true;
                    this.btn_new.Enabled = true;
                    if (this.changeGrid) this.grd_settings.SelectedObject = this.bodies[this.bodies.Count - 1];
                }
                else if (this.lbx_main.SelectedIndex == 1)
                {
                    this.btn_up.Enabled = false;
                    this.btn_down.Enabled = this.bodies.Count != 1;
                    this.btn_del.Enabled = true;
                    this.btn_new.Enabled = true;
                    if (this.changeGrid) this.grd_settings.SelectedObject = this.bodies[0];
                }
                else if (this.lbx_main.SelectedIndex > 1 && this.lbx_main.SelectedIndex < this.bodies.Count)
                {
                    this.btn_up.Enabled = true;
                    this.btn_down.Enabled = true;
                    this.btn_del.Enabled = true;
                    this.btn_new.Enabled = true;
                    if (this.changeGrid) this.grd_settings.SelectedObject = this.bodies[this.lbx_main.SelectedIndex - 1];
                }
            }
            catch (Exception err)
            {
                MessageBox.Show(err.ToString(), "Error!", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void btn_up_Click(object sender, EventArgs e)
        {
            try
            {
                this.changeGrid = false;
                int select = this.lbx_main.SelectedIndex;
                Body3D temp = this.bodies[select - 2];
                this.bodies[select - 2] = this.bodies[select - 1];
                this.bodies[select - 1] = temp;
                this.RefreshList();
                this.lbx_main.SelectedIndex = select - 1;
                this.changeGrid = true;
            }
            catch (Exception err)
            {
                MessageBox.Show(err.ToString(), "Error!", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void btn_down_Click(object sender, EventArgs e)
        {
            try
            {
                this.changeGrid = false;
                int select = this.lbx_main.SelectedIndex;
                Body3D temp = this.bodies[select];
                this.bodies[select] = this.bodies[select - 1];
                this.bodies[select - 1] = temp;
                this.RefreshList();
                this.lbx_main.SelectedIndex = select + 1;
                this.changeGrid = true;
            }
            catch (Exception err)
            {
                MessageBox.Show(err.ToString(), "Error!", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void btn_del_Click(object sender, EventArgs e)
        {
            try
            {
                this.bodies.RemoveAt(this.lbx_main.SelectedIndex - 1);
                this.RefreshList();
                this.grd_settings.SelectedObject = null;
            }
            catch (Exception err)
            {
                MessageBox.Show(err.ToString(), "Error!", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void btn_new_Click(object sender, EventArgs e)
        {
            try
            {
                this.changeGrid = false;
                int select = this.lbx_main.SelectedIndex;
                this.bodies.Add(new Body3D());
                this.RefreshList();
                this.lbx_main.SelectedIndex = select;
                this.changeGrid = true;
            }
            catch (Exception err)
            {
                MessageBox.Show(err.ToString(), "Error!", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void btn_save_Click(object sender, EventArgs e)
        {
            try
            {
                System.IO.File.WriteAllBytes(Application.StartupPath + "\\settings.bdf", this.mainSettings.ToBytes());
                System.IO.File.WriteAllBytes(Application.StartupPath + "\\bodies.bdf", Body3D.ListToBytes(this.bodies, this.mainSettings.G));
                changed = false;
            }
            catch (Exception err)
            {
                MessageBox.Show(err.ToString(), "Error!", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void btn_open_Click(object sender, EventArgs e)
        {
            try
            {
                mainSettings = new MainSettings();
                bodies = new List<Body3D>();
                byte[] bodies_data;
                byte[] settings_data;
                double g = mainSettings.G;
                bool ret = true;
                if (System.IO.File.Exists(Application.StartupPath + "\\bodies.bdf"))
                {
                    bodies_data = System.IO.File.ReadAllBytes(Application.StartupPath + "\\bodies.bdf");
                    bodies = Body3D.ListFromBytes(bodies_data, out ret);
                    g = BitConverter.ToDouble(bodies_data, 12);
                }
                if (System.IO.File.Exists(Application.StartupPath + "\\settings.bdf"))
                {
                    settings_data = System.IO.File.ReadAllBytes(Application.StartupPath + "\\settings.bdf");
                    ret = mainSettings.FromBytes(settings_data, g) && ret;
                }
                if (!ret) MessageBox.Show("The data files provided are written in an outdated version of Bugale Data Format.\r\nWhenever you will save these settings, whey will be changed to the newer format and will not be readable anymore by the outdated version of Bugale N-Body Simulator.", "Outdated Version", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                this.RefreshList();
                this.lbx_main.SelectedIndex = 0;
                this.grd_settings.SelectedObject = this.mainSettings;
                this.btn_new.Enabled = true;
            }
            catch (Exception err)
            {
                MessageBox.Show("An error has occourred during the opening of the files. It is possible that they are corrupted or in use. You may try to create new files using File->New and override the existing files (Creating a backup is recommended). Press OK to see error details.", "Error reading files", MessageBoxButtons.OK, MessageBoxIcon.Error);
                MessageBox.Show(err.ToString(), "Error!", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void grd_settings_PropertyValueChanged(object s, PropertyValueChangedEventArgs e)
        {
            try
            {
                if (e.ChangedItem.Label != "Name") return;
                this.changeGrid = false;
                int select = this.lbx_main.SelectedIndex;
                this.RefreshList();
                this.lbx_main.SelectedIndex = select;
                this.changeGrid = true;
                this.changed = true;
            }
            catch (Exception err)
            {
                MessageBox.Show(err.ToString(), "Error!", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void btn_open_old_Click(object sender, EventArgs e)
        {
            try
            {
                mainSettings = new MainSettings();
                bodies = new List<Body3D>();
                byte[] bodies_data;
                byte[] settings_data;
                double g = mainSettings.G;
                if (System.IO.File.Exists(Application.StartupPath + "\\bodies.dat"))
                {
                    bodies_data = System.IO.File.ReadAllBytes(Application.StartupPath + "\\bodies.dat");
                    bodies = Body3D.ListFromBytes0(bodies_data);
                    g = BitConverter.ToDouble(bodies_data, 0);
                }
                if (System.IO.File.Exists(Application.StartupPath + "\\settings.dat"))
                {
                    settings_data = System.IO.File.ReadAllBytes(Application.StartupPath + "\\settings.dat");
                    mainSettings.FromBytes0(settings_data, g);
                }
                this.RefreshList();
                this.lbx_main.SelectedIndex = 0;
                this.grd_settings.SelectedObject = this.mainSettings;
                this.btn_new.Enabled = true;
            }
            catch (Exception err)
            {
                MessageBox.Show(err.ToString(), "Error!", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void btn_close_Click(object sender, EventArgs e)
        {
            try
            {
                if (this.changed)
                    switch (MessageBox.Show("Do you want to save before closing?", "Attention!", MessageBoxButtons.YesNoCancel))
                    {
                        case System.Windows.Forms.DialogResult.No:
                            changed = false;
                            this.Close();
                            return;
                        case System.Windows.Forms.DialogResult.Yes:
                            btn_save_Click(null, null);
                            this.Close();
                            return;
                    }
            }
            catch (Exception err)
            {
                MessageBox.Show(err.ToString(), "Error!", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            try
            {
                if (this.changed)
                    switch (MessageBox.Show("Do you want to save before closing?", "Attention!", MessageBoxButtons.YesNoCancel))
                    {
                        case System.Windows.Forms.DialogResult.Cancel:
                            e.Cancel = true;
                            return;
                        case System.Windows.Forms.DialogResult.Yes:
                            btn_save_Click(null, null);
                            return;
                    }
            }
            catch (Exception err)
            {
                MessageBox.Show(err.ToString(), "Error!", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void btn_newfile_Click(object sender, EventArgs e)
        {
            try
            {
                mainSettings = new MainSettings();
                bodies = new List<Body3D>();
                double g = mainSettings.G;
                this.RefreshList();
                this.lbx_main.SelectedIndex = 0;
                this.grd_settings.SelectedObject = this.mainSettings;
                this.btn_new.Enabled = true;
            }
            catch (Exception err)
            {
                MessageBox.Show(err.ToString(), "Error!", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }
    }
}

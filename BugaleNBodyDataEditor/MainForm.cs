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

        public MainForm()
        {
            InitializeComponent();
        }

        private void MainForm_Load(object sender, EventArgs e)
        {
            mainSettings = new MainSettings();
            bodies = new List<Body3D>();
            byte[] bodies_data = System.IO.File.ReadAllBytes(Application.StartupPath + "\\bodies.dat");
            byte[] settings_data = System.IO.File.ReadAllBytes(Application.StartupPath + "\\settings.dat");
            if (System.IO.File.Exists(Application.StartupPath + "\\bodies.dat")) bodies = Body3D.ListFromBytes(bodies_data);
            if (System.IO.File.Exists(Application.StartupPath + "\\settings.dat")) mainSettings.FromBytes(settings_data, BitConverter.ToDouble(bodies_data, 0));
            this.RefreshList();
            this.lbx_main.SelectedIndex = 0;
            this.grd_settings.SelectedObject = this.mainSettings;
        }

        private void RefreshList()
        {
            this.lbx_main.Items.Clear();
            this.lbx_main.Items.Add("Main Settings");
            for (int i = 0; i < this.bodies.Count; i++)
                this.lbx_main.Items.Add("Body #" + i.ToString() + ", Name: " + this.bodies[i].Name);
        }

        private void lbx_main_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (this.lbx_main.SelectedIndex == 0)
            {
                this.btn_up.Enabled = false;
                this.btn_down.Enabled = false;
                this.btn_del.Enabled = false;
                this.btn_new.Enabled = true;
                if (this.changeGrid) this.grd_settings.SelectedObject = this.mainSettings;
            }
            else if (this.lbx_main.SelectedIndex == 1)
            {
                this.btn_up.Enabled = false;
                this.btn_down.Enabled = true;
                this.btn_del.Enabled = true;
                this.btn_new.Enabled = true;
                if (this.changeGrid) this.grd_settings.SelectedObject = this.bodies[0];
            }
            else if (this.lbx_main.SelectedIndex == this.bodies.Count)
            {
                this.btn_up.Enabled = true;
                this.btn_down.Enabled = false;
                this.btn_del.Enabled = true;
                this.btn_new.Enabled = true;
                if (this.changeGrid) this.grd_settings.SelectedObject = this.bodies[this.bodies.Count - 1];
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

        private void btn_up_Click(object sender, EventArgs e)
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

        private void btn_down_Click(object sender, EventArgs e)
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

        private void btn_del_Click(object sender, EventArgs e)
        {
            this.bodies.RemoveAt(this.lbx_main.SelectedIndex - 1);
            this.RefreshList();
            this.grd_settings.SelectedObject = null;
        }

        private void btn_new_Click(object sender, EventArgs e)
        {
            this.changeGrid = false;
            int select = this.lbx_main.SelectedIndex;
            this.bodies.Add(new Body3D());
            this.RefreshList();
            this.lbx_main.SelectedIndex = select;
            this.changeGrid = true;
        }

        private void btn_save_Click(object sender, EventArgs e)
        {
            try
            {
                System.IO.File.WriteAllBytes(Application.StartupPath + "\\settings.dat", this.mainSettings.ToBytes());
                System.IO.File.WriteAllBytes(Application.StartupPath + "\\bodies.dat", Body3D.ListToBytes(this.bodies, this.mainSettings.G));
            }
            catch (Exception x)
            {
                MessageBox.Show(x.ToString());
            }
        }

        private void grd_settings_PropertyValueChanged(object s, PropertyValueChangedEventArgs e)
        {
            this.changeGrid = false;
            int select = this.lbx_main.SelectedIndex;
            this.RefreshList();
            this.lbx_main.SelectedIndex = select;
            this.changeGrid = true;
        }
    }
}

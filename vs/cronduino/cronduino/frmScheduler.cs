using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace cronduino
{
    public partial class frmScheduler : Form
    {
        BindingSource bs = new BindingSource();
        SerialPort serial = new SerialPort();
        frmEntry formEntry = new frmEntry();

        public frmScheduler()
        {
            InitializeComponent();
            bs.DataSource = typeof(Entry);

            gridEntries.DataSource = bs;
            gridEntries.AutoGenerateColumns = true;
        }

        private void gridEntries_CellDoubleClick(object sender, DataGridViewCellEventArgs e)
        {
            // formEntry.ShowDialog(this);
        }

        private void gridEntries_CellEndEdit(object sender, DataGridViewCellEventArgs e)
        {
            System.Diagnostics.Debug.WriteLine("Entries: ");
            foreach (Entry entry in bs.List)
            {
                entry.printDebugEntry();
            }
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void fileToolStripMenuItem_DropDownOpening(object sender, EventArgs e)
        {
            List<ToolStripMenuItem> ports = new List<ToolStripMenuItem>();
            ToolStripDropDown portsDropdown = new ToolStripDropDown();
            saveToCronduino.DropDown = portsDropdown;

            foreach (string port in SerialPort.GetPortNames())
            {
                ToolStripMenuItem portMenu = new ToolStripMenuItem(port);
                portMenu.Text = "PORT " + port;
                portMenu.Name = port;
                portMenu.Checked = false;
                saveToCronduino.DropDownItems.Add(portMenu);

                portMenu.Click += portMenu_Click;
            }
        }

        void portMenu_Click(object sender, EventArgs e)
        {
            ToolStripMenuItem menuItem = (ToolStripMenuItem)sender;
            System.Diagnostics.Debug.WriteLine("Connecting to port " + menuItem.Name);

            serial.BaudRate = 9600;
            serial.DataBits = 8;
            serial.StopBits = StopBits.One;
            serial.Parity = Parity.None;
            serial.PortName = menuItem.Name;
            serial.Open();

            serial.Write("BEG"); // initiate begin
            foreach (Entry entry in bs.List)
            {
                serial.Write(entry.entry, 0, 24);
            }
            serial.Write("END"); // initiate end

            serial.Close();

        }

        private void gridEntries_DataError(object sender, DataGridViewDataErrorEventArgs e)
        {
            e.ThrowException = false;
            e.Cancel = true;
            // MessageBox.Show(e.Exception.Message, "Error at " + e.ColumnIndex + ", " + e.RowIndex, MessageBoxButtons.OK);
            
        }
    }
}

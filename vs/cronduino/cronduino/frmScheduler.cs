using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace cronduino
{
    public partial class frmScheduler : Form
    {
        BindingSource bs = new BindingSource();

        frmEntry formEntry = new frmEntry();

        public frmScheduler()
        {
            InitializeComponent();
            bs.DataSource = typeof(Entry);

            gridEntries.DataSource = bs;
            gridEntries.AutoGenerateColumns = true;

            //public string Name { get; set; }
            //public byte Enable { get; set; }
            //public byte DeviceID { get; set; }
            //public byte DeviceState { get; set; }
            //public byte SecondLower { get; set; }
            //public byte SecondClassifier { get; set; }
            //public byte SecondUpper { get; set; }
            //public byte MinutesLower { get; set; }
            //public byte MinutesClassifier { get; set; }
            //public byte MinutesUpper { get; set; }
            //public byte HourLower { get; set; }
            //public byte HourClassifier { get; set; }
            //public byte HourUpper { get; set; }
            //public byte MonthDayLower { get; set; }
            //public byte MonthDayClassifier { get; set; }
            //public byte MonthDayUpper { get; set; }
            //public byte WeekdayLower { get; set; }
            //public byte WeekdayClassifier { get; set; }
            //public byte WeekdayUpper { get; set; }
            //public byte MonthLower { get; set; }
            //public byte MonthClassifier { get; set; }
            //public byte MonthUpper { get; set; }
            //public byte YearLower { get; set; }
            //public byte YearClassifier { get; set; }
            //public byte YearUpper { get; set; }

            //DataGridViewTextBoxColumn col1 = new DataGridViewTextBoxColumn();
            //col1.DataPropertyName = "Name";
            //col1.HeaderText = "Name";
            //col1.Name = "colName";
            //gridEntries.Columns.Add(col1);
        }

        private void gridEntries_CellDoubleClick(object sender, DataGridViewCellEventArgs e)
        {
            formEntry.ShowDialog(this);
        }
    }
}

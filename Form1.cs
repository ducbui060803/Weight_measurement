using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;
using System.Linq.Expressions;

namespace CalibLoadcell

{ 
    public partial class Form1 : Form
    {
        double zero;
        double span;
        double weight;
        double weightprint;
        double calibzero;
        double calibspan;
        double valueADC;
        public Form1()
        {
            InitializeComponent();
            string[] Baudrate = {"1200", "2400", "4800", "9600", "115200" };
            cboBaudrate.Items.AddRange(Baudrate);
            Control.CheckForIllegalCrossThreadCalls = false;
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            cboComPort.DataSource = SerialPort.GetPortNames();
            cboBaudrate.Text = "115200";
            calibzero = 1000;
            calibspan = 1000;
            string Initially1 = "Chưa kết nối";
            txtDataReceive1.Text = Initially1;
            string Initially2 = "Chưa Calib";
            txtDataReceive2.Text = Initially2;
        }

        private void butConnect_Click(object sender, EventArgs e)
        {
            if (!serCOM.IsOpen)
            {
                butConnect.Text = "Disconnect";
                serCOM.PortName = cboComPort.Text;
                serCOM.BaudRate = Convert.ToInt32(cboBaudrate.Text);
                serCOM.Open();
            }
            else
            {
                butConnect.Text = "Connect";
                serCOM.Close();
            }
        }

        private void butExit_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }
        /*Sendata to ESP32*/
        private void butCalib_Click(object sender, EventArgs e)
        {
            if (!serCOM.IsOpen)
            {
                MessageBox.Show("You are not connected to the device");
            }
            serCOM.Write("CALIB");
            txtDataReceive2.Text = "Đã hiệu chỉnh";
        }

        private void CalibReset_Click(object sender, EventArgs e)
        {
            span = 1;
            calibspan = 1; 
            zero = 0;
            calibzero = 0; 
            if (!serCOM.IsOpen)
            {
                MessageBox.Show("You are not connected to the device");
            }
            serCOM.Write("RESET");
            txtDataReceive2.Text = "Bắt đầu hiệu chỉnh";
            unit.Text = "(voltage)";
        }


        private void butZero_Click(object sender, EventArgs e)
        {
            if (!serCOM.IsOpen)
            {
                MessageBox.Show("You are not connected to the device");
            }
            calibzero = valueADC;
            zero = calibzero;
            string dataZero = "CZERO" + Convert.ToString(zero);
            serCOM.Write(dataZero);
            unit.Text = "(kg)";
        }

        private void butSpan_Click(object sender, EventArgs e)
        { 
            if (!serCOM.IsOpen)
            {
                MessageBox.Show("You are not connected to the device");
            }
            calibspan = Convert.ToDouble(txtboxSpan.Text);
            span = calibspan;
            string dataSpan = "CSPAN" + Convert.ToString(span);
            serCOM.Write(dataSpan);
        }

        private void butSend_Click(object sender, EventArgs e)
        {
            string dulieu = sendData.Text;
            serCOM.Write(dulieu);
        }

        private void serCOM_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            string AllData = "";
            AllData = serCOM.ReadLine();
            int len = AllData.Length;
            if (len > 0)
            {
                valueADC = double.Parse(AllData);
                zero = calibzero;
                span = calibspan;
                weight = (valueADC - zero) / span;
                if (Math.Abs(weight) < 0.001)
                {
                    weight = 0; 
                }
                weightprint = Math.Round(weight, 2 );
                txtDataReceive1.Text = Convert.ToString(weightprint); 
            }
        }

        private void SpanScale_Scroll(object sender, EventArgs e)
        {
            calibspan = Convert.ToDouble(SpanScale.Value*0.01);
            span = calibspan;
            txtboxSpan.Text = Convert.ToString(calibspan);
        }
    }
}

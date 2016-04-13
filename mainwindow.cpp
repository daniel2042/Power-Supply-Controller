#include "mainwindow.h"

#define SIZEX 330
#define SIZEY 400



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{

    QFont f( "Arial", 10);

    this->setFont(f);

    this->setGeometry(500,300,SIZEX,SIZEY);
    this->setFixedSize(SIZEX,SIZEY);
    this->setWindowTitle("Power_Supply_Controller_v1");
    this->setWindowIcon(QIcon("./icon.png"));

    line1 = new QFrame(this);
    line1->setGeometry(0,50,SIZEX,45);
    line1->setFrameStyle(QFrame::HLine);

    line2 = new QFrame(this);
    line2->setGeometry(0,295,SIZEX,45);
    line2->setFrameStyle(QFrame::HLine);

    version_combobox = new QComboBox(this);
    version_combobox->setGeometry(10,10,200,25);
    version_combobox->addItem("Zasilacz 10kV");

    serialport_combobox = new QComboBox(this);
    serialport_combobox->setGeometry(10,40,200,25);

    serialport_open_button = new QPushButton(this);
    serialport_open_button->setText("Open");
    serialport_open_button->setGeometry(220,40,100,25);

    refresh = new QPushButton(this);
    refresh->setText("Refresh");
    refresh->setGeometry(220,10,100,25);


    out_voltage_label = new QLabel(this);
    out_voltage_label->setGeometry(10,75,100,25);
    out_voltage_label->setText("Output voltage");

    out_voltage = new QLineEdit(this);
    out_voltage->setGeometry(10,100,180,25);

    out_voltage_unit = new QLabel(this);
    out_voltage_unit->setGeometry(195,97,40,25);
    out_voltage_unit->setText("V");

    out_voltage_button = new QPushButton(this);
    out_voltage_button->setGeometry(220,100,100,25);
    out_voltage_button->setText("Set");



    ramp_label = new QLabel(this);
    ramp_label->setGeometry(10,135,180,25);
    ramp_label->setText("Rise ratio");

    ramp_value = new QLineEdit(this);
    ramp_value->setGeometry(10,160,180,25);

    ramp_unit = new QLabel(this);
    ramp_unit->setGeometry(195,157,40,25);
    ramp_unit->setText("V/s");

    ramp_button = new QPushButton(this);
    ramp_button->setGeometry(220,160,100,25);
    ramp_button->setText("Set");



    sinus_label = new QLabel(this);
    sinus_label->setGeometry(10,190,100,25);
    sinus_label->setText("Sinus amplitude");

    sinus_amplitude = new QLineEdit(this);
    sinus_amplitude->setGeometry(10,215,180,25);

    sinus_unit = new QLabel(this);
    sinus_unit->setGeometry(195,212,40,25);
    sinus_unit->setText("V");

    sinus_button = new QPushButton(this);
    sinus_button->setGeometry(220,215,100,25);
    sinus_button->setText("Set");


    sinus_frequency_label = new QLabel(this);
    sinus_frequency_label->setGeometry(10,240,100,25);
    sinus_frequency_label->setText("Sinus ferquency");

    sinus_frequency = new QSlider(this);
    sinus_frequency->setGeometry(10,265,175,25);
    sinus_frequency->setOrientation(Qt::Horizontal);
    sinus_frequency->setValue(0);
    sinus_frequency->setMinimum(0);
    sinus_frequency->setMaximum(5);

    sinus_frequency_unit = new QLabel(this);
    sinus_frequency_unit->setText("0Hz");
    sinus_frequency_unit->setGeometry(190,262,40,25);

    sinus_frequency_button = new QPushButton(this);
    sinus_frequency_button->setGeometry(220,265,100,25);
    sinus_frequency_button->setText("Set");




    softstart_checkbox = new QCheckBox(this);
    softstart_checkbox->setGeometry(10,290,100,25);
    softstart_checkbox->setText("Soft Start");
    //softstart_checkbox->setChecked(true);

    power_on = new QPushButton(this);
    power_on->setGeometry(10,SIZEY - 75,150,45);
    power_on->setText("Enable Output");
    power_on->setIconSize(QSize(40,40));
    power_on->setIcon(QIcon("warning.png"));

    out_voltage_read = new QLabel(this);
    out_voltage_read->setGeometry(10,SIZEY - 27,200,25);
    out_voltage_read->setText("Output voltage: 0V");

    high_voltage_indicator = new QPushButton(this);
    high_voltage_indicator->setGeometry(180,320,115,80);
    high_voltage_indicator->setIcon(QIcon("high_voltage.png"));
    high_voltage_indicator->setIconSize(QSize(122,80));
    high_voltage_indicator->setFlat(true);
    high_voltage_indicator->setVisible(false);

    tim = new QTimer(this);
    tim->setInterval(10);


    serial = new QSerialPort(this);



    connect(power_on, SIGNAL(clicked(bool)), this, SLOT(Output()));
    connect(out_voltage_button, SIGNAL(clicked(bool)), this, SLOT(Voltage_Set()));
    connect(out_voltage,SIGNAL(returnPressed()), this, SLOT(Voltage_Set()));
    connect(ramp_button, SIGNAL(clicked(bool)), this, SLOT(Rise_Set()));
    connect(ramp_value,SIGNAL(returnPressed()), this, SLOT(Rise_Set()));
    connect(sinus_button, SIGNAL(clicked(bool)), this, SLOT(Sinus_Set()));
    connect(sinus_amplitude,SIGNAL(returnPressed()), this, SLOT(Sinus_Set()));
    connect(sinus_frequency, SIGNAL(valueChanged(int)), this, SLOT(Frequency_Set()));
    connect(sinus_frequency_button, SIGNAL(clicked(bool)), this, SLOT(Frequency_Set()));
    connect(softstart_checkbox, SIGNAL(clicked(bool)),this, SLOT(Soft_Start(bool)));
    connect(refresh,SIGNAL(clicked(bool)), this, SLOT(Find_COM()));
    connect(serialport_open_button, SIGNAL(clicked(bool)), this, SLOT(Open_COM()));
    connect(tim,SIGNAL(timeout()), this, SLOT(COM()));


    out_voltage->setText(QString::number(control.Get_Output_Value()));
    ramp_value->setText(QString::number(control.Get_Rise_Value()));
    sinus_amplitude->setText(QString::number(control.Get_Sinus_Amplitude()));

    Find_COM();
    Open_COM();

}


void MainWindow::Output()
{
    QMessageBox mess;
    mess.setWindowIcon(QIcon("icon.png"));

    if(power_on->text() == "Enable Output")  // Output Disabled
    {
       #ifdef ASK_BEFORE_ENABLE
            if(mess.warning(this,"Warning","Are you sure you want to enable output?", QMessageBox::Yes | QMessageBox::Cancel) == QMessageBox::Yes)
       #endif
            {
                high_voltage_indicator->setVisible(true);
                power_on->setText("Disable Output");
                power_on->setIcon(QIcon(""));
                control.Set_Enable();
            }
    }
    else
    {
        high_voltage_indicator->setVisible(false);
        power_on->setText("Enable Output");
        power_on->setIcon(QIcon("warning.png"));
        control.Set_Disable();
     }

}

void MainWindow::Voltage_Set()
{
    QMessageBox mess;
    bool ok;
    int  val = 0;

    val = out_voltage->text().toInt(&ok);

    if(ok)
    {
        if(val >= 0 && val <= 15000)
        {
            control.Set_Output_Value(val);
            out_voltage->setText(QString::number(control.Get_Output_Value()));
        }
        else
        {
            mess.setText("Voltage out of range");
            mess.setWindowIcon(QIcon("icon.png"));
            mess.setIcon(QMessageBox::Warning);
            mess.exec();
            out_voltage->setText(QString::number(control.Get_Output_Value()));
        }
    }
    else
    {
        mess.setText(("Output voltage value is not an integer"));
        mess.setWindowIcon(QIcon("icon.png"));
        mess.setIcon(QMessageBox::Warning);
        mess.exec();
        out_voltage->setText(QString::number(control.Get_Output_Value()));
    }
}

void MainWindow::Rise_Set()
{
    QMessageBox mess;
    bool ok;
    int  val = 0;

    val = ramp_value->text().toInt(&ok);

    if(ok)
    {
        if(val >= 0 && val < 60000)
        {
            control.Set_Rise_Value(val);
            ramp_value->setText(QString::number(control.Get_Rise_Value()));
        }
        else
        {
            mess.setText("Value out of range");
            mess.setWindowIcon(QIcon("icon.png"));
            mess.setIcon(QMessageBox::Warning);
            mess.exec();
            ramp_value->setText(QString::number(control.Get_Rise_Value()));
        }
    }
    else
    {
        mess.setText(("Value is not an integer"));
        mess.setIcon(QMessageBox::Warning);
        mess.setWindowIcon(QIcon("icon.png"));
        mess.exec();
        ramp_value->setText(QString::number(control.Get_Rise_Value()));
    }
}


void MainWindow::Sinus_Set()
{
    QMessageBox mess;
    bool ok;
    int  val = 0;

    val = sinus_amplitude->text().toInt(&ok);

    if(ok)
    {
        if(val >= 0 && val < 1000)
        {
            control.Set_Sinus_Amplitude(val);
            sinus_amplitude->setText(QString::number(control.Get_Sinus_Amplitude()));
        }
        else
        {
            mess.setText("Value out of range");
            mess.setIcon(QMessageBox::Warning);
            mess.setWindowIcon(QIcon("icon.png"));
            mess.exec();
            sinus_amplitude->setText(QString::number(control.Get_Sinus_Amplitude()));
        }
    }
    else
    {
        mess.setText(("Value is not an integer"));
        mess.setIcon(QMessageBox::Warning);
        mess.setWindowIcon(QIcon("icon.png"));
        mess.exec();
        sinus_amplitude->setText(QString::number(control.Get_Sinus_Amplitude()));
    }
}

void MainWindow::Frequency_Set()
{
    sinus_frequency_unit->setText(QString::number(sinus_frequency->value()) + "Hz");
    control.Set_Sinus_Frequency(sinus_frequency->value());
}


void MainWindow::Soft_Start(bool n)
{
    control.Set_Soft_Start(n);
}


void MainWindow::Find_COM()
{
    serialport_combobox->clear();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {

        QString s = info.portName();
        serialport_combobox->addItem(s);

    }
}

void MainWindow::Open_COM()
{
    QMessageBox mess;

    serial->close();
    tim->stop();
    serial->setPortName(serialport_combobox->itemText(serialport_combobox->currentIndex()));
    serial->setBaudRate(QSerialPort::Baud115200);

    if(!serial->open(QIODevice::ReadWrite))
    {
        mess.setText("Unable to Open " + serialport_combobox->itemText(serialport_combobox->currentIndex()));
        mess.setIcon(QMessageBox::Warning);
        mess.setWindowIcon(QIcon("icon.png"));
        mess.exec();
    }
    else
    {
        mess.setText(serialport_combobox->itemText(serialport_combobox->currentIndex()) + " successfully opened");
        mess.setIcon(QMessageBox::Information);
        mess.setWindowIcon(QIcon("icon.png"));
        //mess.exec();
        tim->start();
         control.Set_DAC(serial);

    }

}

void MainWindow::COM()
{

    QString volt;

    control.Set_DAC(serial);
    int n = control.Get_ADC(serial);

    if(n >= 0)
    {
        volt = QString::number(n/1000) + " ";
        if(n % 1000 < 10)
            volt += "0";
        if(n % 1000 < 100)
            volt += "0";
        volt += QString::number(n% 1000);
        out_voltage_read->setText("Output voltage: " + volt + "V");
        if(n > 500)
            out_voltage_read->setStyleSheet("QLabel { font-weight: bold; color : red; }");
        else
            out_voltage_read->setStyleSheet("QLabel { color : black; }");
    }

}






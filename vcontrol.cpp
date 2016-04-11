
#include "vcontrol.h"


VControl::VControl()
{
    this->enable          = false;
    this->out_value       = 0;
    this->rise_voltage    = 0;
    this->sinus_amplitude = 0;
    this->sinus_frequency = 0;
}
int VControl::Get_Voltage_Value()
{
    return 0;
}

void VControl::Set_Disable()
{
    this->enable = false;
}

void VControl::Set_Enable()
{
    this->enable = true;
}

void VControl::Set_Soft_Start(bool n)
{
    this->soft_start = n;
}
void VControl::Set_Output_Value(int val)
{
    this->out_value = val;
}

int VControl::Get_Output_Value()
{
    return this->out_value;
}
void VControl::Set_Rise_Value(int val)
{
    this->rise_voltage = val;
}

int VControl::Get_Rise_Value()
{
    return this->rise_voltage;
}

void VControl::Set_Sinus_Amplitude(int val)
{
    this->sinus_amplitude = val;
}

int VControl::Get_Sinus_Amplitude()
{
    return this->sinus_amplitude;
}
void VControl::Set_Sinus_Frequency(int val)
{
    this->sinus_frequency = val;
}

void VControl::Set_DAC(QSerialPort* port)
{
    static unsigned int i = 600;

    char val[2];


    val[1] = (char)(unsigned int)i << 4;
    val[0] = (unsigned int)i  >> 4;
    if(i > 2300)
    {
        val[0] = 0;
        val[1] = 0;
    }
    port->write(val,2);

    i++;
    if(i > 4096)
        i = 0;

}
int VControl::Get_ADC(QSerialPort* port)
{
    char str[2] = {0};
    QMessageBox mess;

    int val = -1;
    if(port->read(str,2) > 0)
    {
        val = (int)(unsigned char)str[0] << 4 | (int)(unsigned char)str[1] >> 4;

        val = val * 10000 / 4095;
        return val;
    }
    return -1;

}

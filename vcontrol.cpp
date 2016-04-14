
#include "vcontrol.h"
#include <cmath>

#define DAC_COEFFICIENT 0.251
#define VOLTAGE_MULTIPLIER_RATIO 1.06

VControl::VControl()
{
    this->enable            = false;
    this->soft_start        = false;
    this->out_value         = 0;
    this->rise_voltage      = 0;
    this->sinus_amplitude   = 0;
    this->sinus_frequency   = 0;
    this->current_DAC_value = 0;
}
int VControl::Get_Voltage_Value()
{
    return VOLTAGE_MULTIPLIER_RATIO*(this->actual_DAC_value * 10000) / 4095;
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

int VControl::Calculate_DAC()
{
    QMessageBox m;
    static double rise_val = 0;
    static double rise = 0;
    static double current_val = 350;
    static unsigned int time = 0;
    static int old_rise = 0;
    static int old_out  = 0;

    time++;
    if(this->enable)
    {

        if(soft_start && actual_DAC_value < 410)
        {
            current_val += 0.025;
            current_DAC_value  = current_val;
            rise_val = current_DAC_value;

            return current_DAC_value;
        }
        else
        {


            current_val = 350;

            if(old_rise != rise_voltage || old_out != out_value)
            {
                //rise_val = actual_DAC_value;



                 rise = this->rise_voltage / 500.0f;

                 /*if(rise_voltage == 0)
                    rise = 100;
*/
                 if(old_out > out_value)
                    rise = -rise;

                 old_rise = rise_voltage;
                 old_out  = out_value;
           }




            if(rise > 0)
                if(out_value *DAC_COEFFICIENT < rise_val)
                    rise = 0;
                else
                {
                    rise_val += rise;
                    this->current_DAC_value = rise_val;
                }


            if(rise < 0)
                if(out_value *DAC_COEFFICIENT > rise_val)
                    rise = 0;
            else
                {
                    rise_val += rise;
                    this->current_DAC_value = rise_val;
                }

            if(this->current_DAC_value >= 0)
            {
                //this->current_DAC_value;
                return this->current_DAC_value + 2 * sinus_amplitude * DAC_COEFFICIENT * sin(sinus_frequency* time/16.0);
            }
            else
                return 0;
        }





    }
    else
    {
        return 0;
    }
}


void VControl::Set_DAC(QSerialPort* port)
{

    int tmp = Calculate_DAC();

    char val[2];
#ifdef Q_OS_LINUX
    val[0] = (char)(unsigned int)tmp << 4;
    val[1] = (unsigned int)tmp  >> 4;
#endif
#ifdef Q_OS_WINDOWS
    val[1] = (char)(unsigned int)tmp << 4;
    val[0] = (unsigned int)tmp  >> 4;
#endif
    port->write(val,2);


}

int avrg(int* tab, int n)
{
    int tmp = 0;
    for(int i = 0; i < n; i++)
        tmp += tab[i];

    return tmp / n;
}
int VControl::Get_ADC(QSerialPort* port)
{

    static int buf[10] = {0};
    static int i = 0;

    char str[2] = {0};
    QMessageBox mess;

    int val = -1;
    if(port->read(str,2) > 0)
    {
        val = (int)(unsigned char)str[0] << 4 | (int)(unsigned char)str[1] >> 4;
        buf[i] = val;
        i++;
        if(i > 5) i = 0;



        this->actual_DAC_value = avrg(buf,5) * 1;
        return VOLTAGE_MULTIPLIER_RATIO*(this->actual_DAC_value * 10000) / 4095;
    }
    return -1;

}

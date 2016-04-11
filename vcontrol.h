#ifndef VCONTROL_H
#define VCONTROL_H

#include <QMessageBox>
#include <QtSerialPort/QSerialPort>
class VControl
{
private:
    int out_value;
    int rise_voltage;
    int sinus_amplitude;
    int sinus_frequency;

    bool soft_start;
    bool enable;
public:


    int  Get_ADC(QSerialPort*);
    void Set_DAC(QSerialPort*);

    void Set_Enable();
    void Set_Disable();

    void Set_Soft_Start(bool n);
    void Set_Output_Value(int val);
    int  Get_Output_Value();
    void Set_Rise_Value(int val);
    int  Get_Rise_Value();
    void Set_Sinus_Amplitude(int val);
    int  Get_Sinus_Amplitude();
    void Set_Sinus_Frequency(int val);

    int  Get_Voltage_Value();

    VControl();
};

#endif // VCONTROL_H


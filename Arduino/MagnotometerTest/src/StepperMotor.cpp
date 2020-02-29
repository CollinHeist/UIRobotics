#include <Arduino.h>

#include "StepperMotor.h"

StepperMotor::StepperMotor(bool direction, int pin0, int pin1, int pin2, int pin3) : m_direction(direction),
																					 m_pin0(pin0),
																					 m_pin1(pin1),
																					 m_pin2(pin2),
																					 m_pin3(pin3)
{
	pinMode(m_pin0, OUTPUT);
	pinMode(m_pin1, OUTPUT);
	pinMode(m_pin2, OUTPUT);
	pinMode(m_pin3, OUTPUT);
}

void StepperMotor::Step(int number)
{
	int i = 0;
	while (++i < number)
	{
		switch (m_step)
		{
		case 0:
			digitalWrite(m_pin0, LOW);
			digitalWrite(m_pin1, LOW);
			digitalWrite(m_pin2, LOW);
			digitalWrite(m_pin3, HIGH);
			break;
		case 1:
			digitalWrite(m_pin0, LOW);
			digitalWrite(m_pin1, LOW);
			digitalWrite(m_pin2, HIGH);
			digitalWrite(m_pin3, HIGH);
			break;
		case 2:
			digitalWrite(m_pin0, LOW);
			digitalWrite(m_pin1, LOW);
			digitalWrite(m_pin2, HIGH);
			digitalWrite(m_pin3, LOW);
			break;
		case 3:
			digitalWrite(m_pin0, LOW);
			digitalWrite(m_pin1, HIGH);
			digitalWrite(m_pin2, HIGH);
			digitalWrite(m_pin3, LOW);
			break;
		case 4:
			digitalWrite(m_pin0, LOW);
			digitalWrite(m_pin1, HIGH);
			digitalWrite(m_pin2, LOW);
			digitalWrite(m_pin3, LOW);
			break;
		case 5:
			digitalWrite(m_pin0, HIGH);
			digitalWrite(m_pin1, HIGH);
			digitalWrite(m_pin2, LOW);
			digitalWrite(m_pin3, LOW);
			break;
		case 6:
			digitalWrite(m_pin0, HIGH);
			digitalWrite(m_pin1, LOW);
			digitalWrite(m_pin2, LOW);
			digitalWrite(m_pin3, LOW);
			break;
		case 7:
			digitalWrite(m_pin0, HIGH);
			digitalWrite(m_pin1, LOW);
			digitalWrite(m_pin2, LOW);
			digitalWrite(m_pin3, HIGH);
			break;
		default:
			digitalWrite(m_pin0, LOW);
			digitalWrite(m_pin1, LOW);
			digitalWrite(m_pin2, LOW);
			digitalWrite(m_pin3, LOW);
			break;
		}
		if (m_direction)
		{
			m_step++;
		}
		else
		{
			m_step--;
		}
		if (m_step > 7)
		{
			m_step = 0;
		}
		if (m_step < 0)
		{
			m_step = 7;
		}
		delay(2);
	}
}
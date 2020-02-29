#pragma once

class StepperMotor
{
private:
	int m_pin0;
	int m_pin1;
	int m_pin2;
	int m_pin3;
	bool m_direction = false;
	int m_step = 0;

public:
	StepperMotor() = delete;
	StepperMotor(bool direction, int pin0, int pin1, int pin2, int pin3);

	void Step(int number);
};
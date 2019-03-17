/* 
 * File:   GamepadInput.h
 * Author: Damon
 *
 * Created on January 26, 2019, 3:10 PM
 */

#ifndef GAMEPADINPUT_H
#define	GAMEPADINPUT_H

#include <utility>
#include <string>


class Pair
{
    int m_CompOne;
    int m_CompTwo;
    
public:
    Pair();
    Pair(int one, int two)
    {
        m_CompOne = one;
        m_CompTwo = two;
    }
    
    int GetOne() { return m_CompOne; }
    int GetTwo() { return m_CompTwo; }
    virtual ~Pair();
};

class Buttons
{
private:
    
    int m_ButtonY;
    int m_ButtonB;
    int m_ButtonA;
    int m_ButtonX;
    
public:
    Buttons();
    Buttons(int Y, int B, int A, int X)
    {
        m_ButtonY = Y;
        m_ButtonX = X;
        m_ButtonB = B;
        m_ButtonA = A;
    }
    
    int GetY() { return m_ButtonY; }
    int GetX() { return m_ButtonX; }
    int GetA() { return m_ButtonA; }
    int GetB() { return m_ButtonB; }

    
    virtual ~Buttons();
    
};

class GamepadInput {
private:
    
    
    //int m_SendTime;
    //unsigned int m_LeftTriggerValue;
    //unsigned int m_RightTriggerValue;
    //bool m_LeftTrigger;
    //bool m_RightTrigger;
   
   //Pair m_LeftSticks;
    //Pair m_DirPad;
    
    //int m_RightStickOne;
    //int m_RightStickTwo;
    
    //Buttons m_Buttons;
    
    //int m_StartButton;
    //int m_BackButton;
    
    //std::string m_InputString;
    
public:
    GamepadInput();
    GamepadInput(const GamepadInput& orig);
    virtual ~GamepadInput();
    
    int HandleInputString(char SingleChar);
    int HandleInputString();

    

};


#endif
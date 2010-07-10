/*
 * sweetsourapplecontroller.cpp
 *
 *  Created on: Jul 10, 2010
 *      Author: Wincent Balin
 */

#include "pdqt.h"

using namespace pdqt;

/** Create SourAppleController. */
SweetSourAppleController::SweetSourAppleController(Main* main_)
{
  // Store main interface
  main = main_;

  // No shift on start
  shift = false;

  // Initialize buttons
  buttons[BUTTON_PLAY]     = Button(Qt::Key_Space);
  buttons[BUTTON_MENU]     = Button(Qt::Key_Menu);
  buttons[BUTTON_ACTION]   = Button(Qt::Key_Shift);
  buttons[BUTTON_REWIND]   = Button(Qt::Key_Left);
  buttons[BUTTON_FORWARD]  = Button(Qt::Key_Right);
  buttons[WHEEL_CLOCKWISE] = Button(Qt::Key_Up);
  buttons[WHEEL_COUNTERCLOCKWISE] = Button(Qt::Key_Down);
}

/** Get a button. */
Button& SweetSourAppleController::getButton(enum ButtonID id)
{
  return buttons[id];
}

/** Press a key. */
void SweetSourAppleController::pressKey(const int key)
{
  // Elaborate control logic
  if(buttons[BUTTON_MENU] == key)
  {
    if(main->pdRunning() && !main->pdPaused())
    {
      main->sendMessage("m 1;\n");

      if(main->isStandardView())
      {
        buttons[BUTTON_MENU].press();
      }
    }
  }
  else if(buttons[BUTTON_ACTION] == key)
  {
    if(main->pdRunning() && !main->pdPaused())
    {
      main->sendMessage("b;\n");

      if(main->isStandardView())
      {
        buttons[BUTTON_ACTION].press();
      }
    }

    if(main->pdRunning())
    {
      shift = true;
    }
  }
  else if(buttons[BUTTON_REWIND] == key)
  {
    if(main->pdRunning() && !main->pdPaused())
    {
      main->sendMessage("w 1;\n");

      if(main->isStandardView())
      {
        buttons[BUTTON_REWIND].press();
      }
    }
  }
  else if(buttons[BUTTON_FORWARD] == key)
  {
    if(main->pdRunning() && !main->pdPaused())
    {
      main->sendMessage("f 1;\n");

      if(main->isStandardView())
      {
        buttons[BUTTON_FORWARD].press();
      }
    }
  }
  else if(buttons[WHEEL_COUNTERCLOCKWISE] == key)
  {
    if(main->pdRunning() && !main->pdPaused())
    {
      if(shift)
      {
        wheel -= 10;
        main->sendMessage("l 10;\n");
      }
      else
      {
        wheel -= 1;
        main->sendMessage("l 1;\n");
      }
    }
  }
  else if(buttons[WHEEL_CLOCKWISE] == key)
  {
    if(main->pdRunning() && !main->pdPaused())
    {
      if(shift)
      {
        wheel += 10;
        main->sendMessage("r 10;\n");
      }
      else
      {
        wheel += 1;
        main->sendMessage("r 1;\n");
      }
    }
  }
  else if(buttons[BUTTON_PLAY] == key)
  {
    if(main->pdRunning())
    {
      if(shift)
      {
        main->pdPause(!main->pdPaused());

        if(main->pdPaused())
        {
          main->setStatus("Paused");
          main->sendMessage("p 0;\n");
        }
        else
        {
          main->setStatus("Running patch");
          main->sendMessage("p 1;\n");
        }
      }
      else if(!main->pdPaused())
      {
        main->sendMessage("d 1;\n");

        if(main->isStandardView())
        {
          buttons[BUTTON_PLAY].press();
        }
      }
    }
  }
}

/** Release a key. */
void SweetSourAppleController::unpressKey(const int key)
{
  // Elaborate control logic
  if(buttons[BUTTON_MENU] == key)
  {
    if(main->pdRunning() && !main->pdPaused() && !shift)
    {
      main->sendMessage("m 0;\n");

      if(main->isStandardView())
      {
        buttons[BUTTON_MENU].press(false);
      }
    }
  }
  else if(buttons[BUTTON_ACTION] == key)
  {
    if(main->pdRunning())
    {
      shift = false;
    }
  }
  else if(buttons[BUTTON_REWIND] == key)
  {
    if(main->pdRunning() && !main->pdPaused())
    {
      main->sendMessage("w 0;\n");

      if(main->isStandardView())
      {
        buttons[BUTTON_REWIND].press(false);
      }
    }
  }
  else if(buttons[BUTTON_FORWARD] == key)
  {
    if(main->pdRunning() && !main->pdPaused())
    {
      main->sendMessage("f 0;\n");

      if(main->isStandardView())
      {
        buttons[BUTTON_FORWARD].press(false);
      }
    }
  }
  else if(buttons[BUTTON_PLAY] == key)
  {
    if(main->pdRunning() && !main->pdPaused() && !shift)
    {
      main->sendMessage("d 0;\n");

      if(main->isStandardView())
      {
        buttons[BUTTON_PLAY].press(false);
      }
    }
  }
}

/** Is the key known? */
bool SweetSourAppleController::knownKey(int key)
{
  for(unsigned int i = 0; i < BUTTONS; i++)
  {
    if(buttons[i].key() == key)
    {
      return true;
    }
  }

  return false;
}

/** Is any button pressed? */
bool SweetSourAppleController::anyButtonPressed()
{
  // If any button pressed, return true
  for(unsigned int i = 0; i < BUTTONS; i++)
  {
    if(buttons[i].pressed())
    {
      return true;
    }
  }

  // No button pressed
  return false;
}

/** Is a button pressed? */
bool SweetSourAppleController::buttonPressed(enum ButtonID button)
{
  return buttons[button].pressed();
}

/** Value of the wheel. */
int SweetSourAppleController::wheelValue()
{
  return wheel.value();
}

/** Change value of the wheel. */
void SweetSourAppleController::addToWheel(const int i)
{
  if(i < 0)
  {
    const int counterClockwiseKey = buttons[WHEEL_COUNTERCLOCKWISE].key();

    for(int j = 0; j > i; j--)
    {
      pressKey(counterClockwiseKey);
    }
  }
  else
  {
    const int clockwiseKey = buttons[WHEEL_CLOCKWISE].key();

    for(int j = 0; j < i; j++)
    {
      pressKey(clockwiseKey);
    }
  }
}

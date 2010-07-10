/*
 * sourapplecontroller.cpp
 *
 *  Created on: Jul 10, 2010
 *      Author: Wincent Balin
 */

#include "pdqt.h"

using namespace pdqt;

/** Create SourAppleController. */
SourAppleController::SourAppleController(Main* main_)
{
  // Save message sender
  main = main_;

  // No shift when started
  shift = false;

  // Initialize buttons
  buttons[BUTTON_PLAY]     = Button(Qt::Key_C);
  buttons[BUTTON_MENU]     = Button(Qt::Key_R);
  buttons[BUTTON_ACTION]   = Button(Qt::Key_D);
  buttons[BUTTON_REWIND]   = Button(Qt::Key_S);
  buttons[BUTTON_FORWARD]  = Button(Qt::Key_F);
  buttons[WHEEL_CLOCKWISE] = Button(Qt::Key_E);
  buttons[WHEEL_COUNTERCLOCKWISE] = Button(Qt::Key_X);
}

/** Get a button. */
Button& SourAppleController::getButton(enum ButtonID id)
{
  return buttons[id];
}

/** Press a key. */
void SourAppleController::pressKey(const int key)
{
  // Elaborate control logic
  if(buttons[BUTTON_MENU] == key)
  {
    if(main->pdRunning() && ! main->pdPaused())
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
    if(main->pdRunning())
    {
      if(!main->pdPaused())
      {
        main->sendMessage("b;\n");

        if(main->isStandardView())
        {
          buttons[BUTTON_ACTION].press();
        }
      }

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
void SourAppleController::unpressKey(int key)
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
bool SourAppleController::knownKey(int key)
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
bool SourAppleController::anyButtonPressed()
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
bool SourAppleController::buttonPressed(enum ButtonID button)
{
  return buttons[button].pressed();
}

/** Value of the wheel. */
int SourAppleController::wheelValue()
{
  return wheel.value();
}

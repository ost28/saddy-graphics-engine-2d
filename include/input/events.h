/*! \file events.h
	\author HiddenSeeker

	This file contains implementations for all window events, that are performed in engine.
	Events can contain useful data about cursor position, key part and other.
 */
#pragma once
#include "../keycodes.h"
#include "../sadpoint.h"
#include "../sadsize.h"
#include "../sadstring.h"
#include "../maybe.h"

namespace sad
{

namespace input
{

/*! An abstract event, which is base for all events of engine.
	Can be used for marking or run-time check of events
 */
class AbstractEvent
{
public:
	/*! Kept, for purpose of inheritance
	 */
	virtual ~AbstractEvent();
};

/*! This type of events raised, when sad::Renderer exits sad::Renderer::run(), or
	when  mouse cursor leaves  window area, or on window activation, or
	on window deactivation
 */
class NullEvent: public sad::input::AbstractEvent
{
public:
	/*! Constructs new quit event
	 */
	NullEvent();
	/*! Kept, for purpose of inheritance
	 */
	virtual ~NullEvent();
};

/*! A type of event, which is emitted, when user quits application
 */
typedef NullEvent QuitEvent;
/*! A type of event, which is emitted when mouse cursor leaves window area
 */
typedef NullEvent MouseLeaveEvent;
/*! A type of event, which is emitted when window is activated 
 */
typedef NullEvent ActivateEvent;
/*! A type of event, which is emitted when window is deactivated
 */
typedef NullEvent DeactivateEvent;

/*! This type of events raised, when user presses or releases key
 */
class KeyEvent: public sad::input::AbstractEvent
{
public:
	sad::KeyboardKey Key;   //!< Code of key, pressed by user
	sad::Maybe<sad::String> ReadableKey; // !< A readable part of key
	bool   AltHeld;		    //!< True, if user held ALT key, when pressed
	bool   ShiftHeld;	    //!< True, if user held SHIFT key, when pressed
	bool   CtrlHeld;	    //!< True, if user held CTRL key, when pressed  
	bool   CapsLockPressed; //!< True, if user pressed caps lock
	
	/*! Creates new empty key event
	 */
	inline KeyEvent() 
	: Key(sad::KeyNone), 
	  AltHeld(false), 
	  ShiftHeld(false), 
	  CtrlHeld(false), 
	  CapsLockPressed(false)
	{
	
	}
	/*! Kept, for purpose of inheritance
	 */
	virtual ~KeyEvent();
};

/*! A type of event, which is emitted, when user presses keyboard buttton
 */
typedef  KeyEvent KeyPressEvent;

/*! A type of event, which is emitted, when user releases keyboard button
 */
typedef  KeyEvent KeyReleaseEvent;

/*! A basic event for all mouse events, like sad::input::MouseEvent,
	sad::input::WheelEvent
 */ 
class MouseCursorEvent: public sad::input::AbstractEvent
{
public:
	/*! A point in 3-dimensional space, for those, who don't use orthographic projections
	 */
	sad::Point3D Point3D;

	inline MouseCursorEvent() : Point3D()
	{

	}
	/*! Kept, for purpose of inheritance
	 */	
	virtual ~MouseCursorEvent();
	/*! Sets a point for event. This method is used by renderer to set coordinates of
		cursor. 

		Also it fills a 2-dimensional point field automatically
		
		\param[in] p 3-dimensional point
	 */
	inline void setPoint(const sad::Point3D & p)
	{
		this->Point3D = p;
	}
	/*! Returns position of cursor in 2D space
		\return position of cursor
	 */
	inline const sad::Point2D & pos2D()
	{
		return this->Point3D;
	}

};

/*! A type of event, which is emitted, when user moves a mouse
 */
typedef MouseCursorEvent MouseMoveEvent;
/*! A type of event, which is emitted when mouse cursor enters a window 
 */
typedef MouseCursorEvent MouseEnterEvent;

/*! This type of events raised, when user clicks a mouse, 
	presses a mouse button, releases a mouse button or performs a double click
 */
class MouseEvent: public sad::input::MouseCursorEvent
{
public:
	sad::MouseButton  Button;  //!< A button, which is pressed or held by user

	/*! Inits event with no mouse pressed and zero points
	 */
	inline MouseEvent() : MouseCursorEvent(), Button(sad::MouseNone) 
	{

	}
	/*! Kept, for purpose of inheritance
	 */
	virtual ~MouseEvent();
};

/*! A type of event, which is emitted, when user clicks on a mouse button
 */
typedef MouseEvent MouseClickEvent;

/*! A type of event, which is emitted, when user releases a mouse button
 */
typedef MouseEvent MouseReleaseEvent;

/*! A type of event, which is emitted, when user performs a double click 
	with a mouse button
 */
typedef MouseEvent MouseDoubleClickEvent;

/*! This type of events raised, when user moves a wheel
 */
class WheelEvent: public sad::input::MouseCursorEvent
{
public:
	/*! How much wheel has scrolled. Bigger than zero, means, that wheel was scrolled up,
		Lesser than zero, means, that wheel was scrolled down
	 */
	double Delta;  

	/*! Inits wheel event with zero delta
	 */
	inline WheelEvent() : MouseCursorEvent(), Delta(0)
	{

	}
	/*! Kept, for purpose of inheritance
	 */
	virtual ~WheelEvent();
};

/*! Event, which is raised, when window is being resized, maximized or minimized 
 */
class ResizeEvent: public sad::input::AbstractEvent
{
public:
	sad::Size2I OldSize;     //!< An old size of window
	sad::Size2I NewSize;     //!< A new size of window

	/*! A Constructs new event
	 */
	inline ResizeEvent() 
	{

	}
	/*! Kept, for purpose of inheritance
	 */
	virtual ~ResizeEvent();
};

}

}

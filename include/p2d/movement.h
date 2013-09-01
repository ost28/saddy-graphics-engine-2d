/*! \file movement.h
	\author HiddenSeeker
	Describes a movement in discrete time, using force, weight, velocity 
	and distance.
 */
#pragma once
#include "weight.h"
#include "tickable.h"
#include "vector.h"
#include "force.h"
#include "../templates/hlvector.hpp"
#include "../extra/geometry2d.h"
#include <algorithm>

namespace p2d
{
/*! An abstract listener, used to be called, when movement steps to next 
	step, to synchronize other parameters, with it.
 */
template<
	typename _Value
>
class AbstractMovementDeltaListener
{
 public:
	 /*! Notifies a movement for delta
		 \param[in] delta delta from previous value to current
	  */
	 virtual void notify(const _Value & delta) = 0;
	 virtual ~AbstractMovementDeltaListener() {}
};

/*! A specific movement delta listener, that calls a listener for specific
	movement.
 */
template<typename _Class, typename _Value>
class MovementDeltaListener: public p2d::AbstractMovementDeltaListener<_Value>
{
  public:
	  typedef void (_Class::*method_t)(const _Value &);
  protected:
	  _Class *  m_object; //!< An object
	  method_t  m_fun;      //!< A called pointer to method of class
  public:
	  /*! Defines a listener, that calls a specific method for object
		  \param[in] o object
		  \param[in] f method
	   */
	  inline MovementDeltaListener(_Class * o, method_t f)
	  : m_object(o), m_fun(f)
	  {
	  }
	  /*! Calls a specific method for movement of object.
		  Called when movement changed a current value for positions, or
		  after step, when current value is changed
		  \param[in] delta a difference between previous and current body
	   */
	  virtual void notify(const _Value & delta)
	  {
		  (m_object->*m_fun)(delta);
	  }
	  /*! This class does not own object nor method, so nothing here
	   */
	  ~MovementDeltaListener() {}
};

/*! Describes a movement in specifiec direction, using _Value type as type
	of coordinate axis, or set of axis. 

	The value should behave as numeric type, to be a parameter for movement.
 */
template<
	typename _Value
>
class Movement
{
 public:
	 /*! A specific listener for a values
	  */
	 typedef p2d::AbstractMovementDeltaListener<_Value> * listener_t;
 protected:
	 /*! A steppable forces
		 The force is owned by movement
	  */
	 p2d::ActingForces<_Value> m_force; 
	 /*! A weight of a moving body. By default, a weight of one
		 is considered valid.
	     The weight is not owned by movement
	  */
	 p2d::Weight  * m_weight;       
	 /*! A constant part for velocity for current time step.
	  */
	 _Value  m_velocity;
	 /*! A velocity for next time step, as it should be changed by
		 user instantly.
	  */
	 hst::Maybe<_Value> m_next_velocity;
	 /*! A next velocity time, when it need to be set
	  */
	 hst::Maybe<double> m_next_velocity_time;
	 /*! A position for object at current time step
	  */
	 _Value m_position;
	 /*! A next position, as it can be changed by a user
	  */
	 hst::Maybe<_Value> m_next_position;
	 /*! A next position time, when it need to be set
	  */
	 hst::Maybe<double> m_next_position_time;
	 /*! A listeners for a position changes
	  */
	 hst::vector<listener_t> m_listeners;
 protected:
	 /*! Called, when object moved on step, or by setting a current value
		 \param[in] delta a difference from new value and current value
	  */
	 void fireMovement(const _Value & delta)
	 {
		 for(size_t i = 0;  i < m_listeners.count(); i++)
		 {
			 m_listeners[i]->notify(delta);
		 }
	 }
	 /*! Computes acceleration for movement
		 \param[in] p determined position
	  */
	 void acceleration(_Value & p) const
	 {
		 if (m_weight != NULL)
		 {
			if (m_weight->isInfinite() == false )
			{
				m_force.value(p);
				if (non_fuzzy_zero(m_weight->value()))
				{
				 p /= m_weight->value();
				}
			}
		 }
	 }
 public:
	 /*! By a default  a weight is one, force is empty, and
		 velocity and position should be zeroish
	  */
	 Movement()
	 {
		 m_weight = NULL;
		 m_velocity = p2d::TickableDefaultValue<_Value>::zero();
		 m_position = p2d::TickableDefaultValue<_Value>::zero();
	 }
	 /*! Destroys force and listeners
	  */
	 ~Movement()
	 {
		 clearListeners();
	 }
	 /*! Clears all of movement listeners
	  */
	 void clearListeners()
	 {
		 for(size_t i = 0 ; i < m_listeners.count(); i++)
			delete m_listeners[i];
		 m_listeners.clear();
	 }
	 /*! Adds a listener for movement
		 \param[in] l listener
	  */ 
	 void addListener(listener_t  l)
	 {
		 if (std::find(m_listeners.begin(), m_listeners.end(), l) 
			 == m_listeners.end())
			 m_listeners << l;
	 }
	 /*! Removes a listener for movement
		 \param[in] l listener
	  */
	 void removeListener(listener_t l)
	 {
		 m_listeners.removeFirst(l);
	 }
	 /*! Performs a force step for current time period
		 \param[time] a time step size
	  */
	 void stepForce(double time)
	 {
		 m_force.step(time);
	 }
	 /*! Returns an acting forces	
		 \return an acting forces	
	  */
	 p2d::ActingForces<_Value> & forces() { return m_force; }
	 /*! Determines a difference between velocity end of specified time
	     period. A step size needed to handle situation with simultaneous 
		 jumps in velocity
		 \param[in] time  a time period
		 \param[in] step_size a size for step
	  */
	 _Value velocityDelta(double time, double step_size)
	 {
		 _Value p = p2d::TickableDefaultValue<_Value>::zero(); 
		 if (m_next_velocity.exists())
		 {
			 if (is_fuzzy_equal(time, step_size))
			 {
				 return m_next_velocity.data() - m_velocity;
			 }
			 if (m_next_velocity_time.exists())
			 {
				 if (is_fuzzy_equal(time, m_next_velocity_time.data()))
				 {
					  return m_next_velocity.data() - m_velocity;
				 }
			 }
			 return p;
		 }
		 if (m_force.hasForces())
		 {
			this->acceleration(p);
			p *= time;
		 }
		 return p;
	 }
	 /*! Returns a velocity at specified time. 
		 \param[in] time specified time
		 \param[in] step_size a simulation step size
	  */
	 _Value velocityAt(double time, double step_size)
	 {
		 return m_velocity + velocityDelta(time, step_size);
	 }
	 /*! Returns a position  difference at specified time
	     \param[in] time specified time
		 \param[in] step_size a simulation step size
	  */
	 _Value positionDelta(double time, double step_size)
	 {
		 if (m_next_position.exists())
		 {
			 if (is_fuzzy_equal(time, step_size))
			 {
				 return m_next_position.data() - m_position;
			 }
			 if (m_next_position_time.exists())
			 {
				 if (is_fuzzy_equal(time, m_next_position_time.data()))
				 {
					  return m_next_position.data() - m_position;
				 }
			 }
			 return p2d::TickableDefaultValue<_Value>::zero();
		 }
		 // Compute constant velocity time
		 _Value dp = m_velocity;
		 dp *= time;
		 if (m_force.hasForces())
		 {
		  _Value da = p2d::TickableDefaultValue<_Value>::zero();
		  this->acceleration(da);
		  da *= (time * time) / 2;
		  dp += da;
		 }
		 return dp;
	 }

	 /*! Returns a position at specified time. 
		 \param[in] time specified time
		 \param[in] step_size a simulation step size
	  */
	 _Value positionAt(double time, double step_size)
	 {
		 return m_position + positionDelta(time, step_size);
	 }
	 /*! Steps a position a velocity
		 \param[in] time specified time size
		 \param[in] step_size a size of step for a movement
	  */
	 void step(double time, double step_size)
	 {
		 _Value delta = this->positionDelta(time, step_size);
		 _Value newvelocity = velocityAt(time, step_size);
		 _Value newposition = positionAt(time, step_size);
		 m_velocity = newvelocity;
		 m_position = newposition;
		 if (is_fuzzy_equal(time, step_size))
		 {
			 m_next_velocity.clear();
			 m_next_position.clear();
			 m_next_velocity_time.clear();
			 m_next_position_time.clear();
		 }
		 if (m_next_velocity_time.exists())
		 {
			 if (is_fuzzy_equal(time, m_next_velocity_time.data()))
			 {
				 m_next_velocity.clear();
				 m_next_velocity_time.clear();
			 }
		 }
		 if (m_next_position_time.exists())
		 {
			 if (is_fuzzy_equal(time, m_next_position_time.data()))
			 {
				 m_next_position.clear();
				 m_next_position_time.clear();
			 }
		 }
		 fireMovement(delta);
	 }
	 /*! Current weight of moved body
		 \return current weight of body
	  */
	 p2d::Weight * weight() { return m_weight; }
	 /*! Sets  new weight for body
		 \param[in] w weight of body
	  */
	 void setWeight(p2d::Weight * w) { m_weight = w; }
	 /*! Return current value for velocity
		 \return current value for velocity
	  */
	 const _Value & velocity() const { return m_velocity; }
	 /*! Tests, whether velocity will be changed, due to user call
		 \return whether velocity will be changed, due to user call
	  */
	 bool  willVelocityChange() const { return m_next_velocity.exists(); }
	 /*! Returns next velocity for body
		 \return next velocity for body
	  */
	 _Value nextVelocity() const 
	 {
		 if (m_next_velocity.exists() == false)
			 return p2d::TickableDefaultValue<_Value>::zero();
		 return m_next_velocity.data();
	 }
	 /*! Sets a current velocity for body
		 \param[in] v velocity
	  */
	 void setCurrentVelocity(const _Value & v)
	 {
		 m_velocity = v;
	 }
	 /*! Sets next planned velocity for body
		 \param[in] v velocity
	  */
	 void setNextVelocity(const _Value & v)
	 {
		 m_next_velocity.setValue(v);
	 }
	 /*! Sets next valocity at specified time
	  */
	 void setNextVelocityAt(const _Value & v, double time)
	 {
		 m_next_velocity.setValue(v);
		 m_next_velocity_time.setValue(time);
	 }
	 /*! Return current value for position
		 \return current value for position
	  */
	 const _Value & position() const { return m_position; }
	 /*! Tests, whether position will be changed, due to user call
		 \return whether position will be changed, due to user call
	  */
	 bool  willPositionChange() const { return m_next_position.exists(); }
	 /*! Returns next position for body
		 \return next position for body
	  */
	 _Value nextPosition() const 
	 {
		 if (m_next_position.exists() == false)
			 return p2d::TickableDefaultValue<_Value>::zero();
		 return m_next_position.data();
	 }
	 /*! Sets a current position for body
		 \param[in] v position
	  */
	 void setCurrentPosition(const _Value & v)
	 {
		 _Value delta = v - m_position;
		 m_position = v;
		 fireMovement(delta);
	 }
	 /*! Sets next planned position for body
		 \param[in] v position
	  */
	 void setNextPosition(const _Value & v)
	 {
		 m_next_position.setValue(v);
		 m_next_position_time.clear();
	 }
	 void setNextPositionAt(const _Value & v, double time)
	 {
		 m_next_position.setValue(v);
		 m_next_position_time.setValue(time);
	 }
};

/*! A angular movement, as a rotation around ceneter
 */
typedef p2d::Movement<double> AngularMovement;
/*! A tangential movement, as moving of whole body
 */
typedef p2d::Movement<p2d::Vector> TangentialMovement;

}


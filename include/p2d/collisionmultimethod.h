/*! \file collisionmultimethod.h
	\author HiddenSeeker

	Describes a collision multi-method, used to call
	various type-dependent functions
 */
#include "p2d/collisionshape.h"
#include "../templates/ptrhash.hpp"
#pragma once

namespace p2d
{
/*! Describes a basic method for invocation a multi-method
 */
template<typename _ReturnType>
class BasicCollisionMultiMethodInstance
{
 public:
	 /*! Invokes a method
		 \param[in] a1 first object
		 \param[in] a2 second object
		 \return returned type
	  */
	 virtual _ReturnType invoke(p2d::CollisionShape * a1, p2d::CollisionShape * a2) = 0;
	 /*! Returns true, if arguments will be reversed on execution
		 \return reverse flag
	  */
	 virtual bool reverse() const = 0;
	 virtual ~BasicCollisionMultiMethodInstance() {}
};

/*! Describes a basic method for invocation a multi-method with arguments
 */
template<typename _ReturnType, typename _Arg>
class BasicCollisionMultiMethodInstanceWithArg
{
 public:
	 /*! Invokes a method
		 \param[in] a1 first object
		 \param[in] ak1 argument, related to first object
		 \param[in] a2 second object
		 \param[in] ak2 argument, related to second object
		 \return returned type
	  */
	 virtual _ReturnType invoke(
		 p2d::CollisionShape * a1, 
		 const _Arg & ak1, 
		 p2d::CollisionShape * a2,
		 const _Arg & ak2) = 0;
	 /*! Returns true, if arguments will be reversed on execution
		 \return reverse flag
	  */
	 virtual bool reverse() const = 0;
	 virtual ~BasicCollisionMultiMethodInstanceWithArg() {}
};

template<
typename _ReturnType, 
typename _FirstObject,
typename _SecondObject
>
class CollisionMultiMethodInstance: 
public  p2d::BasicCollisionMultiMethodInstance<_ReturnType>
{
 private:
	 _ReturnType (*m_p)(_FirstObject * o1, _SecondObject * o2); //!< An inner function to call
	 bool m_reverse;  //!< Whether arguments should be reversed	 
 public:
	/*! Constructs new instance
		\param[in] p pointer to free function
		\param[in] reverse whether arguments should be reversed
	 */
	inline CollisionMultiMethodInstance(
		  _ReturnType (*p)(_FirstObject * o1, _SecondObject * o2),
		  bool reverse
    ) : m_p(p), m_reverse(reverse) {}
	/*! Returns true, if arguments will be reversed on exectution
		 \return reverse flag
	  */
	bool reverse() const { return m_reverse; }
	 /*! Invokes a method
		 \param[in] a1 first object
		 \param[in] a2 second object
		 \return returned type
	  */
	 virtual _ReturnType invoke(p2d::CollisionShape * a1, p2d::CollisionShape * a2)
	 {
		 if (!m_reverse)
		 {
		  _FirstObject * _a1 = hst::checked_cast<_FirstObject>(a1);
		  _SecondObject * _a2 = hst::checked_cast<_SecondObject>(a2);
		  return m_p(_a1, _a2);
		 }
		 _FirstObject * _a1 = hst::checked_cast<_FirstObject>(a2);
		 _SecondObject * _a2 = hst::checked_cast<_SecondObject>(a1);
		 return m_p(_a1, _a2);
	 }
	 virtual ~CollisionMultiMethodInstance() {}
};


template<
typename _ReturnType, 
typename _Arg,
typename _FirstObject,
typename _SecondObject
>
class CollisionMultiMethodInstanceWithArg: 
public  p2d::BasicCollisionMultiMethodInstanceWithArg<_ReturnType, _Arg>
{
 public:
	 typedef _ReturnType (*inner_t)(_FirstObject * o1, 
								const _Arg & a1, 
								_SecondObject * o2,
								const _Arg & a2
							   );
 private:
	 inner_t m_p; //!< An inner function to call
	 bool m_reverse;  //!< Whether arguments should be reversed	 
 public:
	/*! Constructs new instance
		\param[in] p pointer to free function
		\param[in] reverse whether arguments should be reversed
	 */
	inline CollisionMultiMethodInstanceWithArg(
		  inner_t p,
		  bool reverse
    ) : m_p(p), m_reverse(reverse) {}
	/*! Returns true, if arguments will be reversed on exectution
		 \return reverse flag
	  */
	bool reverse() const { return m_reverse; }
	 /*! Invokes a method
		 \param[in] a1 first object
		 \param[in] ak1 argument, related to first object
		 \param[in] a2 second object
		 \param[in] ak2 argument, related to second object
		 \return returned type
	  */
	 virtual _ReturnType invoke(
		 p2d::CollisionShape * a1, 
		 const _Arg & ak1, 
		 p2d::CollisionShape * a2,
		 const _Arg & ak2
		 )
	 {
		 if (!m_reverse)
		 {
		  _FirstObject * _a1 = hst::checked_cast<_FirstObject>(a1);
		  _SecondObject * _a2 = hst::checked_cast<_SecondObject>(a2);
		  return m_p(_a1, ak1, _a2, ak2);
		 }
		 _FirstObject * _a1 = hst::checked_cast<_FirstObject>(a2);
		 _SecondObject * _a2 = hst::checked_cast<_SecondObject>(a1);
		 return m_p(_a1, ak2, _a2,  ak1);
	 }
	 virtual ~CollisionMultiMethodInstanceWithArg() {}
};


/*! Defines a multi-method as set of specific methods
 */
template<typename _ReturnType>
class CollisionMultiMethod
{
	public:
		typedef p2d::BasicCollisionMultiMethodInstance<_ReturnType> instance_t;
		typedef hst::ptrhash<hst::string, instance_t> group_t;
		typedef hst::hash<hst::string, group_t> instances_t;
    protected:
		instances_t m_instances; //!< Instances of method
		bool m_init;             //!< Whether multimethod initted
		/*! This function inits all callbacks. 
			You should add your dispatchers here 
		 */
		virtual void init() { }
		/*!  Registers new callbacks. You should place calls of this 
			 functions in init.
		 */
		template<typename _First, typename _Second>
		void add( _ReturnType (*p)(_First *, _Second *) )
		{
			hst::string fst = _First::globalMetaData()->name();
			hst::string snd = _Second::globalMetaData()->name();
			if (m_instances.contains(fst) == false)
			{
				m_instances.insert(fst, group_t());
			}
			if (m_instances[fst].contains(snd) == false)
			{
				p2d::CollisionMultiMethodInstance<_ReturnType, _First, _Second>
				* a = 
				new  
				p2d::CollisionMultiMethodInstance<_ReturnType, _First, _Second>(
				p, false
				);
				m_instances[fst].insert(snd, a);
			}
			// Try insert reverse call
			if (m_instances.contains(snd) == false)
			{
				m_instances.insert(snd, group_t());
			}
			if (m_instances[snd].contains(fst) == false)
			{
				p2d::CollisionMultiMethodInstance<_ReturnType, _First, _Second>
				* a = 
				new  
				p2d::CollisionMultiMethodInstance<_ReturnType, _First, _Second>(
				p, true
				);
				m_instances[snd].insert(fst, a);
			}
		}
		/*! Lookups for needed instance of multimethod
			\param[in] a first shape
			\param[in] b second shape
			\return instance if found
		 */
		instance_t * lookup(CollisionShape * a, CollisionShape * b)
		{
			const hst::string & type1 = a->metaData()->name();
			const hst::string & type2 = b->metaData()->name();
			instance_t * result = NULL;
			if (m_instances.contains(type1))
			{
				if (m_instances[type1].contains(type2))
				{
					result  = m_instances[type1][type2];;
				}
			}
			return result;
		}
	public:
		CollisionMultiMethod() { m_init = false;}
		/*! Invokes a multi-method, if possible. Returns default object,
			if can't handle.
			\param[in] a first shape
			\param[in] b second shape
		 */
		virtual _ReturnType invoke(CollisionShape * a, CollisionShape * b)
		{
			if (!m_init)
			{
				m_init = true;
				init();
			}
			instance_t * d = lookup(a,b);
			if (!d) return _ReturnType();
			return d->invoke(a, b);
		}
		virtual ~CollisionMultiMethod() {}
};

/*! Defines a multi-method as set of specific methods
 */
template<typename _ReturnType, typename _Arg>
class CollisionMultiMethodWithArg
{
	public:
		typedef p2d::BasicCollisionMultiMethodInstanceWithArg<_ReturnType, _Arg> instance_t;
		typedef hst::ptrhash<hst::string, instance_t> group_t;
		typedef hst::hash<hst::string, group_t> instances_t;
    protected:
		instances_t m_instances; //!< Instances of method
		bool m_init;             //!< Whether multimethod initted
		/*! This function inits all callbacks. 
			You should add your dispatchers here 
		 */
		virtual void init() { }
		/*!  Registers new callbacks. You should place calls of this 
			 functions in init.
		 */
		template<typename _First, typename _Second>
		void add( _ReturnType (*p)(_First *,const _Arg&,  _Second *, const _Arg&) )
		{
			hst::string fst = _First::globalMetaData()->name();
			hst::string snd = _Second::globalMetaData()->name();
			if (m_instances.contains(fst) == false)
			{
				m_instances.insert(fst, group_t());
			}
			if (m_instances[fst].contains(snd) == false)
			{
				p2d::CollisionMultiMethodInstanceWithArg<_ReturnType, _Arg, _First, _Second>
				* a = 
				new  
				p2d::CollisionMultiMethodInstanceWithArg<_ReturnType, _Arg, _First, _Second>(
				p, false
				);
				m_instances[fst].insert(snd, a);
			}
			// Try insert reverse call
			if (m_instances.contains(snd) == false)
			{
				m_instances.insert(snd, group_t());
			}
			if (m_instances[snd].contains(fst) == false)
			{
				p2d::CollisionMultiMethodInstanceWithArg<_ReturnType, _Arg, _First, _Second>
				* a = 
				new  
				p2d::CollisionMultiMethodInstanceWithArg<_ReturnType, _Arg, _First, _Second>(
				p, true
				);
				m_instances[snd].insert(fst, a);
			}
		}
		/*! Lookups for needed instance of multimethod
			\param[in] a first shape
			\param[in] b second shape
			\return instance if found
		 */
		instance_t * lookup(CollisionShape * a, CollisionShape * b)
		{
			const hst::string & type1 = a->metaData()->name();
			const hst::string & type2 = b->metaData()->name();
			instance_t * result = NULL;
			if (m_instances.contains(type1))
			{
				if (m_instances[type1].contains(type2))
				{
					result  = m_instances[type1][type2];;
				}
			}
			return result;
		}
		/*! Reverses a parts return type if need to
			\param[in] result result
		 */
		virtual void reverse(_ReturnType & result)
		{
		}
	public:
		CollisionMultiMethodWithArg() { m_init = false;}
		/*! Invokes a multi-method, if possible. Returns default object,
			if can't handle.
			\param[in] a first shape
			\param[in] ka argument, related to first shape
			\param[in] b second shape
			\param[in] kb argument, related to second shape			
		 */
		virtual _ReturnType invoke(CollisionShape * a, const _Arg & ka, CollisionShape * b, const _Arg & kb)
		{
			if (!m_init)
			{
				m_init = true;
				init();
			}
			instance_t * d = lookup(a,b);
			if (!d) return _ReturnType();
			_ReturnType k = d->invoke(a, ka, b, kb);
			if (d->reverse())
				reverse(k);
			return k;
		}
		virtual ~CollisionMultiMethodWithArg() {}
};


}


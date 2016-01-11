/**
 * @file
 * Declares the classes used in the event system.
 *
 * @author Alessandro Antonello
 * @date   jan 11, 2016
 * @since  Super Simple Template Library 1.0
 *
 * @copyright 2016, Paralaxe Tecnologia Ltda.. All rights reserved.
 **/
#ifndef __SSTLEVEN_HPP_DEFINED__
#define __SSTLEVEN_HPP_DEFINED__

#include "sstlfunc.hpp"

namespace ss {

/**
 * Main template class for the event system.
 * The class declares a type to be used as event dispatcher. Events are simple
 * functors lists. They can handle calling a list of bound functors passing
 * the same parameter. Events can be created to call functions with a single
 * parameter and no return value. Since the event can be bound to a list of
 * functions, the return value will never be used and all bound functions
 * should return \b void.
 * @tparam _Param_t Type of the parameter to pass to each bound functor.
 * @since 1.0
 * @ingroup sstl_events
 *//* --------------------------------------------------------------------- */
template <typename _Param_t>
class EventT
{
public:
    // typedef typename ss::FunctorT<void (_Param_t)> Delegate;/*{{{*/
    /**
     * Type of the delegate to be bound to this event.
     * Functors must be of this type to be bound to this event object.
     **/
    typedef typename ss::FunctorT<void (_Param_t)> Delegate;
    /*}}}*/

    /** @name Constructors */ //@{
    // EventT();/*{{{*/
    /**
     * Default constructor.
     * @since 1.0
     **/
    EventT() { }
    /*}}}*/
    //@}

    /** @name Attributes */ //@{
    // size_t count() const;/*{{{*/
    /**
     * Retrieves the number of bound functors.
     * @returns A `size_t` value with the number of bound functors.
     * @since 1.0
     **/
    size_t count() const { return m_delegates.size(); }
    /*}}}*/
    //@}

    /** @name Operations */ //@{
    // void bind(_Target_t *target);/*{{{*/
    /**
     * Add a new functor object in the list of delegates.
     * @tparam _Target_t Type of the target class object which member function
     * must be called when the event is invoked.
     * @tparam _Method Pointer to the member function to be invoked.
     * @param target Pointer to the instance of \a _Target_t object on the \a
     * _Method will be called.
     * @since 1.0
     **/
    template <class _Target_t, void (_Target_t::*_Method)(_Param_t)>
    void bind(_Target_t *target) {
        m_delegates.push_back(Delegate::from<_Target_t, _Method>(target));
    }
    /*}}}*/
    // void unbound(_Target_t *target);/*{{{*/
    /**
     * Removes a delegate from the delegate list.
     * @tparam _Target_t Type of the target class object which member function
     * was used to create the delegate. This template parameter may not be
     * defined. The compiler can deduce it through the function parameter.
     * @param target Pointer to the object instance that was used to create
     * the delegate object. All delegates with this target object will be
     * removed.
     * @since 1.0
     **/
    template <class _Target_t>
    void unbound(_Target_t *target) {
        std::list<Delegate>::iterator it = m_delegates.begin();
        while (it != m_delegates.end())
        {
            if ((*it).isHost((void *)target))
                it = m_delegates.erase(it);
            else
                ++it;
        }
    }
    /*}}}*/
    //@}

    /** @name Overloaded Operators */ //@{
    // EventT& operator <<(const Delegate &delegate);/*{{{*/
    /**
     * Adds a delegate object into the list of this event.
     * @param delegate The delegate object to be added.
     * @return A reference to this event instance.
     * @since 1.0
     **/
    EventT& operator <<(const Delegate &delegate) {
        m_delegates.push_back(delegate);
        return *this;
    }
    /*}}}*/
    // void operator ()(_Param_t param);/*{{{*/
    /**
     * Invokes all delegates in the list of this event.
     * @tparam _Param_t Type of the parameter. This template parameter may not
     * be specified since the compiler can deduce it.
     * @param param Parameter to pass to all bound delegates.
     * @remarks All bound functors will be called in the same sequence as they
     * was added to the list.
     * @since 1.0
     **/
    void operator ()(_Param_t param) {
        std::list< Delegate >::iterator it = m_delegates.begin();

        while (it != m_delegates.end())
            (*it).exec(param);
    }
    /*}}}*/
    //@}

private:
    std::list<Delegate> m_delegates;
};

/**
 * Specialization of the EventT template class for functions with no
 * parameters.
 * @since 1.0
 * @ingroup sstl_events
 *//* --------------------------------------------------------------------- */
template <>
class EventT
{
public:
    // typedef typename ss::FunctorT<void ()> Delegate;/*{{{*/
    /**
     * Type of the delegate to be bound to this event.
     * Functors must be of this type to be bound to this event object.
     **/
    typedef typename ss::FunctorT<void ()> Delegate;
    /*}}}*/

    /** @name Constructors */ //@{
    // EventT();/*{{{*/
    /**
     * Default constructor.
     * @since 1.0
     **/
    EventT() { }
    /*}}}*/
    //@}

    /** @name Attributes */ //@{
    // size_t count() const;/*{{{*/
    /**
     * Retrieves the number of bound functors.
     * @returns A `size_t` value with the number of bound functors.
     * @since 1.0
     **/
    size_t count() const { return m_delegates.size(); }
    /*}}}*/
    //@}

    /** @name Operations */ //@{
    // void bind(_Target_t *target);/*{{{*/
    /**
     * Add a new functor object in the list of delegates.
     * @tparam _Target_t Type of the target class object which member function
     * must be called when the event is invoked.
     * @tparam _Method Pointer to the member function to be invoked.
     * @param target Pointer to the instance of \a _Target_t object on the \a
     * _Method will be called.
     * @since 1.0
     **/
    template <class _Target_t, void (_Target_t::*_Method)()>
    void bind(_Target_t *target) {
        m_delegates.push_back(Delegate::from<_Target_t, _Method>(target));
    }
    /*}}}*/
    // void unbound(_Target_t *target);/*{{{*/
    /**
     * Removes a delegate from the delegate list.
     * @tparam _Target_t Type of the target class object which member function
     * was used to create the delegate. This template parameter may not be
     * defined. The compiler can deduce it through the function parameter.
     * @param target Pointer to the object instance that was used to create
     * the delegate object. All delegates with this target object will be
     * removed.
     * @since 1.0
     **/
    template <class _Target_t>
    void unbound(_Target_t *target) {
        std::list<Delegate>::iterator it = m_delegates.begin();
        while (it != m_delegates.end())
        {
            if ((*it).isHost((void *)target))
                it = m_delegates.erase(it);
            else
                ++it;
        }
    }
    /*}}}*/
    //@}

    /** @name Overloaded Operators */ //@{
    // EventT& operator <<(const Delegate &delegate);/*{{{*/
    /**
     * Adds a delegate object into the list of this event.
     * @param delegate The delegate object to be added.
     * @return A reference to this event instance.
     * @since 1.0
     **/
    EventT& operator <<(const Delegate &delegate) {
        m_delegates.push_back(delegate);
        return *this;
    }
    /*}}}*/
    // void operator ()();/*{{{*/
    /**
     * Invokes all delegates in the list of this event.
     * @remarks All bound functors will be called in the same sequence as they
     * was added to the list.
     * @since 1.0
     **/
    void operator ()() {
        std::list< Delegate >::iterator it = m_delegates.begin();

        while (it != m_delegates.end())
            (*it).exec();
    }
    /*}}}*/
    //@}

private:
    std::list<Delegate> m_delegates;
};

}   /* namespace ss */

#endif /* __SSTLEVEN_HPP_DEFINED__ */

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

#include <list>
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
 * @tparam _Signature_t The signature of the functions to handle this event.
 * Example: `EventT<void(int)> onSelected;` declares an event to call
 * functions returning void and accepting an \b int as parameter.
 * @remarks We could simplify the declaration to the single parameter of the
 * function since there is no reason to create events returning non-void
 * values. We choose to require the function signature to make the event
 * declaration more easy to read and understand.
 * @since 1.0
 * @ingroup sstl_events
 *//* --------------------------------------------------------------------- */
template <typename _Signature_t> class EventT;

/**
 * Specialization of the EventT template class for functions with two
 * arguments.
 * @tparam _Return_t The return type of the function. Should be void.
 * @tparam _Param1_t Type of first function parameter.
 * @tparam _Param2_t Type of second function parameter.
 * @since 1.0
 * @ingroup sstl_events
 *//* --------------------------------------------------------------------- */
template <typename _Return_t, typename _Param1_t, typename _Param2_t>
class EventT<_Return_t (_Param1_t, _Param2_t)>
{
public:
    // typedef typename ss::FunctorT<_Return_t (_Param1_t, _Param2_t)> Delegate;/*{{{*/
    /**
     * Type of the delegate to be bound to this event.
     * Functors must be of this type to be bound to this event object.
     **/
    typedef typename ss::FunctorT<_Return_t (_Param1_t, _Param2_t)> Delegate;
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
    template <class _Target_t, _Return_t (_Target_t::*_Method)(_Param1_t, _Param2_t)>
    void bind(_Target_t *target) {
        Delegate delegate;
        delegate.bind<_Target_t, _Method>(target);
        m_delegates.push_back( delegate );
    }
    /*}}}*/
    // void bind(_Target_t const *target);/*{{{*/
    /**
     * Add a new functor object in the list of delegates.
     * This is an overloaded member function.
     * @tparam _Target_t Type of the target class object which member function
     * must be called when the event is invoked.
     * @tparam _Method Pointer to the member function to be invoked.
     * @param target Pointer to the instance of \a _Target_t object on the \a
     * _Method will be called.
     * @since 1.0
     **/
    template <class _Target_t, _Return_t (_Target_t::*_Method)(_Param1_t, _Param2_t)>
    void bind(_Target_t const *target) {
        Delegate delegate;
        delegate.bind<_Target_t, _Method>(const_cast<_Target_t*>(target));
        m_delegates.push_back( delegate );
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
        typename std::list<Delegate>::iterator it = m_delegates.begin();
        while (it != m_delegates.end())
        {
            if ((*it).isHost((void *)target))
                it = m_delegates.erase(it);
            else
                ++it;
        }
    }
    /*}}}*/
    // void link(EventT<_Return_t (_Param1_t, _Param2_t)> *e);/*{{{*/
    /**
     * Link an event to this event.
     * @param e Pointer to the event object instance to link. Must have the
     * same return value and parameters of this event.
     * @since 1.0
     **/
    void link(EventT<_Return_t (_Param1_t, _Param2_t)> *e) {
        bind<EventT<_Return_t (_Param1_t, _Param2_t)>,
            &EventT<_Return_t (_Param1_t, _Param2_t)>::trigger>(e);
    }
    /*}}}*/
    // void trigger(_Param1_t a1, _Param2_t a2);/*{{{*/
    /**
     * Trigger the functions bound to this event object.
     * @param a1 Argument to pass as the first parameter.
     * @param a2 Argument to pass as the second parameter.
     * @since 1.0
     **/
    void trigger(_Param1_t a1, _Param2_t a2) {
        typename std::list< Delegate >::iterator it = m_delegates.begin();

        while (it != m_delegates.end()) {
            (*it).exec(a1, a2);
            ++it;
        }
    }
    /*}}}*/
    //@}

    /** @name Helpers */ //@{
    // void add(_Target_t *target);/*{{{*/
    /**
     * Add a new functor object in the list of delegates.
     * @tparam _Target_t Type of the target class object which member function
     * must be called when the event is invoked.
     * @tparam _Method Pointer to the member function to be invoked.
     * @param target Pointer to the instance of \a _Target_t object on the \a
     * _Method will be called.
     * @remarks This function uses `add(const Delegate &)` to build and adds
     * a callback to the list of delegates of this event. This means that
     * a delegate already added to this event will not be duplicated.
     * @since 1.0
     **/
    template <class _Target_t, _Return_t (_Target_t::*_Method)(_Param1_t, _Param2_t)>
    void add(_Target_t *target) {
        Delegate d; d.bind<_Target_t, _Method>(target);
        add( d );
    }
    /*}}}*/
    // void add(_Target_t const *target);/*{{{*/
    /**
     * Add a new functor object in the list of delegates.
     * This is an overloaded member function.
     * @tparam _Target_t Type of the target class object which member function
     * must be called when the event is invoked.
     * @tparam _Method Pointer to the member function to be invoked.
     * @param target Pointer to the instance of \a _Target_t object on the \a
     * _Method will be called.
     * @remarks This function uses `add(const Delegate &)` to build and adds
     * a callback to the list of delegates of this event. This means that
     * a delegate already added to this event will not be duplicated.
     * @since 1.0
     **/
    template <class _Target_t, _Return_t (_Target_t::*_Method)(_Param1_t, _Param2_t)>
    void add(_Target_t const *target) {
        Delegate d; d.bind<_Target_t, _Method>(const_cast<_Target_t*>(target));
        add( d );
    }
    /*}}}*/
    // void add(const Delegate &callback);/*{{{*/
    /**
     * Adds a delegate object into the list of this event.
     * @param callback A reference to the Delegate object to add. The function
     * will search the internal list of delegates, if this object was already
     * added the function does nothing. That is, the same delegate will not be
     * added twice to the same event.
     * @since 1.0
     **/
    void add(const Delegate &callback) {
        typename std::list<Delegate>::iterator it = m_delegates.begin();
        while (it != m_delegates.end()) {
            if ((*it) == callback)
                return;
        }
        m_delegates.push_back(callback);
    }
    /*}}}*/
    // void remove(_Target_t *target);/*{{{*/
    /**
     * Removes a delegate from the list of delegates of this event.
     * @tparam _Target_t Type of the target class object which member function
     * used to be called when the event is invoked.
     * @tparam _Method Pointer to the member function to be removed.
     * @param target Pointer to the instance of \a _Target_t object on the \a
     * _Method to be removed.
     * @since 1.0
     **/
    template <class _Target_t, _Return_t (_Target_t::*_Method)(_Param1_t, _Param2_t)>
    void remove(_Target_t *target) {
        Delegate d; d.bind<_Target_t, _Method>(target);
        remove(d);
    }
    /*}}}*/
    // void remove(_Target_t const *target);/*{{{*/
    /**
     * Removes a delegate from the list of delegates of this event.
     * @tparam _Target_t Type of the target class object which member function
     * used to be called when the event is invoked.
     * @tparam _Method Pointer to the member function to be removed.
     * @param target Pointer to the instance of \a _Target_t object on the \a
     * _Method to be removed.
     * @since 1.0
     **/
    template <class _Target_t, _Return_t (_Target_t::*_Method)(_Param1_t, _Param2_t)>
    void remove(_Target_t const *target) {
        Delegate d; d.bind<_Target_t, _Method>(const_cast<_Target_t*>(target));
        remove(d);
    }
    /*}}}*/
    // void remove(const Delegate &callback);/*{{{*/
    /**
     * Removes a callback function from the list of this event.
     * @param callback Reference to the delegate object. This delegate object
     * must be build with the same pointer object and function that was
     * previously added to this event.
     * @since 1.0
     **/
    void remove(const Delegate &callback) {
        typename std::list<Delegate>::iterator it = m_delegates.begin();
        while (it != m_delegates.end()) {
            if ((*it) == callback)
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
    // void operator ()(_Param1_t a1, _Param2_t a2);/*{{{*/
    /**
     * Invokes all delegates in the list of this event.
     * @param a1 Argument for first function parameter.
     * @param a2 Argument for second function parameter.
     * @remarks All bound functors will be called in the same sequence as they
     * was added to the list.
     * @since 1.0
     **/
    void operator ()(_Param1_t a1, _Param2_t a2) {
        this->trigger(a1, a2);
    }
    /*}}}*/
    //@}

private:
    std::list<Delegate> m_delegates;
};

/**
 * Specialization of the EventT template for functions with a parameter.
 * @tparam _Return_t The return type of the function. Should be \b void.
 * @tparam _Param_t Type of the parameter to pass to each bound functor.
 * @since 1.0
 * @ingroup sstl_events
 *//* --------------------------------------------------------------------- */
template <typename _Return_t, typename _Param_t>
class EventT<_Return_t (_Param_t)>
{
public:
    // typedef typename ss::FunctorT<_Return_t (_Param_t)> Delegate;/*{{{*/
    /**
     * Type of the delegate to be bound to this event.
     * Functors must be of this type to be bound to this event object.
     **/
    typedef typename ss::FunctorT<_Return_t (_Param_t)> Delegate;
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
    template <class _Target_t, _Return_t (_Target_t::*_Method)(_Param_t)>
    void bind(_Target_t *target) {
        Delegate delegate;
        delegate.bind<_Target_t, _Method>(target);
        m_delegates.push_back( delegate );
    }
    /*}}}*/
    // void bind(_Target_t const *target);/*{{{*/
    /**
     * Add a new functor object in the list of delegates.
     * This is an overloaded member function.
     * @tparam _Target_t Type of the target class object which member function
     * must be called when the event is invoked.
     * @tparam _Method Pointer to the member function to be invoked.
     * @param target Pointer to the instance of \a _Target_t object on the \a
     * _Method will be called.
     * @since 1.0
     **/
    template <class _Target_t, _Return_t (_Target_t::*_Method)(_Param_t)>
    void bind(const _Target_t *target) {
        Delegate delegate;
        delegate.bind<_Target_t, _Method>(const_cast<_Target_t*>(target));
        m_delegates.push_back( delegate );
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
        typename std::list<Delegate>::iterator it = m_delegates.begin();
        while (it != m_delegates.end())
        {
            if ((*it).isHost((void *)target))
                it = m_delegates.erase(it);
            else
                ++it;
        }
    }
    /*}}}*/
    // void link(EventT<_Return_t (_Param_t)> *e);/*{{{*/
    /**
     * Link an event to this event.
     * @param e Pointer to the event object instance to link. Must have the
     * same return value and parameters of this event.
     * @since 1.0
     **/
    void link(EventT<_Return_t (_Param_t)> *e) {
        bind<EventT<_Return_t (_Param_t)>, &EventT<_Return_t (_Param_t)>::trigger>(e);
    }
    /*}}}*/
    // void trigger(_Param_t param);/*{{{*/
    /**
     * Trigger the functions bound to this event object.
     * @param param Argument to pass to the function.
     * @since 1.0
     **/
    void trigger(_Param_t param) {
        typename std::list< Delegate >::iterator it = m_delegates.begin();

        while (it != m_delegates.end()) {
            (*it).exec(param);
            ++it;
        }
    }
    /*}}}*/
    //@}

    /** @name Helpers */ //@{
    // void add(_Target_t *target);/*{{{*/
    /**
     * Add a new functor object in the list of delegates.
     * @tparam _Target_t Type of the target class object which member function
     * must be called when the event is invoked.
     * @tparam _Method Pointer to the member function to be invoked.
     * @param target Pointer to the instance of \a _Target_t object on the \a
     * _Method will be called.
     * @remarks This function uses `add(const Delegate &)` to build and adds
     * a callback to the list of delegates of this event. This means that
     * a delegate already added to this event will not be duplicated.
     * @since 1.0
     **/
    template <class _Target_t, _Return_t (_Target_t::*_Method)(_Param_t)>
    void add(_Target_t *target) {
        Delegate d; d.bind<_Target_t, _Method>(target);
        add( d );
    }
    /*}}}*/
    // void add(_Target_t const *target);/*{{{*/
    /**
     * Add a new functor object in the list of delegates.
     * This is an overloaded member function.
     * @tparam _Target_t Type of the target class object which member function
     * must be called when the event is invoked.
     * @tparam _Method Pointer to the member function to be invoked.
     * @param target Pointer to the instance of \a _Target_t object on the \a
     * _Method will be called.
     * @remarks This function uses `add(const Delegate &)` to build and adds
     * a callback to the list of delegates of this event. This means that
     * a delegate already added to this event will not be duplicated.
     * @since 1.0
     **/
    template <class _Target_t, _Return_t (_Target_t::*_Method)(_Param_t)>
    void add(_Target_t const *target) {
        Delegate d; d.bind<_Target_t, _Method>(const_cast<_Target_t*>(target));
        add( d );
    }
    /*}}}*/
    // void add(const Delegate &callback);/*{{{*/
    /**
     * Adds a delegate object into the list of this event.
     * @param callback A reference to the Delegate object to add. The function
     * will search the internal list of delegates, if this object was already
     * added the function does nothing. That is, the same delegate will not be
     * added twice to the same event.
     * @since 1.0
     **/
    void add(const Delegate &callback) {
        typename std::list<Delegate>::iterator it = m_delegates.begin();
        while (it != m_delegates.end()) {
            if ((*it) == callback)
                return;
        }
        m_delegates.push_back(callback);
    }
    /*}}}*/
    // void remove(_Target_t *target);/*{{{*/
    /**
     * Removes a delegate from the list of delegates of this event.
     * @tparam _Target_t Type of the target class object which member function
     * used to be called when the event is invoked.
     * @tparam _Method Pointer to the member function to be removed.
     * @param target Pointer to the instance of \a _Target_t object on the \a
     * _Method to be removed.
     * @since 1.0
     **/
    template <class _Target_t, _Return_t (_Target_t::*_Method)(_Param_t)>
    void remove(_Target_t *target) {
        Delegate d; d.bind<_Target_t, _Method>(target);
        remove( d );
    }
    /*}}}*/
    // void remove(_Target_t const *target);/*{{{*/
    /**
     * Removes a delegate from the list of delegates of this event.
     * @tparam _Target_t Type of the target class object which member function
     * used to be called when the event is invoked.
     * @tparam _Method Pointer to the member function to be removed.
     * @param target Pointer to the instance of \a _Target_t object on the \a
     * _Method to be removed.
     * @since 1.0
     **/
    template <class _Target_t, _Return_t (_Target_t::*_Method)(_Param_t)>
    void remove(_Target_t const *target) {
        Delegate d; d.bind<_Target_t, _Method>(const_cast<_Target_t*>(target));
        remove( d );
    }
    /*}}}*/
    // void remove(const Delegate &callback);/*{{{*/
    /**
     * Removes a callback function from the list of this event.
     * @param callback Reference to the delegate object. This delegate object
     * must be build with the same pointer object and function that was
     * previously added to this event.
     * @since 1.0
     **/
    void remove(const Delegate &callback) {
        typename std::list<Delegate>::iterator it = m_delegates.begin();
        while (it != m_delegates.end()) {
            if ((*it) == callback)
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
        this->trigger(param);
    }
    /*}}}*/
    //@}

private:
    std::list<Delegate> m_delegates;
};

/**
 * Specialization of the EventT template class for functions with no
 * parameters.
 * @tparam _Return_t The return type of the function. Should be void.
 * @since 1.0
 * @ingroup sstl_events
 *//* --------------------------------------------------------------------- */
template <typename _Return_t>
class EventT<_Return_t ()>
{
public:
    // typedef typename ss::FunctorT<_Return_t ()> Delegate;/*{{{*/
    /**
     * Type of the delegate to be bound to this event.
     * Functors must be of this type to be bound to this event object.
     **/
    typedef typename ss::FunctorT<_Return_t ()> Delegate;
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
    template <class _Target_t, _Return_t (_Target_t::*_Method)()>
    void bind(_Target_t *target) {
        Delegate delegate;
        delegate.bind<_Target_t, _Method>(target);
        m_delegates.push_back( delegate );
    }
    /*}}}*/
    // void bind(_Target_t const *target);/*{{{*/
    /**
     * Add a new functor object in the list of delegates.
     * This is an overloaded member function.
     * @tparam _Target_t Type of the target class object which member function
     * must be called when the event is invoked.
     * @tparam _Method Pointer to the member function to be invoked.
     * @param target Pointer to the instance of \a _Target_t object on the \a
     * _Method will be called.
     * @since 1.0
     **/
    template <class _Target_t, _Return_t (_Target_t::*_Method)()>
    void bind(_Target_t const *target) {
        Delegate delegate;
        delegate.bind<_Target_t, _Method>(const_cast<_Target_t*>(target));
        m_delegates.push_back( delegate );
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
        typename std::list<Delegate>::iterator it = m_delegates.begin();
        while (it != m_delegates.end())
        {
            if ((*it).isHost((void *)target))
                it = m_delegates.erase(it);
            else
                ++it;
        }
    }
    /*}}}*/
    // void link(EventT<_Return_t ()> *e);/*{{{*/
    /**
     * Link an event to this event.
     * @param e Pointer to the event object instance to link. Must have the
     * same return value and parameters of this event.
     * @since 1.0
     **/
    void link(EventT<_Return_t ()> *e) {
        bind<EventT<_Return_t ()>, &EventT<_Return_t ()>::trigger>(e);
    }
    /*}}}*/
    // void trigger();/*{{{*/
    /**
     * Trigger the functions bound to this event object.
     * @since 1.0
     **/
    void trigger() {
        typename std::list< Delegate >::iterator it = m_delegates.begin();

        while (it != m_delegates.end()) {
            (*it).exec();
            ++it;
        }
    }
    /*}}}*/
    //@}

    /** @name Helpers */ //@{
    // void add(_Target_t *target);/*{{{*/
    /**
     * Add a new functor object in the list of delegates.
     * @tparam _Target_t Type of the target class object which member function
     * must be called when the event is invoked.
     * @tparam _Method Pointer to the member function to be invoked.
     * @param target Pointer to the instance of \a _Target_t object on the \a
     * _Method will be called.
     * @remarks This function uses `add(const Delegate &)` to build and adds
     * a callback to the list of delegates of this event. This means that
     * a delegate already added to this event will not be duplicated.
     * @since 1.0
     **/
    template <class _Target_t, _Return_t (_Target_t::*_Method)()>
    void add(_Target_t *target) {
        Delegate d; d.bind<_Target_t, _Method>(target);
        add( d );
    }
    /*}}}*/
    // void add(_Target_t const *target);/*{{{*/
    /**
     * Add a new functor object in the list of delegates.
     * This is an overloaded member function.
     * @tparam _Target_t Type of the target class object which member function
     * must be called when the event is invoked.
     * @tparam _Method Pointer to the member function to be invoked.
     * @param target Pointer to the instance of \a _Target_t object on the \a
     * _Method will be called.
     * @remarks This function uses `add(const Delegate &)` to build and adds
     * a callback to the list of delegates of this event. This means that
     * a delegate already added to this event will not be duplicated.
     * @since 1.0
     **/
    template <class _Target_t, _Return_t (_Target_t::*_Method)()>
    void add(_Target_t const *target) {
        Delegate d; d.bind<_Target_t, _Method>(const_cast<_Target_t*>(target));
        add( d );
    }
    /*}}}*/
    // void add(const Delegate &callback);/*{{{*/
    /**
     * Adds a delegate object into the list of this event.
     * @param callback A reference to the Delegate object to add. The function
     * will search the internal list of delegates, if this object was already
     * added the function does nothing. That is, the same delegate will not be
     * added twice to the same event.
     * @since 1.0
     **/
    void add(const Delegate &callback) {
        typename std::list<Delegate>::iterator it = m_delegates.begin();
        while (it != m_delegates.end()) {
            if ((*it) == callback)
                return;
        }
        m_delegates.push_back(callback);
    }
    /*}}}*/
    // void remove(_Target_t *target);/*{{{*/
    /**
     * Removes a delegate from the list of delegates of this event.
     * @tparam _Target_t Type of the target class object which member function
     * used to be called when the event is invoked.
     * @tparam _Method Pointer to the member function to be removed.
     * @param target Pointer to the instance of \a _Target_t object on the \a
     * _Method to be removed.
     * @since 1.0
     **/
    template <class _Target_t, _Return_t (_Target_t::*_Method)()>
    void remove(_Target_t *target) {
        Delegate d; d.bind<_Target_t, _Method>(target);
        remove(d);
    }
    /*}}}*/
    // void remove(_Target_t const *target);/*{{{*/
    /**
     * Removes a delegate from the list of delegates of this event.
     * @tparam _Target_t Type of the target class object which member function
     * used to be called when the event is invoked.
     * @tparam _Method Pointer to the member function to be removed.
     * @param target Pointer to the instance of \a _Target_t object on the \a
     * _Method to be removed.
     * @since 1.0
     **/
    template <class _Target_t, _Return_t (_Target_t::*_Method)()>
    void remove(_Target_t const *target) {
        Delegate d; d.bind<_Target_t, _Method>(const_cast<_Target_t*>(target));
        remove(d);
    }
    /*}}}*/
    // void remove(const Delegate &callback);/*{{{*/
    /**
     * Removes a callback function from the list of this event.
     * @param callback Reference to the delegate object. This delegate object
     * must be build with the same pointer object and function that was
     * previously added to this event.
     * @since 1.0
     **/
    void remove(const Delegate &callback) {
        typename std::list<Delegate>::iterator it = m_delegates.begin();
        while (it != m_delegates.end()) {
            if ((*it) == callback)
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
        this->trigger();
    }
    /*}}}*/
    //@}

private:
    std::list<Delegate> m_delegates;
};

}   /* namespace ss */

#endif /* __SSTLEVEN_HPP_DEFINED__ */

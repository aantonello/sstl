/**
 * @file
 * Defines the ss::FunctorT template class.
 *
 * @author Alessandro Antonello
 * @date   jan 08, 2016
 * @since  Super Simple Template Library 1.0
 *
 * @copyright 2016, Paralaxe Tecnologia Ltda.. All rights reserved.
 **/
#ifndef __SSTLFUNC_HPP_DEFINED__
#define __SSTLFUNC_HPP_DEFINED__

namespace ss {

/**
 * Super Simple Functor class template.
 * This class is a basic implementation of a Functor class. It can be used to
 * pass pointers to member functions to be called from another unknown source.
 * It was designed to support C++ 98 compilers, thus, doens't support variadic
 * templates nor `std::forward` technique.
 * @tparam signature_t The function signature. Examples are:
 * - `void(int)`: Function returning void with an \b int parameter.
 * - `int(const std::string&)`: Function returning \b int with a reference to
 *   a const `std::string` object.
 * - `int()`: Function returning int with not parameter at all.
 * .
 * @remarks To use this template you declare an object with the function
 * signature:
 ~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * class MyObject
 * {
 *     int aFunction(int param);
 *     void anotherFunc(double param);
 * };
 * ss::FunctorT<int(int)> aFunctionFunctor;
 * ss::FunctorT<void(double)> anotherFunctionFunctor;
 ~~~~~~~~~~~~~~~~~~~~~
 * Them you must \e bind the objects with the related functions.
 ~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * MyObject object;
 * aFunctionFunctor.bind<MyObject, MyObject::aFunction>(&object);
 * anotherFunctionFunctor.bind<MyObject, MyObject::anotherFunction>(&object);
 ~~~~~~~~~~~~~~~~~~~~~
 * Now every time you write `aFunctionFunctor()` the `MyObject::aFunction()`
 * will be called and the same happens with `anotherFunctionFunctor`.
 * @since 1.0
 * @ingroup sstl_functors
 **/
template <typename signature_t> class FunctorT;

/**
 * Functor for functions with 2 parameters.
 * This specialization is selected when bind functions that have a return
 * value and two parameter. The return value can be `void`.
 * @tparam _Return_t Type of return value of the function to be called.
 * @tparam _Param1_t Type of the first parameter of the function.
 * @tparam _Param2_t Type of the second parameter of the function.
 * @since 1.0
 * @ingroup sstl_functors
 *//* --------------------------------------------------------------------- */
template <typename _Return_t, typename _Param1_t, typename _Param2_t>
class FunctorT<_Return_t (_Param1_t, _Param2_t)>
{
public:
    /** Typedef's the function signature. */
    typedef _Return_t (*signature)(_Param1_t, _Param2_t);

    /** @name Constructors & Destructors */ //@{
    // FunctorT() { }/*{{{*/
    /**
     * Default constructor.
     * Builds an empty functor object.
     * @since 1.0
     **/
    FunctorT() : m_host(NULL), m_call(NULL) { }
    /*}}}*/
    //@}

    // Static Functions
    // static FunctorT from(_Host_t *host) { }/*{{{*/
    /**
     * Creates an ss::FunctorT instance bound to a pointer to member function.
     * @tparam _Host_t Type of the host object. The object that declares the
     * function to be called.
     * @tparam _Method Pointer to the member function to be called.
     * @param host Pointer to the instance of the object where the pointer to
     * member function should be invoked.
     * @return A new instance of an `ss::FunctorT` object.
     * @remarks This function must be called on a declaration of a %FunctorT
     * type. For example, suppose you have an object that declares a function
     * returning `void` with an `int` parameter. You declare the functor type
     * and call this function passing the pointer to member function as
     * template parameter.
     ~~~~~~~~~~~~~~~~~~~~~{.cpp}
     * class SomeObject
     * {
     *     void fun(int data, bool another);
     * };
     *
     * SomeObject object;
     * ss::FunctorT<void(int, bool)> myFunctor;
     * myFunctor = ss::FunctorT<void(int, bool)>::from<SomeObject, SomeObject::fun>(&object);
     ~~~~~~~~~~~~~~~~~~~~~
     * @since 1.0
     **/
    template <class _Host_t, _Return_t (_Host_t::*_Method)(_Param1_t, _Param2_t)>
    static FunctorT from(_Host_t *host) {
        return FunctorT(host, &invoker<_Host_t, _Method>);
    }
    /*}}}*/
    // static FunctorT from(_Host_t const *host) { }/*{{{*/
    /**
     * Overload used on a const pointer to member function.
     * @tparam _Host_t Type of the host object. The object that declares the
     * function to be called.
     * @tparam _Method Pointer to the member function to be called.
     * @param host Pointer to the instance of the object where the pointer to
     * member function should be invoked.
     * @return A new instance of an `ss::FunctorT` object.
     * @remarks This function must be called on a declaration of a %FunctorT
     * type. For example, suppose you have an object that declares a function
     * returning `void` with an `int` parameter. You declare the functor type
     * and call this function passing the pointer to member function as
     * template parameter.
     ~~~~~~~~~~~~~~~~~~~~~{.cpp}
     * class SomeObject
     * {
     *     void fun(int data, bool other) const;
     * };
     *
     * SomeObject object;
     * ss::FunctorT<void(int, bool)> myFunctor;
     * myFunctor = ss::FunctorT<void(int, bool)>::from<SomeObject, SomeObject::fun>(&object);
     ~~~~~~~~~~~~~~~~~~~~~
     * @since 1.0
     **/
    template <class _Host_t, _Return_t (_Host_t::*_Method)(_Param1_t, _Param2_t) const>
    static FunctorT from(_Host_t const *host) {
        return FunctorT(const_cast<_Host_t*>(host), &const_invoker<_Host_t, _Method>);
    }
    /*}}}*/

    /** @name Attributes */ //@{
    // bool isHost(void *ptr) const { }/*{{{*/
    /**
     * Checks whether this object instance points to the specified host.
     * @param ptr Pointer to the instance of the host object.
     * @return \b true if the specified address is the same as the host
     * pointed by this object. \b false otherwise.
     * @since 1.0
     **/
    bool isHost(void *ptr) const {
        return (m_host == ptr);
    }
    /*}}}*/
    // bool valid() const { }/*{{{*/
    /**
     * Checks whether this object is valid.
     * @returns \b true if this object was bound to a host class and member
     * function. \b false otherwise.
     * @note This function doesn't valid the target object and member
     * function. Only checks whether this functor was bound. The host object
     * must be valid until this functor is destroyed.
     * @since 1.0
     **/
    bool valid() const {
        return ((m_host != NULL) && (m_call != NULL));
    }
    /*}}}*/
    //@}

    /** @name Operations */ //@{
    // void bind(_Host_t *host) { }/*{{{*/
    /**
     * Binds a functor with a pointer to member function.
     * @tparam _Host_t Type of the host object. The object that declares the
     * function to be called.
     * @tparam _Method Pointer to the member function to be called.
     * @param host Pointer to the instance of the object where the pointer to
     * member function should be invoked.
     * @remarks This function must be called on a declaration of a %FunctorT
     * type. For example, suppose you have an object that declares a function
     * returning `void` with an `int` parameter. You declare the functor type
     * and call this function passing the pointer to member function as
     * template parameter.
     ~~~~~~~~~~~~~~~~~~~~~{.cpp}
     * class SomeObject
     * {
     *     void fun(int data, bool other);
     * };
     *
     * SomeObject object;
     * ss::FunctorT<void(int, bool)> myFunctor;
     * myFunctor.bind<SomeObject, SomeObject::fun>(&object);
     ~~~~~~~~~~~~~~~~~~~~~
     * @since 1.0
     **/
    template <class _Host_t, _Return_t (_Host_t::*_Method)(_Param1_t, _Param2_t)>
    void bind(_Host_t *host) {
        m_host = (void *)host;
        m_call = &invoker<_Host_t, _Method>;
    }
    /*}}}*/
    // void bind(_Host_t const *host) { }/*{{{*/
    /**
     * Overload used on a const pointer to member function.
     * @tparam _Host_t Type of the host object. The object that declares the
     * function to be called.
     * @tparam _Method Pointer to the member function to be called.
     * @param host Pointer to the instance of the object where the pointer to
     * member function should be invoked.
     * @remarks This function must be called on a declaration of a %FunctorT
     * type. For example, suppose you have an object that declares a function
     * returning `void` with an `int` parameter. You declare the functor type
     * and call this function passing the pointer to member function as
     * template parameter.
     ~~~~~~~~~~~~~~~~~~~~~{.cpp}
     * class SomeObject
     * {
     *     void fun(int data, bool other);
     * };
     *
     * SomeObject object;
     * ss::FunctorT<void(int, bool)> myFunctor;
     * myFunctor.bind<SomeObject, SomeObject::fun>(&object);
     ~~~~~~~~~~~~~~~~~~~~~
     * @since 1.0
     **/
    template <class _Host_t, _Return_t (_Host_t::*_Method)(_Param1_t, _Param2_t) const>
    void bind(_Host_t const *host) {
        m_host = (void *)const_cast<_Host_t *>(host);
        m_call = &const_invoker<_Host_t, _Method>;
    }
    /*}}}*/
    // _Return_t exec(_Param1_t param1, _Param2_t param2) const { }/*{{{*/
    /**
     * Calls the pointer to member function bound to this object.
     * @param param1 First paramter to pass to the function.
     * @param param2 Second parameter to pass to the function.
     * @return The value returned by the called function.
     * @note If no function was bound to this object the behavior is
     * undefined.
     * @since 1.0
     **/
    _Return_t exec(_Param1_t param1, _Param2_t param2) const {
        return (*m_call)(m_host, param1, param2);
    }
    /*}}}*/
    //@}

    /** @name Overloaded Operators */ //@{
    // _Return_t operator ()(_Param1_t param1, _Param2_t param2) const { }/*{{{*/
    /**
     * Call the member function pointed to this object.
     * @param param1 First parameter to pass to the function.
     * @param param2 Second parameter to pass to the function.
     * @return The value returned by the called function.
     * @note If no function was bound to this object the behavior is
     * undefined.
     * @since 1.0
     **/
    _Return_t operator ()(_Param1_t param1, _Param2_t param2) const {
        return exec(param1, param2);
    }
    /*}}}*/
    // operator bool() const { }/*{{{*/
    /**
     * Casting to \b bool operator.
     * Checks the validity of this functor object.
     * @return \b true when the object points to a function and its host. \b
     * false otherwise.
     * @since 1.0
     **/
    operator bool() const {
        return (m_host && m_call);
    }
    /*}}}*/
    // bool operator !() const { }/*{{{*/
    /**
     * Negation operator.
     * Check if this object is invalid. Invalid objects have not been bound to
     * a function or object.
     * @returns \b true when this object is invalid. \b false otherwise.
     * @since 1.0
     **/
    bool operator !() const {
        return (m_host == NULL || m_call == NULL);
    }
    /*}}}*/
    // bool operator ==(const FunctorT<_Return_t (_Param1_t, _Param2_t)> &other) const;/*{{{*/
    /**
     * Comparison operator.
     * @param other Another instance to compare with.
     * @return \b true if both instances points to the same host and function.
     * \b false otherwise.
     * @since 1.0
     **/
    bool operator ==(const FunctorT<_Return_t (_Param1_t, _Param2_t)> &other) const {
        return ((m_host == other.m_host) && (m_call == other.m_call));
    }
    /*}}}*/
    //@}

private:
    /** Type of the invoker function. */
    typedef _Return_t (*invoker_t)(void*, _Param1_t, _Param2_t);

    /** @name Constructors */ //@{
    // FunctorT(void *host, invoker_t caller) { }/*{{{*/
    /**
     * Parametrized constructor.
     * Builds an instance defining the host object and the invoker function
     * used to call the pointer to member of host.
     * @param host The host object to call.
     * @param caller Pointer to the static function that does call the member
     * function of \a host.
     * @since 1.0
     **/
    FunctorT(void *host, invoker_t caller) : m_host(host), m_call(caller) { }
    /*}}}*/
    //@}

    // Static Functions
    // static _Return_t invoker(void *host, _Param_t param) { }/*{{{*/
    /**
     * Calls the function pointed by the object instance.
     * @tparam _Host_t The type of the host object.
     * @tparam _Member Pointer to member function.
     * @param host The address of the host object.
     * @param param1 First parameter to pass to the function.
     * @param param2 Second parameter to pass to the function.
     * @return The same result as the function to be called.
     * @since 1.0
     **/
    template <class _Host_t, _Return_t (_Host_t::*_Member)(_Param1_t, _Param2_t)>
    static _Return_t invoker(void *host, _Param1_t param1, _Param2_t param2) {
        _Host_t *p = static_cast<_Host_t *>(host);
        return (p->*_Member)(param1, param2);
    }
    /*}}}*/
    // static _Return_t const_invoker(void *host, _Param_t param) { }/*{{{*/
    /**
     * Calls the function pointed by the object instance.
     * This implementation is used to support const cv-qualified objects.
     * @tparam _Host_t The type of the host object.
     * @tparam _Member Pointer to member function.
     * @param host The address of the host object.
     * @param param1 First value to pass to the function.
     * @param param2 Second value to pass to the function.
     * @return The same result as the function to be called.
     * @since 1.0
     **/
    template <class _Host_t, _Return_t (_Host_t::*_Member)(_Param1_t, _Param2_t) const>
    static _Return_t const_invoker(void *host, _Param1_t param1, _Param2_t param2) {
        _Host_t const *p = static_cast<_Host_t *>(host);
        return (p->*_Member)(param1, param2);
    }
    /*}}}*/

    // Data Members
    void *m_host;               /**< Pointer to the host object instance. */
    invoker_t m_call;           /**< Pointer to the invoker function.     */
};

/**
 * Functor implementation.
 * This specialization is selected when bind functions that have a return
 * value and a parameter. The return value can be `void`.
 * @tparam _Return_t Type of return value of the function to be called.
 * @tparam _Param_t Type of the single parameter for the function to be
 * called.
 * @since 1.7
 * @ingroup sstl_functors
 *//* --------------------------------------------------------------------- */
template <typename _Return_t, typename _Param_t>
class FunctorT<_Return_t (_Param_t)>
{
public:
    typedef _Return_t (*signature)(_Param_t);   /**< Identifies the signature of this functor. */

    /** @name Constructors & Destructors */ //@{
    // FunctorT() { }/*{{{*/
    /**
     * Default constructor.
     * Builds an empty functor object.
     * @since 1.0
     **/
    FunctorT() : m_host(NULL), m_call(NULL) { }
    /*}}}*/
    //@}

    // Static Functions
    // static FunctorT from(_Host_t *host) { }/*{{{*/
    /**
     * Creates an ss::FunctorT instance bound to a pointer to member function.
     * @tparam _Host_t Type of the host object. The object that declares the
     * function to be called.
     * @tparam _Method Pointer to the member function to be called.
     * @param host Pointer to the instance of the object where the pointer to
     * member function should be invoked.
     * @return A new instance of an `ss::FunctorT` object.
     * @remarks This function must be called on a declaration of a %FunctorT
     * type. For example, suppose you have an object that declares a function
     * returning `void` with an `int` parameter. You declare the functor type
     * and call this function passing the pointer to member function as
     * template parameter.
     ~~~~~~~~~~~~~~~~~~~~~{.cpp}
     * class SomeObject
     * {
     *     void fun(int data);
     * };
     *
     * SomeObject object;
     * ss::FunctorT<void(int)> myFunctor;
     * myFunctor = ss::FunctorT<void(int)>::from<SomeObject, SomeObject::fun>(&object);
     ~~~~~~~~~~~~~~~~~~~~~
     * @since 1.0
     **/
    template <class _Host_t, _Return_t (_Host_t::*_Method)(_Param_t)>
    static FunctorT from(_Host_t *host) {
        return FunctorT(host, &invoker<_Host_t, _Method>);
    }
    /*}}}*/
    // static FunctorT from(_Host_t const *host) { }/*{{{*/
    /**
     * Overload used on a const pointer to member function.
     * @tparam _Host_t Type of the host object. The object that declares the
     * function to be called.
     * @tparam _Method Pointer to the member function to be called.
     * @param host Pointer to the instance of the object where the pointer to
     * member function should be invoked.
     * @return A new instance of an `ss::FunctorT` object.
     * @remarks This function must be called on a declaration of a %FunctorT
     * type. For example, suppose you have an object that declares a function
     * returning `void` with an `int` parameter. You declare the functor type
     * and call this function passing the pointer to member function as
     * template parameter.
     ~~~~~~~~~~~~~~~~~~~~~{.cpp}
     * class SomeObject
     * {
     *     void fun(int data) const;
     * };
     *
     * SomeObject object;
     * ss::FunctorT<void(int)> myFunctor;
     * myFunctor = ss::FunctorT<void(int)>::from<SomeObject, SomeObject::fun>(&object);
     ~~~~~~~~~~~~~~~~~~~~~
     * @since 1.0
     **/
    template <class _Host_t, _Return_t (_Host_t::*_Method)(_Param_t) const>
    static FunctorT from(_Host_t const *host) {
        return FunctorT(const_cast<_Host_t*>(host), &const_invoker<_Host_t, _Method>);
    }
    /*}}}*/

    /** @name Attributes */ //@{
    // bool isHost(void *ptr) const { }/*{{{*/
    /**
     * Checks whether this object instance points to the specified host.
     * @param ptr Pointer to the instance of the host object.
     * @return \b true if the specified address is the same as the host
     * pointed by this object. \b false otherwise.
     * @since 1.0
     **/
    bool isHost(void *ptr) const {
        return (m_host == ptr);
    }
    /*}}}*/
    // bool valid() const { }/*{{{*/
    /**
     * Checks whether this object is valid.
     * @returns \b true if this object was bound to a host class and member
     * function. \b false otherwise.
     * @note This function doesn't valid the target object and member
     * function. Only checks whether this functor was bound. The host object
     * must be valid until this functor is destroyed.
     * @since 1.0
     **/
    bool valid() const {
        return ((m_host != NULL) && (m_call != NULL));
    }
    /*}}}*/
    //@}

    /** @name Operations */ //@{
    // void bind(_Host_t *host) { }/*{{{*/
    /**
     * Binds a functor with a pointer to member function.
     * @tparam _Host_t Type of the host object. The object that declares the
     * function to be called.
     * @tparam _Method Pointer to the member function to be called.
     * @param host Pointer to the instance of the object where the pointer to
     * member function should be invoked.
     * @remarks This function must be called on a declaration of a %FunctorT
     * type. For example, suppose you have an object that declares a function
     * returning `void` with an `int` parameter. You declare the functor type
     * and call this function passing the pointer to member function as
     * template parameter.
     ~~~~~~~~~~~~~~~~~~~~~{.cpp}
     * class SomeObject
     * {
     *     void fun(int data);
     * };
     *
     * SomeObject object;
     * ss::FunctorT<void(int)> myFunctor;
     * myFunctor.bind<SomeObject, SomeObject::fun>(&object);
     ~~~~~~~~~~~~~~~~~~~~~
     * @since 1.0
     **/
    template <class _Host_t, _Return_t (_Host_t::*_Method)(_Param_t)>
    void bind(_Host_t *host) {
        m_host = (void *)host;
        m_call = &invoker<_Host_t, _Method>;
    }
    /*}}}*/
    // void bind(_Host_t const *host) { }/*{{{*/
    /**
     * Overload used on a const pointer to member function.
     * @tparam _Host_t Type of the host object. The object that declares the
     * function to be called.
     * @tparam _Method Pointer to the member function to be called.
     * @param host Pointer to the instance of the object where the pointer to
     * member function should be invoked.
     * @remarks This function must be called on a declaration of a %FunctorT
     * type. For example, suppose you have an object that declares a function
     * returning `void` with an `int` parameter. You declare the functor type
     * and call this function passing the pointer to member function as
     * template parameter.
     ~~~~~~~~~~~~~~~~~~~~~{.cpp}
     * class SomeObject
     * {
     *     void fun(int data);
     * };
     *
     * SomeObject object;
     * ss::FunctorT<void(int)> myFunctor;
     * myFunctor.bind<SomeObject, SomeObject::fun>(&object);
     ~~~~~~~~~~~~~~~~~~~~~
     * @since 1.0
     **/
    template <class _Host_t, _Return_t (_Host_t::*_Method)(_Param_t) const>
    void bind(_Host_t const *host) {
        m_host = (void *)const_cast<_Host_t *>(host);
        m_call = &const_invoker<_Host_t, _Method>;
    }
    /*}}}*/
    // _Return_t exec(_Param_t param) const { }/*{{{*/
    /**
     * Calls the pointer to member function bound to this object.
     * @param param The parameter to be passed to the function.
     * @return The value returned by the called function.
     * @note If no function was bound to this object the behavior is
     * undefined.
     * @since 1.0
     **/
    _Return_t exec(_Param_t param) const {
        return (*m_call)(m_host, param);
    }
    /*}}}*/
    //@}

    /** @name Overloaded Operators */ //@{
    // _Return_t operator ()(_Param_t param) const { }/*{{{*/
    /**
     * Call the member function pointed to this object.
     * @param param The parameter to be passed to the function.
     * @return The value returned by the called function.
     * @note If no function was bound to this object the behavior is
     * undefined.
     * @since 1.0
     **/
    _Return_t operator ()(_Param_t param) const {
        return exec(param);
    }
    /*}}}*/
    // operator bool() const { }/*{{{*/
    /**
     * Casting to \b bool operator.
     * Checks the validity of this functor object.
     * @return \b true when the object points to a function and its host. \b
     * false otherwise.
     * @since 1.0
     **/
    operator bool() const {
        return (m_host && m_call);
    }
    /*}}}*/
    // bool operator !() const { }/*{{{*/
    /**
     * Negation operator.
     * Check if this object is invalid. Invalid objects have not been bound to
     * a function or object.
     * @returns \b true when this object is invalid. \b false otherwise.
     * @since 1.0
     **/
    bool operator !() const {
        return (m_host == NULL || m_call == NULL);
    }
    /*}}}*/
    // bool operator ==(const FunctorT<_Return_t (_Param_t)> &other) const;/*{{{*/
    /**
     * Comparison operator.
     * @param other Another instance to compare with.
     * @return \b true if both instances points to the same host and function.
     * \b false otherwise.
     * @since 1.0
     **/
    bool operator ==(const FunctorT<_Return_t (_Param_t)> &other) const {
        return ((m_host == other.m_host) && (m_call == other.m_call));
    }
    /*}}}*/
    //@}

private:
    /** Type of the invoker function. */
    typedef _Return_t (*invoker_t)(void*, _Param_t);

    /** @name Constructors */ //@{
    // FunctorT(void *host, invoker_t caller) { }/*{{{*/
    /**
     * Parametrized constructor.
     * Builds an instance defining the host object and the invoker function
     * used to call the pointer to member of host.
     * @param host The host object to call.
     * @param caller Pointer to the static function that does call the member
     * function of \a host.
     * @since 1.0
     **/
    FunctorT(void *host, invoker_t caller) : m_host(host), m_call(caller) { }
    /*}}}*/
    //@}

    // Static Functions
    // static _Return_t invoker(void *host, _Param_t param) { }/*{{{*/
    /**
     * Calls the function pointed by the object instance.
     * @tparam _Host_t The type of the host object.
     * @tparam _Member Pointer to member function.
     * @param host The address of the host object.
     * @param param Data to be passed as parameter to the pointer to member
     * function.
     * @return The same result as the function to be called.
     * @since 1.0
     **/
    template <class _Host_t, _Return_t (_Host_t::*_Member)(_Param_t)>
    static _Return_t invoker(void *host, _Param_t param) {
        _Host_t *p = static_cast<_Host_t *>(host);
        return (p->*_Member)(param);
    }
    /*}}}*/
    // static _Return_t const_invoker(void *host, _Param_t param) { }/*{{{*/
    /**
     * Calls the function pointed by the object instance.
     * This implementation is used to support const cv-qualified objects.
     * @tparam _Host_t The type of the host object.
     * @tparam _Member Pointer to member function.
     * @param host The address of the host object.
     * @param param Data to be passed as parameter to the pointer to member
     * function.
     * @return The same result as the function to be called.
     * @since 1.0
     **/
    template <class _Host_t, _Return_t (_Host_t::*_Member)(_Param_t) const>
    static _Return_t const_invoker(void *host, _Param_t param) {
        _Host_t const *p = static_cast<_Host_t *>(host);
        return (p->*_Member)(param);
    }
    /*}}}*/

    // Data Members
    void *m_host;               /**< Pointer to the host object instance. */
    invoker_t m_call;           /**< Pointer to the invoker function.     */
};

/**
 * Another Functor implementation.
 * This specialization is selected when bind functions with no parameters.
 * @tparam _Return_t Type of return value of the function to be called. Can be
 * `void`.
 * @since 1.0
 * @ingroup sstl_functors
 *//* --------------------------------------------------------------------- */
template <typename _Return_t>
class FunctorT<_Return_t ()>
{
public:
    typedef _Return_t (*signature)();   /**< This functor function signature. */

    /** @name Constructors & Destructor */ //@{
    // FunctorT() { }/*{{{*/
    /**
     * Default constructor.
     * Builds an empty functor object.
     * @since 1.0
     **/
    FunctorT() : m_host(NULL), m_call(NULL) { }
    /*}}}*/
    //@}

    // Static Functions
    // static FunctorT from(_Host_t *host) { }/*{{{*/
    /**
     * Creates an ss::FunctorT instance bound to a pointer to member function.
     * @tparam _Host_t Type of the host object. The object that declares the
     * function to be called.
     * @tparam _Method Pointer to the member function to be called.
     * @param host Pointer to the instance of the object where the pointer to
     * member function should be invoked.
     * @return A new instance of an `ss::FunctorT` object.
     * @remarks This function must be called on a declaration of a %FunctorT
     * type. For example, suppose you have an object that declares a function
     * returning `void` with an `int` parameter. You declare the functor type
     * and call this function passing the pointer to member function as
     * template parameter.
     ~~~~~~~~~~~~~~~~~~~~~{.cpp}
     * class SomeObject
     * {
     *     void fun();
     * };
     *
     * SomeObject object;
     * ss::FunctorT<void()> myFunctor;
     * myFunctor = ss::FunctorT<void()>::from<SomeObject, SomeObject::fun>(&object);
     ~~~~~~~~~~~~~~~~~~~~~
     * @since 1.0
     **/
    template <class _Host_t, _Return_t (_Host_t::*_Method)()>
    static FunctorT from(_Host_t *host) {
        return FunctorT(host, &invoker<_Host_t, _Method>);
    }
    /*}}}*/
    // static FunctorT from(_Host_t const *host) { }/*{{{*/
    /**
     * Overload used on a const pointer to member function.
     * @tparam _Host_t Type of the host object. The object that declares the
     * function to be called.
     * @tparam _Method Pointer to the member function to be called.
     * @param host Pointer to the instance of the object where the pointer to
     * member function should be invoked.
     * @return A new instance of an `ss::FunctorT` object.
     * @remarks This function must be called on a declaration of a %FunctorT
     * type. For example, suppose you have an object that declares a function
     * returning `void` with an `int` parameter. You declare the functor type
     * and call this function passing the pointer to member function as
     * template parameter.
     ~~~~~~~~~~~~~~~~~~~~~{.cpp}
     * class SomeObject
     * {
     *     void fun() const;
     * };
     *
     * SomeObject object;
     * ss::FunctorT<void()> myFunctor;
     * myFunctor = ss::FunctorT<void()>::from<SomeObject, SomeObject::fun>(&object);
     ~~~~~~~~~~~~~~~~~~~~~
     * @since 1.0
     **/
    template <class _Host_t, _Return_t (_Host_t::*_Method)() const>
    static FunctorT from(_Host_t const *host) {
        return FunctorT(const_cast<_Host_t*>(host), &const_invoker<_Host_t, _Method>);
    }
    /*}}}*/

    /** @name Attributes */ //@{
    // bool isHost(void *ptr) const { }/*{{{*/
    /**
     * Checks whether this object instance points to the specified host.
     * @param ptr Pointer to the instance of the host object.
     * @return \b true if the specified address is the same as the host
     * pointed by this object. \b false otherwise.
     * @since 1.0
     **/
    bool isHost(void *ptr) const {
        return (m_host == ptr);
    }
    /*}}}*/
    // bool valid() const { }/*{{{*/
    /**
     * Checks whether this object is valid.
     * @returns \b true if this object was bound to a host class and member
     * function. \b false otherwise.
     * @note This function doesn't valid the target object and member
     * function. Only checks whether this functor was bound. The host object
     * must be valid until this functor is destroyed.
     * @since 1.0
     **/
    bool valid() const {
        return ((m_host != NULL) && (m_call != NULL));
    }
    /*}}}*/
    //@}

    /** @name Operations */ //@{
    // void bind(_Host_t *host) { }/*{{{*/
    /**
     * Binds a functor with a pointer to member function.
     * @tparam _Host_t Type of the host object. The object that declares the
     * function to be called.
     * @tparam _Method Pointer to the member function to be called.
     * @param host Pointer to the instance of the object where the pointer to
     * member function should be invoked.
     * @remarks This function must be called on a declaration of a %FunctorT
     * type. For example, suppose you have an object that declares a function
     * returning `void` with an `int` parameter. You declare the functor type
     * and call this function passing the pointer to member function as
     * template parameter.
     ~~~~~~~~~~~~~~~~~~~~~{.cpp}
     * class SomeObject
     * {
     *     void fun();
     * };
     *
     * SomeObject object;
     * ss::FunctorT<void()> myFunctor;
     * myFunctor.bind<SomeObject, SomeObject::fun>(&object);
     ~~~~~~~~~~~~~~~~~~~~~
     * @since 1.0
     **/
    template <class _Host_t, _Return_t (_Host_t::*_Method)()>
    void bind(_Host_t *host) {
        m_host = (void *)host;
        m_call = &invoker<_Host_t, _Method>;
    }
    /*}}}*/
    // void bind(_Host_t const *host) { }/*{{{*/
    /**
     * Overload used on a const pointer to member function.
     * @tparam _Host_t Type of the host object. The object that declares the
     * function to be called.
     * @tparam _Method Pointer to the member function to be called.
     * @param host Pointer to the instance of the object where the pointer to
     * member function should be invoked.
     * @remarks This function must be called on a declaration of a %FunctorT
     * type. For example, suppose you have an object that declares a function
     * returning `void` with an `int` parameter. You declare the functor type
     * and call this function passing the pointer to member function as
     * template parameter.
     ~~~~~~~~~~~~~~~~~~~~~{.cpp}
     * class SomeObject
     * {
     *     void fun();
     * };
     *
     * SomeObject object;
     * ss::FunctorT<void()> myFunctor;
     * myFunctor.bind<SomeObject, SomeObject::fun>(&object);
     ~~~~~~~~~~~~~~~~~~~~~
     * @since 1.0
     **/
    template <class _Host_t, _Return_t (_Host_t::*_Method)() const>
    void bind(_Host_t const *host) {
        m_host = (void *)const_cast<_Host_t *>(host);
        m_call = &const_invoker<_Host_t, _Method>;
    }
    /*}}}*/
    // _Return_t exec() const { }/*{{{*/
    /**
     * Calls the pointer to member function bound to this object.
     * @return The value returned by the called function.
     * @note If no function was bound to this object the behavior is
     * undefined.
     * @since 1.0
     **/
    _Return_t exec() const {
        return (*m_call)(m_host);
    }
    /*}}}*/
    //@}

    /** @name Overloaded Operators */ //@{
    // _Return_t operator ()() const { }/*{{{*/
    /**
     * Call the member function pointed to this object.
     * @return The value returned by the called function.
     * @note If no function was bound to this object the behavior is
     * undefined.
     * @since 1.0
     **/
    _Return_t operator ()() const {
        return exec();
    }
    /*}}}*/
    // operator bool() const { }/*{{{*/
    /**
     * Casting to \b bool operator.
     * Checks the validity of this functor object.
     * @return \b true when the object points to a function and its host. \b
     * false otherwise.
     * @since 1.0
     **/
    operator bool() const {
        return (m_host && m_call);
    }
    /*}}}*/
    // bool operator !() const { }/*{{{*/
    /**
     * Negation operator.
     * Check if this object is invalid. Invalid objects have not been bound to
     * a function or object.
     * @returns \b true when this object is invalid. \b false otherwise.
     * @since 1.0
     **/
    bool operator !() const {
        return (m_host == NULL || m_call == NULL);
    }
    /*}}}*/
    // bool operator ==(const FunctorT<_Return_t ()> &other) const;/*{{{*/
    /**
     * Comparison operator.
     * @param other Another instance to compare with.
     * @return \b true if both instances points to the same host and function.
     * \b false otherwise.
     * @since 1.0
     **/
    bool operator ==(const FunctorT<_Return_t ()> &other) const {
        return ((m_host == other.m_host) && (m_call == other.m_call));
    }
    /*}}}*/
    //@}

private:
    /** Type of the invoker function. */
    typedef _Return_t (*invoker_t)(void*);

    /** @name Constructors */ //@{
    // FunctorT(void *host, invoker_t caller) { }/*{{{*/
    /**
     * Parametrized constructor.
     * Builds an instance defining the host object and the invoker function
     * used to call the pointer to member of host.
     * @param host The host object to call.
     * @param caller Pointer to the static function that does call the member
     * function of \a host.
     * @since 1.0
     **/
    FunctorT(void *host, invoker_t caller) : m_host(host), m_call(caller) { }
    /*}}}*/
    //@}

    // Static Functions
    // static _Return_t invoker(void *host) { }/*{{{*/
    /**
     * Calls the function pointed by the object instance.
     * @tparam _Host_t The type of the host object.
     * @tparam _Member Pointer to member function.
     * @param host The address of the host object.
     * @return The same result as the function to be called.
     * @since 1.0
     **/
    template <class _Host_t, _Return_t (_Host_t::*_Member)()>
    static _Return_t invoker(void *host) {
        _Host_t *p = static_cast<_Host_t *>(host);
        return (p->*_Member)();
    }
    /*}}}*/
    // static _Return_t const_invoker(void *host) { }/*{{{*/
    /**
     * Calls the function pointed by the object instance.
     * This implementation is used to support const cv-qualified objects.
     * @tparam _Host_t The type of the host object.
     * @tparam _Member Pointer to member function.
     * @param host The address of the host object.
     * @return The same result as the function to be called.
     * @since 1.0
     **/
    template <class _Host_t, _Return_t (_Host_t::*_Member)() const>
    static _Return_t const_invoker(void *host) {
        _Host_t const *p = static_cast<_Host_t *>(host);
        return (p->*_Member)();
    }
    /*}}}*/

    // Data Members
    void *m_host;               /**< Pointer to the host object instance. */
    invoker_t m_call;           /**< Pointer to the invoker function.     */
};

}   /* namespace ss */

#endif /* __SSTLFUNC_HPP_DEFINED__ */

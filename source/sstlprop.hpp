/**
 * @file
 * Property like class templates.
 *
 * @author Alessandro Antonello
 * @date   jan 08, 2016
 * @since  Super Simple Template Library 1.0
 *
 * @copyright 2016, Paralaxe Tecnologia Ltda.. All rights reserved.
 **/
#ifndef __SSTLPROP_HPP_DEFINED__
#define __SSTLPROP_HPP_DEFINED__

namespace ss {
/**
 * A property like class template implementation.
 * This object is able to mimic property like member variables in a class
 * definition. The class doesn't hold the data of the property. Instead you
 * must write getter and setter functions and \e bind them to the property
 * object instance. This object has several overloaded operators and is
 * intended to be used with scalar values.
 * @tparam _Value_t The type of the property value.
 * @since 1.0
 * @ingroup sstl_properties
 *//* --------------------------------------------------------------------- */
template <typename _Value_t>
class PropertyT
{
public:
    /** @name Constructor */ //@{
    // PropertyT() { }/*{{{*/
    /**
     * Default constructor.
     * @since 1.0
     **/
    PropertyT() : m_host(NULL), m_get(NULL), m_set(NULL) { }
    /*}}}*/
    //@}

    /** @name Operations */ //@{
    // void bind(_Host_t *host) { }/*{{{*/
    /**
     * Binds the property with the member functions in the host object.
     * @tparam _Host_t Type of the host object.
     * @tparam _Getter Pointer to the getter function.
     * @tparam _Setter Pointer to the setter function.
     * @param host Pointer to the instance of the host object.
     * @since 1.0
     **/
    template <class _Host_t, _Value_t (_Host_t::*_Getter)(), void (_Host_t::*_Setter)(_Value_t)>
    void bind(_Host_t *host) {
        m_host = (void *)host;
        m_get  = &getter<_Host_t, _Getter>;
        m_set  = &setter<_Host_t, _Setter>;
    }
    /*}}}*/
    // void bind(_Host_t const *host) { }/*{{{*/
    /**
     * Binds the property with the member functions in the host object.
     * This overloaded version is selected when the getter function is const.
     * @tparam _Host_t Type of the host object.
     * @tparam _Getter Pointer to the getter function.
     * @tparam _Setter Pointer to the setter function.
     * @param host Pointer to the instance of the host object.
     * @since 1.0
     **/
    template <class _Host_t, _Value_t (_Host_t::*_Getter)() const, void (_Host_t::*_Setter)(_Value_t)>
    void bind(_Host_t const *host) {
        m_host = (void *)const_cast<_Host_t*>(host);
        m_get  = &constget<_Host_t, _Getter>;
        m_set  = &setter<_Host_t, _Setter>;
    }
    /*}}}*/
    // void bind(_Host_t const *host) { }/*{{{*/
    /**
     * Binds the property with the member functions in the host object.
     * This overloaded version is selected when the getter function is const.
     * @tparam _Host_t Type of the host object.
     * @tparam _Getter Pointer to the getter function.
     * @tparam _Setter Pointer to the setter function.
     * @param host Pointer to the instance of the host object.
     * @since 1.0
     **/
    template <class _Host_t, _Value_t (_Host_t::*_Getter)() const, _Host_t& (_Host_t::*_Setter)(_Value_t)>
    void bind(_Host_t const *host) {
        m_host = (void *)const_cast<_Host_t*>(host);
        m_get  = &constget<_Host_t, _Getter>;
        m_set  = &setret<_Host_t, _Setter>;
    }
    /*}}}*/
    // _Value_t get() const;/*{{{*/
    /**
     * Executes the getter function.
     * @return The value returned by the getter function.
     * @since 1.0
     **/
    _Value_t get() const { return (*m_get)(m_host); }
    /*}}}*/
    // void set(_Value_t value);/*{{{*/
    /**
     * Executes the setter function.
     * @param value The value to be set.
     * @since 1.0
     **/
    void set(_Value_t value) { (*m_set)(m_host, value); }
    /*}}}*/
    //@}

    /** @name Overloaded Operators */ //@{
    // operator _Value_t() const;/*{{{*/
    /**
     * Cast to type operator.
     * This operator calls #get().
     * @returns The value returned by the `%get()` function.
     * @since 1.0
     **/
    operator _Value_t() const { return get(); }
    /*}}}*/
    // _Value_t operator ()() const;/*{{{*/
    /**
     * Functor operator.
     * Calls the #get() function. This operator can be used in places where
     * the compiler cannot deduce to use the `operator _Value_t()`.
     * @return The value returned by `%get()`.
     * @since 1.0
     **/
    _Value_t operator ()() const { return get(); }
    /*}}}*/
    // _Value_t operator =(_Value_t value);/*{{{*/
    /**
     * Assignment operator.
     * This operator calls #set().
     * @param value The value to be set.
     * @since 1.0
     **/
    _Value_t operator =(_Value_t value) {
        set(value); return get();
    }
    /*}}}*/
    // _Value_t operator =(const PropertyT<_Value_t> &value);/*{{{*/
    /**
     * Assignment operator overloading.
     * @param value Another property to get its value.
     * @since 1.0
     **/
    _Value_t operator =(const PropertyT<_Value_t> &value) {
        set(value.get()); return get();
    }
    /*}}}*/
    // _Value_t operator =(const PropertyT<T> &value);/*{{{*/
    /**
     * Assignment operator.
     * Copies the value of another property to this one.
     * @tparam T Type of the \a value property. This doesn't need to be
     * defined. The compiler will deduce it.
     * @param value Another property object reference. Can be of another type.
     * But the type must be implicity convertible to the type of this object.
     * @since 1.0
     **/
    template <typename T>
    _Value_t operator =(const PropertyT<T> &value) {
        set(value.get()); return get();
    }
    /*}}}*/
    //@}

    /** @name Comparison Operators */ //@{
    // bool operator ! () const;/*{{{*/
    /**
     * Negation operator.
     * @returns \b true when the value of this property can be convertibal to
     * a \b falsy result. \b false otherwise.
     * @since 1.0
     **/
    bool operator ! () const {
        return !bool(get());
    }
    /*}}}*/
    // bool operator ==(_Type_t value) const;/*{{{*/
    /**
     * Equality operator.
     * Compares the value of the property pointed by this object with the
     * passed value.
     * @tparam _Type_t Data type of the value to be compared. This doesn't
     * need to be informed. The compiler can deduce the type by the passed
     * value.
     * @param value The value to compare to. Must be implicitly convertible to
     * the type of this property object.
     * @return \b true if the passed \a value and the value pointed by this
     * property are equals. \b false otherwise.
     * @since 1.0
     **/
    template <typename _Type_t>
    bool operator ==(_Type_t value) const {
        return (get() == value);
    }
    /*}}}*/
    // bool operator !=(_Type_t value) const;/*{{{*/
    /**
     * Inequality operator.
     * Compares the value of the property pointed by this object with the
     * passed value.
     * @param value The value to compare to.
     * @return \b true if the passed \a value and the value pointed by this
     * property are different. \b false otherwise.
     * @since 1.0
     **/
    template <typename _Type_t>
    bool operator !=(_Type_t value) const {
        return (get() != value);
    }
    /*}}}*/
    // bool operator < (_Type_t value) const;/*{{{*/
    /**
     * Less than operator.
     * Compares the value of the property pointed by this object with the
     * passed value.
     * @param value The value to compare to.
     * @return \b true when the value returned by this property object is
     * \it{less than} the passed \a value. \b false otherwise.
     * @since 1.0
     **/
    template <typename _Type_t>
    bool operator < (_Type_t value) const {
        return (get() < value);
    }
    /*}}}*/
    // bool operator <=(_Type_t value) const;/*{{{*/
    /**
     * Less than or equal operator.
     * Compares the value of the property pointed by this object with the
     * passed value.
     * @param value The value to compare to.
     * @return \b true when the value returned by this property object is
     * \it{less than or equal} the passed \a value. \b false otherwise.
     * @since 1.0
     **/
    template <typename _Type_t>
    bool operator <=(_Type_t value) const {
        return (get() <= value);
    }
    /*}}}*/
    // bool operator > (_Type_t value) const;/*{{{*/
    /**
     * Greater than operator.
     * Compares the value of the property pointed by this object with the
     * passed value.
     * @param value The value to compare to.
     * @return \b true when the value returned by this property object is
     * \it{greater than} the passed \a value. \b false otherwise.
     * @since 1.0
     **/
    template <typename _Type_t>
    bool operator > (_Type_t value) const {
        return (get() > value);
    }
    /*}}}*/
    // bool operator >=(_Type_t value) const;/*{{{*/
    /**
     * Greater than or equal operator.
     * Compares the value of the property pointed by this object with the
     * passed value.
     * @param value The value to compare to.
     * @return \b true when the value returned by this property object is
     * \it{greater than or equal} the passed \a value. \b false otherwise.
     * @since 1.0
     **/
    template <typename _Type_t>
    bool operator >=(_Type_t value) const {
        return (get() >= value);
    }
    /*}}}*/
    // bool operator ==(const PropertyT<_Type_t> &value) const;/*{{{*/
    /**
     * Equality operator.
     * Compares the value of the property pointed by this object with the
     * value pointed by another property.
     * @tparam _Type_t Data type of the property to be compared. This doesn't
     * need to be informed. The compiler can deduce the type by the passed
     * argument.
     * @param value The property to compare to. Must be implicitly convertible
     * to the type of this property object.
     * @return \b true if the passed \a value and the value pointed by this
     * property are equals. \b false otherwise.
     * @since 1.0
     **/
    template <typename _Type_t>
    bool operator ==(const PropertyT<_Type_t> &value) const {
        return (get() == value.get());
    }
    /*}}}*/
    // bool operator !=(const PropertyT<_Type_t> &value) const;/*{{{*/
    /**
     * Inequality operator.
     * Compares the value of the property pointed by this object with the
     * value pointed by another property object.
     * @param value The value to compare to.
     * @return \b true if the passed \a value and the value pointed by this
     * property are different. \b false otherwise.
     * @since 1.0
     **/
    template <typename _Type_t>
    bool operator !=(const PropertyT<_Type_t> &value) const {
        return (get() != value.get());
    }
    /*}}}*/
    // bool operator < (const PropertyT<_Type_t> &value) const;/*{{{*/
    /**
     * Less than operator.
     * Compares the value of the property pointed by this object with the
     * value of another property object.
     * @param value The property to compare to.
     * @return \b true when the value returned by this property object is
     * \it{less than} the passed \a value. \b false otherwise.
     * @since 1.0
     **/
    template <typename _Type_t>
    bool operator < (const PropertyT<_Type_t> &value) const {
        return (get() < value.get());
    }
    /*}}}*/
    // bool operator <=(const PropertyT<_Type_t> &value) const;/*{{{*/
    /**
     * Less than or equal operator.
     * Compares the value of the property pointed by this object with the
     * value of another property object.
     * @param value The property to compare to.
     * @return \b true when the value returned by this property object is
     * \it{less than or equal} the passed \a value. \b false otherwise.
     * @since 1.0
     **/
    template <typename _Type_t>
    bool operator <=(const PropertyT<_Type_t> &value) const {
        return (get() <= value.get());
    }
    /*}}}*/
    // bool operator > (const PropertyT<_Type_t> &value) const;/*{{{*/
    /**
     * Greater than operator.
     * Compares the value of the property pointed by this object with the
     * value of another property object.
     * @param value The property to compare to.
     * @return \b true when the value returned by this property object is
     * \it{greater than} the passed \a value. \b false otherwise.
     * @since 1.0
     **/
    template <typename _Type_t>
    bool operator > (const PropertyT<_Type_t> &value) const {
        return (get() > value.get());
    }
    /*}}}*/
    // bool operator >=(const PropertyT<_Type_t> &value) const;/*{{{*/
    /**
     * Greater than or equal operator.
     * Compares the value of the property pointed by this object with the
     * value of another property object.
     * @param value The property to compare to.
     * @return \b true when the value returned by this property object is
     * \it{greater than or equal} the passed \a value. \b false otherwise.
     * @since 1.0
     **/
    template <typename _Type_t>
    bool operator >=(const PropertyT<_Type_t> &value) const {
        return (get() >= value.get());
    }
    /*}}}*/
    //@}

    /** @name Mathematical Operators */ //@{
    // _Value_t operator + (_Type_t value) const;/*{{{*/
    /**
     * Addition operator.
     * @tparam _Type_t Data type of the value to be compared. This doesn't
     * need to be informed. The compiler can deduce the type by the passed
     * value.
     * @param value Value to be added.
     * @return The value resulting of the mathematical operation.
     * @remarks This operator doesn't change the value of the property pointed
     * by this object neither the value of the \a value parameter. Just the
     * operation is done and the resulting value will be returned.
     * @since 1.0
     **/
    template <typename _Type_t>
    _Value_t operator + (_Type_t value) const {
        return (get() + value);
    }
    /*}}}*/
    // _Value_t operator - (_Type_t value) const;/*{{{*/
    /**
     * Subtraction operator.
     * @tparam _Type_t Data type of the value to be compared. This doesn't
     * need to be informed. The compiler can deduce the type by the passed
     * value.
     * @param value Value to be added.
     * @return The value resulting of the mathematical operation.
     * @remarks This operator doesn't change the value of the property pointed
     * by this object neither the value of the \a value parameter. Just the
     * operation is done and the resulting value will be returned.
     * @since 1.0
     **/
    template <typename _Type_t>
    _Value_t operator - (_Type_t value) const {
        return (get() - value);
    }
    /*}}}*/
    // _Value_t operator * (_Type_t value) const;/*{{{*/
    /**
     * Multiplication operator.
     * @tparam _Type_t Data type of the value to be compared. This doesn't
     * need to be informed. The compiler can deduce the type by the passed
     * value.
     * @param value Value to be added.
     * @return The value resulting of the mathematical operation.
     * @remarks This operator doesn't change the value of the property pointed
     * by this object neither the value of the \a value parameter. Just the
     * operation is done and the resulting value will be returned.
     * @since 1.0
     **/
    template <typename _Type_t>
    _Value_t operator * (_Type_t value) const {
        return (get() * value);
    }
    /*}}}*/
    // _Value_t operator / (_Type_t value) const;/*{{{*/
    /**
     * Division operator.
     * @tparam _Type_t Data type of the value to be compared. This doesn't
     * need to be informed. The compiler can deduce the type by the passed
     * value.
     * @param value Value to be added.
     * @return The value resulting of the mathematical operation.
     * @remarks This operator doesn't change the value of the property pointed
     * by this object neither the value of the \a value parameter. Just the
     * operation is done and the resulting value will be returned.
     * @since 1.0
     **/
    template <typename _Type_t>
    _Value_t operator / (_Type_t value) const {
        return (get() / value);
    }
    /*}}}*/
    // _Value_t operator +=(_Type_t value);/*{{{*/
    /**
     * Addition operator.
     * @tparam _Type_t Data type of the value to be compared. This doesn't
     * need to be informed. The compiler can deduce the type by the passed
     * value.
     * @param value Value to be added.
     * @return The value resulting of the mathematical operation.
     * @remarks This operator changes the value of the property pointed by
     * this object adding the \a value parameter.
     * @since 1.0
     **/
    template <typename _Type_t>
    _Value_t operator +=(_Type_t value) {
        set(get() + value);
        return get();
    }
    /*}}}*/
    // _Value_t operator -=(_Type_t value);/*{{{*/
    /**
     * Subtraction operator.
     * @tparam _Type_t Data type of the value to be compared. This doesn't
     * need to be informed. The compiler can deduce the type by the passed
     * value.
     * @param value Value to be added.
     * @return The value resulting of the mathematical operation.
     * @remarks This operator changes the value of the property pointed by
     * this object adding the \a value parameter.
     * @since 1.0
     **/
    template <typename _Type_t>
    _Value_t operator -=(_Type_t value) {
        set(get() - value);
        return get();
    }
    /*}}}*/
    // _Value_t operator *=(_Type_t value);/*{{{*/
    /**
     * Multiplication operator.
     * @tparam _Type_t Data type of the value to be compared. This doesn't
     * need to be informed. The compiler can deduce the type by the passed
     * value.
     * @param value Value to be added.
     * @return The value resulting of the mathematical operation.
     * @remarks This operator changes the value of the property pointed by
     * this object adding the \a value parameter.
     * @since 1.0
     **/
    template <typename _Type_t>
    _Value_t operator *=(_Type_t value) {
        set(get() * value);
        return get();
    }
    /*}}}*/
    // _Value_t operator /=(_Type_t value);/*{{{*/
    /**
     * Division operator.
     * @tparam _Type_t Data type of the value to be compared. This doesn't
     * need to be informed. The compiler can deduce the type by the passed
     * value.
     * @param value Value to be added.
     * @return The value resulting of the mathematical operation.
     * @remarks This operator changes the value of the property pointed by
     * this object adding the \a value parameter.
     * @since 1.0
     **/
    template <typename _Type_t>
    _Value_t operator /=(_Type_t value) {
        set(get() / value);
        return get();
    }
    /*}}}*/
    // _Value_t operator + (const PropertyT<_Type_t> &value) const;/*{{{*/
    /**
     * Addition operator.
     * @tparam _Type_t Data type of the value to be compared. This doesn't
     * need to be informed. The compiler can deduce the type by the passed
     * value.
     * @param value Another property object with the value to be added.
     * @return The value resulting of the mathematical operation.
     * @remarks This operator doesn't change the value of the property pointed
     * by this object neither the value of the \a value parameter. Just the
     * operation is done and the resulting value will be returned.
     * @since 1.0
     **/
    template <typename _Type_t>
    _Value_t operator + (const PropertyT<_Type_t> &value) const {
        return (get() + value.get());
    }
    /*}}}*/
    // _Value_t operator - (const PropertyT<_Type_t> &value) const;/*{{{*/
    /**
     * Subtraction operator.
     * @tparam _Type_t Data type of the value to be compared. This doesn't
     * need to be informed. The compiler can deduce the type by the passed
     * value.
     * @param value Another property object with the value to be added.
     * @return The value resulting of the mathematical operation.
     * @remarks This operator doesn't change the value of the property pointed
     * by this object neither the value of the \a value parameter. Just the
     * operation is done and the resulting value will be returned.
     * @since 1.0
     **/
    template <typename _Type_t>
    _Value_t operator - (const PropertyT<_Type_t> &value) const {
        return (get() - value.get());
    }
    /*}}}*/
    // _Value_t operator * (const PropertyT<_Type_t> &value) const;/*{{{*/
    /**
     * Multiplication operator.
     * @tparam _Type_t Data type of the value to be compared. This doesn't
     * need to be informed. The compiler can deduce the type by the passed
     * value.
     * @param value Another property object with the value to be added.
     * @return The value resulting of the mathematical operation.
     * @remarks This operator doesn't change the value of the property pointed
     * by this object neither the value of the \a value parameter. Just the
     * operation is done and the resulting value will be returned.
     * @since 1.0
     **/
    template <typename _Type_t>
    _Value_t operator * (const PropertyT<_Type_t> &value) const {
        return (get() * value.get());
    }
    /*}}}*/
    // _Value_t operator / (const PropertyT<_Type_t> &value) const;/*{{{*/
    /**
     * Division operator.
     * @tparam _Type_t Data type of the value to be compared. This doesn't
     * need to be informed. The compiler can deduce the type by the passed
     * value.
     * @param value Another property object with the value to be added.
     * @return The value resulting of the mathematical operation.
     * @remarks This operator doesn't change the value of the property pointed
     * by this object neither the value of the \a value parameter. Just the
     * operation is done and the resulting value will be returned.
     * @since 1.0
     **/
    template <typename _Type_t>
    _Value_t operator / (const PropertyT<_Type_t> &value) const {
        return (get() / value.get());
    }
    /*}}}*/
    // _Value_t operator +=(const PropertyT<_Type_t> &value);/*{{{*/
    /**
     * Addition operator.
     * @tparam _Type_t Data type of the value to be compared. This doesn't
     * need to be informed. The compiler can deduce the type by the passed
     * value.
     * @param value Another property object with the value to be added.
     * @return The value resulting of the mathematical operation.
     * @remarks This operator changes the value of the property pointed by
     * this object adding the \a value parameter.
     * @since 1.0
     **/
    template <typename _Type_t>
    _Value_t operator +=(const PropertyT<_Type_t> &value) {
        set(get() + value.get());
        return get();
    }
    /*}}}*/
    // _Value_t operator -=(const PropertyT<_Type_t> &value);/*{{{*/
    /**
     * Subtraction operator.
     * @tparam _Type_t Data type of the value to be compared. This doesn't
     * need to be informed. The compiler can deduce the type by the passed
     * value.
     * @param value Another property object with the value to be added.
     * @return The value resulting of the mathematical operation.
     * @remarks This operator changes the value of the property pointed by
     * this object adding the \a value parameter.
     * @since 1.0
     **/
    template <typename _Type_t>
    _Value_t operator -=(const PropertyT<_Type_t> &value) {
        set(get() - value.get());
        return get();
    }
    /*}}}*/
    // _Value_t operator *=(const PropertyT<_Type_t> &value);/*{{{*/
    /**
     * Multiplication operator.
     * @tparam _Type_t Data type of the value to be compared. This doesn't
     * need to be informed. The compiler can deduce the type by the passed
     * value.
     * @param value Another property object with the value to be added.
     * @return The value resulting of the mathematical operation.
     * @remarks This operator changes the value of the property pointed by
     * this object adding the \a value parameter.
     * @since 1.0
     **/
    template <typename _Type_t>
    _Value_t operator *=(const PropertyT<_Type_t> &value) {
        set(get() * value.get());
        return get();
    }
    /*}}}*/
    // _Value_t operator /=(const PropertyT<_Type_t> &value);/*{{{*/
    /**
     * Division operator.
     * @tparam _Type_t Data type of the value to be compared. This doesn't
     * need to be informed. The compiler can deduce the type by the passed
     * value.
     * @param value Another property object with the value to be added.
     * @return The value resulting of the mathematical operation.
     * @remarks This operator changes the value of the property pointed by
     * this object adding the \a value parameter.
     * @since 1.0
     **/
    template <typename _Type_t>
    _Value_t operator /=(const PropertyT<_Type_t> &value) {
        set(get() / value.get());
        return get();
    }
    /*}}}*/
    //@}

    /** @name Bitwise Operators */ //@{
    // _Value_t operator | (_Type_t value) const;/*{{{*/
    /**
     * Bitwise OR opeator.
     * "OR" the value of this property object with the passed \a value.
     * @tparam _Type_t Data type of the passed parameter. This doesn't need to
     * be informed. The compiler can deduce the type by the passed value.
     * @param value The value to be "OR"ed with the value of this property.
     * @return The result of the "OR"ed operation.
     * @remarks The value pointed by this property object is not changed in
     * this operation.
     * @since 1.0
     **/
    template <typename _Type_t>
    _Value_t operator | (_Type_t value) const {
        return (get() | value);
    }
    /*}}}*/
    // _Value_t operator & (_Type_t value) const;/*{{{*/
    /**
     * Bitwise AND opeator.
     * "AND" the value of this property object with the passed \a value.
     * @tparam _Type_t Data type of the passed parameter. This doesn't need to
     * be informed. The compiler can deduce the type by the passed value.
     * @param value The value to be "AND"ed with the value of this property.
     * @return The result of the "AND"ed operation.
     * @remarks The value pointed by this property object is not changed in
     * this operation.
     * @since 1.0
     **/
    template <typename _Type_t>
    _Value_t operator & (_Type_t value) const {
        return (get() & value);
    }
    /*}}}*/
    // _Value_t operator ^ (_Type_t value) const;/*{{{*/
    /**
     * Bitwise XOR opeator.
     * "XOR" the value of this property object with the passed \a value.
     * @tparam _Type_t Data type of the passed parameter. This doesn't need to
     * be informed. The compiler can deduce the type by the passed value.
     * @param value The value to be "XOR"ed with the value of this property.
     * @return The result of the "XOR"ed operation.
     * @remarks The value pointed by this property object is not changed in
     * this operation.
     * @since 1.0
     **/
    template <typename _Type_t>
    _Value_t operator ^ (_Type_t value) const {
        return (get() ^ value);
    }
    /*}}}*/
    // _Value_t operator ~ () const;/*{{{*/
    /**
     * Bitwise NOT opeator.
     * "NOT" the value of this property object.
     * @return The result of the "NOT"ed operation.
     * @since 1.0
     **/
    _Value_t operator ~ () const {
        return ~get();
    }
    /*}}}*/
    // _Value_t operator |=(_Type_t value);/*{{{*/
    /**
     * Bitwise OR opeator.
     * "OR" the value of this property object with the passed \a value.
     * @tparam _Type_t Data type of the passed parameter. This doesn't need to
     * be informed. The compiler can deduce the type by the passed value.
     * @param value The value to be "OR"ed with the value of this property.
     * @return The result of the "OR"ed operation.
     * @remarks The value pointed by this property object is changed and,
     * then, returned by this operation.
     * @since 1.0
     **/
    template <typename _Type_t>
    _Value_t operator |=(_Type_t value) {
        set(get() | value); return get();
    }
    /*}}}*/
    // _Value_t operator &=(_Type_t value);/*{{{*/
    /**
     * Bitwise AND opeator.
     * "AND" the value of this property object with the passed \a value.
     * @tparam _Type_t Data type of the passed parameter. This doesn't need to
     * be informed. The compiler can deduce the type by the passed value.
     * @param value The value to be "AND"ed with the value of this property.
     * @return The result of the "AND"ed operation.
     * @remarks The value pointed by this property object is changed and,
     * then, returned by this operation.
     * @since 1.0
     **/
    template <typename _Type_t>
    _Value_t operator &=(_Type_t value) {
        set(get() & value); return get();
    }
    /*}}}*/
    // _Value_t operator ^=(_Type_t value);/*{{{*/
    /**
     * Bitwise XOR opeator.
     * "XOR" the value of this property object with the passed \a value.
     * @tparam _Type_t Data type of the passed parameter. This doesn't need to
     * be informed. The compiler can deduce the type by the passed value.
     * @param value The value to be "XOR"ed with the value of this property.
     * @return The result of the "XOR"ed operation.
     * @remarks The value pointed by this property object is changed and,
     * then, returned by this operation.
     * @since 1.0
     **/
    template <typename _Type_t>
    _Value_t operator ^=(_Type_t value) {
        set(get() ^ value); return get();
    }
    /*}}}*/
    // _Value_t operator | (const PropertyT<_Type_t> &value) const;/*{{{*/
    /**
     * Bitwise OR opeator.
     * "OR" the value of this property object with the passed \a value.
     * @tparam _Type_t Data type of the passed parameter. This doesn't need to
     * be informed. The compiler can deduce the type by the passed value.
     * @param value Another PropertyT object with the value to be "OR"ed with
     * the value of this property.
     * @return The result of the "OR"ed operation.
     * @remarks The value pointed by this property object is not changed in
     * this operation.
     * @since 1.0
     **/
    template <typename _Type_t>
    _Value_t operator | (const PropertyT<_Type_t> &value) const {
        return (get() | value.get());
    }
    /*}}}*/
    // _Value_t operator & (const PropertyT<_Type_t> &value) const;/*{{{*/
    /**
     * Bitwise AND opeator.
     * "AND" the value of this property object with the passed \a value.
     * @tparam _Type_t Data type of the passed parameter. This doesn't need to
     * be informed. The compiler can deduce the type by the passed value.
     * @param value Another PropertyT object with the value to be "AND"ed with
     * the value of this property.
     * @return The result of the "AND"ed operation.
     * @remarks The value pointed by this property object is not changed in
     * this operation.
     * @since 1.0
     **/
    template <typename _Type_t>
    _Value_t operator & (const PropertyT<_Type_t> &value) const {
        return (get() & value.get());
    }
    /*}}}*/
    // _Value_t operator ^ (const PropertyT<_Type_t> &value) const;/*{{{*/
    /**
     * Bitwise XOR opeator.
     * "XOR" the value of this property object with the passed \a value.
     * @tparam _Type_t Data type of the passed parameter. This doesn't need to
     * be informed. The compiler can deduce the type by the passed value.
     * @param value Another PropertyT object with the value to be "XOR"ed with
     * the value of this property.
     * @return The result of the "XOR"ed operation.
     * @remarks The value pointed by this property object is not changed in
     * this operation.
     * @since 1.0
     **/
    template <typename _Type_t>
    _Value_t operator ^ (const PropertyT<_Type_t> &value) const {
        return (get() ^ value.get());
    }
    /*}}}*/
    // _Value_t operator |=(const PropertyT<_Type_t> &value);/*{{{*/
    /**
     * Bitwise OR opeator.
     * "OR" the value of this property object with the passed \a value.
     * @tparam _Type_t Data type of the passed parameter. This doesn't need to
     * be informed. The compiler can deduce the type by the passed value.
     * @param value Another PropertyT object with the value to be "OR"ed with
     * the value of this property.
     * @return The result of the "OR"ed operation.
     * @remarks The value pointed by this property object is changed in
     * this operation and, them, returned.
     * @since 1.0
     **/
    template <typename _Type_t>
    _Value_t operator |=(const PropertyT<_Type_t> &value) {
        set(get() | value.get()); return get();
    }
    /*}}}*/
    // _Value_t operator &=(const PropertyT<_Type_t> &value);/*{{{*/
    /**
     * Bitwise AND opeator.
     * "AND" the value of this property object with the passed \a value.
     * @tparam _Type_t Data type of the passed parameter. This doesn't need to
     * be informed. The compiler can deduce the type by the passed value.
     * @param value Another PropertyT object with the value to be "AND"ed with
     * the value of this property.
     * @return The result of the "AND"ed operation.
     * @remarks The value pointed by this property object is changed in
     * this operation and, them, returned.
     * @since 1.0
     **/
    template <typename _Type_t>
    _Value_t operator &=(const PropertyT<_Type_t> &value) {
        set(get() & value.get()); return get();
    }
    /*}}}*/
    // _Value_t operator ^=(const PropertyT<_Type_t> &value);/*{{{*/
    /**
     * Bitwise XOR opeator.
     * "XOR" the value of this property object with the passed \a value.
     * @tparam _Type_t Data type of the passed parameter. This doesn't need to
     * be informed. The compiler can deduce the type by the passed value.
     * @param value Another PropertyT object with the value to be "XOR"ed with
     * the value of this property.
     * @return The result of the "XOR"ed operation.
     * @remarks The value pointed by this property object is changed in
     * this operation and, them, returned.
     * @since 1.0
     **/
    template <typename _Type_t>
    _Value_t operator ^=(const PropertyT<_Type_t> &value) {
        set(get() ^ value.get()); return get();
    }
    /*}}}*/
    //@}

    /** @name Boolean Operators */ //@{
    // bool operator ||(_Type_t value) const;/*{{{*/
    /**
     * Boolean "OR" operator.
     * @tparam _Type_t Data type of the passed parameter. This doesn't need to
     * be informed. The compiler can deduce the type by the passed value.
     * @param value The value to be compared.
     * @return A boolean value result of the comparison.
     * @since 1.0
     **/
    template <typename _Type_t>
    bool operator ||(_Type_t value) const {
        return (get() || _Value_t(value));
    }
    /*}}}*/
    // bool operator &&(_Type_t value) const;/*{{{*/
    /**
     * Boolean "AND" operator.
     * @tparam _Type_t Data type of the passed parameter. This doesn't need to
     * be informed. The compiler can deduce the type by the passed value.
     * @param value The value to be compared.
     * @return A boolean value result of the comparison.
     * @since 1.0
     **/
    template <typename _Type_t>
    bool operator &&(_Type_t value) const {
        return (get() && _Value_t(value));
    }
    /*}}}*/
    // bool operator ||(const PropertyT<_Type_t> &value) const;/*{{{*/
    /**
     * Boolean "OR" operator.
     * @tparam _Type_t Data type of the passed parameter. This doesn't need to
     * be informed. The compiler can deduce the type by the passed value.
     * @param value The value to be compared.
     * @return A boolean value result of the comparison.
     * @since 1.0
     **/
    template <typename _Type_t>
    bool operator ||(const PropertyT<_Type_t> &value) const {
        return (get() || (_Value_t)value.get());
    }
    /*}}}*/
    // bool operator &&(const PropertyT<_Type_t> &value) const;/*{{{*/
    /**
     * Boolean "AND" operator.
     * @tparam _Type_t Data type of the passed parameter. This doesn't need to
     * be informed. The compiler can deduce the type by the passed value.
     * @param value The value to be compared.
     * @return A boolean value result of the comparison.
     * @since 1.0
     **/
    template <typename _Type_t>
    bool operator &&(const PropertyT<_Type_t> &value) const {
        return (get() && (_Value_t)value.get());
    }
    /*}}}*/
    //@}

private:
    /** @name Disabled Constructor */ //@{
    // PropertyT(const PropertyT<_Value_t> &other);/*{{{*/
    /**
     * Disabled copy constructor.
     * @param other Another object to copy its values.
     * @since 1.0
     **/
    PropertyT(const PropertyT<_Value_t> &other) {
        other.get();
    }
    /*}}}*/
    //@}

    typedef _Value_t (*getfn_t)(void*);
    typedef void (*setfn_t)(void*, _Value_t);

    // Static Functions
    // static _Value_t getter(void *host) { }/*{{{*/
    /**
     * Calls the getter member function in \a host.
     * @tparam _Host_t The type of the host object.
     * @tparam _Method The member function to call.
     * @param host Pointer to the host instance.
     * @returns The same value returned by the \a host member function \a
     * _Method.
     * @since 1.0
     **/
    template <class _Host_t, _Value_t (_Host_t::*_Method)()>
    static _Value_t getter(void *host) {
        _Host_t *p = static_cast<_Host_t*>(host);
        return (p->*_Method)();
    }
    /*}}}*/
    // static _Value_t constget(void *host) { }/*{{{*/
    /**
     * Calls the getter member function in \a host.
     * @tparam _Host_t The type of the host object.
     * @tparam _Method The member function to call.
     * @param host Pointer to the host instance.
     * @returns The same value returned by the \a host member function \a
     * _Method.
     * @remarks This overload is selected when the getter function is
     * cv-qualified.
     * @since 1.0
     **/
    template <class _Host_t, _Value_t (_Host_t::*_Method)() const>
    static _Value_t constget(void *host) {
        _Host_t const *p = static_cast<_Host_t*>(host);
        return (p->*_Method)();
    }
    /*}}}*/
    // static void setter(void *host, _Value_t value) { }/*{{{*/
    /**
     * Calls the setter member function in \a host.
     * @tparam _Host_t The type of the host object.
     * @tparam _Method The member function to call.
     * @param host Pointer to the host instance.
     * @param value The value to pass to the member function.
     * @since 1.0
     **/
    template <class _Host_t, void (_Host_t::*_Method)(_Value_t)>
    static void setter(void *host, _Value_t value) {
        _Host_t *p = static_cast<_Host_t*>(host);
        (p->*_Method)(value);
    }
    /*}}}*/
    // static void setret(void *host, _Value_t value) { }/*{{{*/
    /**
     * Calls the setter member function in \a host.
     * @tparam _Host_t The type of the host object.
     * @tparam _Method The member function to call.
     * @param host Pointer to the host instance.
     * @param value The value to pass to the member function.
     * @since 1.0
     **/
    template <class _Host_t, _Host_t& (_Host_t::*_Method)(_Value_t)>
    static void setret(void *host, _Value_t value) {
        _Host_t *p = static_cast<_Host_t*>(host);
        (p->*_Method)(value);
    }
    /*}}}*/

    // Data Members
    void *m_host;           /**< Void pointer to the host object. */
    getfn_t m_get;          /**< Getter function pointer.         */
    setfn_t m_set;          /**< Setter function pointer.         */
};

}   /* namespace ss */

// #define ssplink(_Property_, _Pointer_, _Type_, _Getter_, _Setter_) \/*{{{*/
/**
 * Links getter and setter functions to a `ss::PropertyT` object.
 * @param _Property_ The property object instance.
 * @param _Pointer_ Pointer to the class instance defining the getter and
 * setter member functions.
 * @param _Type_ Type of the object pointed by \a _Pointer_.
 * @param _Getter_ Name of the getter member function. Just the name.
 * @param _Setter_ Name of the setter member function. Just the name.
 * @remarks This macro constructs the call to `ss::PropertyT::bind()` function
 * packing the passed parameters. An example explains better how to use this
 * macro:
 * ~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * class MyObject {
 * public:
 *     ss::PropertyT<int> intProperty;
 *
 *     MyObject() {
 *         ssplink(intProperty, this, MyObject, get_int, set_int);
 *         // This will print the code:
 *         intProperty.bind<MyObject, &MyObject::get_int, &MyObject::set_int>(this);
 *     }
 * private:
 *     int get_int() const {
 *         return m_intMember;
 *     }
 *     void set_int(int val) {
 *         m_intMember = val;
 *     }
 *     int m_intMember;
 * };
 * ~~~~~~~~~~~~~~~~~~~~~
 * This macro works for both `ss::PropertyT` and `rw::PropertyT` properties
 * objects.
 * @since 1.0
 **/
#define ssplink(_Property_, _Pointer_, _Type_, _Getter_, _Setter_) \
    _Property_.bind<_Type_, &_Type_::_Getter_, &_Type_::_Setter_>(_Pointer_)
/*}}}*/

namespace rw {
/**
 * Read write property class template.
 * This object is able to mimic property like member variables in a class
 * definition. The class doesn't hold the data of the property. Instead you
 * must write getter and setter functions and \e bind them to the property
 * object instance. This object is intended to be used for values like others
 * object instances where usually the setter function has a different
 * construction than the getter function. For example, suppose you have an
 * object with a member of type `std::string`. A tipical getter function would
 * return a copy of the internal member variable:
 * `std::string getter() const`. But the setter function will require a const
 * reference to another object: `void setter(const std::string&)`. This is
 * common construction on getters and setters for object like data members.
 * @tparam _Value_t The type of the value returned by the getter function.
 * @tparam _Param_t The type of the parameter passed to the setter function.
 * @since 1.0
 * @ingroup sstl_properties
 *//* --------------------------------------------------------------------- */
template <typename _Value_t, typename _Param_t>
class PropertyT
{
public:
    /** @name Constructor */ //@{
    // PropertyT();/*{{{*/
    /**
     * Default constructor.
     * @since 1.0
     **/
    PropertyT() : m_host(NULL), m_get(NULL), m_set(NULL) { }
    /*}}}*/
    //@}

    /** @name Operations */ //@{
    // void bind(_Host_t *host) { }/*{{{*/
    /**
     * Binds the property with the member functions in the host object.
     * @tparam _Host_t Type of the host object.
     * @tparam _Getter Pointer to the getter function.
     * @tparam _Setter Pointer to the setter function.
     * @param host Pointer to the instance of the host object.
     * @since 1.0
     **/
    template <class _Host_t, _Value_t (_Host_t::*_Getter)(), void (_Host_t::*_Setter)(_Param_t)>
    void bind(_Host_t *host) {
        m_host = (void *)host;
        m_get  = &getter<_Host_t, _Getter>;
        m_set  = &setter<_Host_t, _Setter>;
    }
    /*}}}*/
    // void bind(_Host_t *host) { }/*{{{*/
    /**
     * Binds the property with the member functions in the host object.
     * This overloaded version is selected when the getter function is const.
     * @tparam _Host_t Type of the host object.
     * @tparam _Getter Pointer to the getter function.
     * @tparam _Setter Pointer to the setter function.
     * @param host Pointer to the instance of the host object.
     * @since 1.0
     **/
    template <class _Host_t, _Value_t (_Host_t::*_Getter)() const, void (_Host_t::*_Setter)(_Param_t)>
    void bind(_Host_t const *host) {
        m_host = (void *)const_cast<_Host_t*>(host);
        m_get  = &constget<_Host_t, _Getter>;
        m_set  = &setter<_Host_t, _Setter>;
    }
    /*}}}*/
    // void bind(_Host_t *host) { }/*{{{*/
    /**
     * Binds the property with the member functions in the host object.
     * This overloaded version is selected when the getter function is const.
     * @tparam _Host_t Type of the host object.
     * @tparam _Getter Pointer to the getter function.
     * @tparam _Setter Pointer to the setter function.
     * @param host Pointer to the instance of the host object.
     * @since 1.0
     **/
    template <class _Host_t, _Value_t (_Host_t::*_Getter)() const, _Host_t& (_Host_t::*_Setter)(_Param_t)>
    void bind(_Host_t const *host) {
        m_host = (void *)const_cast<_Host_t*>(host);
        m_get  = &constget<_Host_t, _Getter>;
        m_set  = &setret<_Host_t, _Setter>;
    }
    /*}}}*/
    // _Value_t get() const;/*{{{*/
    /**
     * Executes the getter function.
     * @return The value returned by the getter function.
     * @since 1.0
     **/
    _Value_t get() const { return (*m_get)(m_host); }
    /*}}}*/
    // void set(_Param_t value);/*{{{*/
    /**
     * Executes the setter function.
     * @param value The value to be set.
     * @since 1.0
     **/
    void set(_Param_t value) { (*m_set)(m_host, value); }
    /*}}}*/
    //@}

    /** @name Overloaded Operators */ //@{
    // operator _Value_t() const;/*{{{*/
    /**
     * Cast to type operator.
     * This operator calls #get().
     * @returns The value returned by the `%get()` function.
     * @since 1.0
     **/
    operator _Value_t() const { return get(); }
    /*}}}*/
    // _Value_t operator ()() const;/*{{{*/
    /**
     * Functor operator.
     * Calls the #get() function. This operator can be used in places where
     * the compiler cannot deduce to use the `operator _Value_t()`.
     * @return The value returned by `%get()`.
     * @since 1.0
     **/
    _Value_t operator ()() const { return get(); }
    /*}}}*/
    // _Value_t operator =(_Param_t value);/*{{{*/
    /**
     * Assignment operator.
     * This operator calls #set().
     * @param value The value to be set.
     * @since 1.0
     **/
    _Value_t operator =(_Param_t value) {
        set(value); return get();
    }
    /*}}}*/
    // _Value_t operator =(const PropertyT<_Value_t, _Param_t> &value);/*{{{*/
    /**
     * Assignment operator overloading.
     * @param value Another property to get its value.
     * @since 1.0
     **/
    _Value_t operator =(const PropertyT<_Value_t, _Param_t> &value) {
        set(value.get()); return get();
    }
    /*}}}*/
    // _Value_t operator =(const PropertyT<G, S> &value);/*{{{*/
    /**
     * Assignment operator.
     * Copies the value of another property to this one.
     * @param value Referece to another property object.
     * @since 1.0
     **/
    template <typename G, typename S>
    _Value_t operator =(const PropertyT<G, S> &value) {
        set(value.get()); return get();
    }
    /*}}}*/
    //@}

    /** @name Comparison Operators */ //@{
    // bool operator ==(_Type_t value) const;/*{{{*/
    /**
     * Equality operator.
     * Compares the value of the property pointed by this object with the
     * passed value.
     * @tparam _Type_t Data type of the value to be compared. This doesn't
     * need to be informed. The compiler can deduce the type by the passed
     * value.
     * @param value The value to compare to. Must be implicitly convertible to
     * the type of this property object.
     * @return \b true if the passed \a value and the value pointed by this
     * property are equals. \b false otherwise.
     * @since 1.0
     **/
    template <typename _Type_t>
    bool operator ==(_Type_t value) const {
        return (get() == value);
    }
    /*}}}*/
    // bool operator !=(_Type_t value) const;/*{{{*/
    /**
     * Inequality operator.
     * Compares the value of the property pointed by this object with the
     * passed value.
     * @param value The value to compare to.
     * @return \b true if the passed \a value and the value pointed by this
     * property are different. \b false otherwise.
     * @since 1.0
     **/
    template <typename _Type_t>
    bool operator !=(_Type_t value) const {
        return (get() != value);
    }
    /*}}}*/
    // bool operator ==(const PropertyT<G, S> &value) const;/*{{{*/
    /**
     * Equality operator.
     * Compares the value of the property pointed by this object with the
     * value pointed by another property.
     * @tparam G Data type of the property to be compared. This doesn't
     * need to be informed. The compiler can deduce the type.
     * @tparam S Data type of the property value setter. This doesn't need to
     * be informed. The compiler can deduce the type.
     * @param value The property to compare to. Must be implicitly convertible
     * to the type of this property object.
     * @return \b true if the passed \a value and the value pointed by this
     * property are equals. \b false otherwise.
     * @since 1.0
     **/
    template <typename G, typename S>
    bool operator ==(const PropertyT<G, S> &value) const {
        return (get() == value.get());
    }
    /*}}}*/
    // bool operator !=(const PropertyT<G, S> &value) const;/*{{{*/
    /**
     * Inequality operator.
     * Compares the value of the property pointed by this object with the
     * value pointed by another property object.
     * @param value The value to compare to.
     * @return \b true if the passed \a value and the value pointed by this
     * property are different. \b false otherwise.
     * @since 1.0
     **/
    template <typename G, typename S>
    bool operator !=(const PropertyT<G, S> &value) const {
        return (get() != value.get());
    }
    /*}}}*/
    //@}

private:
    /** @name Disabled Constructor */ //@{
    // PropertyT(const PropertyT<_Value_t, _Param_t> &other);/*{{{*/
    /**
     * Disabled copy constructor.
     * @param other Another object to copy its values.
     * @since 1.0
     **/
    PropertyT(const PropertyT<_Value_t, _Param_t> &other) {
        other.get();
    }
    /*}}}*/
    //@}

    typedef _Value_t (*getfn_t)(void*);
    typedef void (*setfn_t)(void*, _Param_t);

    // Static Functions
    // static _Value_t getter(void *host) { }/*{{{*/
    /**
     * Calls the getter member function in \a host.
     * @tparam _Host_t The type of the host object.
     * @tparam _Method The member function to call.
     * @param host Pointer to the host instance.
     * @returns The same value returned by the \a host member function \a
     * _Method.
     * @since 1.0
     **/
    template <class _Host_t, _Value_t (_Host_t::*_Method)()>
    static _Value_t getter(void *host) {
        _Host_t *p = static_cast<_Host_t*>(host);
        return (p->*_Method)();
    }
    /*}}}*/
    // static _Value_t constget(void *host) { }/*{{{*/
    /**
     * Calls the getter member function in \a host.
     * @tparam _Host_t The type of the host object.
     * @tparam _Method The member function to call.
     * @param host Pointer to the host instance.
     * @returns The same value returned by the \a host member function \a
     * _Method.
     * @remarks This overload is selected when the getter function is
     * cv-qualified.
     * @since 1.0
     **/
    template <class _Host_t, _Value_t (_Host_t::*_Method)() const>
    static _Value_t constget(void *host) {
        _Host_t const *p = static_cast<_Host_t*>(host);
        return (p->*_Method)();
    }
    /*}}}*/
    // static void setter(void *host, _Value_t value) { }/*{{{*/
    /**
     * Calls the setter member function in \a host.
     * @tparam _Host_t The type of the host object.
     * @tparam _Method The member function to call.
     * @param host Pointer to the host instance.
     * @param value The value to pass to the member function.
     * @since 1.0
     **/
    template <class _Host_t, void (_Host_t::*_Method)(_Param_t)>
    static void setter(void *host, _Param_t value) {
        _Host_t *p = static_cast<_Host_t*>(host);
        (p->*_Method)(value);
    }
    /*}}}*/
    // static void setret(void *host, _Value_t value) { }/*{{{*/
    /**
     * Calls the setter member function in \a host.
     * @tparam _Host_t The type of the host object.
     * @tparam _Method The member function to call.
     * @param host Pointer to the host instance.
     * @param value The value to pass to the member function.
     * @since 1.0
     **/
    template <class _Host_t, _Host_t& (_Host_t::*_Method)(_Param_t)>
    static void setret(void *host, _Param_t value) {
        _Host_t *p = static_cast<_Host_t*>(host);
        (p->*_Method)(value);
    }
    /*}}}*/

    // Data Members
    void *m_host;           /**< Void pointer to the host object. */
    getfn_t m_get;          /**< Getter function pointer.         */
    setfn_t m_set;          /**< Setter function pointer.         */
};

}   /* namespace rw */

namespace ro {
/**
 * Read only property class template.
 * This object is able to mimic property like member variables in a class
 * definition. The class doesn't hold the data of the property. Instead you
 * must write a getter function and \e bind it to the property
 * object instance. This version can be used both for scalar and object like
 * data members but, different of `ss::PropertyT` it doens't overload any
 * common operator. To force the object to return the property value and use
 * it where automatic conversions are not supported use the `operator ()`
 * operator.
 * @tparam _Value_t The type of the property value.
 * @since 1.0
 *//* --------------------------------------------------------------------- */
template <typename _Value_t>
class PropertyT
{
public:
    /** @name Constructor */ //@{
    // PropertyT();/*{{{*/
    /**
     * Default constructor.
     * @since 1.0
     **/
    PropertyT() : m_host(NULL), m_get(NULL) { }
    /*}}}*/
    //@}

    /** @name Operations */ //@{
    // void bind(_Host_t *host) { }/*{{{*/
    /**
     * Binds the property with the member functions in the host object.
     * @tparam _Host_t Type of the host object.
     * @tparam _Method Pointer to the getter function.
     * @param host Pointer to the instance of the host object.
     * @since 1.0
     **/
    template <class _Host_t, _Value_t (_Host_t::*_Method)()>
    void bind(_Host_t *host) {
        m_host = (void *)host;
        m_get  = &getter<_Host_t, _Method>;
    }
    /*}}}*/
    // void bind(_Host_t *host) { }/*{{{*/
    /**
     * Binds the property with the member functions in the host object.
     * This overloaded version is selected when the getter function is const.
     * @tparam _Host_t Type of the host object.
     * @tparam _Method Pointer to the getter function.
     * @param host Pointer to the instance of the host object.
     * @since 1.0
     **/
    template <class _Host_t, _Value_t (_Host_t::*_Method)() const>
    void bind(_Host_t const *host) {
        m_host = (void *)const_cast<_Host_t*>(host);
        m_get  = &constget<_Host_t, _Method>;
    }
    /*}}}*/
    // _Value_t get() const;/*{{{*/
    /**
     * Executes the getter function.
     * @return The value returned by the getter function.
     * @since 1.0
     **/
    _Value_t get() const { return (*m_get)(m_host); }
    /*}}}*/
    //@}

    /** @name Overloaded Operators */ //@{
    // operator _Value_t() const;/*{{{*/
    /**
     * Cast to type operator.
     * This operator calls #get().
     * @returns The value returned by the `%get()` function.
     * @since 1.0
     **/
    operator _Value_t() const { return get(); }
    /*}}}*/
    // _Value_t operator ()() const;/*{{{*/
    /**
     * Functor operator.
     * Calls the #get() function. This operator can be used in places where
     * the compiler cannot deduce to use the `operator _Value_t()`.
     * @return The value returned by `%get()`.
     * @since 1.0
     **/
    _Value_t operator ()() const { return get(); }
    /*}}}*/
    //@}

    /** @name Comparison Operators */ //@{
    // bool operator ! () const;/*{{{*/
    /**
     * Negation operator.
     * @returns \b true when the value of this property can be convertibal to
     * a \b falsy result. \b false otherwise.
     * @since 1.0
     **/
    bool operator ! () const {
        return !bool(get());
    }
    /*}}}*/
    // bool operator ==(_Type_t value) const;/*{{{*/
    /**
     * Equality operator.
     * Compares the value of the property pointed by this object with the
     * passed value.
     * @tparam _Type_t Data type of the value to be compared. This doesn't
     * need to be informed. The compiler can deduce the type by the passed
     * value.
     * @param value The value to compare to. Must be implicitly convertible to
     * the type of this property object.
     * @return \b true if the passed \a value and the value pointed by this
     * property are equals. \b false otherwise.
     * @since 1.0
     **/
    template <typename _Type_t>
    bool operator ==(_Type_t value) const {
        return (get() == value);
    }
    /*}}}*/
    // bool operator !=(_Type_t value) const;/*{{{*/
    /**
     * Inequality operator.
     * Compares the value of the property pointed by this object with the
     * passed value.
     * @param value The value to compare to.
     * @return \b true if the passed \a value and the value pointed by this
     * property are different. \b false otherwise.
     * @since 1.0
     **/
    template <typename _Type_t>
    bool operator !=(_Type_t value) const {
        return (get() != value);
    }
    /*}}}*/
    // bool operator < (_Type_t value) const;/*{{{*/
    /**
     * Less than operator.
     * Compares the value of the property pointed by this object with the
     * passed value.
     * @param value The value to compare to.
     * @return \b true when the value returned by this property object is
     * \it{less than} the passed \a value. \b false otherwise.
     * @since 1.0
     **/
    template <typename _Type_t>
    bool operator < (_Type_t value) const {
        return (get() < value);
    }
    /*}}}*/
    // bool operator <=(_Type_t value) const;/*{{{*/
    /**
     * Less than or equal operator.
     * Compares the value of the property pointed by this object with the
     * passed value.
     * @param value The value to compare to.
     * @return \b true when the value returned by this property object is
     * \it{less than or equal} the passed \a value. \b false otherwise.
     * @since 1.0
     **/
    template <typename _Type_t>
    bool operator <=(_Type_t value) const {
        return (get() <= value);
    }
    /*}}}*/
    // bool operator > (_Type_t value) const;/*{{{*/
    /**
     * Greater than operator.
     * Compares the value of the property pointed by this object with the
     * passed value.
     * @param value The value to compare to.
     * @return \b true when the value returned by this property object is
     * \it{greater than} the passed \a value. \b false otherwise.
     * @since 1.0
     **/
    template <typename _Type_t>
    bool operator > (_Type_t value) const {
        return (get() > value);
    }
    /*}}}*/
    // bool operator >=(_Type_t value) const;/*{{{*/
    /**
     * Greater than or equal operator.
     * Compares the value of the property pointed by this object with the
     * passed value.
     * @param value The value to compare to.
     * @return \b true when the value returned by this property object is
     * \it{greater than or equal} the passed \a value. \b false otherwise.
     * @since 1.0
     **/
    template <typename _Type_t>
    bool operator >=(_Type_t value) const {
        return (get() >= value);
    }
    /*}}}*/
    // bool operator ==(const PropertyT<_Type_t> &value) const;/*{{{*/
    /**
     * Equality operator.
     * Compares the value of the property pointed by this object with the
     * value pointed by another property.
     * @tparam _Type_t Data type of the property to be compared. This doesn't
     * need to be informed. The compiler can deduce the type by the passed
     * argument.
     * @param value The property to compare to. Must be implicitly convertible
     * to the type of this property object.
     * @return \b true if the passed \a value and the value pointed by this
     * property are equals. \b false otherwise.
     * @since 1.0
     **/
    template <typename _Type_t>
    bool operator ==(const PropertyT<_Type_t> &value) const {
        return (get() == value.get());
    }
    /*}}}*/
    // bool operator !=(const PropertyT<_Type_t> &value) const;/*{{{*/
    /**
     * Inequality operator.
     * Compares the value of the property pointed by this object with the
     * value pointed by another property object.
     * @param value The value to compare to.
     * @return \b true if the passed \a value and the value pointed by this
     * property are different. \b false otherwise.
     * @since 1.0
     **/
    template <typename _Type_t>
    bool operator !=(const PropertyT<_Type_t> &value) const {
        return (get() != value.get());
    }
    /*}}}*/
    // bool operator < (const PropertyT<_Type_t> &value) const;/*{{{*/
    /**
     * Less than operator.
     * Compares the value of the property pointed by this object with the
     * value of another property object.
     * @param value The property to compare to.
     * @return \b true when the value returned by this property object is
     * \it{less than} the passed \a value. \b false otherwise.
     * @since 1.0
     **/
    template <typename _Type_t>
    bool operator < (const PropertyT<_Type_t> &value) const {
        return (get() < value.get());
    }
    /*}}}*/
    // bool operator <=(const PropertyT<_Type_t> &value) const;/*{{{*/
    /**
     * Less than or equal operator.
     * Compares the value of the property pointed by this object with the
     * value of another property object.
     * @param value The property to compare to.
     * @return \b true when the value returned by this property object is
     * \it{less than or equal} the passed \a value. \b false otherwise.
     * @since 1.0
     **/
    template <typename _Type_t>
    bool operator <=(const PropertyT<_Type_t> &value) const {
        return (get() <= value.get());
    }
    /*}}}*/
    // bool operator > (const PropertyT<_Type_t> &value) const;/*{{{*/
    /**
     * Greater than operator.
     * Compares the value of the property pointed by this object with the
     * value of another property object.
     * @param value The property to compare to.
     * @return \b true when the value returned by this property object is
     * \it{greater than} the passed \a value. \b false otherwise.
     * @since 1.0
     **/
    template <typename _Type_t>
    bool operator > (const PropertyT<_Type_t> &value) const {
        return (get() > value.get());
    }
    /*}}}*/
    // bool operator >=(const PropertyT<_Type_t> &value) const;/*{{{*/
    /**
     * Greater than or equal operator.
     * Compares the value of the property pointed by this object with the
     * value of another property object.
     * @param value The property to compare to.
     * @return \b true when the value returned by this property object is
     * \it{greater than or equal} the passed \a value. \b false otherwise.
     * @since 1.0
     **/
    template <typename _Type_t>
    bool operator >=(const PropertyT<_Type_t> &value) const {
        return (get() >= value.get());
    }
    /*}}}*/
    // bool operator ==(const ss::PropertyT<_Type_t> &value) const;/*{{{*/
    /**
     * Equality operator.
     * Compares the value of the property pointed by this object with the
     * value pointed by another property.
     * @tparam _Type_t Data type of the property to be compared. This doesn't
     * need to be informed. The compiler can deduce the type by the passed
     * argument.
     * @param value The property to compare to. Must be implicitly convertible
     * to the type of this property object.
     * @return \b true if the passed \a value and the value pointed by this
     * property are equals. \b false otherwise.
     * @since 1.0
     **/
    template <typename _Type_t>
    bool operator ==(const ss::PropertyT<_Type_t> &value) const {
        return (get() == value.get());
    }
    /*}}}*/
    // bool operator !=(const ss::PropertyT<_Type_t> &value) const;/*{{{*/
    /**
     * Inequality operator.
     * Compares the value of the property pointed by this object with the
     * value pointed by another property object.
     * @param value The value to compare to.
     * @return \b true if the passed \a value and the value pointed by this
     * property are different. \b false otherwise.
     * @since 1.0
     **/
    template <typename _Type_t>
    bool operator !=(const ss::PropertyT<_Type_t> &value) const {
        return (get() != value.get());
    }
    /*}}}*/
    // bool operator < (const ss::PropertyT<_Type_t> &value) const;/*{{{*/
    /**
     * Less than operator.
     * Compares the value of the property pointed by this object with the
     * value of another property object.
     * @param value The property to compare to.
     * @return \b true when the value returned by this property object is
     * \it{less than} the passed \a value. \b false otherwise.
     * @since 1.0
     **/
    template <typename _Type_t>
    bool operator < (const ss::PropertyT<_Type_t> &value) const {
        return (get() < value.get());
    }
    /*}}}*/
    // bool operator <=(const ss::PropertyT<_Type_t> &value) const;/*{{{*/
    /**
     * Less than or equal operator.
     * Compares the value of the property pointed by this object with the
     * value of another property object.
     * @param value The property to compare to.
     * @return \b true when the value returned by this property object is
     * \it{less than or equal} the passed \a value. \b false otherwise.
     * @since 1.0
     **/
    template <typename _Type_t>
    bool operator <=(const ss::PropertyT<_Type_t> &value) const {
        return (get() <= value.get());
    }
    /*}}}*/
    // bool operator > (const ss::PropertyT<_Type_t> &value) const;/*{{{*/
    /**
     * Greater than operator.
     * Compares the value of the property pointed by this object with the
     * value of another property object.
     * @param value The property to compare to.
     * @return \b true when the value returned by this property object is
     * \it{greater than} the passed \a value. \b false otherwise.
     * @since 1.0
     **/
    template <typename _Type_t>
    bool operator > (const ss::PropertyT<_Type_t> &value) const {
        return (get() > value.get());
    }
    /*}}}*/
    // bool operator >=(const ss::PropertyT<_Type_t> &value) const;/*{{{*/
    /**
     * Greater than or equal operator.
     * Compares the value of the property pointed by this object with the
     * value of another property object.
     * @param value The property to compare to.
     * @return \b true when the value returned by this property object is
     * \it{greater than or equal} the passed \a value. \b false otherwise.
     * @since 1.0
     **/
    template <typename _Type_t>
    bool operator >=(const ss::PropertyT<_Type_t> &value) const {
        return (get() >= value.get());
    }
    /*}}}*/
    // bool operator ==(const rw::PropertyT<_Get_t, _Set_t> &value) const;/*{{{*/
    /**
     * Equality operator.
     * Compares the value of the property pointed by this object with the
     * value pointed by another property.
     * @tparam _Get_t,_Set_t Data types of the `rw::PropertyT` template.
     * @param value The property to compare to. Must be implicitly convertible
     * to the type of this property object.
     * @return \b true if the passed \a value and the value pointed by this
     * property are equals. \b false otherwise.
     * @since 1.0
     **/
    template <typename _Get_t, typename _Set_t>
    bool operator ==(const rw::PropertyT<_Get_t, _Set_t> &value) const {
        return (get() == value.get());
    }
    /*}}}*/
    // bool operator !=(const rw::PropertyT<_Get_t, _Set_t> &value) const;/*{{{*/
    /**
     * Inequality operator.
     * Compares the value of the property pointed by this object with the
     * value pointed by another property object.
     * @tparam _Get_t,_Set_t Data types of the `rw::PropertyT` template.
     * @param value The value to compare to.
     * @return \b true if the passed \a value and the value pointed by this
     * property are different. \b false otherwise.
     * @since 1.0
     **/
    template <typename _Get_t, typename _Set_t>
    bool operator !=(const rw::PropertyT<_Get_t, _Set_t> &value) const {
        return (get() != value.get());
    }
    /*}}}*/
    // bool operator < (const rw::PropertyT<_Get_t, _Set_t> &value) const;/*{{{*/
    /**
     * Less than operator.
     * Compares the value of the property pointed by this object with the
     * value of another property object.
     * @tparam _Get_t,_Set_t Data types of the `rw::PropertyT` template.
     * @param value The property to compare to.
     * @return \b true when the value returned by this property object is
     * \it{less than} the passed \a value. \b false otherwise.
     * @since 1.0
     **/
    template <typename _Get_t, typename _Set_t>
    bool operator < (const rw::PropertyT<_Get_t, _Set_t> &value) const {
        return (get() < value.get());
    }
    /*}}}*/
    // bool operator <=(const rw::PropertyT<_Get_t, _Set_t> &value) const;/*{{{*/
    /**
     * Less than or equal operator.
     * Compares the value of the property pointed by this object with the
     * value of another property object.
     * @tparam _Get_t,_Set_t Data types of the `rw::PropertyT` template.
     * @param value The property to compare to.
     * @return \b true when the value returned by this property object is
     * \it{less than or equal} the passed \a value. \b false otherwise.
     * @since 1.0
     **/
    template <typename _Get_t, typename _Set_t>
    bool operator <=(const rw::PropertyT<_Get_t, _Set_t> &value) const {
        return (get() <= value.get());
    }
    /*}}}*/
    // bool operator > (const rw::PropertyT<_Get_t, _Set_t> &value) const;/*{{{*/
    /**
     * Greater than operator.
     * Compares the value of the property pointed by this object with the
     * value of another property object.
     * @tparam _Get_t,_Set_t Data types of the `rw::PropertyT` template.
     * @param value The property to compare to.
     * @return \b true when the value returned by this property object is
     * \it{greater than} the passed \a value. \b false otherwise.
     * @since 1.0
     **/
    template <typename _Get_t, typename _Set_t>
    bool operator > (const rw::PropertyT<_Get_t, _Set_t> &value) const {
        return (get() > value.get());
    }
    /*}}}*/
    // bool operator >=(const rw::PropertyT<_Get_t, _Set_t> &value) const;/*{{{*/
    /**
     * Greater than or equal operator.
     * Compares the value of the property pointed by this object with the
     * value of another property object.
     * @tparam _Get_t,_Set_t Data types of the `rw::PropertyT` template.
     * @param value The property to compare to.
     * @return \b true when the value returned by this property object is
     * \it{greater than or equal} the passed \a value. \b false otherwise.
     * @since 1.0
     **/
    template <typename _Get_t, typename _Set_t>
    bool operator >=(const rw::PropertyT<_Get_t, _Set_t> &value) const {
        return (get() >= value.get());
    }
    /*}}}*/
    //@}

    /** @name Mathematical Operators */ //@{
    // _Value_t operator + (_Type_t value) const;/*{{{*/
    /**
     * Addition operator.
     * @tparam _Type_t Data type of the value to be compared. This doesn't
     * need to be informed. The compiler can deduce the type by the passed
     * value.
     * @param value Value to be added.
     * @return The value resulting of the mathematical operation.
     * @remarks This operator doesn't change the value of the property pointed
     * by this object neither the value of the \a value parameter. Just the
     * operation is done and the resulting value will be returned.
     * @since 1.0
     **/
    template <typename _Type_t>
    _Value_t operator + (_Type_t value) const {
        return (get() + value);
    }
    /*}}}*/
    // _Value_t operator - (_Type_t value) const;/*{{{*/
    /**
     * Subtraction operator.
     * @tparam _Type_t Data type of the value to be compared. This doesn't
     * need to be informed. The compiler can deduce the type by the passed
     * value.
     * @param value Value to be added.
     * @return The value resulting of the mathematical operation.
     * @remarks This operator doesn't change the value of the property pointed
     * by this object neither the value of the \a value parameter. Just the
     * operation is done and the resulting value will be returned.
     * @since 1.0
     **/
    template <typename _Type_t>
    _Value_t operator - (_Type_t value) const {
        return (get() - value);
    }
    /*}}}*/
    // _Value_t operator * (_Type_t value) const;/*{{{*/
    /**
     * Multiplication operator.
     * @tparam _Type_t Data type of the value to be compared. This doesn't
     * need to be informed. The compiler can deduce the type by the passed
     * value.
     * @param value Value to be added.
     * @return The value resulting of the mathematical operation.
     * @remarks This operator doesn't change the value of the property pointed
     * by this object neither the value of the \a value parameter. Just the
     * operation is done and the resulting value will be returned.
     * @since 1.0
     **/
    template <typename _Type_t>
    _Value_t operator * (_Type_t value) const {
        return (get() * value);
    }
    /*}}}*/
    // _Value_t operator / (_Type_t value) const;/*{{{*/
    /**
     * Division operator.
     * @tparam _Type_t Data type of the value to be compared. This doesn't
     * need to be informed. The compiler can deduce the type by the passed
     * value.
     * @param value Value to be added.
     * @return The value resulting of the mathematical operation.
     * @remarks This operator doesn't change the value of the property pointed
     * by this object neither the value of the \a value parameter. Just the
     * operation is done and the resulting value will be returned.
     * @since 1.0
     **/
    template <typename _Type_t>
    _Value_t operator / (_Type_t value) const {
        return (get() / value);
    }
    /*}}}*/
    // _Value_t operator + (const PropertyT<_Type_t> &value) const;/*{{{*/
    /**
     * Addition operator.
     * @tparam _Type_t Data type of the value to be compared. This doesn't
     * need to be informed. The compiler can deduce the type by the passed
     * value.
     * @param value Another property object with the value to be added.
     * @return The value resulting of the mathematical operation.
     * @remarks This operator doesn't change the value of the property pointed
     * by this object neither the value of the \a value parameter. Just the
     * operation is done and the resulting value will be returned.
     * @since 1.0
     **/
    template <typename _Type_t>
    _Value_t operator + (const PropertyT<_Type_t> &value) const {
        return (get() + value.get());
    }
    /*}}}*/
    // _Value_t operator - (const PropertyT<_Type_t> &value) const;/*{{{*/
    /**
     * Subtraction operator.
     * @tparam _Type_t Data type of the value to be compared. This doesn't
     * need to be informed. The compiler can deduce the type by the passed
     * value.
     * @param value Another property object with the value to be added.
     * @return The value resulting of the mathematical operation.
     * @remarks This operator doesn't change the value of the property pointed
     * by this object neither the value of the \a value parameter. Just the
     * operation is done and the resulting value will be returned.
     * @since 1.0
     **/
    template <typename _Type_t>
    _Value_t operator - (const PropertyT<_Type_t> &value) const {
        return (get() - value.get());
    }
    /*}}}*/
    // _Value_t operator * (const PropertyT<_Type_t> &value) const;/*{{{*/
    /**
     * Multiplication operator.
     * @tparam _Type_t Data type of the value to be compared. This doesn't
     * need to be informed. The compiler can deduce the type by the passed
     * value.
     * @param value Another property object with the value to be added.
     * @return The value resulting of the mathematical operation.
     * @remarks This operator doesn't change the value of the property pointed
     * by this object neither the value of the \a value parameter. Just the
     * operation is done and the resulting value will be returned.
     * @since 1.0
     **/
    template <typename _Type_t>
    _Value_t operator * (const PropertyT<_Type_t> &value) const {
        return (get() * value.get());
    }
    /*}}}*/
    // _Value_t operator / (const PropertyT<_Type_t> &value) const;/*{{{*/
    /**
     * Division operator.
     * @tparam _Type_t Data type of the value to be compared. This doesn't
     * need to be informed. The compiler can deduce the type by the passed
     * value.
     * @param value Another property object with the value to be added.
     * @return The value resulting of the mathematical operation.
     * @remarks This operator doesn't change the value of the property pointed
     * by this object neither the value of the \a value parameter. Just the
     * operation is done and the resulting value will be returned.
     * @since 1.0
     **/
    template <typename _Type_t>
    _Value_t operator / (const PropertyT<_Type_t> &value) const {
        return (get() / value.get());
    }
    /*}}}*/
    // _Value_t operator + (const ss::PropertyT<_Type_t> &value) const;/*{{{*/
    /**
     * Addition operator.
     * @tparam _Type_t Data type of the value to be compared. This doesn't
     * need to be informed. The compiler can deduce the type by the passed
     * value.
     * @param value Another property object with the value to be added.
     * @return The value resulting of the mathematical operation.
     * @remarks This operator doesn't change the value of the property pointed
     * by this object neither the value of the \a value parameter. Just the
     * operation is done and the resulting value will be returned.
     * @since 1.0
     **/
    template <typename _Type_t>
    _Value_t operator + (const ss::PropertyT<_Type_t> &value) const {
        return (get() + value.get());
    }
    /*}}}*/
    // _Value_t operator - (const ss::PropertyT<_Type_t> &value) const;/*{{{*/
    /**
     * Subtraction operator.
     * @tparam _Type_t Data type of the value to be compared. This doesn't
     * need to be informed. The compiler can deduce the type by the passed
     * value.
     * @param value Another property object with the value to be added.
     * @return The value resulting of the mathematical operation.
     * @remarks This operator doesn't change the value of the property pointed
     * by this object neither the value of the \a value parameter. Just the
     * operation is done and the resulting value will be returned.
     * @since 1.0
     **/
    template <typename _Type_t>
    _Value_t operator - (const ss::PropertyT<_Type_t> &value) const {
        return (get() - value.get());
    }
    /*}}}*/
    // _Value_t operator * (const ss::PropertyT<_Type_t> &value) const;/*{{{*/
    /**
     * Multiplication operator.
     * @tparam _Type_t Data type of the value to be compared. This doesn't
     * need to be informed. The compiler can deduce the type by the passed
     * value.
     * @param value Another property object with the value to be added.
     * @return The value resulting of the mathematical operation.
     * @remarks This operator doesn't change the value of the property pointed
     * by this object neither the value of the \a value parameter. Just the
     * operation is done and the resulting value will be returned.
     * @since 1.0
     **/
    template <typename _Type_t>
    _Value_t operator * (const ss::PropertyT<_Type_t> &value) const {
        return (get() * value.get());
    }
    /*}}}*/
    // _Value_t operator / (const ss::PropertyT<_Type_t> &value) const;/*{{{*/
    /**
     * Division operator.
     * @tparam _Type_t Data type of the value to be compared. This doesn't
     * need to be informed. The compiler can deduce the type by the passed
     * value.
     * @param value Another property object with the value to be added.
     * @return The value resulting of the mathematical operation.
     * @remarks This operator doesn't change the value of the property pointed
     * by this object neither the value of the \a value parameter. Just the
     * operation is done and the resulting value will be returned.
     * @since 1.0
     **/
    template <typename _Type_t>
    _Value_t operator / (const ss::PropertyT<_Type_t> &value) const {
        return (get() / value.get());
    }
    /*}}}*/
    // _Value_t operator + (const rw::PropertyT<_Get_t, _Set_t> &value) const;/*{{{*/
    /**
     * Addition operator.
     * @tparam _Get_t,_Set_t Data types of the `rw::PropertyT` template.
     * @param value Another property object with the value to be added.
     * @return The value resulting of the mathematical operation.
     * @remarks This operator doesn't change the value of the property pointed
     * by this object neither the value of the \a value parameter. Just the
     * operation is done and the resulting value will be returned.
     * @since 1.0
     **/
    template <typename _Get_t, typename _Set_t>
    _Value_t operator + (const rw::PropertyT<_Get_t, _Set_t> &value) const {
        return (get() + value.get());
    }
    /*}}}*/
    // _Value_t operator - (const rw::PropertyT<_Get_t, _Set_t> &value) const;/*{{{*/
    /**
     * Subtraction operator.
     * @tparam _Get_t,_Set_t Data types of the `rw::PropertyT` template.
     * @param value Another property object with the value to be added.
     * @return The value resulting of the mathematical operation.
     * @remarks This operator doesn't change the value of the property pointed
     * by this object neither the value of the \a value parameter. Just the
     * operation is done and the resulting value will be returned.
     * @since 1.0
     **/
    template <typename _Get_t, typename _Set_t>
    _Value_t operator - (const rw::PropertyT<_Get_t, _Set_t> &value) const {
        return (get() - value.get());
    }
    /*}}}*/
    // _Value_t operator * (const rw::PropertyT<_Get_t, _Set_t> &value) const;/*{{{*/
    /**
     * Multiplication operator.
     * @tparam _Get_t,_Set_t Data types of the `rw::PropertyT` template.
     * @param value Another property object with the value to be added.
     * @return The value resulting of the mathematical operation.
     * @remarks This operator doesn't change the value of the property pointed
     * by this object neither the value of the \a value parameter. Just the
     * operation is done and the resulting value will be returned.
     * @since 1.0
     **/
    template <typename _Get_t, typename _Set_t>
    _Value_t operator * (const rw::PropertyT<_Get_t, _Set_t> &value) const {
        return (get() * value.get());
    }
    /*}}}*/
    // _Value_t operator / (const rw::PropertyT<_Get_t, _Set_t> &value) const;/*{{{*/
    /**
     * Division operator.
     * @tparam _Get_t,_Set_t Data types of the `rw::PropertyT` template.
     * @param value Another property object with the value to be added.
     * @return The value resulting of the mathematical operation.
     * @remarks This operator doesn't change the value of the property pointed
     * by this object neither the value of the \a value parameter. Just the
     * operation is done and the resulting value will be returned.
     * @since 1.0
     **/
    template <typename _Get_t, typename _Set_t>
    _Value_t operator / (const rw::PropertyT<_Get_t, _Set_t> &value) const {
        return (get() / value.get());
    }
    /*}}}*/
    //@}

    /** @name Bitwise Operators */ //@{
    // _Value_t operator | (_Type_t value) const;/*{{{*/
    /**
     * Bitwise OR opeator.
     * "OR" the value of this property object with the passed \a value.
     * @tparam _Type_t Data type of the passed parameter. This doesn't need to
     * be informed. The compiler can deduce the type by the passed value.
     * @param value The value to be "OR"ed with the value of this property.
     * @return The result of the "OR"ed operation.
     * @remarks The value pointed by this property object is not changed in
     * this operation.
     * @since 1.0
     **/
    template <typename _Type_t>
    _Value_t operator | (_Type_t value) const {
        return (get() | value);
    }
    /*}}}*/
    // _Value_t operator & (_Type_t value) const;/*{{{*/
    /**
     * Bitwise AND opeator.
     * "AND" the value of this property object with the passed \a value.
     * @tparam _Type_t Data type of the passed parameter. This doesn't need to
     * be informed. The compiler can deduce the type by the passed value.
     * @param value The value to be "AND"ed with the value of this property.
     * @return The result of the "AND"ed operation.
     * @remarks The value pointed by this property object is not changed in
     * this operation.
     * @since 1.0
     **/
    template <typename _Type_t>
    _Value_t operator & (_Type_t value) const {
        return (get() & value);
    }
    /*}}}*/
    // _Value_t operator ^ (_Type_t value) const;/*{{{*/
    /**
     * Bitwise XOR opeator.
     * "XOR" the value of this property object with the passed \a value.
     * @tparam _Type_t Data type of the passed parameter. This doesn't need to
     * be informed. The compiler can deduce the type by the passed value.
     * @param value The value to be "XOR"ed with the value of this property.
     * @return The result of the "XOR"ed operation.
     * @remarks The value pointed by this property object is not changed in
     * this operation.
     * @since 1.0
     **/
    template <typename _Type_t>
    _Value_t operator ^ (_Type_t value) const {
        return (get() ^ value);
    }
    /*}}}*/
    // _Value_t operator ~ () const;/*{{{*/
    /**
     * Bitwise NOT opeator.
     * "NOT" the value of this property object.
     * @return The result of the "NOT"ed operation.
     * @since 1.0
     **/
    _Value_t operator ~ () const {
        return ~get();
    }
    /*}}}*/
    // _Value_t operator | (const PropertyT<_Type_t> &value) const;/*{{{*/
    /**
     * Bitwise OR opeator.
     * "OR" the value of this property object with the passed \a value.
     * @tparam _Type_t Data type of the passed parameter. This doesn't need to
     * be informed. The compiler can deduce the type by the passed value.
     * @param value Another PropertyT object with the value to be "OR"ed with
     * the value of this property.
     * @return The result of the "OR"ed operation.
     * @remarks The value pointed by this property object is not changed in
     * this operation.
     * @since 1.0
     **/
    template <typename _Type_t>
    _Value_t operator | (const PropertyT<_Type_t> &value) const {
        return (get() | value.get());
    }
    /*}}}*/
    // _Value_t operator & (const PropertyT<_Type_t> &value) const;/*{{{*/
    /**
     * Bitwise AND opeator.
     * "AND" the value of this property object with the passed \a value.
     * @tparam _Type_t Data type of the passed parameter. This doesn't need to
     * be informed. The compiler can deduce the type by the passed value.
     * @param value Another PropertyT object with the value to be "AND"ed with
     * the value of this property.
     * @return The result of the "AND"ed operation.
     * @remarks The value pointed by this property object is not changed in
     * this operation.
     * @since 1.0
     **/
    template <typename _Type_t>
    _Value_t operator & (const PropertyT<_Type_t> &value) const {
        return (get() & value.get());
    }
    /*}}}*/
    // _Value_t operator ^ (const PropertyT<_Type_t> &value) const;/*{{{*/
    /**
     * Bitwise XOR opeator.
     * "XOR" the value of this property object with the passed \a value.
     * @tparam _Type_t Data type of the passed parameter. This doesn't need to
     * be informed. The compiler can deduce the type by the passed value.
     * @param value Another PropertyT object with the value to be "XOR"ed with
     * the value of this property.
     * @return The result of the "XOR"ed operation.
     * @remarks The value pointed by this property object is not changed in
     * this operation.
     * @since 1.0
     **/
    template <typename _Type_t>
    _Value_t operator ^ (const PropertyT<_Type_t> &value) const {
        return (get() ^ value.get());
    }
    /*}}}*/
    // _Value_t operator | (const ss::PropertyT<_Type_t> &value) const;/*{{{*/
    /**
     * Bitwise OR opeator.
     * "OR" the value of this property object with the passed \a value.
     * @tparam _Type_t Data type of the passed parameter. This doesn't need to
     * be informed. The compiler can deduce the type by the passed value.
     * @param value Another PropertyT object with the value to be "OR"ed with
     * the value of this property.
     * @return The result of the "OR"ed operation.
     * @remarks The value pointed by this property object is not changed in
     * this operation.
     * @since 1.0
     **/
    template <typename _Type_t>
    _Value_t operator | (const ss::PropertyT<_Type_t> &value) const {
        return (get() | (_Value_t)value.get());
    }
    /*}}}*/
    // _Value_t operator & (const ss::PropertyT<_Type_t> &value) const;/*{{{*/
    /**
     * Bitwise AND opeator.
     * "AND" the value of this property object with the passed \a value.
     * @tparam _Type_t Data type of the passed parameter. This doesn't need to
     * be informed. The compiler can deduce the type by the passed value.
     * @param value Another PropertyT object with the value to be "AND"ed with
     * the value of this property.
     * @return The result of the "AND"ed operation.
     * @remarks The value pointed by this property object is not changed in
     * this operation.
     * @since 1.0
     **/
    template <typename _Type_t>
    _Value_t operator & (const ss::PropertyT<_Type_t> &value) const {
        return (get() & (_Value_t)value.get());
    }
    /*}}}*/
    // _Value_t operator ^ (const ss::PropertyT<_Type_t> &value) const;/*{{{*/
    /**
     * Bitwise XOR opeator.
     * "XOR" the value of this property object with the passed \a value.
     * @tparam _Type_t Data type of the passed parameter. This doesn't need to
     * be informed. The compiler can deduce the type by the passed value.
     * @param value Another PropertyT object with the value to be "XOR"ed with
     * the value of this property.
     * @return The result of the "XOR"ed operation.
     * @remarks The value pointed by this property object is not changed in
     * this operation.
     * @since 1.0
     **/
    template <typename _Type_t>
    _Value_t operator ^ (const ss::PropertyT<_Type_t> &value) const {
        return (get() ^ (_Value_t)value.get());
    }
    /*}}}*/
    // _Value_t operator | (const rw::PropertyT<_Get_t, _Set_t> &value) const;/*{{{*/
    /**
     * Bitwise OR opeator.
     * "OR" the value of this property object with the passed \a value.
     * @tparam _Get_t,_Set_t Data types of the `rw::PropertyT` template.
     * @param value Another PropertyT object with the value to be "OR"ed with
     * the value of this property.
     * @return The result of the "OR"ed operation.
     * @remarks The value pointed by this property object is not changed in
     * this operation.
     * @since 1.0
     **/
    template <typename _Get_t, typename _Set_t>
    _Value_t operator | (const rw::PropertyT<_Get_t, _Set_t> &value) const {
        return (get() | (_Value_t)value.get());
    }
    /*}}}*/
    // _Value_t operator & (const rw::PropertyT<_Get_t, _Set_t> &value) const;/*{{{*/
    /**
     * Bitwise AND opeator.
     * "AND" the value of this property object with the passed \a value.
     * @tparam _Get_t,_Set_t Data types of the `rw::PropertyT` template.
     * @param value Another PropertyT object with the value to be "AND"ed with
     * the value of this property.
     * @return The result of the "AND"ed operation.
     * @remarks The value pointed by this property object is not changed in
     * this operation.
     * @since 1.0
     **/
    template <typename _Get_t, typename _Set_t>
    _Value_t operator & (const rw::PropertyT<_Get_t, _Set_t> &value) const {
        return (get() & (_Value_t)value.get());
    }
    /*}}}*/
    // _Value_t operator ^ (const rw::PropertyT<_Get_t, _Set_t> &value) const;/*{{{*/
    /**
     * Bitwise XOR opeator.
     * "XOR" the value of this property object with the passed \a value.
     * @tparam _Get_t,_Set_t Data types of the `rw::PropertyT` template.
     * @param value Another PropertyT object with the value to be "XOR"ed with
     * the value of this property.
     * @return The result of the "XOR"ed operation.
     * @remarks The value pointed by this property object is not changed in
     * this operation.
     * @since 1.0
     **/
    template <typename _Get_t, typename _Set_t>
    _Value_t operator ^ (const rw::PropertyT<_Get_t, _Set_t> &value) const {
        return (get() ^ (_Value_t)value.get());
    }
    /*}}}*/
    //@}

    /** @name Boolean Operators */ //@{
    // bool operator ||(_Type_t value) const;/*{{{*/
    /**
     * Boolean "OR" operator.
     * @tparam _Type_t Data type of the passed parameter. This doesn't need to
     * be informed. The compiler can deduce the type by the passed value.
     * @param value The value to be compared.
     * @return A boolean value result of the comparison.
     * @since 1.0
     **/
    template <typename _Type_t>
    bool operator ||(_Type_t value) const {
        return (get() || value);
    }
    /*}}}*/
    // bool operator &&(_Type_t value) const;/*{{{*/
    /**
     * Boolean "AND" operator.
     * @tparam _Type_t Data type of the passed parameter. This doesn't need to
     * be informed. The compiler can deduce the type by the passed value.
     * @param value The value to be compared.
     * @return A boolean value result of the comparison.
     * @since 1.0
     **/
    template <typename _Type_t>
    bool operator &&(_Type_t value) const {
        return (get() && value);
    }
    /*}}}*/
    // bool operator ||(const PropertyT<_Type_t> &value) const;/*{{{*/
    /**
     * Boolean "OR" operator.
     * @tparam _Type_t Data type of the passed parameter. This doesn't need to
     * be informed. The compiler can deduce the type by the passed value.
     * @param value The value to be compared.
     * @return A boolean value result of the comparison.
     * @since 1.0
     **/
    template <typename _Type_t>
    bool operator ||(const PropertyT<_Type_t> &value) const {
        return (get() || value.get());
    }
    /*}}}*/
    // bool operator &&(const PropertyT<_Type_t> &value) const;/*{{{*/
    /**
     * Boolean "AND" operator.
     * @tparam _Type_t Data type of the passed parameter. This doesn't need to
     * be informed. The compiler can deduce the type by the passed value.
     * @param value The value to be compared.
     * @return A boolean value result of the comparison.
     * @since 1.0
     **/
    template <typename _Type_t>
    bool operator &&(const PropertyT<_Type_t> &value) const {
        return (get() && value.get());
    }
    /*}}}*/
    // bool operator ||(const rw::PropertyT<_Get_t, _Set_t> &value) const;/*{{{*/
    /**
     * Boolean "OR" operator.
     * @tparam _Get_t,_Set_t Data types of the `rw::PropertyT` template.
     * @param value The value to be compared.
     * @return A boolean value result of the comparison.
     * @since 1.0
     **/
    template <typename _Get_t, typename _Set_t>
    bool operator ||(const rw::PropertyT<_Get_t, _Set_t> &value) const {
        return (get() || value.get());
    }
    /*}}}*/
    // bool operator &&(const rw::PropertyT<_Get_t, _Set_t> &value) const;/*{{{*/
    /**
     * Boolean "AND" operator.
     * @tparam _Get_t,_Set_t Data types of the `rw::PropertyT` template.
     * @param value The value to be compared.
     * @return A boolean value result of the comparison.
     * @since 1.0
     **/
    template <typename _Get_t, typename _Set_t>
    bool operator &&(const rw::PropertyT<_Get_t, _Set_t> &value) const {
        return (get() && value.get());
    }
    /*}}}*/
    // bool operator ||(const ss::PropertyT<_Type_t> &value) const;/*{{{*/
    /**
     * Boolean "OR" operator.
     * @tparam _Type_t Data type of the passed parameter. This doesn't need to
     * be informed. The compiler can deduce the type by the passed value.
     * @param value The value to be compared.
     * @return A boolean value result of the comparison.
     * @since 1.0
     **/
    template <typename _Type_t>
    bool operator ||(const ss::PropertyT<_Type_t> &value) const {
        return (get() || value.get());
    }
    /*}}}*/
    // bool operator &&(const ss::PropertyT<_Type_t> &value) const;/*{{{*/
    /**
     * Boolean "AND" operator.
     * @tparam _Type_t Data type of the passed parameter. This doesn't need to
     * be informed. The compiler can deduce the type by the passed value.
     * @param value The value to be compared.
     * @return A boolean value result of the comparison.
     * @since 1.0
     **/
    template <typename _Type_t>
    bool operator &&(const ss::PropertyT<_Type_t> &value) const {
        return (get() && value.get());
    }
    /*}}}*/
    //@}

private:
    /** @name Disabled Constructor */ //@{
    // PropertyT(const PropertyT<_Value_t> &other);/*{{{*/
    /**
     * Disabled copy constructor.
     * @param other Another object to copy its values.
     * @since 1.0
     **/
    PropertyT(const PropertyT<_Value_t> &other) {
        other.get();
    }
    /*}}}*/
    //@}

    /** @name Disabled Operators */ //@{
    // void operator =(const PropertyT<_Value_t> &value);/*{{{*/
    /**
     * Assignment operator overloading.
     * @param value Another property to get its value.
     * @since 1.0
     **/
    void operator =(const PropertyT<_Value_t> &value) {
        value.get();
    }
    /*}}}*/
    // void operator =(const PropertyT<T> &value);/*{{{*/
    /**
     * The assignment operator is disabled.
     * @since 1.0
     **/
    template <typename T>
    void operator =(const PropertyT<T> &value) {
        value.get();
    }
    /*}}}*/
    //@}

    // Typedefs
    typedef _Value_t (*getfn_t)(void*);

    // Static Functions
    // static _Value_t getter(void *host) { }/*{{{*/
    /**
     * Calls the getter member function in \a host.
     * @tparam _Host_t The type of the host object.
     * @tparam _Method The member function to call.
     * @param host Pointer to the host instance.
     * @returns The same value returned by the \a host member function \a
     * _Method.
     * @since 1.0
     **/
    template <class _Host_t, _Value_t (_Host_t::*_Method)()>
    static _Value_t getter(void *host) {
        _Host_t *p = static_cast<_Host_t*>(host);
        return (p->*_Method)();
    }
    /*}}}*/
    // static _Value_t constget(void *host) { }/*{{{*/
    /**
     * The invoker method.
     * @since 1.0
     **/
    template <class _Host_t, _Value_t (_Host_t::*_Method)() const>
    static _Value_t constget(void *host) {
        _Host_t const *p = static_cast<_Host_t*>(host);
        return (p->*_Method)();
    }
    /*}}}*/

    // Data Members
    void *m_host;           /**< Void pointer to the host object. */
    getfn_t m_get;          /**< Getter function pointer.         */
};

}   /* namespace ro */

// bool operator ==(const ss::PropertyT<T> &left, const rw::PropertyT<G, S> &right) {/*{{{*/
/**
 * Comparison operator.
 * @param left An `ss::PropertyT` object to compare to.
 * @param right An `rw::PropertyT` object to compare with.
 * @return \b true if both properties have compatible and equal values. \b
 * false otherwise.
 * @remarks The underlining values of both properties must be implicit
 * convertible.
 * @since 1.0
 * @ingroup sstl_properties_operators
 **/
template <typename T, typename G, typename S>
bool operator ==(const ss::PropertyT<T> &left, const rw::PropertyT<G, S> &right) {
    return (left.get() == right.get());
}
/*}}}*/
// bool operator ==(const ss::PropertyT<T> &left, const ro::PropertyT<G> &right) {/*{{{*/
/**
 * Comparison operator.
 * @param left An `ss::PropertyT` object to compare to.
 * @param right An `ro::PropertyT` object to compare with.
 * @return \b true if both properties have compatible and equal values. \b
 * false otherwise.
 * @remarks The underlining values of both properties must be implicit
 * convertible.
 * @since 1.0
 * @ingroup sstl_properties_operators
 **/
template <typename T, typename G>
bool operator ==(const ss::PropertyT<T> &left, const ro::PropertyT<G> &right) {
    return (left.get() == right.get());
}
/*}}}*/
// bool operator ==(const rw::PropertyT<G, S> &left, const ss::PropertyT<T> &right) {/*{{{*/
/**
 * Comparison operator.
 * @param left An `rw::PropertyT` object to compare to.
 * @param right An `ss::PropertyT` object to compare with.
 * @return \b true if both properties have compatible and equal values. \b
 * false otherwise.
 * @remarks The underlining values of both properties must be implicit
 * convertible.
 * @since 1.0
 * @ingroup sstl_properties_operators
 **/
template <typename G, typename S, typename T>
bool operator ==(const rw::PropertyT<G, S> &left, const ss::PropertyT<T> &right) {
    return (left.get() == right.get());
}
/*}}}*/
// bool operator ==(const rw::PropertyT<G, S> &left, const ro::PropertyT<T> &right) {/*{{{*/
/**
 * Comparison operator.
 * @param left An `rw::PropertyT` object to compare to.
 * @param right An `ro::PropertyT` object to compare with.
 * @return \b true if both properties have compatible and equal values. \b
 * false otherwise.
 * @remarks The underlining values of both properties must be implicit
 * convertible.
 * @since 1.0
 * @ingroup sstl_properties_operators
 **/
template <typename G, typename S, typename T>
bool operator ==(const rw::PropertyT<G, S> &left, const ro::PropertyT<T> &right) {
    return (left.get() == right.get());
}
/*}}}*/

// bool operator !=(const ss::PropertyT<T> &left, const rw::PropertyT<G, S> &right) {/*{{{*/
/**
 * Inequality operator.
 * @param left An `ss::PropertyT` object to compare to.
 * @param right An `rw::PropertyT` object to compare with.
 * @return \b false if both properties have compatible and equal values. \b
 * true otherwise.
 * @remarks The underlining values of both properties must be implicit
 * convertible.
 * @since 1.0
 * @ingroup sstl_properties_operators
 **/
template <typename T, typename G, typename S>
bool operator !=(const ss::PropertyT<T> &left, const rw::PropertyT<G, S> &right) {
    return (left.get() != right.get());
}
/*}}}*/
// bool operator !=(const ss::PropertyT<T> &left, const ro::PropertyT<G> &right) {/*{{{*/
/**
 * Inequality operator.
 * @param left An `ss::PropertyT` object to compare to.
 * @param right An `ro::PropertyT` object to compare with.
 * @return \b false if both properties have compatible and equal values. \b
 * true otherwise.
 * @remarks The underlining values of both properties must be implicit
 * convertible.
 * @since 1.0
 * @ingroup sstl_properties_operators
 **/
template <typename T, typename G>
bool operator !=(const ss::PropertyT<T> &left, const ro::PropertyT<G> &right) {
    return (left.get() != right.get());
}
/*}}}*/
// bool operator !=(const rw::PropertyT<G, S> &left, const ss::PropertyT<T> &right) {/*{{{*/
/**
 * Inequality operator.
 * @param left An `rw::PropertyT` object to compare to.
 * @param right An `ss::PropertyT` object to compare with.
 * @return \b false if both properties have compatible and equal values. \b
 * true otherwise.
 * @remarks The underlining values of both properties must be implicit
 * convertible.
 * @since 1.0
 * @ingroup sstl_properties_operators
 **/
template <typename G, typename S, typename T>
bool operator !=(const rw::PropertyT<G, S> &left, const ss::PropertyT<T> &right) {
    return (left.get() != right.get());
}
/*}}}*/
// bool operator !=(const rw::PropertyT<G, S> &left, const ro::PropertyT<T> &right) {/*{{{*/
/**
 * Inequality operator.
 * @param left An `rw::PropertyT` object to compare to.
 * @param right An `ro::PropertyT` object to compare with.
 * @return \b false if both properties have compatible and equal values. \b
 * true otherwise.
 * @remarks The underlining values of both properties must be implicit
 * convertible.
 * @since 1.0
 * @ingroup sstl_properties_operators
 **/
template <typename G, typename S, typename T>
bool operator !=(const rw::PropertyT<G, S> &left, const ro::PropertyT<T> &right) {
    return (left.get() != right.get());
}
/*}}}*/

// bool operator < (const ss::PropertyT<T> &left, const rw::PropertyT<G, S> &right) {/*{{{*/
/**
 * Less than operator.
 * @param left An `ss::PropertyT` object to compare to.
 * @param right An `rw::PropertyT` object to compare with.
 * @return \b true when the value returned by this property object is
 * \it{less than} the passed \a value. \b false otherwise.
 * @remarks The underlining values of both properties must be implicit
 * convertible.
 * @since 1.0
 * @ingroup sstl_properties_operators
 **/
template <typename T, typename G, typename S>
bool operator < (const ss::PropertyT<T> &left, const rw::PropertyT<G, S> &right) {
    return (left.get() <  right.get());
}
/*}}}*/
// bool operator < (const ss::PropertyT<T> &left, const ro::PropertyT<G> &right) {/*{{{*/
/**
 * Less than operator.
 * @param left An `ss::PropertyT` object to compare to.
 * @param right An `ro::PropertyT` object to compare with.
 * @return \b true when the value returned by this property object is
 * \it{less than} the passed \a value. \b false otherwise.
 * @remarks The underlining values of both properties must be implicit
 * convertible.
 * @since 1.0
 * @ingroup sstl_properties_operators
 **/
template <typename T, typename G>
bool operator < (const ss::PropertyT<T> &left, const ro::PropertyT<G> &right) {
    return (left.get() <  right.get());
}
/*}}}*/
// bool operator < (const rw::PropertyT<G, S> &left, const ss::PropertyT<T> &right) {/*{{{*/
/**
 * Less than operator.
 * @param left An `rw::PropertyT` object to compare to.
 * @param right An `ss::PropertyT` object to compare with.
 * @return \b true when the value returned by this property object is
 * \it{less than} the passed \a value. \b false otherwise.
 * @remarks The underlining values of both properties must be implicit
 * convertible.
 * @since 1.0
 * @ingroup sstl_properties_operators
 **/
template <typename G, typename S, typename T>
bool operator < (const rw::PropertyT<G, S> &left, const ss::PropertyT<T> &right) {
    return (left.get() <  right.get());
}
/*}}}*/
// bool operator < (const rw::PropertyT<G, S> &left, const ro::PropertyT<T> &right) {/*{{{*/
/**
 * Less than operator.
 * @param left An `rw::PropertyT` object to compare to.
 * @param right An `ro::PropertyT` object to compare with.
 * @return \b true when the value returned by this property object is
 * \it{less than} the passed \a value. \b false otherwise.
 * @remarks The underlining values of both properties must be implicit
 * convertible.
 * @since 1.0
 * @ingroup sstl_properties_operators
 **/
template <typename G, typename S, typename T>
bool operator < (const rw::PropertyT<G, S> &left, const ro::PropertyT<T> &right) {
    return (left.get() <  right.get());
}
/*}}}*/

// bool operator <=(const ss::PropertyT<T> &left, const rw::PropertyT<G, S> &right) {/*{{{*/
/**
 * Less than or equal operator.
 * @param left An `ss::PropertyT` object to compare to.
 * @param right An `rw::PropertyT` object to compare with.
 * @return \b true when the value returned by this property object is
 * \it{less than or equal} the passed \a value. \b false otherwise.
 * @remarks The underlining values of both properties must be implicit
 * convertible.
 * @since 1.0
 * @ingroup sstl_properties_operators
 **/
template <typename T, typename G, typename S>
bool operator <=(const ss::PropertyT<T> &left, const rw::PropertyT<G, S> &right) {
    return (left.get() <= right.get());
}
/*}}}*/
// bool operator <=(const ss::PropertyT<T> &left, const ro::PropertyT<G> &right) {/*{{{*/
/**
 * Less than or equal operator.
 * @param left An `ss::PropertyT` object to compare to.
 * @param right An `ro::PropertyT` object to compare with.
 * @return \b true when the value returned by this property object is
 * \it{less than or equal} the passed \a value. \b false otherwise.
 * @remarks The underlining values of both properties must be implicit
 * convertible.
 * @since 1.0
 * @ingroup sstl_properties_operators
 **/
template <typename T, typename G>
bool operator <=(const ss::PropertyT<T> &left, const ro::PropertyT<G> &right) {
    return (left.get() <= right.get());
}
/*}}}*/
// bool operator <=(const rw::PropertyT<G, S> &left, const ss::PropertyT<T> &right) {/*{{{*/
/**
 * Less than or equal operator.
 * @param left An `rw::PropertyT` object to compare to.
 * @param right An `ss::PropertyT` object to compare with.
 * @return \b true when the value returned by this property object is
 * \it{less than or equal} the passed \a value. \b false otherwise.
 * @remarks The underlining values of both properties must be implicit
 * convertible.
 * @since 1.0
 * @ingroup sstl_properties_operators
 **/
template <typename G, typename S, typename T>
bool operator <=(const rw::PropertyT<G, S> &left, const ss::PropertyT<T> &right) {
    return (left.get() <= right.get());
}
/*}}}*/
// bool operator <=(const rw::PropertyT<G, S> &left, const ro::PropertyT<T> &right) {/*{{{*/
/**
 * Less than or equal operator.
 * @param left An `rw::PropertyT` object to compare to.
 * @param right An `ro::PropertyT` object to compare with.
 * @return \b true when the value returned by this property object is
 * \it{less than or equal} the passed \a value. \b false otherwise.
 * @remarks The underlining values of both properties must be implicit
 * convertible.
 * @since 1.0
 * @ingroup sstl_properties_operators
 **/
template <typename G, typename S, typename T>
bool operator <=(const rw::PropertyT<G, S> &left, const ro::PropertyT<T> &right) {
    return (left.get() <= right.get());
}
/*}}}*/

// bool operator > (const ss::PropertyT<T> &left, const rw::PropertyT<G, S> &right) {/*{{{*/
/**
 * Greater than operator.
 * @param left An `ss::PropertyT` object to compare to.
 * @param right An `rw::PropertyT` object to compare with.
 * @return \b true when the value returned by this property object is
 * \it{greater than} the passed \a value. \b false otherwise.
 * @remarks The underlining values of both properties must be implicit
 * convertible.
 * @since 1.0
 * @ingroup sstl_properties_operators
 **/
template <typename T, typename G, typename S>
bool operator > (const ss::PropertyT<T> &left, const rw::PropertyT<G, S> &right) {
    return (left.get() >  right.get());
}
/*}}}*/
// bool operator > (const ss::PropertyT<T> &left, const ro::PropertyT<G> &right) {/*{{{*/
/**
 * Greater than operator.
 * @param left An `ss::PropertyT` object to compare to.
 * @param right An `ro::PropertyT` object to compare with.
 * @return \b true when the value returned by this property object is
 * \it{greater than} the passed \a value. \b false otherwise.
 * @remarks The underlining values of both properties must be implicit
 * convertible.
 * @since 1.0
 * @ingroup sstl_properties_operators
 **/
template <typename T, typename G>
bool operator > (const ss::PropertyT<T> &left, const ro::PropertyT<G> &right) {
    return (left.get() >  right.get());
}
/*}}}*/
// bool operator > (const rw::PropertyT<G, S> &left, const ss::PropertyT<T> &right) {/*{{{*/
/**
 * Greater than operator.
 * @param left An `rw::PropertyT` object to compare to.
 * @param right An `ss::PropertyT` object to compare with.
 * @return \b true when the value returned by this property object is
 * \it{greater than} the passed \a value. \b false otherwise.
 * @remarks The underlining values of both properties must be implicit
 * convertible.
 * @since 1.0
 * @ingroup sstl_properties_operators
 **/
template <typename G, typename S, typename T>
bool operator > (const rw::PropertyT<G, S> &left, const ss::PropertyT<T> &right) {
    return (left.get() >  right.get());
}
/*}}}*/
// bool operator > (const rw::PropertyT<G, S> &left, const ro::PropertyT<T> &right) {/*{{{*/
/**
 * Greater than operator.
 * @param left An `rw::PropertyT` object to compare to.
 * @param right An `ro::PropertyT` object to compare with.
 * @return \b true when the value returned by this property object is
 * \it{greater than} the passed \a value. \b false otherwise.
 * @remarks The underlining values of both properties must be implicit
 * convertible.
 * @since 1.0
 * @ingroup sstl_properties_operators
 **/
template <typename G, typename S, typename T>
bool operator > (const rw::PropertyT<G, S> &left, const ro::PropertyT<T> &right) {
    return (left.get() >  right.get());
}
/*}}}*/

// bool operator >=(const ss::PropertyT<T> &left, const rw::PropertyT<G, S> &right) {/*{{{*/
/**
 * Greater than or equal operator.
 * @param left An `ss::PropertyT` object to compare to.
 * @param right An `rw::PropertyT` object to compare with.
 * @return \b true when the value returned by this property object is
 * \it{greater than or equal} the passed \a value. \b false otherwise.
 * @remarks The underlining values of both properties must be implicit
 * convertible.
 * @since 1.0
 * @ingroup sstl_properties_operators
 **/
template <typename T, typename G, typename S>
bool operator >=(const ss::PropertyT<T> &left, const rw::PropertyT<G, S> &right) {
    return (left.get() >= right.get());
}
/*}}}*/
// bool operator >=(const ss::PropertyT<T> &left, const ro::PropertyT<G> &right) {/*{{{*/
/**
 * Greater than or equal operator.
 * @param left An `ss::PropertyT` object to compare to.
 * @param right An `ro::PropertyT` object to compare with.
 * @return \b true when the value returned by this property object is
 * \it{greater than or equal} the passed \a value. \b false otherwise.
 * @remarks The underlining values of both properties must be implicit
 * convertible.
 * @since 1.0
 * @ingroup sstl_properties_operators
 **/
template <typename T, typename G>
bool operator >=(const ss::PropertyT<T> &left, const ro::PropertyT<G> &right) {
    return (left.get() >= right.get());
}
/*}}}*/
// bool operator >=(const rw::PropertyT<G, S> &left, const ss::PropertyT<T> &right) {/*{{{*/
/**
 * Greater than or equal operator.
 * @param left An `rw::PropertyT` object to compare to.
 * @param right An `ss::PropertyT` object to compare with.
 * @return \b true when the value returned by this property object is
 * \it{greater than or equal} the passed \a value. \b false otherwise.
 * @remarks The underlining values of both properties must be implicit
 * convertible.
 * @since 1.0
 * @ingroup sstl_properties_operators
 **/
template <typename G, typename S, typename T>
bool operator >=(const rw::PropertyT<G, S> &left, const ss::PropertyT<T> &right) {
    return (left.get() >= right.get());
}
/*}}}*/
// bool operator >=(const rw::PropertyT<G, S> &left, const ro::PropertyT<T> &right) {/*{{{*/
/**
 * Greater than or equal operator.
 * @param left An `rw::PropertyT` object to compare to.
 * @param right An `ro::PropertyT` object to compare with.
 * @return \b true when the value returned by this property object is
 * \it{greater than or equal} the passed \a value. \b false otherwise.
 * @remarks The underlining values of both properties must be implicit
 * convertible.
 * @since 1.0
 * @ingroup sstl_properties_operators
 **/
template <typename G, typename S, typename T>
bool operator >=(const rw::PropertyT<G, S> &left, const ro::PropertyT<T> &right) {
    return (left.get() >= right.get());
}
/*}}}*/

// T operator + (const ss::PropertyT<T> &left, const rw::PropertyT<G, S> &right) {/*{{{*/
/**
 * Subtraction operator.
 * @param left An `ss::PropertyT` object to compare to.
 * @param right An `rw::PropertyT` object to compare with.
 * @return The value resulting of the mathematical operation.
 * @remarks The underlining values of both properties must be implicit
 * convertible.
 * @since 1.0
 * @ingroup sstl_properties_operators
 **/
template <typename T, typename G, typename S>
T operator + (const ss::PropertyT<T> &left, const rw::PropertyT<G, S> &right) {
    return (left.get() +  right.get());
}
/*}}}*/
// T operator + (const ss::PropertyT<T> &left, const ro::PropertyT<G> &right) {/*{{{*/
/**
 * Subtraction operator.
 * @param left An `ss::PropertyT` object to compare to.
 * @param right An `ro::PropertyT` object to compare with.
 * @return The value resulting of the mathematical operation.
 * @remarks The underlining values of both properties must be implicit
 * convertible.
 * @since 1.0
 * @ingroup sstl_properties_operators
 **/
template <typename T, typename G>
T operator + (const ss::PropertyT<T> &left, const ro::PropertyT<G> &right) {
    return (left.get() +  right.get());
}
/*}}}*/
// T operator + (const rw::PropertyT<G, S> &left, const ss::PropertyT<T> &right) {/*{{{*/
/**
 * Subtraction operator.
 * @param left An `rw::PropertyT` object to compare to.
 * @param right An `ss::PropertyT` object to compare with.
 * @return The value resulting of the mathematical operation.
 * @remarks The underlining values of both properties must be implicit
 * convertible.
 * @since 1.0
 * @ingroup sstl_properties_operators
 **/
template <typename G, typename S, typename T>
T operator + (const rw::PropertyT<G, S> &left, const ss::PropertyT<T> &right) {
    return (left.get() +  right.get());
}
/*}}}*/
// T operator + (const rw::PropertyT<G, S> &left, const ro::PropertyT<T> &right) {/*{{{*/
/**
 * Addition operator.
 * @param left An `rw::PropertyT` object to compare to.
 * @param right An `ro::PropertyT` object to compare with.
 * @return The value resulting of the mathematical operation.
 * @remarks The underlining values of both properties must be implicit
 * convertible.
 * @since 1.0
 * @ingroup sstl_properties_operators
 **/
template <typename G, typename S, typename T>
T operator + (const rw::PropertyT<G, S> &left, const ro::PropertyT<T> &right) {
    return (left.get() +  right.get());
}
/*}}}*/

// T operator - (const ss::PropertyT<T> &left, const rw::PropertyT<G, S> &right) {/*{{{*/
/**
 * Subtraction operator.
 * @param left An `ss::PropertyT` object to compare to.
 * @param right An `rw::PropertyT` object to compare with.
 * @return The value resulting of the mathematical operation.
 * @remarks The underlining values of both properties must be implicit
 * convertible.
 * @since 1.0
 * @ingroup sstl_properties_operators
 **/
template <typename T, typename G, typename S>
T operator - (const ss::PropertyT<T> &left, const rw::PropertyT<G, S> &right) {
    return (left.get() -  right.get());
}
/*}}}*/
// T operator - (const ss::PropertyT<T> &left, const ro::PropertyT<G> &right) {/*{{{*/
/**
 * Subtraction operator.
 * @param left An `ss::PropertyT` object to compare to.
 * @param right An `ro::PropertyT` object to compare with.
 * @return The value resulting of the mathematical operation.
 * @remarks The underlining values of both properties must be implicit
 * convertible.
 * @since 1.0
 * @ingroup sstl_properties_operators
 **/
template <typename T, typename G>
T operator - (const ss::PropertyT<T> &left, const ro::PropertyT<G> &right) {
    return (left.get() -  right.get());
}
/*}}}*/
// T operator - (const rw::PropertyT<G, S> &left, const ss::PropertyT<T> &right) {/*{{{*/
/**
 * Subtraction operator.
 * @param left An `rw::PropertyT` object to compare to.
 * @param right An `ss::PropertyT` object to compare with.
 * @return The value resulting of the mathematical operation.
 * @remarks The underlining values of both properties must be implicit
 * convertible.
 * @since 1.0
 * @ingroup sstl_properties_operators
 **/
template <typename G, typename S, typename T>
T operator - (const rw::PropertyT<G, S> &left, const ss::PropertyT<T> &right) {
    return (left.get() -  right.get());
}
/*}}}*/
// T operator - (const rw::PropertyT<G, S> &left, const ro::PropertyT<T> &right) {/*{{{*/
/**
 * Subtraction operator.
 * @param left An `rw::PropertyT` object to compare to.
 * @param right An `ro::PropertyT` object to compare with.
 * @return The value resulting of the mathematical operation.
 * @remarks The underlining values of both properties must be implicit
 * convertible.
 * @since 1.0
 * @ingroup sstl_properties_operators
 **/
template <typename G, typename S, typename T>
T operator - (const rw::PropertyT<G, S> &left, const ro::PropertyT<T> &right) {
    return (left.get() -  right.get());
}
/*}}}*/

// T operator * (const ss::PropertyT<T> &left, const rw::PropertyT<G, S> &right) {/*{{{*/
/**
 * Multiplication operator.
 * @param left An `ss::PropertyT` object to compare to.
 * @param right An `rw::PropertyT` object to compare with.
 * @return The value resulting of the mathematical operation.
 * @remarks The underlining values of both properties must be implicit
 * convertible.
 * @since 1.0
 * @ingroup sstl_properties_operators
 **/
template <typename T, typename G, typename S>
T operator * (const ss::PropertyT<T> &left, const rw::PropertyT<G, S> &right) {
    return (left.get() *  right.get());
}
/*}}}*/
// T operator * (const ss::PropertyT<T> &left, const ro::PropertyT<G> &right) {/*{{{*/
/**
 * Multiplication operator.
 * @param left An `ss::PropertyT` object to compare to.
 * @param right An `ro::PropertyT` object to compare with.
 * @return The value resulting of the mathematical operation.
 * @remarks The underlining values of both properties must be implicit
 * convertible.
 * @since 1.0
 * @ingroup sstl_properties_operators
 **/
template <typename T, typename G>
T operator * (const ss::PropertyT<T> &left, const ro::PropertyT<G> &right) {
    return (left.get() *  right.get());
}
/*}}}*/
// T operator * (const rw::PropertyT<G, S> &left, const ss::PropertyT<T> &right) {/*{{{*/
/**
 * Multiplication operator.
 * @param left An `rw::PropertyT` object to compare to.
 * @param right An `ss::PropertyT` object to compare with.
 * @return The value resulting of the mathematical operation.
 * @remarks The underlining values of both properties must be implicit
 * convertible.
 * @since 1.0
 * @ingroup sstl_properties_operators
 **/
template <typename G, typename S, typename T>
T operator * (const rw::PropertyT<G, S> &left, const ss::PropertyT<T> &right) {
    return (left.get() *  right.get());
}
/*}}}*/
// T operator * (const rw::PropertyT<G, S> &left, const ro::PropertyT<T> &right) {/*{{{*/
/**
 * Multiplication operator.
 * @param left An `rw::PropertyT` object to compare to.
 * @param right An `ro::PropertyT` object to compare with.
 * @return The value resulting of the mathematical operation.
 * @remarks The underlining values of both properties must be implicit
 * convertible.
 * @since 1.0
 * @ingroup sstl_properties_operators
 **/
template <typename G, typename S, typename T>
T operator * (const rw::PropertyT<G, S> &left, const ro::PropertyT<T> &right) {
    return (left.get() *  right.get());
}
/*}}}*/

// T operator / (const ss::PropertyT<T> &left, const rw::PropertyT<G, S> &right) {/*{{{*/
/**
 * Division operator.
 * @param left An `ss::PropertyT` object to compare to.
 * @param right An `rw::PropertyT` object to compare with.
 * @return The value resulting of the mathematical operation.
 * @remarks The underlining values of both properties must be implicit
 * convertible.
 * @since 1.0
 * @ingroup sstl_properties_operators
 **/
template <typename T, typename G, typename S>
T operator / (const ss::PropertyT<T> &left, const rw::PropertyT<G, S> &right) {
    return (left.get() /  right.get());
}
/*}}}*/
// T operator / (const ss::PropertyT<T> &left, const ro::PropertyT<G> &right) {/*{{{*/
/**
 * Division operator.
 * @param left An `ss::PropertyT` object to compare to.
 * @param right An `ro::PropertyT` object to compare with.
 * @return The value resulting of the mathematical operation.
 * @remarks The underlining values of both properties must be implicit
 * convertible.
 * @since 1.0
 * @ingroup sstl_properties_operators
 **/
template <typename T, typename G>
T operator / (const ss::PropertyT<T> &left, const ro::PropertyT<G> &right) {
    return (left.get() /  right.get());
}
/*}}}*/
// T operator / (const rw::PropertyT<G, S> &left, const ss::PropertyT<T> &right) {/*{{{*/
/**
 * Division operator.
 * @param left An `rw::PropertyT` object to compare to.
 * @param right An `ss::PropertyT` object to compare with.
 * @return The value resulting of the mathematical operation.
 * @remarks The underlining values of both properties must be implicit
 * convertible.
 * @since 1.0
 * @ingroup sstl_properties_operators
 **/
template <typename G, typename S, typename T>
T operator / (const rw::PropertyT<G, S> &left, const ss::PropertyT<T> &right) {
    return (left.get() /  right.get());
}
/*}}}*/
// T operator / (const rw::PropertyT<G, S> &left, const ro::PropertyT<T> &right) {/*{{{*/
/**
 * Division operator.
 * @param left An `rw::PropertyT` object to compare to.
 * @param right An `ro::PropertyT` object to compare with.
 * @return The value resulting of the mathematical operation.
 * @remarks The underlining values of both properties must be implicit
 * convertible.
 * @since 1.0
 * @ingroup sstl_properties_operators
 **/
template <typename G, typename S, typename T>
T operator / (const rw::PropertyT<G, S> &left, const ro::PropertyT<T> &right) {
    return (left.get() /  right.get());
}
/*}}}*/

// T operator | (const ss::PropertyT<T> &left, const rw::PropertyT<G, S> &right) {/*{{{*/
/**
 * Bitwise OR opeator.
 * @param left An `ss::PropertyT` object to compare to.
 * @param right An `rw::PropertyT` object to compare with.
 * @return The result of the "OR"ed operation.
 * @remarks The underlining values of both properties must be implicit
 * convertible.
 * @since 1.0
 * @ingroup sstl_properties_operators
 **/
template <typename T, typename G, typename S>
T operator | (const ss::PropertyT<T> &left, const rw::PropertyT<G, S> &right) {
    return (left.get() |  right.get());
}
/*}}}*/
// T operator | (const ss::PropertyT<T> &left, const ro::PropertyT<G> &right) {/*{{{*/
/**
 * Bitwise OR opeator.
 * @param left An `ss::PropertyT` object to compare to.
 * @param right An `ro::PropertyT` object to compare with.
 * @return The result of the "OR"ed operation.
 * @remarks The underlining values of both properties must be implicit
 * convertible.
 * @since 1.0
 * @ingroup sstl_properties_operators
 **/
template <typename T, typename G>
T operator | (const ss::PropertyT<T> &left, const ro::PropertyT<G> &right) {
    return (left.get() |  right.get());
}
/*}}}*/
// T operator | (const rw::PropertyT<G, S> &left, const ss::PropertyT<T> &right) {/*{{{*/
/**
 * Bitwise OR opeator.
 * @param left An `rw::PropertyT` object to compare to.
 * @param right An `ss::PropertyT` object to compare with.
 * @return The result of the "OR"ed operation.
 * @remarks The underlining values of both properties must be implicit
 * convertible.
 * @since 1.0
 * @ingroup sstl_properties_operators
 **/
template <typename G, typename S, typename T>
T operator | (const rw::PropertyT<G, S> &left, const ss::PropertyT<T> &right) {
    return (left.get() |  right.get());
}
/*}}}*/
// T operator | (const rw::PropertyT<G, S> &left, const ro::PropertyT<T> &right) {/*{{{*/
/**
 * Bitwise OR opeator.
 * @param left An `rw::PropertyT` object to compare to.
 * @param right An `ro::PropertyT` object to compare with.
 * @return The result of the "OR"ed operation.
 * @remarks The underlining values of both properties must be implicit
 * convertible.
 * @since 1.0
 * @ingroup sstl_properties_operators
 **/
template <typename G, typename S, typename T>
T operator | (const rw::PropertyT<G, S> &left, const ro::PropertyT<T> &right) {
    return (left.get() |  right.get());
}
/*}}}*/

// T operator & (const ss::PropertyT<T> &left, const rw::PropertyT<G, S> &right) {/*{{{*/
/**
 * Bitwise AND opeator.
 * @param left An `ss::PropertyT` object to compare to.
 * @param right An `rw::PropertyT` object to compare with.
 * @return The result of the "AND"ed operation.
 * @remarks The underlining values of both properties must be implicit
 * convertible.
 * @since 1.0
 * @ingroup sstl_properties_operators
 **/
template <typename T, typename G, typename S>
T operator & (const ss::PropertyT<T> &left, const rw::PropertyT<G, S> &right) {
    return (left.get() &  right.get());
}
/*}}}*/
// T operator & (const ss::PropertyT<T> &left, const ro::PropertyT<G> &right) {/*{{{*/
/**
 * Bitwise AND opeator.
 * @param left An `ss::PropertyT` object to compare to.
 * @param right An `ro::PropertyT` object to compare with.
 * @return The result of the "AND"ed operation.
 * @remarks The underlining values of both properties must be implicit
 * convertible.
 * @since 1.0
 * @ingroup sstl_properties_operators
 **/
template <typename T, typename G>
T operator & (const ss::PropertyT<T> &left, const ro::PropertyT<G> &right) {
    return (left.get() &  right.get());
}
/*}}}*/
// T operator & (const rw::PropertyT<G, S> &left, const ss::PropertyT<T> &right) {/*{{{*/
/**
 * Bitwise AND opeator.
 * @param left An `rw::PropertyT` object to compare to.
 * @param right An `ss::PropertyT` object to compare with.
 * @return The result of the "AND"ed operation.
 * @remarks The underlining values of both properties must be implicit
 * convertible.
 * @since 1.0
 * @ingroup sstl_properties_operators
 **/
template <typename G, typename S, typename T>
T operator & (const rw::PropertyT<G, S> &left, const ss::PropertyT<T> &right) {
    return (left.get() &  right.get());
}
/*}}}*/
// T operator & (const rw::PropertyT<G, S> &left, const ro::PropertyT<T> &right) {/*{{{*/
/**
 * Bitwise AND opeator.
 * @param left An `rw::PropertyT` object to compare to.
 * @param right An `ro::PropertyT` object to compare with.
 * @return The result of the "AND"ed operation.
 * @remarks The underlining values of both properties must be implicit
 * convertible.
 * @since 1.0
 * @ingroup sstl_properties_operators
 **/
template <typename G, typename S, typename T>
T operator & (const rw::PropertyT<G, S> &left, const ro::PropertyT<T> &right) {
    return (left.get() &  right.get());
}
/*}}}*/

// T operator ^ (const ss::PropertyT<T> &left, const rw::PropertyT<G, S> &right) {/*{{{*/
/**
 * Bitwise XOR opeator.
 * @param left An `ss::PropertyT` object to compare to.
 * @param right An `rw::PropertyT` object to compare with.
 * @return The result of the "XOR"ed operation.
 * @remarks The underlining values of both properties must be implicit
 * convertible.
 * @since 1.0
 * @ingroup sstl_properties_operators
 **/
template <typename T, typename G, typename S>
T operator ^ (const ss::PropertyT<T> &left, const rw::PropertyT<G, S> &right) {
    return (left.get() ^  right.get());
}
/*}}}*/
// T operator ^ (const ss::PropertyT<T> &left, const ro::PropertyT<G> &right) {/*{{{*/
/**
 * Bitwise XOR opeator.
 * @param left An `ss::PropertyT` object to compare to.
 * @param right An `ro::PropertyT` object to compare with.
 * @return The result of the "XOR"ed operation.
 * @remarks The underlining values of both properties must be implicit
 * convertible.
 * @since 1.0
 * @ingroup sstl_properties_operators
 **/
template <typename T, typename G>
T operator ^ (const ss::PropertyT<T> &left, const ro::PropertyT<G> &right) {
    return (left.get() ^  right.get());
}
/*}}}*/
// T operator ^ (const rw::PropertyT<G, S> &left, const ss::PropertyT<T> &right) {/*{{{*/
/**
 * Bitwise XOR opeator.
 * @param left An `rw::PropertyT` object to compare to.
 * @param right An `ss::PropertyT` object to compare with.
 * @return The result of the "XOR"ed operation.
 * @remarks The underlining values of both properties must be implicit
 * convertible.
 * @since 1.0
 * @ingroup sstl_properties_operators
 **/
template <typename G, typename S, typename T>
T operator ^ (const rw::PropertyT<G, S> &left, const ss::PropertyT<T> &right) {
    return (left.get() ^  right.get());
}
/*}}}*/
// T operator ^ (const rw::PropertyT<G, S> &left, const ro::PropertyT<T> &right) {/*{{{*/
/**
 * Bitwise XOR opeator.
 * @param left An `rw::PropertyT` object to compare to.
 * @param right An `ro::PropertyT` object to compare with.
 * @return The result of the "XOR"ed operation.
 * @remarks The underlining values of both properties must be implicit
 * convertible.
 * @since 1.0
 * @ingroup sstl_properties_operators
 **/
template <typename G, typename S, typename T>
T operator ^ (const rw::PropertyT<G, S> &left, const ro::PropertyT<T> &right) {
    return (left.get() ^  right.get());
}
/*}}}*/

// bool operator ||(const ss::PropertyT<T> &left, const rw::PropertyT<G, S> &right) {/*{{{*/
/**
 * Boolean "OR" operator.
 * @param left An `ss::PropertyT` object to compare to.
 * @param right An `rw::PropertyT` object to compare with.
 * @return A boolean value result of the comparison.
 * @remarks The underlining values of both properties must be implicit
 * convertible.
 * @since 1.0
 * @ingroup sstl_properties_operators
 **/
template <typename T, typename G, typename S>
bool operator ||(const ss::PropertyT<T> &left, const rw::PropertyT<G, S> &right) {
    return (left.get() || right.get());
}
/*}}}*/
// bool operator ||(const ss::PropertyT<T> &left, const ro::PropertyT<G> &right) {/*{{{*/
/**
 * Boolean "OR" operator.
 * @param left An `ss::PropertyT` object to compare to.
 * @param right An `ro::PropertyT` object to compare with.
 * @return A boolean value result of the comparison.
 * @remarks The underlining values of both properties must be implicit
 * convertible.
 * @since 1.0
 * @ingroup sstl_properties_operators
 **/
template <typename T, typename G>
bool operator ||(const ss::PropertyT<T> &left, const ro::PropertyT<G> &right) {
    return (left.get() || right.get());
}
/*}}}*/
// bool operator ||(const rw::PropertyT<G, S> &left, const ss::PropertyT<T> &right) {/*{{{*/
/**
 * Boolean "OR" operator.
 * @param left An `rw::PropertyT` object to compare to.
 * @param right An `ss::PropertyT` object to compare with.
 * @return A boolean value result of the comparison.
 * @remarks The underlining values of both properties must be implicit
 * convertible.
 * @since 1.0
 * @ingroup sstl_properties_operators
 **/
template <typename G, typename S, typename T>
bool operator ||(const rw::PropertyT<G, S> &left, const ss::PropertyT<T> &right) {
    return (left.get() || right.get());
}
/*}}}*/
// bool operator ||(const rw::PropertyT<G, S> &left, const ro::PropertyT<T> &right) {/*{{{*/
/**
 * Boolean "OR" operator.
 * @param left An `rw::PropertyT` object to compare to.
 * @param right An `ro::PropertyT` object to compare with.
 * @return A boolean value result of the comparison.
 * @remarks The underlining values of both properties must be implicit
 * convertible.
 * @since 1.0
 * @ingroup sstl_properties_operators
 **/
template <typename G, typename S, typename T>
bool operator ||(const rw::PropertyT<G, S> &left, const ro::PropertyT<T> &right) {
    return (left.get() || right.get());
}
/*}}}*/

// bool operator &&(const ss::PropertyT<T> &left, const rw::PropertyT<G, S> &right) {/*{{{*/
/**
 * Boolean "AND" operator.
 * @param left An `ss::PropertyT` object to compare to.
 * @param right An `rw::PropertyT` object to compare with.
 * @return A boolean value result of the comparison.
 * @remarks The underlining values of both properties must be implicit
 * convertible.
 * @since 1.0
 * @ingroup sstl_properties_operators
 **/
template <typename T, typename G, typename S>
bool operator &&(const ss::PropertyT<T> &left, const rw::PropertyT<G, S> &right) {
    return (left.get() && right.get());
}
/*}}}*/
// bool operator &&(const ss::PropertyT<T> &left, const ro::PropertyT<G> &right) {/*{{{*/
/**
 * Boolean "AND" operator.
 * @param left An `ss::PropertyT` object to compare to.
 * @param right An `ro::PropertyT` object to compare with.
 * @return A boolean value result of the comparison.
 * @remarks The underlining values of both properties must be implicit
 * convertible.
 * @since 1.0
 * @ingroup sstl_properties_operators
 **/
template <typename T, typename G>
bool operator &&(const ss::PropertyT<T> &left, const ro::PropertyT<G> &right) {
    return (left.get() && right.get());
}
/*}}}*/
// bool operator &&(const rw::PropertyT<G, S> &left, const ss::PropertyT<T> &right) {/*{{{*/
/**
 * Boolean "AND" operator.
 * @param left An `rw::PropertyT` object to compare to.
 * @param right An `ss::PropertyT` object to compare with.
 * @return A boolean value result of the comparison.
 * @remarks The underlining values of both properties must be implicit
 * convertible.
 * @since 1.0
 * @ingroup sstl_properties_operators
 **/
template <typename G, typename S, typename T>
bool operator &&(const rw::PropertyT<G, S> &left, const ss::PropertyT<T> &right) {
    return (left.get() && right.get());
}
/*}}}*/
// bool operator &&(const rw::PropertyT<G, S> &left, const ro::PropertyT<T> &right) {/*{{{*/
/**
 * Boolean "AND" operator.
 * @param left An `rw::PropertyT` object to compare to.
 * @param right An `ro::PropertyT` object to compare with.
 * @return A boolean value result of the comparison.
 * @remarks The underlining values of both properties must be implicit
 * convertible.
 * @since 1.0
 * @ingroup sstl_properties_operators
 **/
template <typename G, typename S, typename T>
bool operator &&(const rw::PropertyT<G, S> &left, const ro::PropertyT<T> &right) {
    return (left.get() && right.get());
}
/*}}}*/

// #define GS(_Class_t, _GetterFn, _SetterFn)  _Class_t, & _Class_t :: _GetterFn, & _Class_t :: _SetterFn/*{{{*/
/**
 * Macro to help pass arguments to `ss::PropertyT` and `rw::PropertyT`
 * objects.
 * @param _Class_t Type of the class holding the getter and setter functions.
 * @param _GetterFn Name of the getter member function.
 * @param _SetterFn Name of the setter member function.
 * @remarks The macro expands its arguments in a way that they will fit in the
 * list of template parameter of the functions `bind()`. This macro can be
 * used in the following way:
 ~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * class MyObject {
 * public:
 *     ss::PropertyT<int> count;
 *
 *     MyObject();
 *
 * private:
 *     int getCount() const;
 *     void setCount(int value);
 * };
 *
 * // In the implementation of the constructor:
 * MyObject::MyObject() {
 *     count.bind< GS(MyObject, getCount, setCount) >(this);
 * }
 ~~~~~~~~~~~~~~~~~~~~~
 * The example shows usage with `ss::PropertyT` class but it is also valid for
 * `rw::PropertyT` classes.
 * @note \b GS stands for \bf{G}etter and \bf{S}etter.
 * @since 1.0
 **/
#define GS(_Class_t, _GetterFn, _SetterFn)  _Class_t, & _Class_t :: _GetterFn, & _Class_t :: _SetterFn
/*}}}*/

#endif /* __SSTLPROP_HPP_DEFINED__ */

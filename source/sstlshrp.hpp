/**
 * @file
 * Declares the SharedT class template.
 *
 * @author Alessandro Antonello
 * @date   jan 12, 2016
 * @since  Super Simple Template Library 1.0
 *
 * @copyright 2016, Paralaxe Tecnologia Ltda.. All rights reserved.
 **/
#ifndef __SSTLSHRP_HPP_DEFINED__
#define __SSTLSHRP_HPP_DEFINED__

namespace ss {

/**
 * Shared pointer template implementation.
 * Provides a shared pointer with a reference counting mechanism. A shared
 * pointer can be created on any object pointer. Same template instantiations
 * can share the same pointer instance safelly. The pointer is guarded until
 * the last reference is released. Then, the pointer is deleted from memory.
 * @tparam _Class_t The pointer type.
 * @par Example:
 ~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * class MyClass {
 * public:
 *     MyClass();
 *
 *     int data;
 *     int operation();
 * };
 *
 * ss::SharedT<MyClass> pointerA(new MyClass);  // Build shared pointer.
 * if (pointerA)        // Check if it is bounded.
 *     pointerA->operation();   // Call pointer members.
 *
 * ss::SharedT<MyClass> pointerB(pointerA);    // Sharing the pointer.
 * pointerB->data = 5;      // Both pointers have the same value.
 *
 * ss::SharedT<MyClass> pointerC;           // Default constructor has no bound.
 * if (!pointerC)
 *     pointerC = pointerB;     // All 3 pointer share the same instance of MyClass.
 *
 * // When all pointers goes out of scope, the MyClass instance will be deleted.
 ~~~~~~~~~~~~~~~~~~~~~
 * @since 1.0
 * @ingroup sstl_shared
 *//* --------------------------------------------------------------------- */
template <class _Class_t>
class SharedT
{
public:
    // Data Types
    typedef _Class_t class_t;           /**< Typedef of the class type.     */
    typedef void (*release_t)(class_t *data);   /**< Release function type. */
    // struct pointer_t;/*{{{*/
    /**
     * holds the data bound to this template.
     * @since 1.0
     **/
    struct pointer_t {
        volatile intptr_t refs;             /**< Reference count.   */
        class_t *data;                      /**< Shared pointer.    */
        release_t deleteFun;                /**< Deleter function.  */

        // pointer_t(class_t *ptr);/*{{{*/
        /**
         * Default constructor.
         * @param ptr Pointer to bound to this object.
         * @since 1.0
         **/
        pointer_t(class_t *ptr) : refs(1), data(ptr),
            deleteFun(&pointer_t::release)      /* Use default. */
        { }
        /*}}}*/
        // pointer_t(class_t *ptr, release_t funPtr);/*{{{*/
        /**
         * Parametrized constructor.
         * @param ptr The data pointer to hold.
         * @param funPtr Pointer to the function that will release the data of
         * the data pointer. Can be \b NULL.
         * @since 1.0
         **/
        pointer_t(class_t *ptr, release_t funPtr) :
            refs(1), data(ptr), deleteFun(funPtr)
        {
            if (!funPtr) deleteFun = &pointer_t::release;
        }
        /*}}}*/
        // ~pointer_t();/*{{{*/
        /**
         * Destructor.
         * Deletes the bounded pointer.
         * @since 1.0
         **/
        ~pointer_t() {
            (*deleteFun)(data);
        }
        /*}}}*/

        // static void release(class_t *pointer);/*{{{*/
        /**
         * Default release function when no other is defined.
         * @param pointer Pointer to the data to be released.
         * @since 1.0
         **/
        static void release(class_t *pointer) {
            if (!pointer) return;
            delete pointer;
        }
        /*}}}*/
    };
    /*}}}*/

public:
    /** @name Constructors & Destructor */ //@{
    // SharedT();/*{{{*/
    /**
     * Default constructor.
     * Builds an empty object. It is bound to anything.
     * @since 1.0
     **/
    SharedT() { m_pointer = NULL; }
    /*}}}*/
    // SharedT(const SharedT<class_t> &other);/*{{{*/
    /**
     * Copy constructor.
     * @param other Another template class instance built with the same class
     * type. The pointer held by \p other will be shared with this instance.
     * Reference counting will be incremented.
     * @since 1.0
     **/
    SharedT(const SharedT<class_t> &other) {
        m_pointer = other.m_pointer;
        retain();
    }
    /*}}}*/
    // explicit SharedT(class_t *ptr);/*{{{*/
    /**
     * Parametrized constructor.
     * @param ptr Pointer to the object to be bound. No reference increment
     * will be done. This instance will be the first reference of this object.
     * It must not be held on any other place and be shared only using
     * instances of \c SharedT objects.
     * @since 1.0
     **/
    explicit SharedT(class_t *ptr) {
        m_pointer = new pointer_t(ptr);
    }
    /*}}}*/
    // SharedT(class_t *ptr, void (*destructor)(_Class_t*));/*{{{*/
    /**
     * Parametrized constructor.
     * @param ptr Pointer to the object to be bound. No reference increment
     * will be done. This instance will be the first reference of this object.
     * It must not be held on any other place and be shared only using
     * instances of \c SharedT objects.
     * @param destructor Pointer to a destructor function. When provided this
     * function will be used to release the data passed in the \p ptr
     * parameter. Cannot be \b NULL.
     * @since 1.0
     **/
    SharedT(class_t *ptr, void (*destructor)(_Class_t*)) :
        m_pointer(new pointer_t(ptr), destructor) { }
    /*}}}*/
    // ~SharedT();/*{{{*/
    /**
     * Default destructor.
     * Decrements the reference counting of the bound pointer. When the
     * counting reaches zero the pointer is deleted from memory.
     * @since 1.0
     **/
    ~SharedT() {
        release();
    }
    /*}}}*/
    //@}

    /** @name Attributes */ //@{
    // class_t* data() const;/*{{{*/
    /**
     * Retrieves the internal data pointer, if any.
     * @return A pointer to the internal shared object.
     * @warning This function should be used only to share the internal
     * pointer with another instance of %SharedT object. Also, you
     * should never __delete__ the returned pointer.
     * @since 1.0
     **/
    class_t* data() const {
        return ((m_pointer != NULL) ? m_pointer->data : NULL);
    }
    /*}}}*/
    // intptr_t shares() const;/*{{{*/
    /**
     * Retrieves the number of shared instances using the underlined pointer.
     * @return An signed integer. If this number is 1, this is the only
     * instance holding the bound data. If the resulting number is 0, the
     * bound data was deleted.
     * @since 1.0
     **/
    intptr_t shares() const {
        return (m_pointer ? m_pointer->refs : 0);
    }
    /*}}}*/
    //@}

    /** @name Operations */ //@{
    // SharedT<class_t>& assign(const SharedT<class_t> &other);/*{{{*/
    /**
     * Assignment operation.
     * @param other Another template class instance built with the same class
     * type. The pointer held by \p other will be shared with this instance.
     * Reference counting will be incremented.
     * @return A reference to \b this instance.
     * @since 1.0
     **/
    SharedT<class_t>& assign(const SharedT<class_t> &other) {
        if (m_pointer == other.m_pointer) return *this;     /* Nothing needed. */
        release();
        m_pointer = other.m_pointer;
        retain();
        return *this;
    }
    /*}}}*/
    // SharedT<class_t>& assign(class_t *ptr, void (*destructor)(class_t*));/*{{{*/
    /**
     * Assignment Operation.
     * @param ptr Pointer to the object to be bound. No reference increment
     * will be done. This instance will be the first reference of this object.
     * It must not be held on any other place and be shared only using
     * instances of \c SharedT objects.
     * @param destructor Custom function to use as destructor of the object
     * passed through \p ptr parameter. Must not be \b NULL.
     * @return A reference to this instance.
     * @since 1.0
     **/
    SharedT<class_t>& assign(class_t *ptr, void (*destructor)(class_t*)) {
        if (ptr == data()) return *this;
        release();
        m_pointer = new pointer_t(ptr, destructor);
        return *this;
    }
    /*}}}*/
    //@}

    /** @name Overloaded Operators */ //@{
    // operator bool() const;/*{{{*/
    /**
     * Casting to \b bool operator.
     * @returns \b true if this instance is bound to a pointer. \b false
     * otherwise.
     * @remarks This operator is useful to be used on \b if constructions as
     * of it was a simple pointer:
     ~~~~~~~~~~~~~~~~~~~~~{.cpp}
     * SharedT<AClass> pointer(new AClass);
     * if (pointer)
     *     // pointer is bound to an instance of AClass.
     ~~~~~~~~~~~~~~~~~~~~~
     * @since 1.0
     **/
    operator bool() const {
        return (data() != NULL);
    }
    /*}}}*/
    // bool operator!() const;/*{{{*/
    /**
     * Negation operator.
     * Useful on \b if constructions to check whether the shared pointer is
     * not bounded to a class instance.
     ~~~~~~~~~~~~~~~~~~~~~{.cpp}
     * SharedT<AClass> pointer;
     * if (!pointer)
     *     pointer = aclass_ptr;
     ~~~~~~~~~~~~~~~~~~~~~
     * @return \b true when the pointer is not bounded to any instance. \b
     * false otherwise.
     * @since 1.0
     **/
    bool operator!() const {
        return (data() == NULL);
    }
    /*}}}*/
    // class_t* operator->() const;/*{{{*/
    /**
     * Point to member operator.
     * Allows access to members of the internal pointer.
     * @return The internal shared pointer instance. Can be \b NULL.
     * @since 1.0
     **/
    class_t* operator->() const {
        return data();
    }
    /*}}}*/
    // SharedT<class_t>& operator=(const SharedT<class_t> &other);/*{{{*/
    /**
     * Assignment operator.
     * @param other Another template class instance built with the same class
     * type. The pointer held by \p other will be shared with this instance.
     * Reference counting will be incremented.
     * @return A reference to \b this instance.
     * @since 1.0
     **/
    SharedT<class_t>& operator=(const SharedT<class_t> &other) {
        return assign(other);
    }
    /*}}}*/
    // SharedT<class_t>& operator=(class_t *ptr);/*{{{*/
    /**
     * Assignment operator overload.
     * @param ptr Pointer to the object to be bound. No reference increment
     * will be done. This instance will be the first reference of this object.
     * It must not be held on any other place and be shared only using
     * instances of \c SharedT objects.
     * @remarks Using this operation will not allow to define a custom
     * function to release the pointer memory. To overcome this problem you
     * can use the `assign()` function.
     * @since 1.0
     **/
    SharedT<class_t>& operator=(class_t *ptr) {
        return assign(ptr, &pointer_t::release);
    }
    /*}}}*/
    //@}

private:
    /** @name Implementation */ //@{
    // intptr_t retain();/*{{{*/
    /**
     * Increment the reference counting of the bound pointer.
     * @returns The number of references after the increment.
     * @remarks This operation is atomic with allows to be called from
     * different threads without race conditions.
     * @since 1.0
     **/
    intptr_t retain() {
        if (!m_pointer) return 0;
        return ++m_pointer->refs;
    }
    /*}}}*/
    // intptr_t release();/*{{{*/
    /**
     * Decrement the reference counting.
     * @return The number of references after decrement the value.
     * @remarks When the reference counting reaches 0 the bounded pointer is
     * deleted from memory.
     * @since 1.0
     **/
    intptr_t release() {
        if (!m_pointer) return 0;
        intptr_t result = --m_pointer->refs;
        if (result <= 0) { delete m_pointer; }
        m_pointer = NULL;
        return result;
    }
    /*}}}*/
    //@}

private:
    // Data Members
    pointer_t *m_pointer;           /**< The shared pointer holder. */
};

}   /* namespace ss */

/**
 * @defgroup sstl_shared_operators Overloaded Operators
 * List of overloaded operators used with `ss::SharedT` template class.
 * They allow comparison between pointers of different types.
 * @since 1.0
 * @ingroup sstl_shared
 **/
// bool operator ==(const ss::SharedT<T> &one, const ss::SharedT<U> &another);/*{{{*/
/**
 * Overloaded comparison operator.
 * Compares two instances of ss::SharedT object.
 * @param one One instance of %ss::SharedT object.
 * @param another Another instance of %ss::SharedT object.
 * @return \b true if both instances hold the same shared pointer object. \b
 * false otherwise.
 * @since 1.0
 * @ingroup sstl_shared_operators
 **/
template <class T, class U>
bool operator ==(const ss::SharedT<T> &one, const ss::SharedT<U> &another) {
    return (one.data() == another.data());
}
/*}}}*/
// bool operator ==(const ss::SharedT<T> &one, int ptr);/*{{{*/
/**
 * Comparison to \b NULL operator.
 * @param one One instance of ss::SharedT object.
 * @param ptr An \b void pointer (e.g.: \b NULL).
 * @returns \b true when the shared pointer object passed at \p one is bound
 * to the pointer passed to \p ptr. This includes \b NULL pointers.
 * @since 1.0
 * @ingroup sstl_shared_operators
 **/
template <class T>
bool operator ==(const ss::SharedT<T> &one, int ptr) {
    return (ptr == (int)one.data());
}
/*}}}*/
// bool operator !=(const ss::SharedT<T> &one, const ss::SharedT<U> &another);/*{{{*/
/**
 * Overloaded inequality operator.
 * Compares two instances of ss::SharedT object.
 * @param one One instance of %ss::SharedT object.
 * @param another Another instance of %ss::SharedT object.
 * @return \b true if both instances hold different shared pointers. \b false
 * otherwise.
 * @since 1.0
 * @ingroup sstl_shared_operators
 **/
template <class T, class U>
bool operator !=(const ss::SharedT<T> &one, const ss::SharedT<U> &another) {
    return (one.data() != another.data());
}
/*}}}*/
// bool operator !=(const ss::SharedT<T> &one, int ptr);/*{{{*/
/**
 * Comparison to \b NULL operator.
 * @param one One instance of ss::SharedT object.
 * @param ptr An \b void pointer (e.g.: \b NULL).
 * @returns \b true when the shared pointer object passed at \p one is bound
 * to a different pointer passed to \p ptr. This includes \b NULL pointers.
 * @since 1.0
 * @ingroup sstl_shared_operators
 **/
template <class T>
bool operator !=(const ss::SharedT<T> &one, int ptr) {
    return (ptr != (int)one.data());
}
/*}}}*/

#endif /* __SSTLSHRP_HPP_DEFINED__ */

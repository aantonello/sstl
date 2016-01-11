/**
 * @file
 * Doxygen documentation only.
 *
 * @author Alessandro Antonello
 * @date   jan 08, 2016
 * @since  Super Simple Template Library 1.0
 *
 * @copyright 2016, Paralaxe Tecnologia Ltda.. All rights reserved.
 **/
#ifndef __DOXYFILE_H_DEFINED__
#define __DOXYFILE_H_DEFINED__

/**
 * @defgroup sstl_functors Functors Templates
 * Group of super simple functor classes.
 * Functors are objects that act like functions. That is, a class that
 * overloades the `operator ()` operator and can be used like a function call:
 ~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * class AFunctor {
 *    bool operator ()() {
 *        // do something...
 *        return true;
 *    }
 * };
 *
 * AFunctor functor;
 * bool result = functor();
 ~~~~~~~~~~~~~~~~~~~~~
 * The example above shows a simple functor class. A class that overloads the
 * `operator ()`. A class instance is created then it is used like a function
 * call. Functors in this library are template classes that call bound pointer
 * to member functions when its `operator ()` is invoked. The implementation
 * in this library is simple and doesn't support functions with more than one
 * parameter. We made this way becouse we are still using C++98 and cannot
 * change to C++11 yet. So functors need to compile in old compilers. The
 * current implementation doesn't have the same power as `std::function`
 * template but are usable for most of your needs.
 * @since 1.0
 **/

/**
 * @defgroup sstl_properties Properties
 * Property like class templates.
 * This group have property like class templates. C++ has no directy support
 * for properties in classes or objects, unfotunatelly. This is a common path
 * on most used languages but not on C++. We built a set of class templates to
 * mimic this funcionality. The implementation is totally C++ compliant and
 * can be used with compilers that doesn't support C++11 yet.
 *
 * To use the templates you must declare a member in your class, choosing the
 * right template to use, and write getters (and setters, if applicable) to
 * give functionality to the properties. We have support for read-write
 * properties and read-only properties. For write-only properties is thing
 * that setter functions are better and there is no support for that in this
 * library. You can declare properties in the following way:
 ~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * class MyObject
 * {
 * private:
 *    int aNumber;
 *    std::string aText;
 *    double anotherNumber;
 *
 *    int getNumber() const { return aNumber; }
 *    void setNumber(int n) { aNumber = n;    }
 *
 *    std::string getText() const { return aText; }
 *    void setText(const std::string &t) { aText = t; }
 *
 *    double getAnotherNumber();
 * };
 ~~~~~~~~~~~~~~~~~~~~~
 * For scalar types (char, int, long, etc...) we can use `ss::PropertyT` for
 * read-write properties. It takes only one parameter that is the property
 * type. For the read only `getAnotherNumber()` property, we use
 * `ro::PropertyT`.
 ~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * ss::PropertyT<int> number;
 * ro::PropertyT<double> floatNumber;
 ~~~~~~~~~~~~~~~~~~~~~
 * For the `std::string` member, the getter function returns a copy of the
 * internal class member while the setter function needs a reference to an
 * object instance. In this case, we use the `rw::PropertyT` and set the type
 * of the getter and the type of the setter.
 ~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * rw::PropertyT<std::string, const std::string&> text;
 ~~~~~~~~~~~~~~~~~~~~~
 * The `rw::PropertyT` template should be used when types of getter and setter
 * doesn't match equally. This is also true for scalar types. For example, if
 * we have declared a member that is a const pointer to anything and write the
 * setter and getter in this way:
 ~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * const void *theMember;
 * void* theGetter() const;
 * void theSetter(const void *);
 ~~~~~~~~~~~~~~~~~~~~~
 * We must use the `rw::PropertyT` template class to declare a property for
 * it: `rw::PropertyT<void*, const void*> property`.
 *
 * This is not enough. We also have the configure the property objects to call
 * the getters and setters correctly. To do that you `bind()` the functions to
 * the property objects in your constructor.
 ~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * MyObject::MyObject() : aNumber(0), anotherNumber(0.0)
 * {
 *     number.bind<MyObject, MyObject::getNumber, MyObject::setNumber>(this);
 *     text.bind<MyObject, MyObject::getText, MyObject::setText>(this);
 *     floatNumber.bind<MyObject, MyObject::getAnotherNumber>(this);
 * }
 ~~~~~~~~~~~~~~~~~~~~~
 * Notice that the initialization is done inside the constructor function and
 * not in the initialization area. This assure that your object is fully
 * constructed when the `bind()` function is called. Although, if your class
 * is inherited by another classes, the inheritance will not be completely
 * constructed yet. Also notice that we pass the member functions as template
 * parameters and not as function parameters. This is completely valid C++
 * syntax and allows us to generate static functions to call your member
 * functions, avoiding allocating space in the heap to hold on the function
 * pointers. Using an static function the compiler is able to optimize the
 * call and, in the final executable binary, remove any overhead.
 *
 * All objects overload the `operator ()` operator allowing then to be used
 * like a function call. You can use this operator in situations where the
 * casting operator will not be generated by the compiler. When passing the
 * property value to a `printf()` like function is an example:
 ~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * printf("Current number: %d\n", object.number());
 ~~~~~~~~~~~~~~~~~~~~~
 * In the example above using the `operator ()` in the property `number`
 * assures that the property value will be passed to the `printf()` function.
 * If we didn't have used it, the `printf()` function would received a pointer
 * to the property object it self.
 * @since 1.0
 **/

/**
 * @defgroup sstl_events Event System
 * Simple yet useful event system.
 * The event system is built using the functors declared in the @ref
 * sstl_functors module. An event is just another kind of functor object that
 * can be bound to a list of functors having the same function signature. For
 * example, support you have an object that has an indexed list of objects.
 * Every time the current selected index changes you could adivese all bound
 * objects through an event. You could write your class like this:
 ~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * class IndexedList
 * {
 * public:
 *     ss::EventT<int> onSelectedChanged;
 *
 *     void select(int index)
 *     {
 *         if (index != m_currentIndex)
 *         {
 *             m_currentIndex = index;
 *             onSelectedChanged(m_currentIndex);
 *         }
 *     }
 * private:
 *     int m_currentIndex;
 * };
 ~~~~~~~~~~~~~~~~~~~~~
 * In the function `select(int)` above, when the current selected object is
 * really changed, the function updates the member variable and call the
 * `operator()` of the event object. All objects bound to that event will
 * receive the index of the selected item.
 *
 * To bound functions on the above example clients must declare a function
 * with the same signature as the event. Then create a functor object or use
 * the `ss::EventT::bind()` function.
 ~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * class MyObject
 * {
 * private:
 *     void whenSelectionChanges(int currentIndex);
 *     IndexedList m_list;
 *
 * public:
 *     MyObject()
 *     {
 *         m_list.onSelectedChanged << ss::FunctorT<void(int)>::from<MyObject, MyObject::whenSelectionChanges>(this);
 *         // OR...
 *         m_list.onSelectedChanged.bind<MyObject, MyObject::whenSelectionChanges>(this);
 *     }
 * };
 ~~~~~~~~~~~~~~~~~~~~~
 * Notice that events are not thread safe and cannot be used to cross thread
 * communication. Also, objects bound to functors bound to events must remain
 * valid white the event is valid. You must remove the bound delegate from the
 * event list when an object is destroyed before the event it self.
 * @since 1.0
 **/

/**
 * @namespace ss
 * Default namespace.
 * Almost all public classes are declared in this namespace. The main idea is
 * not use the namespace with the instruction `using namespace ss`, but
 * declare all class instances prefixed with the namespace. We use namespaces
 * to separate behavior in this library. For example, the most common
 * `PropertyT` class is declared in the `ss` namespace but we declare others
 * `PropertyT` classes in another namespace with different functionality. That
 * said you must always use `ss::PropertyT` (or `rw::PropertyT` or, yet,
 * `ro::PropertyT`).
 * @since 1.0
 **/

/**
 * @namespace sstl
 * Tools and support implementation.
 * This namespace has classes and templates used only for support the publicly
 * available (and documented) classes. You may use the classes and templates
 * declared in this namespace. But its not supported and the behavior can
 * change without notice.
 * @since 1.0
 **/

/**
 * @namespace rw
 * Read and Write objects.
 * This namespace is used to declare read-write template classes. Most
 * important, it is used to differenciate the `ss::PropertyT` class from
 * `rw::PropertyT` class.
 * @since 1.0
 **/

/**
 * @namespace ro
 * Read only objects.
 * This namespace is used to declare read-only template classes. Most
 * important, it is used to differenciate the `ro::PropertyT` class from
 * `ss::PropertyT` and `rw::PropertyT` classes.
 * @since 1.0
 **/

#endif /* __DOXYFILE_H_DEFINED__ */

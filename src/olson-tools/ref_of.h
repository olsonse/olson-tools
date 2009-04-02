#ifndef olson_tools_ref_of_h
#define olson_tools_ref_of_h

namespace olson_tools {

  /** Map a reference type onto a pointer type or a ref type. */
  template < typename T >
  struct Value {
    typedef T ref_type;
    typedef T* pointer_type;

    static T & ref( T & t ) {
      return t;
    }

    static T * pointer( T & t ) {
      return &t;
    }
  };

  /** Map a pointer type onto a pointer type or a ref type. */
  template < typename T >
  struct Value<T*> {
    typedef T ref_type;
    typedef T* pointer_type;

    static T & ref( T * t ) {
      return *t;
    }

    static T * pointer( T * t ) {
      return t;
    }
  };

  template < typename T >
  inline T & ref_of(T & t) {
    return Value<T>::ref(t);
  }

  template < typename T >
  inline T & ref_of(T * t) {
    return Value<T>::ref(*t);
  }

}/* namespace olson_tools */

#endif // olson_tools_ref_of_h

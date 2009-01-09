
#ifndef OLSON_TOOLS_ITERATORRANGE_H
#define OLSON_TOOLS_ITERATORRANGE_H

namespace olson_tools {

    template <class Iter>
    class IteratorRange {
      private:
        Iter i;
        Iter f;
      public:
        typedef Iter iter_type;
    
        IteratorRange(const Iter & i = Iter(),
                      const Iter & f = Iter()) : i(i), f(f) {}
        const Iter & begin() const { return i; }
        const Iter & end() const { return f; }
        int size() const { return f - i; }
    };

}

#endif // OLSON_TOOLS_ITERATORRANGE_H

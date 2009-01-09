#ifndef PHYSICAL_CALC_SYMBOL_H
#define PHYSICAL_CALC_SYMBOL_H

#include <boost/regex.hpp>
//#include <regex> // have to wait until the new std:: stl

#include <string>

namespace physical {
    namespace calc {

        /* *** BEGIN SYMBOL EXCEPTIONS *** */
        struct symbol_error : physical::exception {
            typedef physical::exception super;
            symbol_error(const std::string & err) : super(err) {}
        };
        
        struct symbol_immutable : symbol_error {
            symbol_immutable(const std::string & name) :
                symbol_error("cannot change symbol:  " + name) {}
        };
        
        struct undefined_symbol : symbol_error {
            undefined_symbol(const std::string & typ,
                             const std::string & var) :
                symbol_error("undefined " + typ + ":  " + var) {}
        };
        
        struct undefined_variable : undefined_symbol {
            undefined_variable(const std::string & var) :
                undefined_symbol("variable",var) {}
        };
        
        struct undefined_function : undefined_symbol {
            undefined_function(const std::string & var) :
                undefined_symbol("function", var) {}
        };
         
        /* ***   END SYMBOL EXCEPTIONS *** */
        
        
        
        template <class Container, class Iter>
        void erase_items(Container & c, Iter i, const Iter f) {
            for (; i!=f; c.erase(*i), i++);
        }
        
        
        struct symbol {
        
            /** /Type of a single pair in the symbol table. */
            typedef std::pair<const std::string, symbol > entry;
            
            /** Symbol table for storing variables, constants, and math functions. */
            typedef std::map<std::string, symbol > table;
        
            typedef physical::Quantity (*math_function)(const physical::Quantity &);
            enum TYPE {
                UNDEFINED,
                VARIABLE,
                CONSTANT,
                FUNCTION
            };
        
            enum TYPE type;
        
            struct {
                physical::Quantity dval;
                math_function mfunc;
            } value;
        
            symbol() : type(UNDEFINED) {}
        
            symbol(const physical::Quantity & _dval, const enum TYPE & _type = CONSTANT) {
                type = _type;
                value.dval = _dval;
            }
        
            symbol(const math_function & _mfunc) {
                type = FUNCTION;
                value.mfunc = _mfunc;
            }
        
        
            template <class table>
            static typename table::const_iterator next_lower_bound(const table & tab, const std::string & txt) {
                std::string txtp1 = txt;
                txtp1[txt.length()-1] = ((char)*(txt.end()-1)) + (char)1;
                return tab.lower_bound(txtp1);
            }
        
            static void import(table & tab, const std::string & base, const std::string & regex, const std::string & new_ns = "") {
                try {
                    bool import_all = regex == "*";
                    boost::regex expression(import_all ? ".*" : regex);
        
                    if (base.length() <1)
                        /* Import doesn't mean much without a base that is to be
                         * removed. */
                        return;
        
                    std::string to = (new_ns == "") ? "" : new_ns + "::";
                    table scratch;
                    table::const_iterator i = tab.lower_bound(base),
                                          f = next_lower_bound(tab, base);
                    for (; i != f; i++) {
                        std::string newkey = i->first.substr(base.length());
                        if(newkey.length() < 1) continue;
                        if(newkey.length() > 2 && newkey.substr(0,2) == "::")
                            newkey = newkey.substr(2);
        
                        /* test the regex */
                        if (!import_all &&
                            !boost::regex_match(newkey.begin(), newkey.end(), expression))
                                continue;
        
                        scratch[to + newkey] = i->second;
                    }
        
                    /* now copy the scratch space over into the real table. */
                    tab.insert(scratch.begin(), scratch.end()); 
                } catch (boost::regex_error & e) {
                    /* rethrow */
                    throw symbol_error(e.what());
                }
            }
        
            static void clear(table & tab, const std::string & base, const std::string & regex) {
                try {
                    std::cout << "from " << base << " clear '" << regex << "'\n" << std::flush;
        
                    if (regex == "") {
                        /* We've been given a single case to try and clear. */
                        table::iterator i = tab.find(base);
                        if (i != tab.end())
                            tab.erase(i);
                        return;
                    }
        
                    /* We've been given a possible pattern to match many symbols to
                     * clear. */
                    bool clear_all = regex == "*";
                    boost::regex expression(clear_all ? ".*" : regex);
        
                    std::vector<std::string> scratch;
        
                    table::const_iterator i, f;
        
                    if ( base.length() > 0) {
                        i = tab.lower_bound(base);
                        f = next_lower_bound(tab,base);
                    } else {
                        i = tab.begin();
                        f = tab.end();
                    }
        
                    for (; i != f; i++) {
                        std::cout << "sym " << i->first << std::endl;
                        std::string newkey = i->first.substr(base.length());
                        if(newkey.length() < 1) continue;
                        if(newkey.length() > 2 && newkey.substr(0,2) == "::")
                            newkey = newkey.substr(2);
        
                        /* test the regex */
                        if (!clear_all &&
                            !boost::regex_match(newkey.begin(), newkey.end(), expression))
                                continue;
        
                        scratch.push_back(i->first);
                    }
        
                    /* now delete each item listed in the scratch space */
                    erase_items(tab, scratch.begin(), scratch.end());
                } catch (boost::regex_error & e) {
                    /* rethrow */
                    throw symbol_error(e.what());
                }
            }
        
        };
        
        inline std::ostream & operator<<(std::ostream &out, const symbol & sym) {
            switch(sym.type) {
                case symbol::UNDEFINED:
                    out << "undefined";
                    break;
        
                case symbol::VARIABLE:
                    out << sym.value.dval;
                    break;
        
                case symbol::CONSTANT:
                    out << sym.value.dval << " (const)";
                    break;
        
                case symbol::FUNCTION:
                    out << "()";
                    break;
            }
            return out;
        }



    }
}

#endif // PHYSICAL_CALC_SYMBOL_H

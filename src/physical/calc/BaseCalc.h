#ifndef  PHYSICAL_CALC_CALC_H
#define  PHYSICAL_CALC_CALC_H

#include "../quantity.h"
#include "symbol.h"
#include "units.h"
#include "math.h"

#include <sstream>

namespace physical {
    namespace calc {

        struct syntax_error : physical::exception {
            typedef physical::exception super;
            syntax_error(      std::string::iterator stop,
                         const std::string & msg = "") :
                super(msg), stop(stop) {}
            std::string::iterator stop;
        };

        /** The base class for all physical calculator types. */
        template<class sub, class Engine>
        struct BaseCalc {
            symbol::table symbols;
            Engine & engine;
        
            BaseCalc(Engine & engine) : symbols(), engine(engine) {}

            void addPhysicalUnits() { physical::calc::addPhysicalUnits(symbols); }
        
            void addMathLib() { physical::calc::addMathLib(symbols); }

            /** Parse a input string and return the results in units of
             * q_units_out.
             *
             * If the results do not match the base units of the q_units_out
             * parameter, then a UnitsMismatchR exception is thrown.
             *
             * @return the coefficient of the resulting physical::Quantity in
             * units of q_units_out (result/q_units_out).
             * */
            Quantity::coeff_type parse(const std::string & in, const Quantity & q_units_out) {
                Quantity q = parse(in);
                if (q.units != q_units_out.units)
                    throw exception(UnitsMismatchR);
                return q.coeff / q_units_out.coeff;
            }
        
            /** Parse a input string and check the results against the units of
             * q_check.
             *
             * If the results do not match the base units of the q_check
             * parameter, then a UnitsMismatchR exception is thrown.
             *
             * @return the coefficient of the resulting physical::Quantity.
             * */
            Quantity::coeff_type parse_w_check_only(const std::string & in, const Quantity & q_check) {
                Quantity q = parse(in);
                if (q.units != q_check.units)
                    throw exception(UnitsMismatchR);
                return q.coeff / q_check.coeff;
            }

            Quantity parse(std::string s) {
                bool result_set;
                return parse(s, result_set);
            }

            Quantity parse(std::string s, bool & result_set) {
                std::string::iterator first = s.begin();
                bool finished = false;
                Quantity result;
                do {
                    try {
                        result = parse(first, s.end(), finished, result_set);
                    } catch (syntax_error & e) {
                        // Display a caret that points to the position where the error
                        // was detected.
                        std::ostringstream outs;
                        int w = e.stop - s.begin();
                        outs << s << '\n'
                             << std::setw(w) << (w ? " " : "") << "^ error\n";
                        throw syntax_error(e.stop, outs.str());
                    }
                } while(!finished);
                if (!result_set)
                    return Quantity();
                else
                    return result;
            }

            Quantity parse( std::string::iterator & first,
                            const std::string::iterator last,
                            bool & finished,
                            bool & result_set) {
                using boost::spirit::parse;
                using boost::spirit::space_p;
                using boost::spirit::parse_info;

                engine.reset_result_set();

                parse_info<std::string::iterator> info;
                info = parse(first, last, engine, space_p);

                if (!info.hit)
                    throw syntax_error(info.stop);
                if (!info.full) {
                    // Keep track of where to start
                    // parsing the next statement.
                    first = info.stop;
                    finished = false;
                } else
                    finished = true;

                result_set = engine.result_set();
                if (!result_set)
                    return Quantity();
                else
                    return engine.result();
            }
        };
    } /* namespace calc */
} /* namespace physical */

#endif // PHYSICAL_CALC_CALC_H

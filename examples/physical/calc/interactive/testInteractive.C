#include <olson-tools/physical/calc/infix.h>

#include <olson-tools/olson-tools-config.h>

#ifdef OLSON_TOOLS_HAVE_LIBREADLINE
#  if defined(OLSON_TOOLS_HAVE_READLINE_READLINE_H)
#    include <readline/readline.h>
#  elif defined(OLSON_TOOLS_HAVE_READLINE_H)
#    include <readline.h>
#  else /* !defined(OLSON_TOOLS_HAVE_READLINE_H) */
extern char *readline ();
#  endif /* !defined(OLSON_TOOLS_HAVE_READLINE_H) */
char *cmdline = NULL;
#else /* !defined(OLSON_TOOLS_HAVE_READLINE_READLINE_H) */
  /* no readline */


/* I'll define my own readline function */
char * readline(const std::string & prompt) {
    std::cout << prompt << std::flush;
    std::string ins;
    std::getline(std::cin, ins);
    char * s = (char*)malloc(ins.length()+1);
    strcpy(s,ins.c_str());
    return s;
}

#endif /* OLSON_TOOLS_HAVE_LIBREADLINE */

#ifdef OLSON_TOOLS_HAVE_READLINE_HISTORY
#  if defined(OLSON_TOOLS_HAVE_READLINE_HISTORY_H)
#    include <readline/history.h>
#  elif defined(OLSON_TOOLS_HAVE_HISTORY_H)
#    include <history.h>
#  else /* !defined(OLSON_TOOLS_HAVE_HISTORY_H) */
extern void add_history ();
extern int write_history ();
extern int read_history ();
#  endif /* defined(OLSON_TOOLS_HAVE_READLINE_HISTORY_H) */
  /* no history */
#endif /* OLSON_TOOLS_HAVE_READLINE_HISTORY */








physical::calc::InfixCalc calc;

typedef physical::calc::symbol sym;

char * variable_completer(const char * text, int state) {
    static sym::table::const_iterator i;

    if (state == 0) {
        /* first try and find an exact match for the text. */
        i = calc.symbols.find(text);
        if (i == calc.symbols.end())
            i = calc.symbols.lower_bound(text);
        if (i == calc.symbols.end())
            return NULL;
    }

    if (i == calc.symbols.end())
        return NULL;

    std::string txt = (*i).first;

    /* now iterate to the next item in the current namespace. */
//  sym::table::const_iterator j;
//  size_t nsp_end = txt.find("::", strlen(text));
//  if (nsp_end == std::string::npos)
//      // current item is not a namespace, so just iterate to the next item.
        i++;
//  else {
//      /* current item is in a different namespace, so truncate to the
//       * sub-namespace and iterate past that namespace. */
//      txt = txt.substr(0,nsp_end + 2);
//      i = sym::next_lower_bound(calc.symbols, txt);
//  }

    if (txt.compare(0,strlen(text), text) != 0)
        return NULL;

    return strncpy((char*)malloc(txt.length()+1), txt.c_str(), txt.length() + 1);
}

namespace bs = boost::spirit;
using physical::calc::symbol;

int main() {
#ifdef OLSON_TOOLS_HAVE_LIBREADLINE
    rl_completion_entry_function = variable_completer;
#endif

    calc.addMathLib();
    calc.addPhysicalUnits();

    char * line_cstr = NULL;
    while ((line_cstr = readline(">  ")) != NULL) {
        std::string line(line_cstr);
        free(line_cstr);

#ifdef OLSON_TOOLS_HAVE_READLINE_HISTORY
        add_history(line.c_str());
#endif

        // Read one line of text and parse it. If the parser does not consume
        // the entire string, keep parsing the same string until an error occurs
        // or the string is consumed. Then go back and read another string.
        std::string::iterator first = line.begin();
        bool finished = false;
        bool result_set = false;
        do {
            try {
                physical::Quantity result = calc.parse(first, line.end(), finished, result_set);
                if (result_set)
                    std::cout << result << std::endl;
            } catch (physical::calc::symbol_error & e) {
                std::cout << e.what() << std::endl;
                break;
            } catch (physical::calc::syntax_error & e) {
                // Display a caret that points to the position where the error
                // was detected.
                std::cerr << std::setw(e.stop - line.begin() + 3) << " " << "^ error\n";
                break;
            } catch (physical::exception & e) {
                std::cout << e.what() << std::endl;
                break;
            }
        } while(!finished);
    }
    return 0;
}



/* This program shows a calculation that causes a floating point exception. 
 * By linking in the trapfe library and compiling with the -g option, these
 * floating point exceptions can be caught and the program paused.  This
 * allows you to load the running, yet paused, program into a debugger to
 * pin-point the location of the floating point exception. */

int main() {
    double a = 0.0;
    double b = 9.0;
    double c = b / a;
    return 0;
}

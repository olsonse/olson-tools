
/** \page fit Fit library
    A fitting library intended for various use.
    This is a part of the \ref main "OlsonTools" package.  

    This document is a work in progress.

    \section sec_fit Fitting algorithms
        Some of the fitting algorithms included in this library:
            - \ref sec_fit_ga Genetic Algorithm
            - \ref sec_fit_smplx Simplex Algorithm
            .
        A lot of effort has been made to make this fitting library generic and
        useful for a number of problems.

    \subsection sec_fit_ga Genetic Algorithm
        Todo:  include the basic algorithm of the GA.  Include a link to
        Shannon Lunt's paper.

    \subsection sec_fit_smplx Simplex Algorithm
        Todo:  include a nice pictoral representation of a simplex algorithm
        (as well as explanation).

    \subsection fit_history Historical Development
        Todo:  edit and update.

    Originally begun by Prof. R. Steven Turley of Brigham Young University to
    find solutions for designing the special mirrors for the EUV Imager of the
    Imager Mission (NASA).  The EUV Imager was to image the earth's
    magnetoshpere.  Because of the situation, it was required that the mirrors
    of this detector to reflect light at 30.4 nm. and NOT reflect light at
    58.4 nm.  Because of the difficulty of calculating a design of such a
    mirror, attention was turned to a genetic algorithm to perhaps find the
    best practical solution.\n

    The study of this genetic algorithm then became the senior thesis for
    Shannon lunt, an undergraduate student in the XUV Research Group of
    Brigham Young University.  Shannon's work revolved around characterizing
    the function of this code primarily for the Imager Mission.\n

    Spencer Olson, also an undergraduate in the group, then undertook to
    optimize and improve the algorithm and the associating program.  After
    including newer algorithms to calculate reflectivies from another program
    written by Prof. Turley, the name of the program became XUVMulti.\n

    Currently, the distribution of this program includes a general fitting
    library, a multilayer library, a database of optical constants of
    materials, and several user interfaces.  Interfaces included (or to be)
    are text-based, web-based, X-Windows-based, and Microsoft Windows-based.

*/

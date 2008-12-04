/** \file
 * A multitude of physical constants and units (mks where applicable)
 * organized into a bunch of namespaces.
 *
 * Much of the data for the units and constants listed in this header are
 * freely available on usenet servers.  The reference for the original
 * header is comp.lang.cpp/2004-10/2217 by E. Robert Tisdale (10/17/04).
 * The namespace layout is original to the posting by E. Robert Tisdale.
 *
 * E. R. Tisdale <E.Robert.Tisdale_at_jpl.nasa.gov>.
 *
 * There were a lot of other comments on usenet for improving this file,
 * including British spelling of various units and so on.  This version is the
 * unaltered version from E. R. Tisdale.
 *
 * */

#ifndef PHYSICAL_DATA_H
#define PHYSICAL_DATA_H

namespace physical {

#ifndef PHYSICAL_DATA_QUANTITY
/** The units/constants library defaults to just providing the information via
 * doubles. */
#  define PHYSICAL_DATA_QUANTITY double
#endif

#ifndef PHYSICAL_QUANTITY_INITIALIZER_1
/** The default initializer for the library only sets the coefficient. */
#  define _PQINIT1(c)           c
#  define _PQINIT2(c,u)         c
#  define _PQINIT3(c,u,n)       c
#else
/** The initializer allows to be specialized so that units and name can be
 * used. */
#  define _PQINIT1(c)       PHYSICAL_QUANTITY_INITIALIZER_1(c)
#  define _PQINIT2(c,u)     PHYSICAL_QUANTITY_INITIALIZER_2(c,u)
#  define _PQINIT3(c,u,n)   PHYSICAL_QUANTITY_INITIALIZER_3(c,u,n)
#endif

    /** The base type for all items in the library depends.
     * This type can be a scalar value, or perhaps a class that checks type
     * information.
     *
     * @see PHYSICAL_DATA_QUANTITY
     * */
    typedef PHYSICAL_DATA_QUANTITY Quantity;

    // #### BEGIN OF UNIT CREATION #### #
    namespace unit { // conversion factor
        const Quantity radian           = _PQINIT2(1.0,units_pair("radian",1));
        const Quantity radians          = radian;
        const Quantity rad              = radian;
        const Quantity steradian        = _PQINIT2(1.0, units_pair("steradian",1));
        const Quantity steradians       = steradian;
        const Quantity sr               = steradian;
        const Quantity dollar           = _PQINIT2(1.0, units_pair("dollar",1));
        const Quantity dollars          = dollar;
        const Quantity cent             = dollar/100.0;
        const Quantity cents            = cent;
        const Quantity dozen            = 12.0;
        const Quantity doz              = dozen;
        const Quantity dz               = dozen;
        namespace bakers {
            const Quantity dozen        = 13.0;
            const Quantity doz          = dozen;
            const Quantity dz           = dozen;
        }
        const Quantity gross            = 12.0*dozen;
        const Quantity gro              = gross;
        const Quantity quire            = 25.0;
        const Quantity quires           = quire;
        const Quantity ream             = 500.0;
        const Quantity reams            = ream;
        const Quantity percent          = 1.0/100.0;
        const Quantity proof            = percent/2.0;
        const Quantity karat            = 1.0/24.0;
        const Quantity karats           = karat;
        const Quantity mole             = 6.0221367e+23;
        const Quantity moles            = mole;
        const Quantity mol              = mole;
        const Quantity pi               = 3.14159265358979323846*radians;
        namespace arc {
            const Quantity degree       = pi/180.0;
            const Quantity degrees      = degree;
            const Quantity minute       = degree/60.0;
            const Quantity minutes      = minute;
            const Quantity min          = minute;
            const Quantity second       = minute/60.0;
            const Quantity seconds      = second;
            const Quantity sec          = second;
            const Quantity grade        = 0.9*degrees;
            const Quantity grades       = grade;
            namespace centesimal {
              const Quantity minute     = grade/100.0;
              const Quantity minutes    = minute;
              const Quantity min        = minute;
              const Quantity second     = grade/10000.0;
              const Quantity seconds    = second;
              const Quantity sec        = second;
              }
            }

        // SI units (mks)
        // length
        const Quantity meter            = _PQINIT2(1.0,units_pair("m",1));
        const Quantity meters           = meter;
        const Quantity m                = meter;
        const Quantity kilometer        = 1000.0*meters;
        const Quantity kilometers       = kilometer;
        const Quantity km               = kilometer;
        const Quantity decimeter        = meters/10.0;
        const Quantity decimeters       = decimeter;
        const Quantity dm               = decimeter;
        const Quantity centimeter       = meters/100.0;
        const Quantity centimeters      = centimeter;
        const Quantity cm               = centimeter;
        const Quantity millimeter       = meters/1000.0;
        const Quantity millimeters      = millimeter;
        const Quantity mm               = millimeter;
        const Quantity micron           = meter/1000000.0;
        const Quantity microns          = micron;
        const Quantity um               = micron;
        const Quantity nanometer        = meter/1000000000.0;
        const Quantity nanometers       = nanometer;
        const Quantity nm               = nanometer;
        const Quantity decinanometer    = meter/10000000000.0;
        const Quantity decinanometers   = decinanometer;
        const Quantity Angstrom         = decinanometer;
        const Quantity Angstroms        = Angstrom;
        const Quantity Xunit            = 1.00202e-13*meters;
        const Quantity Xunits           = Xunit;
        const Quantity Fermi            = meter/1000000000000000.0;
        const Quantity Fermis           = Fermi;
        // area
        const Quantity hectare          = 10000.0*meter*meter;
        const Quantity hectares         = hectare;
        const Quantity ha               = hectare;
        // volume
        const Quantity stere            = meter*meter*meter;
        const Quantity steres           = stere;
        const Quantity liter            = stere/1000.0;
        const Quantity liters           = liter;
        const Quantity l                = liter;
        const Quantity milliliter       = stere/1000000.0;
        const Quantity milliliters      = milliliter;
        const Quantity ml               = milliliter;
        const Quantity cc               = milliliter;
        namespace displacement {
            const Quantity ton          = stere;
            const Quantity tons         = ton;
            const Quantity t            = ton;
        }
        // mass
        const Quantity kilogram         = _PQINIT2(1.0, units_pair("kg",1));
        const Quantity kilograms        = kilogram;
        const Quantity kg               = kilogram;
        const Quantity quintal          = 100.0*kilograms;
        const Quantity quintals         = quintal;
        const Quantity doppelzentner    = quintal;
        const Quantity doppelzentners   = doppelzentner;
        const Quantity gram             = kilograms/1000.0;
        const Quantity grams            = gram;
        const Quantity g                = gram;
        const Quantity milligram        = kilogram/1000000.0;
        const Quantity milligrams       = milligram;
        const Quantity mg               = milligram;
        namespace metric { // weight
            const Quantity carat        = gram/5.0;
            const Quantity carats       = carat;
            const Quantity ton          = 1000.0*kilograms;
            const Quantity tons         = ton;
            const Quantity t            = ton;
        }
        // time
        const Quantity second           = _PQINIT2(1.0, units_pair("s", 1));
        const Quantity seconds          = second;
        const Quantity sec              = second;
        const Quantity s                = second;
        const Quantity millisecond      = second/1000.0;
        const Quantity milliseconds     = millisecond;
        const Quantity ms               = millisecond;
        const Quantity microsecond      = second/1000000.0;
        const Quantity microseconds     = microsecond;
        const Quantity us               = microsecond;
        const Quantity nanosecond       = second/1000000000.0;
        const Quantity nanoseconds      = nanosecond;
        const Quantity ns               = nanosecond;
        const Quantity picosecond       = second/1000000000000.0;
        const Quantity picoseconds      = picosecond;
        const Quantity minute           = 60.0*seconds;
        const Quantity minutes          = minute;
        const Quantity min              = minute;
        const Quantity hour             = 60.0*minutes;
        const Quantity hours            = hour;
        const Quantity hr               = hour;
        const Quantity day              = 24.0*hours;
        const Quantity days             = day;
        const Quantity da               = day;
        const Quantity week             = 7.0*days;
        const Quantity weeks            = week;
        const Quantity fortnight        = 2.0*weeks;
        const Quantity fortnights       = fortnight;
        const Quantity year             = 365.2421896698*days;
        const Quantity years            = year;
        const Quantity yr               = year;
        const Quantity month            = year/12.0;
        const Quantity months           = month;
        const Quantity mo               = month;
        const Quantity decade           = 10.0*years;
        const Quantity decades          = decade;
        const Quantity century          = 100.0*years;
        const Quantity centuries        = century;
        const Quantity millenium        = 1000.0*years;
        const Quantity millenia         = millenium;
        // temporal frequency
        const Quantity Hertz            = 1.0/second;
        const Quantity Hz               = Hertz;
        const Quantity kiloHertz        = 1000.0*Hertz;
        const Quantity kHz              = kiloHertz;
        const Quantity megaHertz        = 1000000.0*Hertz;
        const Quantity MHz              = megaHertz;
        const Quantity gigaHertz        = 1000000000.0*Hertz;
        const Quantity GHz              = gigaHertz;
        const Quantity teraHertz        = 1000000000000.0*Hertz;
        const Quantity THz              = teraHertz;
        // spacial frequency
        const Quantity diopter          = 1.0/meter;
        const Quantity diopters         = diopter;
        // speed
        const Quantity kph              = kilometers/hour;
        // radioactivity
        const Quantity Becquerel        = 1.0/second;
        const Quantity Becquerels       = Becquerel;
        const Quantity Bq               = Becquerel;
        const Quantity Rutherford       = 1000000.0*Becquerels;
        const Quantity Rutherfords      = Rutherford;
        const Quantity Curie            = 3.7e+10*Becquerels;
        const Quantity Curies           = Curie;
        const Quantity Ci               = Curie;
        // force
        const Quantity Newton           = kilogram*meter/(second*second);
        const Quantity Newtons          = Newton;
        const Quantity N                = Newton;
        const Quantity dyne             = Newton/100000.0;
        const Quantity dynes            = dyne;
        const Quantity dyn              = dyne;
        // pressure
        const Quantity Pascal           = Newton/(meter*meter);
        const Quantity Pascals          = Pascal;
        const Quantity Pa               = Pascal;
        const Quantity Barie            = Pascal/10.0;
        const Quantity Baries           = Barie;
        const Quantity Barye            = Barie;
        const Quantity Baryes           = Barye;
        const Quantity pieze            = 1000.0*Pascals;
        const Quantity piezes           = pieze;
        const Quantity pz               = pieze;
        const Quantity bar              = 10000.0*Pascals;
        const Quantity bars             = bar;
        const Quantity Torr             = 133.3224*Pascals;
        const Quantity atmosphere       = 760.0*Torr;
        const Quantity atmospheres      = atmosphere;
        const Quantity atm              = atmosphere;
        // energy
        const Quantity Joule            = Newton*meter;
        const Quantity Joules           = Joule;
        const Quantity J                = Joule;
        const Quantity erg              = Joule/10000000.0;
        const Quantity ergs             = erg;
        const Quantity kiloWatthour     = 3600000.0*Joules;
        const Quantity kiloWatthours    = kiloWatthour;
        const Quantity kWh              = kiloWatthour;
        // power
        const Quantity Watt             = Joule/second;
        const Quantity Watts            = Watt;
        const Quantity W                = Watt;
        const Quantity kiloWatt         = 1000.0*Watts;
        const Quantity kiloWatts        = kiloWatt;
        const Quantity kW               = kiloWatt;
        const Quantity megaWatt         = 1000000.0*Watts;
        const Quantity megaWatts        = megaWatt;
        const Quantity MW               = megaWatt;
        const Quantity milliWatt        = Watt/1000.0;
        const Quantity milliWatts       = milliWatt;
        const Quantity mW               = milliWatt;
        const Quantity microWatt        = Watt/1000000.0;
        const Quantity microWatts       = microWatt;
        const Quantity uW               = microWatt;
        const Quantity nanoWatt         =  Watt/1000000000.0;
        const Quantity nanoWatts        = nanoWatt;
        const Quantity nW               = nanoWatt;
        namespace dose { // energy
            const Quantity Gray         = Joule/kilogram;
            const Quantity Grays        = Gray;
            const Quantity Gy           = Gray;
            const Quantity Sievert      = Gray;
            const Quantity Sieverts     = Sievert;
            const Quantity rad          = Gray/100.0;
            const Quantity rads         = rad;
            const Quantity rd           = rad;
        }
        // electrical current
        const Quantity Ampere           = _PQINIT2(1.0, units_pair("A",1));
        const Quantity Amperes          = Ampere;
        const Quantity A                = Ampere;
        const Quantity Biot             = 10.0*Amperes;
        const Quantity Biots            = Biot;
        // electrical potential
        const Quantity Volt             = Watt/Ampere;
        const Quantity Volts            = Volt;
        const Quantity V                = Volt;
        // electrical resistance
        const Quantity Ohm              = Volt/Ampere;
        const Quantity Ohms             = Ohm;
        // electrical conductance
        const Quantity mho              = 1.0/Ohm;
        const Quantity mhos             = mho;
        const Quantity Siemens          = mho;
        const Quantity S                = Siemens;
        // electrical charge
        const Quantity Coulomb          = Ampere*second;
        const Quantity Coulombs         = Coulomb;
        const Quantity C                = Coulomb;
        const Quantity Franklin         = 3.33564e-10*Coulombs;
        const Quantity Franklins        = Franklin;
        // electrical capacity
        const Quantity Farad            = Coulomb/Volt;
        const Quantity Farads           = Farad;
        const Quantity F                = Farad;
        // magnetic flux
        const Quantity Weber            = Volt*second;
        const Quantity Webers           = Weber;
        const Quantity Wb               = Weber;
        const Quantity Maxwell          = Weber/100000000.0;
        const Quantity Maxwells         = Maxwell;
        const Quantity M                = Maxwell;
        // magnetic field B
        const Quantity Tesla            = Weber/(meter*meter);
        const Quantity Teslas           = Tesla;
        const Quantity T                = Tesla;
        const Quantity Gauss            = Tesla/10000.0;
        const Quantity gamma            = Tesla/1000000000.0;
        // magnetic field H
        const Quantity Oerstedt         = 79.57747*Ampere/meter;
        const Quantity Oerstedts        = Oerstedt;
        const Quantity Oe               = Oerstedt;
        // magnetic inductivity
        const Quantity Henry            = Weber/Ampere;
        const Quantity Henrys           = Henry;
        const Quantity H                = Henry;
        const Quantity milliHenry       = Henry/1000.0;
        const Quantity milliHenrys      = milliHenry;
        const Quantity mH               = milliHenry;
        // temperature
        const Quantity Kelvin           = _PQINIT2(1.0,units_pair("K",1));
        const Quantity Kelvins          = Kelvin;
        const Quantity K                = Kelvin;
        const Quantity milliKelvin      = Kelvin*1e-3;
        const Quantity milliKelvins     = milliKelvin;
        const Quantity mK               = milliKelvin;
        const Quantity microKelvin      = Kelvin*1e-6;
        const Quantity microKelvins     = microKelvin;
        const Quantity uK               = microKelvin;
        // luminous intensity
        const Quantity candela          = _PQINIT2(1.0,units_pair("candela",1));
        const Quantity candelas         = candela;
        const Quantity cd               = candela;
        const Quantity apostilb         = candelas/meter/meter;
        const Quantity apostilbs        = apostilb;
        const Quantity nit              = apostilb;
        const Quantity nits             = nit;
        const Quantity skot             = apostilb/1000.0;
        const Quantity skots            = skot;
        const Quantity stilb            = 10000.0*apostilbs;
        const Quantity stilbs           = stilb;
        const Quantity Blondel          = apostilb/pi;
        const Quantity Blondels         = Blondel;
        const Quantity Lambert          = 10000.0*Blondels;
        const Quantity Lamberts         = Lambert;
        // light flux
        const Quantity lumen            = candela*steradian;
        const Quantity lumens           = lumen;
        const Quantity lm               = lumen;
        // light intensity
        const Quantity lux              = lumens/meter/meter;
        const Quantity luxes            = lux;
        const Quantity luces            = lux;
        const Quantity lx               = lux;
        const Quantity nox              = lux/1000.0;
        const Quantity phot             = lumens/centimeter/centimeter;
        const Quantity phots            = phot;
        namespace equivalent {
            const Quantity lux          = unit::lux/pi;
            const Quantity luxes        = lux;
            const Quantity luces        = lux;
            const Quantity lx           = lux;
            const Quantity lumen        = unit::lumen/pi;
            const Quantity lumens       = lumen;
            const Quantity lm           = lumen;
            const Quantity phot         = apostilb/pi;
            const Quantity phots        = phot;
        }
        // acceleration
        const Quantity Galileo          = centimeters/second/second;
        const Quantity Galileos         = Galileo;
        // standard gravitational acceleration at sea level
        const Quantity gravity          = _PQINIT2(9.80665*meters/second/second,"standard gravitational acceleration at sea level");
        // mass
        const Quantity kilohyl          = kilogram*gravity*second*second/meter;
        const Quantity kilohyls         = kilohyl;
        const Quantity hyl              = kilohyl/1000.0;
        const Quantity hyls             = hyl;

        // English Units
        // length
        const Quantity inch             = 0.0254*meters;
        const Quantity inches           = inch;
        const Quantity in               = inch;
        const Quantity mil              = inch/1000.0;
        const Quantity mils             = mil;
        const Quantity point            = inch/72.27;
        const Quantity points           = point;
        const Quantity pt               = point;
        const Quantity bottommeasure    = inch/40.0;
        const Quantity bottommeasures   = bottommeasure;
        const Quantity line             = inch/12.0;
        const Quantity lines            = line;
        const Quantity pica             = 12.0*points;
        const Quantity picas            = pica;
        const Quantity barleycorn       = inch/3.0;
        const Quantity barleycorns      = barleycorn;
        const Quantity finger           = 7.0*inches/8.0;
        const Quantity fingers          = finger;
        const Quantity palm             = 3.0*inches;
        const Quantity palms            = palm;
        const Quantity hand             = 4.0*inches;
        const Quantity hands            = hand;
        const Quantity link             = 7.92*inches;
        const Quantity links            = link;
        const Quantity li               = link;
        const Quantity span             = 9.0*inches;
        const Quantity spans            = span;
        const Quantity foot             = 12.0*inches;
        const Quantity feet             = foot;
        const Quantity ft               = foot;
        const Quantity cubit            = 18.0*inches;
        const Quantity cubits           = cubit;
        const Quantity yard             = 3.0*feet;
        const Quantity yards            = yard;
        const Quantity yd               = yard;
        const Quantity nail             = yard/16.0;
        const Quantity nails            = nail;
        const Quantity ell              = 45.0*inches;
        const Quantity ells             = ell;
        const Quantity pace             = 5.0*feet;
        const Quantity paces            = pace;
        const Quantity fathom           = 6.0*feet;
        const Quantity fathoms          = fathom;
        const Quantity fm               = fathom;
        const Quantity rod              = 198.0*inches;
        const Quantity rods             = rod;
        const Quantity rd               = rod;
        const Quantity pole             = rod;
        const Quantity poles            = pole;
        const Quantity p                = pole;
        const Quantity perch            = rod;
        const Quantity perches          = perch;
        const Quantity rope             = 20.0*feet;
        const Quantity ropes            = rope;
        const Quantity bolt             = 40.0*yards;
        const Quantity bolts            = bolt;
        const Quantity chain            = 4.0*rods;
        const Quantity chains           = chain;
        const Quantity ch               = chain;
        namespace Gunters {
            const Quantity chain        = unit::chain;
            const Quantity chains       = chain;
        }
        namespace engineers {
            const Quantity link         = foot;
            const Quantity links        = link;
            const Quantity chain        = 100.0*feet;
            const Quantity chains       = chain;
        }
        const Quantity skein            = 120.*yards;
        const Quantity skeins           = skein;
        const Quantity furlong          = 220.*yards;
        const Quantity furlongs         = furlong;
        const Quantity spindle          = 14400.*yards;
        const Quantity spindles         = spindle;
        namespace US {
            const Quantity cable_length = 120.0*fathoms;
            const Quantity cable_lengths= cable_length;
        }
        namespace British {
            const Quantity cable_length = 100.0*fathoms;
            const Quantity cable_lengths= cable_length;
        }
        namespace statute {
            const Quantity mile         = 5280.0*feet;
            const Quantity miles        = mile;
            const Quantity mi           = mile;
            const Quantity league       = 3.0*miles;
            const Quantity leagues      = league;
        }
        namespace nautical {
            const Quantity mile         = 1852.*meters;
            const Quantity miles        = mile;
            const Quantity nm           = mile;
            const Quantity league       = 3.0*miles;
            const Quantity leagues      = league;
        }
        namespace marine                = nautical;
        namespace geodetic {
            const Quantity foot         = (1200.0/3937.0)*meters;
            const Quantity feet         = foot;
            const Quantity ft           = foot;
        }
        namespace geographical {
            const Quantity mile         = nautical::mile;
            const Quantity miles        = mile;
            const Quantity mi           = mile;
        }
        const Quantity parasang         = 3.5*statute::miles;
        const Quantity parasangs        = parasang;
        const Quantity arpentcan        = 27.52*statute::miles;
        const Quantity arpentcans       = arpentcan;
        const Quantity arpentlin        = 191.835*feet;
        const Quantity arpentlins       = arpentlin;
        const Quantity astronomical_unit= 1.49597871e11*meters;
        const Quantity astronomical_units = astronomical_unit;
        const Quantity AU               = astronomical_unit;
        const Quantity lightyear        = 9.4605e15*meters;
        const Quantity lightyears       = lightyear;
        const Quantity ly               = lightyear;
        const Quantity parsec           = AU*radians/arc::second;
        const Quantity parsecs          = parsec;
        const Quantity pc               = parsec;
        // area
        const Quantity barn             = 1.0e-28*meter*meter;
        const Quantity barns            = barn;
        const Quantity b                = barn;
        const Quantity circular_inch    = 0.25*pi*inch*inch;
        const Quantity circular_inches  = circular_inch;
        const Quantity circular_mil     = 0.25*pi*mil*mil;
        const Quantity circular_mils    = circular_mil;
        const Quantity sabin            = foot*foot;
        const Quantity sabins           = sabin;
        const Quantity square           = 100.0*sabin;
        const Quantity squares          = square;
        const Quantity are              = 100.0*meter*meter;
        const Quantity ares             = are;
        const Quantity a                = are;
        const Quantity rood             = 40.0*rod*rod;
        const Quantity roods            = rood;
        const Quantity ro               = rood;
        const Quantity acre             = 4.0*roods;
        const Quantity acres            = acre;
        const Quantity section          = statute::mile*statute::mile;
        const Quantity sections         = section;
        const Quantity homestead        = section/4.0;
        const Quantity homesteads       = homestead;
        const Quantity township         = 36.0*sections;
        const Quantity townships        = township;
        // volume
        const Quantity minim            = 6.161152e-8*(m*m*m);
        const Quantity minims           = minim;
        const Quantity drop             = 0.03*cc;
        const Quantity drops            = drop;
        const Quantity teaspoon         = 4.928922*cc;
        const Quantity teaspoons        = teaspoon;
        const Quantity tablespoon       = 3.0*teaspoons;
        const Quantity tablespoons      = tablespoon;
        namespace US {
            namespace liquid {
                const Quantity dram     = 60.0*minims;
                const Quantity drams    = dram;
                const Quantity dr       = dram;
                const Quantity ounce    = 8.0*drams;
                const Quantity ounces   = ounce;
                const Quantity oz       = ounce;
                const Quantity gill     = 4.0*ounces;
                const Quantity gills    = gill;
                const Quantity gl       = gill;
                const Quantity pint     = 4.0*gills;
                const Quantity pints    = pint;
                const Quantity pt       = pint;
                const Quantity quart    = 2.0*pints;
                const Quantity quarts   = quart;
                const Quantity qt       = quart;
                const Quantity magnum   = 2.0*quarts;
                const Quantity magnums  = magnum;
                const Quantity gallon   = 4.0*quarts;
                const Quantity gallons  = gallon;
                const Quantity gal      = gallon;
            }
            namespace dry {
                const Quantity pint     = 550.61047*cc;
                const Quantity pints    = pint;
                const Quantity pt       = pint;
                const Quantity quart    = 2.0*pints;
                const Quantity quarts   = quart;
                const Quantity qt       = quart;
            }
            const Quantity peck         = 8.0*dry::quarts;
            const Quantity pecks        = peck;
            const Quantity pk           = peck;
            const Quantity bushel       = 4.0*pecks;
            const Quantity bushels      = bushel;
            const Quantity bu           = bushel;
            const Quantity barrel       = 31.5*liquid::gallons;
            const Quantity barrels      = barrel;
            const Quantity bbl          = barrel;
            const Quantity bl           = barrel;
        }
        namespace British {
            namespace fluid {
              const Quantity drachm     = 60.0*minims;
              const Quantity drachms    = drachm;
              const Quantity dr         = drachm;
              const Quantity ounce      = 8.0*drachms;
              const Quantity ounces     = ounce;
              const Quantity oz         = ounce;
              const Quantity gill       = 5.0*ounces;
              const Quantity gills      = gill;
              const Quantity gi         = gill;
              const Quantity pint       = 4.0*gills;
              const Quantity pints      = pint;
              const Quantity pt         = pint;
              const Quantity quart      = 2.0*pints;
              const Quantity quarts     = quart;
              const Quantity qt         = quart;
              const Quantity gallon     = 4.0*quarts;
              const Quantity gallons    = gallon;
              const Quantity gal        = gallon;
              }
            const Quantity peck         = 2.0*fluid::gallons;
            const Quantity pecks        = peck;
            const Quantity pk           = peck;
            const Quantity bushel       = 4.0*pecks;
            const Quantity bushels      = bushel;
            const Quantity bu           = bushel;
            const Quantity barrel       = 36.0*fluid::gallons;
            const Quantity barrels      = barrel;
            const Quantity bbl          = barrel;
            const Quantity bl           = barrel;
        }
        const Quantity noggin           = 2.0*US::liquid::ounces;
        const Quantity noggins          = noggin;
        const Quantity cup              = 8.0*US::liquid::ounces;
        const Quantity cups             = cup;
        const Quantity fifth            = US::liquid::gallon/5.0;
        const Quantity fifths           = fifth;
        const Quantity jeroboam         = 4.0*fifths;
        const Quantity jeroboams        = jeroboam;
        const Quantity firkin           = 9.0*US::liquid::gallons;
        const Quantity firkins          = firkin;
        const Quantity kilderkin        = 18.0*US::liquid::gallons;
        const Quantity kilderkins       = kilderkin;
        const Quantity strike           = 2.0*US::bushels;
        const Quantity strikes          = strike;
        const Quantity sack             = 3.0*US::bushels;
        const Quantity sacks            = sack;
        const Quantity coomb            = 4.0*US::bushels;
        const Quantity coombs           = coomb;
        const Quantity seam             = 8.0*US::bushels;
        const Quantity seams            = seam;
        const Quantity wey              = 40.0*US::bushels;
        const Quantity weys             = wey;
        const Quantity last             = 80.0*US::bushels;
        const Quantity lasts            = last;
        const Quantity register_ton     = 100.0*(ft*ft*ft);
        const Quantity register_tons    = register_ton;
        const Quantity register_tn      = register_ton;
        const Quantity cord             = 128.0*(ft*ft*ft);
        const Quantity cords            = cord;
        const Quantity cordfoot         = cord;
        const Quantity cordfeet         = cordfoot;
        const Quantity boardfoot        = 144.0*inch*inch*inch;
        const Quantity boardfeet        = boardfoot;
        const Quantity timberfoot       = foot*foot*foot;
        const Quantity timberfeet       = timberfoot;
        const Quantity hogshead         = 2.0*US::barrels;
        const Quantity hogsheads        = hogshead;
        const Quantity pipe             = 4.0*US::barrels;
        const Quantity pipes            = pipe;
        const Quantity tun              = 8.0*US::barrels;
        const Quantity tuns             = tun;
        // mass
        const Quantity grain            = 0.06479891*grams;
        const Quantity grains           = grain;
        const Quantity gr               = grain;
        const Quantity pennyweight      = 24.0*grains;
        const Quantity dwt              = pennyweight;
        namespace apothecary { // weight
            const Quantity scruple      = 20.0*grains;
            const Quantity scruples     = scruple;
            const Quantity s            = scruple;
            const Quantity dram         = 3.0*scruples;
            const Quantity drams        = dram;
            const Quantity dr           = dram;
            const Quantity ounce        = 8.0*drams;
            const Quantity ounces       = ounce;
            const Quantity oz           = ounce;
            const Quantity pound        = 12.0*ounces;
            const Quantity pounds       = pound;
            const Quantity lb           = pound;
        }
        namespace troy = apothecary;
        namespace ap = apothecary;
        namespace t = apothecary;
        namespace avoirdupois { // weight
            const Quantity pound        = 7000.0*grains;
            const Quantity pounds       = pound;
            const Quantity lb           = pound;
            const Quantity ounce        = pound/16.0;
            const Quantity ounces       = ounce;
            const Quantity oz           = ounce;
            const Quantity dram         = ounce/16.0;
            const Quantity drams        = dram;
            const Quantity dr           = dram;
        }
        namespace avdp = avoirdupois;
        namespace av = avoirdupois;
        const Quantity stone            = 14.0*avoirdupois::pounds;
        const Quantity stones           = stone;
        const Quantity st               = stone;
        namespace US { // short
            const Quantity hundredweight= 100.0*avoirdupois::pounds;
            const Quantity cwt          = hundredweight;
            const Quantity quarter      = hundredweight/4.0;
            const Quantity quarters     = quarter;
            const Quantity qr           = quarter;
            const Quantity ton          = 20.0*hundredweight;
            const Quantity tons         = ton;
            const Quantity tn           = ton;
            const Quantity deadweight   = ton;
        }
        namespace British { // long
            const Quantity hundredweight= 112.0*avoirdupois::pounds;
            const Quantity cwt          = hundredweight;
            const Quantity quarter      = hundredweight/4.0;
            const Quantity quarters     = quarter;
            const Quantity qr           = quarter;
            const Quantity ton          = 20.0*hundredweight;
            const Quantity tons         = ton;
            const Quantity tn           = ton;
        }
        namespace English = British;
        namespace Imperial = British;
        const Quantity crith            = 0.0906*grams;
        const Quantity criths           = crith;
        const Quantity bag              = 94.0*avoirdupois::pounds;
        const Quantity bags             = bag;
        const Quantity cental           = 100.0*avoirdupois::pounds;
        const Quantity centals          = cental;
        const Quantity weymass          = 252.0*avoirdupois::pounds;
        // rate
        const Quantity mgd              = 1000000.0*US::liquid::gallons/day;
        const Quantity cfs              = foot*foot*foot/second;
        const Quantity minersinch       = 1.5*foot*foot*foot/minute;
        const Quantity mpg              = statute::miles/US::liquid::gallon;
        // speed
        const Quantity mph              = statute::miles/hour;
        const Quantity knot             = nautical::miles/hour;
        const Quantity knots            = knot;
        namespace admiralty {
            const Quantity knot         = 6980.0*feet/hour;
            const Quantity knots        = knot;
        }
        // force
        const Quantity poundal          = avdp::pound*foot/(second*second);
        const Quantity poundals         = poundal;
        const Quantity pdl              = poundal;
        const Quantity lbf              = avoirdupois::pound*gravity;
        // pressure
        const Quantity psi              = lbf/(inch*inch);
        // energy
        const Quantity calorie          = 4.1868*Joules;
        const Quantity calories         = calorie;
        const Quantity cal              = calorie;
        const Quantity kilocalorie      = 1000.0*calories;
        const Quantity kilocalories     = kilocalorie;
        const Quantity kcal             = kilocalorie;
        const Quantity Frigorie         = kilocalorie;
        const Quantity Frigories        = Frigorie;
        const Quantity Btu              = 1055.06*Joules;
        const Quantity therm            = 10000.0*Btu;
        const Quantity therms           = therm;
        const Quantity thermie          = 1000000.0*calories;
        const Quantity thermies         = thermie;
        // power
        const Quantity horsepower       = 735.49875*Watts;
        const Quantity HP               = horsepower;
        // electrical current
        const Quantity Gilbert          = 0.795775*Amperes;
        const Quantity Gilberts         = Gilbert;
        // temperature
        const Quantity Rankin           = 1.8*Kelvins;
        const Quantity Rankins          = Rankin;
        const Quantity R                = Rankin;
        // luminous intensity
        const Quantity candle           = 1.02*candelas;
        const Quantity candles          = candle;
        namespace Hefner {
            const Quantity candle       = 0.9*unit::candles;
            const Quantity candles      = candle;
        }
        // light intensity
        const Quantity foot_candle      = lumens/foot/foot;
        const Quantity foot_candles     = foot_candle;
        const Quantity fc               = foot_candle;
        const Quantity foot_Lambert     = candelas/foot/foot/pi;
        const Quantity foot_Lamberts    = foot_Lambert;
        namespace equivalent {
            const Quantity foot_candle  = unit::foot_candle/pi;
            const Quantity foot_candles = foot_candle;
            const Quantity fc           = foot_candle;
        }
    }
    namespace units = unit;
    // ####   END OF UNIT CREATION #### #

    // #### BEGIN OF CONSTANT CREATION #### #
    namespace constant {
        /* These using clauses will import these symbols into the constant
         * namespace, but oh well... */
        using unit::pi;
        using unit::m;
        using unit::s;
        using unit::J;
        using unit::C;
        using unit::V;
        using unit::kg;
        using unit::F;
        using unit::K;
        using unit::A;
        using unit::barns;
        using unit::T;
        using unit::rad;
        using unit::W;
        using unit::N;
        const Quantity c                = _PQINIT2(2.99792458e8*m/s,            "speed of light");
        const Quantity Mach             = _PQINIT2(331.46*m/s,                  "speed of sound");
        const Quantity h                = _PQINIT2(6.6260755e-34*J*s,           "Planck constant");
        const Quantity h_bar            = _PQINIT2(h/(2.0*pi),                  "Plank constant");
        const Quantity g                = _PQINIT2(unit::gravity,               "standard gravitational acceleration at sea level");
        // electron charge
        const Quantity e                = _PQINIT2(1.60217733e-19*C,            "electron charge");
        const Quantity eV               = _PQINIT2(e*V,                         "electron Volt");
        const Quantity keV              = _PQINIT2(1000.0*eV,                   "kilo-electron Volt");
        const Quantity MeV              = _PQINIT2(1000000.0*eV,                "Mega-electron Volt");
        const Quantity GeV              = _PQINIT2(1000000000.0*eV,             "Giga-electron Volt");
        const Quantity Rydberg          = _PQINIT2(13.6054*eV,                  "Rydberg energy");
        const Quantity Rydbergs         = Rydberg;
        const Quantity m_e              = _PQINIT2(9.1093897e-31*kg,            "electron mass");
        const Quantity m_P              = _PQINIT2(1.6726231e-27*kg,            "proton mass");
        // deuteron mass
        const Quantity m_D              = _PQINIT2(1875.61339*MeV/(c*c),        "deuteron mass");
        // unified atomic mass unit
        const Quantity atomic_mass_unit = _PQINIT2(1.6605402e-27*kg,            "unified atomic mass unit");
        const Quantity atomic_mass_units= atomic_mass_unit;
        const Quantity amu              = atomic_mass_unit;
        const Quantity Dalton           = atomic_mass_unit;
        const Quantity Daltons          = Dalton;
        const Quantity epsilon          = _PQINIT2(8.854187817e-12*F/m,         "permittivity of free space");
        const Quantity mu               = _PQINIT2(12.566370614e-7*N/(A*A),     "permeability of free space");
        const Quantity alpha            = _PQINIT3(1.0/137.0359895, units_map(),"fine-structure constant");
        const Quantity r_e              = _PQINIT2(2.81794092e-15*m,            "classical electron radius");
        const Quantity lambda_bar       = _PQINIT2(3.86159323e-13*m,            "electron Compton wavelength");
        const Quantity a_0              = _PQINIT2(0.529177249e-10*m,           "Bohr radius");
        const Quantity lambda_1eV       = _PQINIT2(1.23984244e-6*m,             "wavelength of 1 eV/c particle");
        const Quantity sigma_0          = _PQINIT2(0.66524616*barns,            "Thomson cross section");
        const Quantity mu_B             = _PQINIT2(5.78838263e-11*MeV/T,        "Bohr magneton");
        const Quantity mu_N             = _PQINIT2(3.15245166e-14*MeV/T,        "nuclear magneton");
        const Quantity E_M_e            = _PQINIT2(1.75881962e11*C/kg*(rad/(s*T)), "electron cyclotron frequency/field");
        const Quantity E_M_P            = _PQINIT2(9.5788309e7*C/kg*(rad/(s*T)),"proton cyclotron frequency/field");
        const Quantity G                = _PQINIT2(6.67259e-11*m*m*m/(kg*s*s),  "gravitational constant");
        const Quantity N_A              = _PQINIT3(6.0221367e23, units_map(),   "Avogadro's constant");
        const Quantity K_B              = _PQINIT2(1.380658e-23*J/K,            "Boltzmann constant");
        const Quantity V_molar          = _PQINIT2(2.897756e-3*m*K,             "molar volume, ideal gas at standard temperature and pressure");
        const Quantity sigma_SB         = _PQINIT2(5.67051e-8*W/(m*m*K*K*K*K),  "Stefan-Boltzmann constant");
    }
    namespace constants = constant;
    // ####   END OF CONSTANT CREATION #### #


    // #### BEGIN OF ELEMENT CREATION #### #
    // FIXME:  finish filling out this periodic table data
    /* This data is taken from physics.nist.gov. */
#define _ELEMENT(name,sym,n,m,i) \
    namespace name { \
        const Quantity number = n; \
        const Quantity mass = m * constant::amu; \
        const Quantity ionization = i * constant::eV; \
    } \
    namespace sym = name;

    namespace element {
        //       long-name    abrv.   n        mass       outer-electron ionization (eV)
        _ELEMENT(hydrogen,      H,    1,      1.00794,      13.5984 );
        _ELEMENT(helium,        He,   2,      4.002602,     24.5874 );
        _ELEMENT(lithium,       Li,   3,      6.941,         5.3917 );
        _ELEMENT(neon,          Ne,  10,      20.1797,      21.5645 );
        _ELEMENT(sodium,        Na,  11,      22.989770,     5.1391 );
        _ELEMENT(argon,         Ar,  18,      39.948,       15.7596 );
        _ELEMENT(potassium,      K,  19,      39.0983,       4.3407 );
        _ELEMENT(rubidium,      Rb,  37,      85.4678,       4.1771 );
        _ELEMENT(cesium,        Cs,  55,     132.90545,      3.8939 );
        _ELEMENT(mercury,       Hg,  80,     200.59,        10.4375 );
        _ELEMENT(francium,      Fr,  87,     223,            4.0727 );
    }
    namespace elements = element;
    // ####   END OF ELEMENT CREATION #### #
}


#endif // PHYSICAL_DATA_H

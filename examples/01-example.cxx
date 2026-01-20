// Use 64-bit integers as the underlying unit type
// #define UNIT_LIB_DEFAULT_TYPE int64_t

// For some embedded applications, it may be desirable to remove all
// references to <iostream> in order to reduce compiled binary size
// and RAM requirements.
// #define UNIT_LIB_DISABLE_IOSTREAM

// Only use length and time
// #define DISABLE_PREDEFINED_UNITS
// #define ENABLE_PREDEFINED_LENGTH_UNITS
// #define ENABLE_PREDEFINED_TIME_UNITS

// #undef pascal

// #undef _U
// #undef _L
// #undef _N
// #undef _S
// #undef _P
// #undef _C
// #undef _X
// #undef _B

#include <units.h>

using namespace units;
using namespace units::length;
using namespace units::time;
using namespace units::area;
using namespace units::velocity;
using namespace units::literals;




class myClass
{
  public:
    myClass() : m_speed(make_unit<miles_per_hour_t>(100)) {}
  private:
    miles_per_hour_t m_speed;
};


namespace length
{
	using meters = units::unit<std::ratio<1>, units::category::length_unit>;	// meters are (1) unit of length in the SI system.
	using feet   = units::unit<std::ratio<381, 1250>, meters>;					      // feet are 0.3048 meters.
}

struct RightTriangle
{
	using a = unit_value_t<meters, 3>;
	using b = unit_value_t<meters, 4>;
	using c = unit_value_sqrt<unit_value_add<unit_value_power<a, 2>, unit_value_power<b, 2>>>;
};


namespace time_ns
{
	using seconds = units::unit<std::ratio<1>,   units::category::time_unit>;
	using minutes = units::unit<std::ratio<60>,  seconds>;
	using hours   = units::unit<std::ratio<60>,  minutes>;
	using days    = units::unit<std::ratio<24>,  hours>;
	using weeks   = units::unit<std::ratio<7>,   days>;
	using years   = units::unit<std::ratio<365>, days>;
}


// UNIT_ADD(length, foot, feet, ft, unit<std::ratio<381, 1250>, meters>)
// UNIT_ADD_DECIBEL(power, watt, dBW)
// UNIT_ADD_CATEGORY_TRAIT(length, meter)


template<typename Units>
bool isMinimumSize(Units x)
{
	return x >= square_feet_t(80.0);
}


template<typename Units>
bool isMinimumSize2(Units x)
{
	static_assert(units::traits::is_area_unit<Units>::value, "Input value x must represent an area quantity.");
	return x >= square_feet_t(80.0);
}




int main() {
  {
    double          area     = 15 * 5 + 10 * 10;                // 175 m^2?
    square_meter_t  area_sqm = 15_m * 5_m + 10_m * 10_m;        // 175 m^2
  }
  {
    foot_t              len   = 5_m;                            // simple implicit conversion
    meters_per_second_t speed = 60_mi / 1_hr;                   // more complex implicit conversion
    square_meter_t      area  = 15_m * 5_m + 1000_cm * 1000_cm; // previous example with mixed units
  }
  {
    // meter_t  area = 15_m * 5_m + 10_m * 10_m;               // oops, m * m = m^2
    auto result = 15_m * 5_m + 10_m * 10_m;                 //  m^2
    auto speed  = 60_mi / 1_hr;                             //  60 mph
  }
  {
    using namespace units::math;
    meter_t a = 3_m;
    meter_t b = 4_m;
    meter_t c = sqrt(pow<2>(a) + pow<2>(b));    // Pythagorean threorem.
    std::cout << c << std::endl;                // prints: "5 m"
  }
  {
    meter_t distance_m(10); // Explicit initialization from double
    meter_t distance(10_m); // Explicit initialization from unit literal
    meter_t dist(100_ft);   // Explicit initialization from unit literal of a different type
  }
  {
    using meter_t = units::unit_t<units::length::meter, double, units::linear_scale>;
  }
  {
    using meter_t = units::unit_t<units::length::meter>;
  }
  {
    auto objectVelocity = meter_t(100.0) / second_t(2.0);
  }
  {
    // units::velocity::meters_per_second objectVelocity = square_meter_t(100.0) / second_t(2.0); // Error: Unit types are not compatible.`
  }
  {
    units::time::second_t a;
    units::time::minute_t b(1.0);
    a = b;	// a == 60.0
  }
  {
    using namespace units::length;
    using namespace units::area;
    using namespace units::dimensionless;

    meter_t a_m(1.0), b_m(2.0), c_m;
    foot_t	a_ft(1.0), b_ft(2.0), c_ft;

    c_m = a_m + b_m;                            // OK. c == 3m
    c_ft = a_m + b_m;                           // OK. resulting 3m is converted to ft.
    auto result = a_m + b_ft;                   // OK. result is `meter_t` (left-most unit)

    auto result_sm  = a_m * b_m;                 // OK. result_sm is `square_meter_t`.
    auto result_s   = a_m / b_m;                 // OK. result_s is `dimensionless_t`.
    auto result_mft = a_m * b_ft;                // OK. result is `square_meter_t` (left-most unit)

    auto result_sqm = a_m * square_meter_t(1.0);    // OK. units can always be multiplied. Result is `cubed<meter_t>`.
    auto result_sc  = a_m * scalar_t(1.0);          // OK. units can always be multiplied. Result is `meter_t`.
  }
  {
    using namespace units::literals;
    meter_t dist	= 10_m;		// 10 m
    meter_t dist2	= 1_km;		// 1000 m
    auto area  = units::length::meter_t(5) * units::length::meter_t(10);	// without literals
    auto area2 = 5_m * 10_m;							// with literals
  }
  {
    meter_t x(2.0);
    meter_t y(3.0);
    square_meter_t z(1.0);
    square_meter_t result;

    // result = fma(x, y, z);												// Error: ambiguous
    // double result = fma(x.toDouble(), y.toDouble(), z.toDouble());		// Warning: Unsafe!
    result = math::fma(x, y, z);										// OK.
    square_meter_t m2 = units::math::pow<2>(meter_t(5.0));	// m2 == 25.0
    meter_t m = units::math::sqrt(square_meter_t(4.0));		  // m == 2.0
  }
  {
    using namespace units;
    using namespace units::length;

    // Get double value from a unit container (double is the default underlying type of the units library)
    meter_t dist(10);
    double dval = unit_cast<double>(dist);
    double dval2 = dist.to<double>();

    // Get integer value (potentially narrowing, be careful!)
    int ival = unit_cast<int>(dist);
    int ival2 = dist.to<int>();
  }
  {
    auto a = RightTriangle::a::value();	// a is `meter_t(3)`
    auto b = RightTriangle::b::value();	// b is `meter_t(4)`
    auto c = RightTriangle::c::value();	// c is `meter_t(5)`
  }
  {
    double val_in = convert<feet, inches>(1.0);	// val_in == 12.0
  }
}

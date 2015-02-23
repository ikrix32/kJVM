package java.lang;

/**
 * Mathematical functions.
 *
 * @author <a href="mailto:bbagnall@escape.ca">Brian Bagnall</a>
 * /
public final class Math {

	// Math constants
	public static final float E = 2.718281828459045f;
	public static final float PI = 3.141592653589793f;
	public static final float NaN = 0.0f / 0.0f;

	public static final float PI2 = 1.570796326794897f;
	public static final float ln10 = 2.30258509299405f;
	public static final float ln2 = 0.69314718055995f;

	// Used by log() and exp() methods
	private static final float LOWER_BOUND = 0.9999999f;
	private static final float UPPER_BOUND = 1.0f;

	// Used to generate random numbers.
	// bh private static java.util.Random RAND = new
	// java.util.Random(System.currentTimeMillis());

	public static boolean isNaN(float d) {
		return d != d;
	}

	private Math() {
	} // To make sure this class is not instantiated

	// Private because it only works when -1 < x < 1 but it is used by atan2
	private static float ArcTan(float x) // Using a Chebyshev-Pade approximation
	{
		float x2 = x * x;
		return (0.7162721433f + 0.2996857769f * x2)
				* x
				/ (0.7163164576f + (0.5377299313f + 0.3951620469e-1f * x2) * x2);
	}

	/**
	 * Returns the smallest (closest to negative infinity) float value that is
	 * not less than the argument and is equal to a mathematical integer.
	 * /
	public static float ceil(float a) {
		return ((a < 0.f) ? (int) a : (int) (a + 1.f));
	}

	/**
	 * Returns the largest (closest to positive infinity) float value that is
	 * not greater than the argument and is equal to a mathematical integer.
	 * /
	public static float floor(float a) {
		return ((a < 0.f) ? (int) (a - 1.f) : (int) a);
	}

	public static float rint(float a) {
		float fl = floor(a);
		float cl = ceil(a);
		if (a - fl == cl - a) {
			return (fl % 2 == 0) ? fl : cl;
		}
		return round(a);
	}

	/**
	 * Returns the closest int to the argument.
	 * /
	public static int round(float a) {
		return (int) floor(a + 0.5f);
	}

	/**
	 * Returns the lesser of two integer values.
	 * /
	public static int min(int a, int b) {
		return ((a < b) ? a : b);
	}

	/**
	 * Returns the lesser of two float values.
	 * /
	public static float min(float a, float b) {
		return ((a < b) ? a : b);
	}

	/**
	 * Returns the greater of two integer values.
	 * /
	public static int max(int a, int b) {
		return ((a > b) ? a : b);
	}

	/**
	 * Returns the greater of two float values.
	 * /
	public static float max(float a, float b) {
		return ((a > b) ? a : b);
	}

	/**
	 * Random number generator. Returns a float greater than 0.0 and less than
	 * 1.0
	 * /
	/*
	 * bh public static float random() { final int MAX_INT = 2147483647; int n =
	 * MAX_INT;
	 * 
	 * // Just to ensure it does not return 1.0 while(n == MAX_INT) n = abs
	 * (RAND.nextInt()) ;
	 * 
	 * return n * (1.0f / MAX_INT); }
	 */
	/**
	 * Exponential function. Returns E^x (where E is the base of natural
	 * logarithms). Thanks to David Edwards of England for conceiving the code
	 * and Martin E. Nielsen for modifying it to handle large arguments. = sum
	 * a^n/n!, i.e. 1 + x + x^2/2! + x^3/3!
	 * <P>
	 * Seems to work better for +ve numbers so force argument to be +ve.
	 * /
	public static float exp(float a) {
		if (a == 0.f) {
			return 1.f;
		}

		boolean neg;
		if (a < 0.f) {
			neg = true;
			a = -a;
		} else {
			neg = false;
		}

		int fac = 1;
		float term = a;
		float sum = 0.f;
		float oldsum = 0.f;

		do {
			oldsum = sum;
			sum += term;
			term *= a / (++fac);
		} while (oldsum == 0 || (sum / oldsum < LOWER_BOUND)
				|| (sum / oldsum > UPPER_BOUND));

		sum += 1.0f;
		return neg ? 1.0f / sum : sum;
	}

	/**
	 * Natural log function. Returns log(a) to base E Replaced with an algorithm
	 * that does not use exponents and so works with large arguments.
	 * 
	 * @see <a
	 *      href="http://www.geocities.com/zabrodskyvlada/aat/a_contents.html">here</a>
	 * /
	public static float log(float x) {
		if (x <= 0.f) {
			throw new ArithmeticException();
		}

		if (x == 1.0f) {
			return 0.f;
		}

		if (x < 1.0f) {
			return -log(1.0f / x);
		}

		float m = -1.0f;
		float p = 1.0f;

		while (p < x) {
			m++;
			p = p * 2.f;
		}

		float z = x / (p / 2.f);

		float zeta = (1.0f - z) / (1.0f + z);
		float n = zeta;
		float ln = zeta;
		float zetasup = zeta * zeta;

		for (int j = 1; true; j++) {
			n = n * zetasup;
			float newln = ln + n / (2.f * j + 1.f);
			float term = ln / newln;
			if (term >= LOWER_BOUND && term <= UPPER_BOUND) {
				return m * ln2 - 2.f * ln;
			}
			ln = newln;
		}

	}

	/**
	 * Power function. This is a slow but accurate method. Thanks to David
	 * Edwards of England for conceiving the code.
	 * /
	public static float pow(float a, float b) {
		return exp(b * log(a));
	}

	/**
	 * Returns the absolute value of a float value. If the argument is not
	 * negative, the argument is returned. If the argument is negative, the
	 * negation of the argument is returned.
	 * /
	public static float abs(float a) {
		return ((a < 0.f) ? -a : a);
	}

	/**
	 * Returns the absolute value of an integer value. If the argument is not
	 * negative, the argument is returned. If the argument is negative, the
	 * negation of the argument is returned.
	 * /
	public static int abs(int a) {
		return ((a < 0) ? -a : a);
	}

	/**
	 * Sine function using a Chebyshev-Pade approximation. Thanks to Paulo Costa
	 * for donating the code.
	 * /
	public static float sin(float x) // Using a Chebyshev-Pade approximation
	{
		return cos(x - PI2);
	}

	/**
	 * Cosine function using a Chebyshev-Pade approximation. Thanks to Paulo
	 * Costa for donating the code.
	 * /
	public static float cos(float x) {
		int n = (int) (x / PI2) + 1;
		if (n < 1)
			n = n - 1;
		x = x - (n & 0xFFFFFFFE) * PI2; // reduce to the 4th and 1st quadrants

		float x2 = x * x;

		float si = 1.f;
		if ((n & 2) != 0)
			si = -1.f; // if it from the 2nd or the 3rd quadrants
		return si
				* (0.9457092528f + (-0.4305320537f + 0.1914993010e-1f * x2)
						* x2)
				/ (0.9457093212f + (0.4232119630e-1f + 0.9106317690e-3f * x2)
						* x2);
	}

	/**
	 * Square root - thanks to Paulo Costa for donating the code.
	 * /
	public static float sqrt(float x) {
		float root = x, guess = 0.f;

		if (x < 0.f)
			return NaN;

		// the accuarcy test is percentual
		for (int i = 0; (i < 16)
				&& ((guess > x * (1f + 5e-7f)) || (guess < x * (1f - 5e-7f))); i++) {
			root = (root + x / root) * 0.5f; // a multiplication is faster than
												// a division
			guess = root * root; // cache the square to the test
		}
		return root;
	}

	/**
	 * Tangent function.
	 * /
	public static float tan(float a) {
		return sin(a) / cos(a);
	}

	/**
	 * Arc tangent function. Thanks to Paulo Costa for donating the code.
	 * /
	public static float atan(float x) {
		return atan2(x, 1.f);
	}

	/**
	 * Arc tangent function valid to the four quadrants y and x can have any
	 * value without sigificant precision loss atan2(0,0) returns 0. Thanks to
	 * Paulo Costa for donating the code.
	 * /
	public static float atan2(float y, float x) {
		float ax = (float) abs(x);
		float ay = (float) abs(y);

		if ((ax < 1e-7f) && (ay < 1e-7f))
			return 0.f;

		if (ax > ay) {
			if (x < 0.f) {
				if (y >= 0.f)
					return ArcTan(y / x) + PI;
				else
					return ArcTan(y / x) - PI;
			} else
				return ArcTan(y / x);
		} else {
			if (y < 0.f)
				return ArcTan(-x / y) - PI / 2.f;
			else
				return ArcTan(-x / y) + PI / 2.f;
		}
	}

	/**
	 * Arc cosine function.
	 * /
	public static float acos(float a) {
		return atan(sqrt(1.f - a * a) / a);
	}

	/**
	 * Arc sine function.
	 * /
	public static float asin(float a) {
		return atan(a / sqrt(1.f - a * a));
	}

	/**
	 * Converts radians to degrees.
	 * /
	public static float toDegrees(float angrad) {
		return angrad * (360.0f / (2.f * PI));
	}

	/**
	 * Converts degrees to radians.
	 * /
	public static float toRadians(float angdeg) {
		return angdeg * ((2.f * PI) / 360.0f);
	}
}*/

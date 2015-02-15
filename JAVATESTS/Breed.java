public enum Breed {
	/**
	 * Dachshund, smooth and shaggy
	 */
	DACHSHUND,

	/**
	 * Dalmatian
	 */
	DALMATIAN,

	/**
	 * Labrador, black and golden
	 */
	LABRADOR;
	// -------------------------- PUBLIC STATIC METHODS
	// --------------------------

	/**
	 * is this a lap dog?
	 * 
	 * @param breed
	 *            breed of dog
	 * 
	 * @return true if it is a lap dog
	 */
	public static boolean lap(Breed breed) {
		// Example use of an enum in a switch
		switch (breed) {
		// Note how you do not specify Breed.DALMATIAN in the case label.
		// Java knows all case labels are Breed., because breed is a Breed.
		case DALMATIAN:
		case LABRADOR:
		default:
			return false;

		case DACHSHUND:
			return true;
		}
	}

	// --------------------------- CONSTRUCTORS ---------------------------

	// I could have used lower case enum constant names
	// e.g. dalmatian, or mixed case, e.g. Dalmatian
	// but since enums are actually constants,
	// Java convention says they should be all caps.
	// Traditionally you order them alphabetically.
	// Eclipse does that when you reorder methods.
	/**
	 * constructor, implicitly public
	 */
	Breed() {
	}

	// --------------------------- main() method ---------------------------

	/**
	 * Breed Test harness
	 * 
	 * @param args
	 *            not used
	 */
	// @SuppressWarnings( { "UnusedParameters" } )
	public static void main(String[] args) {
		// assignment
		Breed cedar = Breed.LABRADOR;

		// prints "LABRADOR";
		System.out.println(cedar);

		// Prints 2. DACHSHUND is 0. ordinals are 0-based as you would expect.
		System.out.println(cedar.ordinal());

		// prints "DACHSHUND";
		System.out.println(Breed.DACHSHUND);

		// comparison
		if (cedar.compareTo(Breed.DALMATIAN) > 0) {
			// should print this:
			System.out.println("LABRADOR comes after DALMATIAN");
		} else {
			System.out.println("DALMATIAN comes after LABRADOR");
		}

		// set to none of the above
		cedar = null;

		// converting from String to enum, peculiar but slick
		// saves reams of ifs or hashMap lookups.
		// The compiler magically generates this method for you.
		// You will not find item is the base Enum class.
		// Works for any String matching one of the breeds.
		// If there is no match for the String, valueOf will throw an
		// IllegalArgumentException.
		Breed myDogsBreed = Breed.valueOf("Dachshund".toUpperCase());
		System.out.println(myDogsBreed);

		// converting int to enum
		// breed 1 is Dalmatian.
		int i = 1;
		Breed theBreed = Breed.values()[i];
		System.out.println(theBreed);

		// Example iterating over all possibilities.
		// Print out a list of all possible breeds.
		System.out.println("All possible breeds");
		for (Breed breed : Breed.values()) {
			System.out.println(breed);
		}
		// end main
	}
}
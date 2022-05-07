class Syntax {
	public static void Main() {
		// null check
		string tst = null;
		char[] tstOut1 = tst?.ToCharArray(); // = tst == null ? null : tst.ToCharArray()
		//char[] tstOut2 = tst.ToCharArray(); // throws error
	
		// extension methods
		string src = "Hello, world!";
		src = src.Convert(src);
	}
}

static class ConversionExtensions {
	// can be called with src.Convert();
	public static string Convert(this string src) {
		// do stuff
		return src.ToUpper();
	}
}
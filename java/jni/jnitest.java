public class jnitest {
    
	// JNI Library laden
    static { System.loadLibrary("jnitest"); }

    // Construktor Aufruf, so dass die interne
	// Variable mit dem Pointerwert belegt wird
    public jnitest( int x ) {
		this.cpp_ptr = cpp_ctor(x);
	};

	// setNumber Methode, die via JNI an das C++ Objekt
	// weiter geleitet wird
    public native void setNumber( int n );

	// getNumber Methode, die via JNI die Methode
	// des C++ Objektes aufrufen
    public native int getNumber();
    
	// private Methode, die aus dem Konstruktor
	// aufgerufen wird und per JNI das C++ Objekt erzeugt
    private native long cpp_ctor( int x );

	// internet Pointer Variable, in der für das Java
	// der C++ Pointer gespeichert wird. Die Variable
	// wird "final", um Veränderungen auszuschließen
	// und nur durch den Construktor gesetzt werden kann
	private final long cpp_ptr;
	
	// dispose Methode, um das C++ Objekt vom Heap zu entfernen
	public native void dispose();
	
	// überladener finalize Aufruf 
	protected void finalize() { this.dispose(); }
}
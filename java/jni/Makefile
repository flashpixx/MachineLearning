# baut die dynamische Bibliothek
all: jnitest.o
	g++ -shared -W1 -soname -o libjnitest.dylib jnitest.o
	
jnitest.o: jnitest.cpp
	g++ -fPIC -c jnitest.cpp

# erzeugt die Javaklasse und den Stub-Aufruf für die
# JNI Calls und zeiger Signaturen & Typen der Javaklasse an
stub:
	javac jnitest.java
	javah jnitest	
	javap -s -p jnitest
	
# Java Compiler Aufruf
java:
	javac testprog.java
	javac jnitest.java
	
clean:
	rm *.o
	
clearall:
	rm *.o
	rm *.dylib
	rm *.class
	

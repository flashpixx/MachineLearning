#include "jnitest.h"
#include "jnitest_cpp.h"
#include <iostream>
#include <stdexcept>


// globale Variable für den Wert des Pointers für das C++ Objekt
// schnellerer Zugriff auf das Feld, nachdem ein Objekt instantiiert
// wurde
jfieldID g_fieldid = NULL;




/** gekapselte setNumber Methode
 * @param p_env Pointer auf die JVM
 * @param p_object Referenz auf das aufrufende Objekt / Klasse 
 * @param p_value übergebener Wert
 **/
JNIEXPORT void JNICALL Java_jnitest_setNumber(JNIEnv* p_env, jobject p_object, jint p_value)
{
	if (!g_fieldid)
		throw std::runtime_error("field ID not set");
		
	jnitest* l_ptr = (jnitest*) p_env->GetLongField(p_object, g_fieldid);
	if (!l_ptr)
		throw std::runtime_error("pointer is empty");

	l_ptr->setNumber( p_value );
}


/** gekapselte getNumber Methode
 * @param p_env Pointer auf die JVM
 * @param p_object Referenz auf das aufrufende Objekt / Klasse 
 * @return Rückgabewert der Methode
 **/
JNIEXPORT jint JNICALL Java_jnitest_getNumber(JNIEnv* p_env, jobject p_object)
{
	if (!g_fieldid)
		throw std::runtime_error("field ID not set");
		
	jnitest* l_ptr = (jnitest*) p_env->GetLongField(p_object, g_fieldid);
	if (!l_ptr)
		throw std::runtime_error("pointer is empty");
	
    return l_ptr->getNumber();
}


/** gekapselter privater ctor Aufruf
 * @param p_env Pointer auf die JVM
 * @param p_object Referenz auf das aufrufende Objekt / Klasse 
 * @param p_value übergebener Wert
 **/
JNIEXPORT jlong JNICALL Java_jnitest_cpp_1ctor(JNIEnv* p_env, jobject p_object, jint p_value)
{
	// speichern der FeldID für das "cpp_ptr" Feld global
	if (!g_fieldid) {
		jclass l_class = p_env->GetObjectClass( p_object );
		g_fieldid      = p_env->GetFieldID( l_class, "cpp_ptr", "J" );
	}
	
	// prüfe Wert des globalen Feld
	if (!g_fieldid)
		throw std::runtime_error("field ID not set");

	// wir erzeugen das C++ Objekt auf dem Heap und setze Pointerwert in das Javaobjekt
	// via return, da das dann in die final Variable gesetzt wird
	return (jlong)(new jnitest(p_value));
}


/** gekapselter privater ctor Aufruf
 * @param p_env Pointer auf die JVM
 * @param p_object Referenz auf das aufrufende Objekt / Klasse
**/
JNIEXPORT void JNICALL Java_jnitest_dispose(JNIEnv* p_env, jobject p_object)
{
	if (!g_fieldid)
		throw std::runtime_error("field ID not set");
		
	jnitest* l_ptr = (jnitest*) p_env->GetLongField(p_object, g_fieldid);
	if (!l_ptr)
		throw std::runtime_error("pointer is empty");
	
	// rufe Destruktor auf und setze final-Field auf 0, das geht via JNI ;-)
	delete(l_ptr);
	p_env->SetLongField(p_object, g_fieldid, 0);
	
}
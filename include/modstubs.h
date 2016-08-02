void * operator new (size_t, void * p)  { return p ; }
void * operator new[] (size_t, void * p)  { return p ; }
void operator delete (void *, void *)  { }
void operator delete[] (void *, void *) { }
extern "C" void __cxa_pure_virtual()
{
    // Do nothing or print an error message.
}
/** \brief beginning of constructor table */
extern "C" void(*start_ctors)(); 
/** \brief end of constructor table */
extern "C" void(*end_ctors)();
/** \brief start of destructor table */
extern "C" void(*start_dtors)();
/** \brief end of destructor table */
extern "C" void(*end_dtors)();
void doCtors() {
    for(void(**i)()=&start_ctors;i<&end_ctors;i++)
        (*i)(); //Calling constructors
}
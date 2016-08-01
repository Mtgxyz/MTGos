/** \brief beginning of constructor table */
extern "C" void(*start_ctors)(); 
/** \brief end of constructor table */
extern "C" void(*end_ctors)();
/** \brief start of destructor table */
extern "C" void(*start_dtors)();
/** \brief end of destructor table */
extern "C" void(*end_dtors)();
/**
 * \function _start()
 * \brief Initializes the kernel
 */
extern "C" void _start() {
    for(void(**i)()=&start_ctors;i<&end_ctors;i++)
        (*i)(); //Calling constructors
    for(void(**i)()=&start_dtors;i<&end_dtors;i++)
        (*i)(); //Calling destructors
    for(;;);
}
/**
 * \function __cxa_pure_virtual()
 * \brief Called when a function tries to call a pure-virtual function
 */
extern "C" void __cxa_pure_virtual()
{
    // Do nothing or print an error message.
}
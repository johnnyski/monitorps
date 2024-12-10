#define CCC
/*#undef CCC*/
#ifdef CCC
#define MONITORPS_PORT (unsigned short)5000 /* Also, communicate to our neighbors. */
#else
#define MONITORPS_PORT (unsigned short)5010 /* Also, communicate to our neighbors. */
#endif

#pragma once
#ifdef ENABLE_ROTATE_MODEL

#ifdef DEV
    #define ONLYDEV(code) code 
#else
    #define ONLYDEV(code)
#endif

#endif
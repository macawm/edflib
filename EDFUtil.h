/**
 @file EDFUtil.h
 @brief A set of utility functions.
 
 @author Anthony Magee
 @date 11/6/2010
 */

#ifndef _EDFUTIL_H
#define	_EDFUTIL_H

#include <string>

std::string convertSpaces(const std::string&);
std::string convertUnderscores(const std::string&);
std::string trim(std::string);

#ifdef DEBUG
#define DI(x) x;
#else
#define DI(x)
#endif

// i loop var name, b begin point, e end point, d incrementor value
#define range_loop(i, b, e, d) for (auto i = (b); i < (e); i+=d)

#endif	/* _EDFUTIL_H */


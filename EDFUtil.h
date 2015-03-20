/* 
 * File:   EDFUtil.h
 *
 * Created on November 6, 2010, 11:55 PM
 */

#ifndef _EDFUTIL_H
#define	_EDFUTIL_H

#include <string>

double a2f(std::string);
int a2i(std::string);
std::string convertSpaces(const std::string&);
std::string convertUnderscores(const std::string&);
std::string trim(std::string);

#ifdef DEBUG
#define DI(x) x;
#else
#define DI(x)
#endif

#endif	/* _EDFUTIL_H */


/* 
 * File:   EDFUtil.h
 *
 * Created on November 6, 2010, 11:55 PM
 */

#ifndef _EDFUTIL_H
#define	_EDFUTIL_H

#include <sstream>
#include <string>

double& a2f(std::string);
double& a2i(std::string);
string convertSpaces(const std::string&);
string convertUnderscores(const std::string&);
string trim(string);

#endif	/* _EDFUTIL_H */


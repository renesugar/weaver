/*
 * =====================================================================================
 *
 *       Filename:  property.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  Friday 12 October 2012 01:28:02  EDT
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ayush Dubey (), dubey@cs.cornell.edu
 *   Organization:  Cornell University
 *
 * =====================================================================================
 */

#ifndef __PROPERTY__
#define __PROPERTY__

//C
#include <stdlib.h>
#include <string.h>

namespace db 
{
namespace element
{
	class property
	{
		public:
			property ();
			//property (char* _key, char* _value);
			property (uint32_t, uint32_t);
		
		public:
			char* __key;
			char* __value;
			uint32_t key;
			uint32_t value;
			bool operator==(property p2) const;
	};

	inline
	property :: property ()
	{
	}

	inline
	property :: property (uint32_t _key, uint32_t _value)
	{
		key = _key;
		value = _value;
		/*
		key = (char*) malloc (strlen(_key));
		strncpy (key, _key, strlen(_key));
		value = (char*) malloc (strlen(_value));
		strncpy (value, _value, strlen(_value));
		*/
	}

	inline bool
	property :: operator==(property p2) const
	{
		/*
		if ((strcmp (key, p2.key) == 0) &&
			(strcmp (value, p2.value) == 0))
		{
			return true;
		} else
		{
			return false;
		}
		*/
		return ((key == p2.key) && (value == p2.value));
	}
}
}

#endif //__PROPERTY__
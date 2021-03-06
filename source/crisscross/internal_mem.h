/*
 *   CrissCross
 *   A multi-purpose cross-platform library.
 *
 *   A product of Uplink Laboratories.
 *
 *   (c) 2006-2009 Steven Noonan.
 *   Licensed under the New BSD License.
 *
 */

#ifndef __included_cc_internal_mem_h
#define __included_cc_internal_mem_h

#include <crisscross/debug.h>

namespace CrissCross
{
	namespace Data
	{
		/*! @cond */

		/* These functions are designed for internal use. */
		/* You generally shouldn't use them unless you know what you're doing. */

		template <class T>
		__inline T Duplicate(T const &_data)
		{
			return _data;
		}

		template <>
		__inline char *Duplicate(char * const &_data)
		{
			if (!_data) return NULL;

			return ::cc_strdup(_data);
		}

		template <>
		__inline const char *Duplicate(const char * const &_data)
		{
			if (!_data) return NULL;

			return ::cc_strdup(_data);
		}

		template <class T>	__inline T		NullKey() { return T(); }
		template <>		__inline cc_int8_t	NullKey() { return 0; }
		template <>		__inline cc_uint8_t	NullKey() { return 0; }
		template <>		__inline cc_int16_t	NullKey() { return 0; }
		template <>		__inline cc_uint16_t	NullKey() { return 0; }
		template <>		__inline cc_int32_t	NullKey() { return 0; }
		template <>		__inline cc_uint32_t	NullKey() { return 0; }
		template <>		__inline cc_int64_t	NullKey() { return 0; }
		template <>		__inline cc_uint64_t	NullKey() { return 0; }
		template <>		__inline cc_long_t	NullKey() { return 0; }
		template <>		__inline cc_ulong_t	NullKey() { return 0; }
		template <>		__inline char *		NullKey() { return NULL; }
		template <>		__inline const char *	NullKey() { return NULL; }
		template <>		__inline std::string	NullKey() { return std::string(""); }

		template <class T>
		__inline void Dealloc(__unused_param T &_data)
		{
		}

		template <>
		__inline void Dealloc(char * &_data)
		{
			if (_data) {
				::free(_data);
				_data = NULL;
			}
		}

		template <>
		__inline void Dealloc(const char * &_data)
		{
			if (_data) {
				::free((char *)_data);
				_data = NULL;
			}
		}

		/*! @endcond */
	}
}

#endif

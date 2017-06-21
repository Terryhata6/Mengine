#	include "UID.h"

#	include <vector>
#	include <random>
#	include <functional>
#	include <algorithm>

namespace Menge
{
	namespace Helper
	{
		//////////////////////////////////////////////////////////////////////////
		static const Char char_array[] = {
			'0', '1', '2', '3', '4',
			'5', '6', '7', '8', '9',
			'A', 'B', 'C', 'D', 'E', 'F',
			'G', 'H', 'I', 'J', 'K',
			'L', 'M', 'N', 'O', 'P',
			'Q', 'R', 'S', 'T', 'U',
			'V', 'W', 'X', 'Y', 'Z',
			'a', 'b', 'c', 'd', 'e', 'f',
			'g', 'h', 'i', 'j', 'k',
			'l', 'm', 'n', 'o', 'p',
			'q', 'r', 's', 't', 'u',
			'v', 'w', 'x', 'y', 'z'
		};
		//////////////////////////////////////////////////////////////////////////
		String makeUID( size_t _length )
		{
			std::default_random_engine rng( std::random_device{}() );

			size_t length = 20;

			std::uniform_int_distribution<> dist( 0, sizeof( char_array ) );

			String uid( length, 0 );
			std::generate_n( uid.begin(), length, [&dist, &rng]() {size_t e = dist( rng ); return char_array[e]; } );

			return uid;
		}
	}
}
/**
  * Bloom Filter
  * 
  * Author : Chitresh Kakwani <chitreshkakwani@gmail.com>
  *
  */

#include <stdint.h>


inline uint32_t rotl32 ( uint32_t x, int8_t r )
{
	return (x << r) | (x >> (32 - r));
}

#define ROTL32(x,y)	rotl32(x,y)

inline uint32_t getblock ( const uint32_t * p, int i )
{
	return p[i];
}

class BloomFilter
{

public:

	inline void insert(const unsigned char* element, int length)
	{

	}

	template<typename T>
		inline void insert(const T& obj)
		{
			insert(reinterpret_cast<const unsigned char*>(&obj), sizeof(obj));
		}



	// Finalization mix - force all bits of a hash block to avalanche

	inline uint32_t fmix ( uint32_t h )
	{
		h ^= h >> 16;
		h *= 0x85ebca6b;
		h ^= h >> 13;
		h *= 0xc2b2ae35;
		h ^= h >> 16;

		return h;
	}


	void murmur_hash ( const void * key, int len, uint32_t seed, void * out )
	{
		const uint8_t * data = (const uint8_t*)key;
		const int nblocks = len / 4;

		uint32_t h1 = seed;

		uint32_t c1 = 0xcc9e2d51;
		uint32_t c2 = 0x1b873593;

		//----------
		// body

		const uint32_t * blocks = (const uint32_t *)(data + nblocks*4);

		for(int i = -nblocks; i; i++)
		{
			uint32_t k1 = getblock(blocks,i);

			k1 *= c1;
			k1 = ROTL32(k1,15);
			k1 *= c2;

			h1 ^= k1;
			h1 = ROTL32(h1,13); 
			h1 = h1*5+0xe6546b64;
		}

		//----------
		// tail

		const uint8_t * tail = (const uint8_t*)(data + nblocks*4);

		uint32_t k1 = 0;

		switch(len & 3)
		{
			case 3: k1 ^= tail[2] << 16;
			case 2: k1 ^= tail[1] << 8;
			case 1: k1 ^= tail[0];
				k1 *= c1; k1 = ROTL32(k1,15); k1 *= c2; h1 ^= k1;
		};

		//----------
		// finalization

		h1 ^= len;

		h1 = fmix(h1);

		*(uint32_t*)out = h1;
	}

private:

	unsigned char* m_bitset;
	long m_bitset_size;
	long m_inserted_elements;
};


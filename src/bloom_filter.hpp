/**
  * Bloom Filter
  * 
  * Author : Chitresh Kakwani <chitreshkakwani@gmail.com>
  *
  */

#include <stdint.h>


static const unsigned int bits_per_char = 8;
static const unsigned char bit_mask[bits_per_char] = {
							0x01,  //00000001
							0x02,  //00000010
							0x04,  //00000100 
							0x08,  //00001000
							0x10,  //00010000
							0x20,  //00100000
							0x40,  //01000000
							0x80   //10000000
					 	     };

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

	BloomFilter():m_bitset_size(0), m_hash_count(5)
	{
	}


	void init(long filterSize)
	{
		// TODO: Calculate the optimal number of hashes
		m_hashes = new uint32_t[m_hash_count];
		m_bitset_size = filterSize;
		m_bitset = new unsigned char[m_bitset_size];
	}	
	
	inline void insert(const unsigned char* element, int length)
	{
		// Calculate indices
		hash(element, length, m_hashes);
		
		int charIndex = 0;
		int bitIndex = 0;
		// Set the correspoding bits in bitset
		for(int i = 0; i < m_hash_count; i++)
		{
			charIndex = m_hashes[i] % m_bitset_size;
			bitIndex = charIndex % bits_per_char;
			charIndex /= bits_per_char;
			m_bitset[charIndex] |= bit_mask[bitIndex];
		}
			
	}


	inline bool contains(const unsigned char* element, int length)
	{
		hash(element, length, m_hashes);
		int charIndex = 0;
		int bitIndex = 0;
		
		for(int i=0; i < m_hash_count; i++)
		{
			charIndex = m_hashes[i] % m_bitset_size;
			bitIndex = charIndex % bits_per_char;
			charIndex /= bits_per_char;
			unsigned char temp = m_bitset[charIndex];
			if(!(temp & bit_mask[bitIndex]))
				return false;
		}
		return true;
	}

	inline void hash(const unsigned char* data, int len, uint32_t* hashes)
	{
		uint32_t hash1;
		uint32_t hash2;

		murmur_hash(data, len, 0, &hash1);
		murmur_hash(data, len, hash1, &hash2);
	
		for(int i = 0; i < m_hash_count; i++)
		{
			hashes[i] = (hash1 + i * hash2);
		}
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
	int m_hash_count;
	uint32_t* m_hashes;
	long m_inserted_elements;
};


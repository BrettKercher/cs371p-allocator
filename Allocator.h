// ------------------------------
// projects/allocator/Allocator.h
// Copyright (C) 2014
// Glenn P. Downing
// ------------------------------

#ifndef Allocator_h
#define Allocator_h

// --------
// includes
// --------

#include <cassert>   // assert
#include <cstddef>   // ptrdiff_t, size_t
#include <new>       // bad_alloc, new
#include <stdexcept> // invalid_argument

// ---------
// Allocator
// ---------

template <typename T, int N>
class Allocator {
    public:
        // --------
        // typedefs
        // --------

        typedef T                 value_type;

        typedef std::size_t       size_type;
        typedef std::ptrdiff_t    difference_type;

        typedef       value_type*       pointer;
        typedef const value_type* const_pointer;

        typedef       value_type&       reference;
        typedef const value_type& const_reference;

    public:
        // -----------
        // operator ==
        // -----------

        friend bool operator == (const Allocator&, const Allocator&) {
            return true;}                                              // this is correct

        // -----------
        // operator !=
        // -----------

        friend bool operator != (const Allocator& lhs, const Allocator& rhs) {
            return !(lhs == rhs);}

    private:
        // ----
        // data
        // ----

        char a[N];
		
		// -----
        // first_free
        // -----
		
		int first_free (size_type n)
		{
			int current_position = 0;
			int sentinel_value = view(current_position);
			int abs_sentinel_value = (sentinel_value < 0 ? -sentinel_value : sentinel_value);
			
			while(current_position < N)
			{
				if(sentinel_value >= (int)n)
					return current_position;
				
				current_position += abs_sentinel_value + (2*sizeof(int));
				if(current_position == N)
					return -1;
				
				sentinel_value = view(current_position);
				abs_sentinel_value = (sentinel_value < 0 ? -sentinel_value : sentinel_value);
			}
			
			return -1;
		}

        // -----
        // valid
        // -----

        /**
         * O(1) in space
         * O(n) in time
         * valid walks through a[] by going from sentinel to sentinel, making sure that sentinel pairs match
         */
        bool valid () const {
            
			int current_position = 0;
			int sentinel_value = view(current_position);
			int abs_sentinel_value = (sentinel_value < 0 ? -sentinel_value : sentinel_value);
			
			while(current_position < N)
			{				 
				if(view(current_position + abs_sentinel_value + sizeof(int)) != sentinel_value)
					return false;
				
				
				current_position += abs_sentinel_value + (2*sizeof(int));
				if(current_position == N)
					return true;
				
				sentinel_value = view(current_position);
				abs_sentinel_value = (sentinel_value < 0 ? -sentinel_value : sentinel_value);
			}
			
            return false;}

        /**
         * O(1) in space
         * O(1) in time
         * view allows you to interpret a[i] as an int for sentinels
         */
        int& view (int i) {
            return *reinterpret_cast<int*>(&a[i]);}

    public:
        // ------------
        // constructors
        // ------------

        /**
         * O(1) in space
         * O(1) in time
         * throw a bad_alloc exception, if N is less than sizeof(T) + (2 * sizeof(int))
         */
        Allocator () 
		{
			if(N < (sizeof(T) + 2*sizeof(int)) )
				throw(std::bad_alloc());
			
            int sentinel_value = N - 8; // There are two sentinels of size 4, so the total usable space is N - 4 - 4, or N - 8
            
            view(0) = sentinel_value; // Set the first sentinel value
            
            view(N-4) = sentinel_value; // Set the last sentinel value
            
            assert(valid());
		}

        // Default copy, destructor, and copy assignment
        // Allocator  (const Allocator&);
        // ~Allocator ();
        // Allocator& operator = (const Allocator&);

        // --------
        // allocate
        // --------

        /**
         * O(1) in space
         * O(n) in time
         * after allocation there must be enough space left for a valid block
         * the smallest allowable block is sizeof(T) + (2 * sizeof(int))
         * choose the first block that fits
         * return 0, if allocation fails
         */
        pointer allocate (size_type n) 
		{
            //find free block of sufficient size
            
            //modify the first sentinel to be -n*sizeof(T), add the second matching sentinel
            
            //Add a sentinel after second used sentinel of old free space - n*sizeof(T) + 8
			
			//int old_sentinel;
			int p = first_free(n);
            
			if(p < 0)
				return 0;
			else
			{
				//old_sentinel = view(p);
			}
			
            assert(valid());
            return 0;
		}

        // ---------
        // construct
        // ---------

        /**
         * O(1) in space
         * O(1) in time
         * <your documentation>
         */
        void construct (pointer p, const_reference v) {
            new (p) T(v);                               // this is correct and exempt
            assert(valid());}                           // from the prohibition of new

        // ----------
        // deallocate
        // ----------

        /**
         * O(1) in space
         * O(1) in time
         * after deallocation adjacent free blocks must be coalesced
         * <your documentation>
         */
        void deallocate (pointer p, size_type) {
            // <your code>
            assert(valid());}

        // -------
        // destroy
        // -------

        /**
         * O(1) in space
         * O(1) in time
         * throw an invalid_argument exception, if pointer is invalid
         * <your documentation>
         */
        void destroy (pointer p) {
            p->~T();               // this is correct
            assert(valid());}

        /**
         * O(1) in space
         * O(1) in time
         * <your documentation>
         */
        const int& view (int i) const {
            return *reinterpret_cast<const int*>(&a[i]);}};

#endif // Allocator_h
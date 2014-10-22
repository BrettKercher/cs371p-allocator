// ------------------------------------
// projects/allocator/TestAllocator.c++
// Copyright (C) 2014
// Glenn P. Downing
// ------------------------------------

// --------
// includes
// --------

#include <algorithm> // count
#include <memory>    // allocator

#include "gtest/gtest.h"

#include "Allocator.h"

// -------------
// TestAllocator
// -------------

template <typename A>
struct TestAllocator : testing::Test {
    // --------
    // typedefs
    // --------

    typedef          A                  allocator_type;
    typedef typename A::value_type      value_type;
    typedef typename A::difference_type difference_type;
    typedef typename A::pointer         pointer;
};

typedef testing::Types<
            std::allocator<int>,
            std::allocator<double>,
            Allocator<int, 100>,
            Allocator<double, 100> >
        my_types;

TYPED_TEST_CASE(TestAllocator, my_types);

TYPED_TEST(TestAllocator, One) 
{
    typedef typename TestFixture::allocator_type  allocator_type;
    typedef typename TestFixture::value_type      value_type;
    typedef typename TestFixture::difference_type difference_type;
    typedef typename TestFixture::pointer         pointer;

    allocator_type x;
    const difference_type s = 1;
    const value_type      v = 2;
    const pointer         p = x.allocate(s);
    if (p != 0) 
	{
        x.construct(p, v);
        ASSERT_EQ(v, *p);
        x.destroy(p);
        x.deallocate(p, s);
	}
}

TYPED_TEST(TestAllocator, Ten) 
{
    typedef typename TestFixture::allocator_type  allocator_type;
    typedef typename TestFixture::value_type      value_type;
    typedef typename TestFixture::difference_type difference_type;
    typedef typename TestFixture::pointer         pointer;

    allocator_type x;
    const difference_type s = 10;
    const value_type      v = 2;
    const pointer         b = x.allocate(s);
    if (b != 0) 
	{
        pointer e = b + s;
        pointer p = b;
        try 
        {
            while (p != e) 
			{
                x.construct(p, v);
                ++p;
			}
		}
        catch (...) 
		{
            while (b != p) 
			{
                --p;
                x.destroy(p);
			}
            x.deallocate(b, s);
            throw;
		}
        ASSERT_EQ(s, std::count(b, e, v));
        while (b != e) 
		{
            --e;
            x.destroy(e);
		}
        x.deallocate(b, s);
	}
}
            
 
TYPED_TEST(TestAllocator, test_constructor)
{
	typedef typename TestFixture::allocator_type  allocator_type;
    typedef typename TestFixture::value_type      value_type;
    typedef typename TestFixture::difference_type difference_type;
    typedef typename TestFixture::pointer         pointer;
	
	bool pass;
	
	try
	{
		const allocator_type x;
		pass = true;
	}
	catch (const std::bad_alloc& e)
	{
		pass = false;
	}
	
	ASSERT_EQ(pass, true);
} 

TYPED_TEST(TestAllocator, test_equality)
{
	typedef typename TestFixture::allocator_type  allocator_type;
    typedef typename TestFixture::value_type      value_type;
    typedef typename TestFixture::difference_type difference_type;
    typedef typename TestFixture::pointer         pointer;
	
	try
	{
		const allocator_type x;
		const allocator_type y;
		
		ASSERT_EQ(x, y);
	}
	catch (const std::bad_alloc& e)
	{
		ASSERT_TRUE(false);
	}
}
 
/***********************
*						*
* 	Test My Allocator	* 
*						*
************************/


struct my_struct
{
	int s_int;
	double s_double;
	bool s_bool;
	my_struct* next;
};
      
TEST(TestMyAllocator, test_my_constructor_1)
{
	bool pass;
	try
	{
		Allocator<int, 11> x;
		pass = false;
	}
	catch (const std::bad_alloc& e)
	{
		pass = true;
	}
	
	ASSERT_EQ(pass, true);
}

TEST(TestMyAllocator, test_my_constructor_2)
{
	bool pass;
	
	try
	{
		Allocator<my_struct, sizeof(my_struct)> x;
		pass = false;
	}
	catch (const std::bad_alloc& e)
	{
		pass = true;
	}
	
	ASSERT_EQ(pass, true);
}

TEST(TestMyAllocator, test_constructor_3)
{	
	bool pass;
	
	try
	{
		const Allocator<int, 100> x;
		
		ASSERT_EQ(x.view(0), 92);
		
		pass = true;
	}
	catch (const std::bad_alloc& e)
	{
		pass = false;
	}
	
	ASSERT_EQ(pass, true);
} 

TEST(TestMyAllocator, test_constructor_4)
{	
	bool pass;
	
	try
	{
		const Allocator<int, 100> x;
		
		ASSERT_EQ(x.view(96), 92);
		
		pass = true;
	}
	catch (const std::bad_alloc& e)
	{
		pass = false;
	}
	
	ASSERT_EQ(pass, true);
} 

TEST(TestMyAllocator, test_allocate_1)
{	
	try
	{
		Allocator<int, 100> x;
		const Allocator<int, 100>& y = x;
		
		ASSERT_EQ(y.view(0), 92);
		ASSERT_EQ(y.view(96), 92);
		
		x.allocate(1);
		
		ASSERT_EQ(y.view(0), -4);
		ASSERT_EQ(y.view(8), -4);

	}
	catch (const std::bad_alloc& e)
	{
		ASSERT_TRUE(false);
	}
}  

TEST(TestMyAllocator, test_allocate_2)
{	
	try
	{
		Allocator<int, 100> x;
		const Allocator<int, 100>& y = x;
		
		ASSERT_EQ(y.view(0), 92);
		ASSERT_EQ(y.view(96), 92);
		
		x.allocate(1);
		
		ASSERT_EQ(y.view(12), 80);
		ASSERT_EQ(y.view(96), 80);
	}
	catch (const std::bad_alloc& e)
	{
		ASSERT_TRUE(false);
	}
} 

TEST(TestMyAllocator, test_allocate_3)
{	
	try
	{
		Allocator<int, 100> x;
		const Allocator<int, 100>& y = x;
		
		ASSERT_EQ(y.view(0), 92);
		ASSERT_EQ(y.view(96), 92);
		
		x.allocate(1);
		
		x.allocate(1);
		
		ASSERT_EQ(y.view(12), -4);
		ASSERT_EQ(y.view(20), -4);
	}
	catch (const std::bad_alloc& e)
	{
		ASSERT_TRUE(false);
	}
} 

TEST(TestMyAllocator, test_allocate_4)
{	
	try
	{
		Allocator<int, 100> x;
		const Allocator<int, 100>& y = x;
		
		ASSERT_EQ(y.view(0), 92);
		ASSERT_EQ(y.view(96), 92);
		
		x.allocate(1);
		
		x.allocate(1);
		
		ASSERT_EQ(y.view(24), 68);
		ASSERT_EQ(y.view(96), 68);
	}
	catch (const std::bad_alloc& e)
	{
		ASSERT_TRUE(false);
	}
} 

TEST(TestMyAllocator, test_allocate_5)
{	
	try
	{
		Allocator<int, 13> x;
		const Allocator<int, 13>& y = x;
		
		ASSERT_EQ(y.view(0), 5);
		ASSERT_EQ(y.view(9), 5);
		
		x.allocate(1);
		
		ASSERT_EQ(y.view(0), -5);
		ASSERT_EQ(y.view(9), -5);
	}
	catch (const std::bad_alloc& e)
	{
		ASSERT_TRUE(false);
	}
} 
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
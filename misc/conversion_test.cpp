#include <iostream>
#include <cstdint>

template< int n >
struct A;
struct B;
struct C {
	C() = delete;
	C( int t, const int n ) : a(t), n_(n) {};
	int a;
	const int n_;
};

template<int n >
struct A {
	constexpr operator C() const { return C(a, n_); }
	A( int t ) : a(t), n_(n) {};
	int a;
	const int n_;
};

template<int n1, int n2>
auto operator +( const A<n1>& lhs, const A<n2>& rhs ) {
	if constexpr ( n1 < n2 )
		return A< n2 >(lhs.a+rhs.a);
	else 
		return A< n1 >(lhs.a+rhs.a);
}
/*
auto operator +( const C& lhs, const C& rhs ) { 
	constexpr const int a = lhs.n_;
	const int b = rhs.n_;
	int sum  = lhs.a + rhs.a;

	return A<a>(sum);
	
};//C( lhs.a + rhs.a ); }
*/

int main() {
	A<10> a(5000);
	A<11> b(1234);
	auto c = a+b;
	auto d = b+a; // the same but swapped, I wish it wouldnt recreate another + operator though...
	//A<12> e = a + b; incorrect since it will instantiate a A<11>

	std::cout << "Test " << c.n_ << "\n";
	std::cout << "Test " << d.n_ << "\n";
	std::cout << "Test " << e.n_ << "\n";
	return 0;
}

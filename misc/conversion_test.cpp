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

auto operator +( const C& lhs, const C& rhs ) { 
	const int a = lhs.n_;
	const int b = rhs.n_;
	int sum  = lhs.a + rhs.a;

	return A<23>(sum);
	
};//C( lhs.a + rhs.a ); }

int main() {
	A<10> a(5000);
	A<11> b(1234);
	auto c = a+b;

	std::cout << "Test " << c.n_ << "\n";
	return 0;
}

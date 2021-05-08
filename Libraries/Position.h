#include <vector>
#include <math.h>

// The seven deadly symmetries
u32 RootReflect( u32 x , int size, int Half) {
	return x + 2*( Half - x % size);
}

u32 RootRotate( u32 x , int size, int Half ) {
	return size *( 2*Half - x%size) + x/size;
}

u32 Rotate2( u32 x, int size, int Half ) {
	// int a = Half - x%size;
	// int b = Half - x/size;
	return size * (2*Half - x/size) + 2*Half - x%size;
}

u32 BackRotate( u32 x, int size, int Half ) {
	// int a = Half - x/size;
	// int b = x%size;
	return size * (x%size) + 2*Half - x/size;
}

u32 AllRotate(u32 x, int size, int Half, int r=0) {
	return size * (2*Half*(r == 0) - (x/size)*(r%2 == 1) - (x%size)*(r%2 == 0)) + (2*Half)*(r > 0) + (r > 0? -1:1)*((x/size)*(r%2==0) + (x%size)*(r%2 == 1)) ;
}

u32 AllRotate2(u32 x, int size, int Half, int r=0) {
	switch(r) {
		case 0:
			r = size *( 2*Half - x%size) + x/size;
			break;
		case 1:
			r = size * (2*Half - x/size) + 2*Half - x%size;
			break;
		case 2:
			r = size * (x%size) + 2*Half - x/size;
			break;
	}
	return r;
}

u32 TLBR( u32 x, int size, int Half ) {
	return RootReflect( RootRotate(x, size, Half) );
}

u32 TB( u32 x, int size, int Half ) {
	return RootReflect( Rotate2(x, size, Half) );
}

u32 BLTR( u32 x, int size, int Half ) {
	return RootReflect( BackRotate(x, size, Half) );
}

u32 AllReflect(u32 x, int size, int Half, int r=0) {
	switch(r) {
		case 0:
			r = RootReflect(x, size, Half);
			break;
		case 1:
			r = RootReflect( RootRotate(x, size, Half), size, Half );
			break;
		case 2:
			r = RootReflect( Rotate2(x, size, Half), size, Half );
			break;
		case 3:
			r = RootReflect( BackRotate(x, size, Half), size, Half );
			break;
		return r;
	}
}

namespace TicTacToe {

class Grid5x5 {
public:
	static const int size = 5;
	static const int Half = size/2;

	char move_history[25];

	typedef unsigned long u64;
	typedef unsigned int u32;

public:
	// The seven deadly symmetries
	u32 RootReflect( u32 x ) {
		return x + 2*( Half - x % size);
	}

	u32 RootRotate( u32 x ) {
		return size *( 2*Half - x%size) + x/size;
	}

	u32 Rotate2( u32 x ) {
		// int a = Half - x%size;
		// int b = Half - x/size;
		return size * (2*Half - x/size) + 2*Half - x%size;
	}

	u32 BackRotate( u32 x ) {
		// int a = Half - x/size;
		// int b = x%size;
		return size * (x%size) + 2*Half - x/size;
	}

	u32 AllRotate(u32 x, int r=0) {
		return size * (2*Half*(r == 0) - (x/size)*(r%2 == 1) - (x%size)*(r%2 == 0)) + (2*Half)*(r > 0) + (r > 0? -1:1)*((x/size)*(r%2==0) + (x%size)*(r%2 == 1)) ;
	}

	u32 AllRotate2(u32 x, int r=0) {
		switch(r) {
			case 0:
				r = size *( 2*Half - x%size) + x/size;
				break;
			case 1:
				r = size * (2*Half - x/size) + 2*Half - x%size;
				break;
			case 2:
				r = size * (x%size) + 2*Half - x/size;
				break;
		}
		return r;
	}

	u32 TLBR( u32 x ) {
		return RootReflect( RootRotate(x) );
	}

	u32 TB( u32 x ) {
		return RootReflect( Rotate2(x) );
	}

	u32 BLTR( u32 x ) {
		return RootReflect( BackRotate(x) );
	}

	u32 AllReflect(u32 x, int r=0) {
		switch(r) {
			case 0:
				r = RootReflect(x);
				break;
			case 1:
				r = RootReflect( RootRotate(x) );
				break;
			case 2:
				r = RootReflect( Rotate2(x) );
				break;
			case 3:
				r = RootReflect( BackRotate(x) );
				break;
			return r;
		}
	}

	u64 rotate( u64 x ) {
		u32 temp = x;
		x = 0;
		for(u32 i = popcount(temp); i >= 1; i --) {
			u32 digit = Fdigit(temp);
			x |= AllRotate2(digit, r);
			temp ^= (1ULL << digit);
		}
		return x;
	}

	u64 reflect( u64 x, int r=0 ) {
		u32 temp = x;
		x = 0;
		for(u32 i = popcount(temp); i >= 1; i --) {
			u32 digit = Fdigit(temp);
			x |= AllReflect(digit, r);
			temp ^= (1ULL << digit);
		}
		return x;
	}

	// End of 7 deadly symmetires

	// u64 AllRotate( u32 x, int n ) {
	// 	for(int i = 0; i < n; i++) x = RootRotate(x);
	// 	return x;
	// }

	Grid5x5(): mask{0}, current_position{0}, moves{0}; {}

	int getBit( u64 x, int digit ) {return (x >> (digit))&1;}

	u32 popcount( u64 m ) {
		u32 c = 0;
		for(c = 0; m; c++) m &=m -1;
		return c;
	}

	u64 first( u64 m ) {
		int count = popcount(m)-2;
		for(int x = 0; x < count; x++) m &= m - 1;
		return m;
	}

	u32 Fdigit( u64 m ) {
		return (int)log2(m);
	}

	bool Edge(int power, int dir) {
		bool r = power < 8 && dir > 4;
		r |= power > 55 && dir < 3;
		r |= (power % 8 == 7) && (dir == 0)|(dir==3)|(dir==5);
		r |= (power % 8 == 0) && (dir == 2)|(dir==4)|(dir==7);
		return r;
	}

	u64 AllPossible( u64 mask ) {
		return ~mask;
	}

	u64 UniPossible( u64 mask ) {
		
		return ~mask;
	}

	int negaAB( u64 mask, u64 curr, int color, int α, int β, int end) {
		if(end == 0 || popcount(~mask) == 0 || popcount(possible(mask,curr)) == 0) {
		  return color * popcount(curr);
		}
		int value = -10000;
		u64 children = possible(mask, curr);
		for(u32 x = popcount(children); x >= 1; x--) {
		  u64 child = first(children);
		  value = max(value,-negaAB(mask|child, curr|child, -β, -α, -color,end-1));
		  α = max(α,value);
		  if(α >= β) {
		    break;
		  }
		  children ^= child;
		}
		return value;
	}

	void AImove(depth) {

	}

	void play(u64 move) {
		current_position ^= move;
		mask |= move;
		moves++;
	}

	u64 key() const {
		return current_position + mask;
	}

	u64 SymmetricalKey() {
		u64 r = 0;
		u64 r1 |= RootReflect(r);
		for(int i = 0; i < 7; i++) {
			r = ;
		}
		return r;
	}

private:
	u64 mask;
	u64 current_position;
	u32 moves;
	
}

}// namespace TicTacToe

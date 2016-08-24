#define BIT(x) (1 << (x))
#define SETBITS(x,y) ((x) |= (y))
#define CLRBITS(x,y) ((x) &= (~(y)))
#define SETBIT(x,y) SETBITS((x), (BIT((y))))
#define CLRBIT(x,y) CLRBITS((x), (BIT((y))))

#define W 1
#define R 0

#include <time.h>
#include <inttypes.h>
#include "natural.c"

// checkout gmp

typedef struct test_result {
	bool failed;
	wide expected, got;
	char *opname;
} test_result;

typedef test_result (*testfp)(wide,wide,natural *, natural *);


// rand guaranteed 16 bits
wide rand_wide() {
	return (((wide) rand() ));
}

bool test_spam(testfp test, int n) {
	for (int i=0; i<n; i++) {
		wide x = rand_wide();
		wide y = rand_wide();
		natural *n = natural_from_wide(x);
		natural *m = natural_from_wide(y);

		test_result result = test(x,y,n,m);

		const char *fmt = 
			"\n---------------\n"
			"%s \t%"PRId64" %"PRId64"\n";

			"\t\t= %"PRId64"\n"
			"but:\n";

		if (result.failed) {
			printf(fmt, result.opname, x, y, result.expected);
			printf("\t");
			natural_print(n);
			printf("\n");
			printf("\t");
			natural_print(m);
			printf("\n");
			printf("\t");
			printf("%"PRId64, result.got);
			printf("\n");
		}
	}
}

test_result test_add(wide x, wide y, natural *n, natural *m) {
	wide answer = natural_to_wide(natural_add(n, m));
	test_result result = {
		.failed = !(answer == x+y),	
		.expected = x+y,	
		.got = answer,
		.opname = "add",	
	};

	return result;
}

void test_add_sc() {
	for (int i = 0; i < 5; i++) {
		wide x=rand(), y=rand();
		natural *n=natural_from_wide(x);
		natural *m=natural_from_wide(y);
		
		if ( (x+y) != natural_to_wide( natural_add(n,m))) {
			char *fmt = "-------------\n"
						"add\n"
						"\t  %"PRId64"\n"
						"\t  %"PRId64"\n"
						"\t= %"PRId64"\n";

			printf(fmt, x, y, x+y);
			natural_print(n); printf("\n"); 
			natural_print(m); printf("\n");
			natural_print( natural_add(n,m) ); printf("\n");
			printf("FAIL\n");
		} else {
			printf("Add test passed.\n");
		}
	}
}

void test_subtract_sc() {
	for (int i = 0; i < 5; i++) {
		wide x=rand(), y=rand();
		if (x < y) {
			wide t = y;
			y = x;
			x = t;
		}
		
		natural *n=natural_from_wide(x);
		natural *m=natural_from_wide(y);
		
		if ( (x-y) != natural_to_wide( natural_subtract(n,m))) {
			char *fmt = "-------------\n"
						"subtract\n"
						"\t  %"PRId64"\n"
						"\t  %"PRId64"\n"
						"\t= %"PRId64"\n";

			printf(fmt, x, y, x-y);
			natural_print(n); printf("\n"); 
			natural_print(m); printf("\n");
			natural_print( natural_subtract(n,m) ); printf("\n");
			printf("FAIL\n");
		} else {
			printf("Subtraction test passed.\n");
		}
	}
}

void test_multiply_sc() {
	for (int i = 0; i < 5; i++) {
		wide x= (rand()>>8), y= (rand()>>12); //keep 'em small
		
		natural *n=natural_from_wide(x);
		natural *m=natural_from_wide(y);

		if ( (x*y) != natural_to_wide( natural_multiply(n,m))) {
			char *fmt = "-------------\n"
						"multiply\n"
						"\t  %"PRId64"\n"
						"\t  %"PRId64"\n"
						"\t= %"PRId64"\n";

			printf(fmt, x, y, x*y);
			natural_print(n); printf("\n"); 
			natural_print(m); printf("\n");
			natural_print( natural_multiply(n,m) ); printf("\n");
			printf("FAIL\n");
		} else {
			printf("Multiplication test passed.\n");
		}
	}
}

test_result test_subtract(wide x, wide y, natural *n, natural *m) {
	// assume x > y
	if (x < y) {
		x, y = y, x;
		n, m = m, n;
	}
	wide answer = natural_to_wide(natural_subtract(n, m));
	test_result result = {
		.failed = !(answer == x-y),	
		.expected = x-y,	
		.got = answer,
		.opname = "subtract",	
	};

	return result;
}

bool test_compare(wide x, wide y, natural *n, natural *m) {
	int answer = natural_compare(n, m);
	switch (answer) {
	case GRTR:
		return (x > y);
		break;
	case LESS:
		return (x < y);
		break;
	case EQUAL:
		return (x == y);
		break;
	}
}

int main(int argc, char *argv[]) {
	srand((unsigned int)time(NULL));

	int ntest = 20;
	test_add_sc();
	test_subtract_sc();
	test_multiply_sc();
}

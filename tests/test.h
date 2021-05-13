#ifndef UTF8_DECODER_TEST_H
#define UTF8_DECODER_TEST_H

#define BEGIN_TESTS int number_of_tests = 0; int passed_tests = 0; int t;
#define END_TESTS printf("Passed %i/%i (failed %i)\n", passed_tests, number_of_tests, number_of_tests - passed_tests);

#define REGISTER_TEST number_of_tests++
#define RUN(e, r) t = (e == r); passed_tests += t
#define PRINT(s) printf("test %s: %i\n", s, t);

#define TEST(expected, real, message) REGISTER_TEST; RUN(expected, real); PRINT(message);

#endif //UTF8_DECODER_TEST_H

#ifndef UTF8_DECODER_TEST_H
#define UTF8_DECODER_TEST_H

#define REGISTER_TEST number_of_tests++
#define RUN(e, r) t = (e == r); passed_tests += t
#define PRINT(s) printf("test %s: %i\n", s, t);

#define PRINT_END printf("Passed %i/%i (failed %i)\n", passed_tests, number_of_tests, number_of_tests - passed_tests)
#define RETURN_END return (number_of_tests - passed_tests > 0)

#define TEST(expected, real, message) REGISTER_TEST; RUN(expected, real); PRINT(message)

#define BEGIN_TESTS int number_of_tests = 0; int passed_tests = 0; int t;
#define END_TESTS PRINT_END; RETURN_END;

#endif //UTF8_DECODER_TEST_H

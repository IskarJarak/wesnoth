/* $Id$ */
/*
   Copyright (C) 2008 by Pauli Nieminen <paniemin@cc.hut.fi>
   Part of the Battle for Wesnoth Project http://www.wesnoth.org/

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License version 2
   or at your option any later version.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#ifndef TESTS_UTILS_AUTO_PARAMETERIZED_HPP_INCLUDED
#define TESTS_UTILS_AUTO_PARAMETERIZED_HPP_INCLUDED

#include <boost/test/unit_test_suite.hpp>
#include <boost/test/parameterized_test.hpp>

namespace test_utils {

#define WESNOTH_PARAMETERIZED_TEST_CASE( test_name, type_name, values, param_name )   \
struct test_name : public BOOST_AUTO_TEST_CASE_FIXTURE                  \
{ void test_method(const type_name&); };                                \
\
type_name* BOOST_JOIN(test_name, _begin) = &values[0];    \
type_name* BOOST_JOIN(test_name, _end) = BOOST_JOIN(test_name, _begin) + (sizeof(values)/sizeof(values[0])); \
static void BOOST_AUTO_TC_INVOKER( test_name )(const type_name& param_name ) \
{ \
	test_name t;                                                        \
    t.test_method(param_name);       \
}                                                                       \
                                                                        \
struct BOOST_AUTO_TC_UNIQUE_ID( test_name ) {};                         \
                                                                        \
BOOST_AUTO_TU_REGISTRAR( test_name )( \
		BOOST_PARAM_TEST_CASE(&BOOST_AUTO_TC_INVOKER( test_name ),      \
			BOOST_JOIN(test_name, _begin), BOOST_JOIN(test_name, _end)));		\
                                                                       \
void test_name::test_method(const type_name& param_name)                \
/**/

}
#endif

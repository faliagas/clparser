## Makefile.am - automake script.
## Copyright (C) 2017-23 apostol <apostol.faliagas@gmail.com>
## Rev. Tue 27 Jun 2023 12:14:58 PM EEST

AM_CXXFLAGS = -std=c++11
AM_CPPFLAGS = -I$(srcdir)/include
ACLOCAL_AMFLAGS = -I m4
ARGPCPPFLAGS = -I$(srcdir)/include -DARGP_INTERFACE

lib_LTLIBRARIES = libclparser.la libclparser-argp.la

libclparser_argp_la_SOURCES = src/clparser_argp.cpp \
  include/clparser.hpp
libclparser_argp_la_CPPFLAGS = $(ARGPCPPFLAGS)
libclparser_argp_la_LDFLAGS = -version-info 1:0:1

libclparser_la_SOURCES = src/clparser.cpp \
  include/clparser.hpp
libclparser_la_LDFLAGS = -version-info 1:0:1

# Compilation of examples
if COMPILE_EXAMPLES
  bin_PROGRAMS = example-11 example-12 example-13 example-14 example-15 example-16 \
    example-21 example-22 example-22a example-22b example-22c \
    example-31 example-32 example-33 \
    example-41 example-41a example-42 \
    example-gnu-1 example-gnu-2 example-gnu-3 example-gnu-3a example-gnu-3b example-gnu-4 \
    example-62 example-62a example-64 example-65 example-65a \
    example-71 example-72 example-72a example-72b \
    example-81 example-82 \
    example-91 example-93

  example_11_SOURCES = examples/example-11.cpp
  example_11_LDADD = libclparser.la
  example_12_SOURCES = examples/example-12.cpp
  example_12_LDADD = libclparser.la
  example_13_SOURCES = examples/example-13.cpp
  example_13_LDADD = libclparser.la
  example_14_SOURCES = examples/example-14.cpp
  example_14_LDADD = libclparser.la
  example_15_SOURCES = examples/example-15.cpp
  example_15_LDADD = libclparser.la
  example_16_SOURCES = examples/example-16.cpp
  example_16_LDADD = libclparser.la
  example_21_SOURCES = examples/example-21.cpp
  example_21_LDADD = libclparser.la
  example_22_SOURCES = examples/example-22.cpp
  example_22_LDADD = libclparser.la
  example_22a_SOURCES = examples/example-22a.cpp
  example_22a_LDADD = libclparser.la
  example_22b_SOURCES = examples/example-22b.cpp
  example_22b_LDADD = libclparser.la
  example_22c_SOURCES = examples/example-22c.cpp
  example_22c_LDADD = libclparser.la
  example_31_SOURCES = examples/example-31.cpp
  example_31_LDADD = libclparser.la
  example_32_SOURCES = examples/example-32.cpp
  example_32_LDADD = libclparser.la
  example_33_SOURCES = examples/example-33.cpp
  example_33_LDADD = libclparser.la
  example_41_SOURCES = examples/example-41.cpp
  example_41_LDADD = libclparser.la
  example_41a_SOURCES = examples/example-41a.cpp
  example_41a_LDADD = libclparser.la
  example_42_SOURCES = examples/example-42.cpp
  example_42_LDADD = libclparser.la

  example_62_SOURCES = examples/example-62.cpp
  example_62_LDADD = libclparser-argp.la
  example_62_CPPFLAGS = $(ARGPCPPFLAGS)
  example_62a_SOURCES = examples/example-62a.cpp
  example_62a_LDADD = libclparser-argp.la
  example_62a_CPPFLAGS = $(ARGPCPPFLAGS)
  example_64_SOURCES = examples/example-64.cpp
  example_64_LDADD = libclparser-argp.la
  example_64_CPPFLAGS = $(ARGPCPPFLAGS)
  example_65_SOURCES = examples/example-65.cpp
  example_65_LDADD = libclparser-argp.la
  example_65_CPPFLAGS = $(ARGPCPPFLAGS)
  example_65a_SOURCES = examples/example-65a.cpp
  example_65a_LDADD = libclparser-argp.la
  example_65a_CPPFLAGS = $(ARGPCPPFLAGS)
  example_71_SOURCES = examples/example-71.cpp
  example_71_LDADD = libclparser-argp.la
  example_71_CPPFLAGS = $(ARGPCPPFLAGS)
  example_72_SOURCES = examples/example-72.cpp
  example_72_LDADD = libclparser-argp.la
  example_72_CPPFLAGS = $(ARGPCPPFLAGS)
  example_72a_SOURCES = examples/example-72a.cpp
  example_72a_LDADD = libclparser-argp.la
  example_72a_CPPFLAGS = $(ARGPCPPFLAGS)
  example_72b_SOURCES = examples/example-72b.cpp
  example_72b_LDADD = libclparser-argp.la
  example_72b_CPPFLAGS = $(ARGPCPPFLAGS)
  example_81_SOURCES = examples/example-81.cpp
  example_81_LDADD = libclparser-argp.la
  example_81_CPPFLAGS = $(ARGPCPPFLAGS)
  example_82_SOURCES = examples/example-82.cpp
  example_82_LDADD = libclparser-argp.la
  example_82_CPPFLAGS = $(ARGPCPPFLAGS)
  example_91_SOURCES = examples/example-91.cpp
  example_91_LDADD = libclparser-argp.la
  example_91_CPPFLAGS = $(ARGPCPPFLAGS)
  example_93_SOURCES = examples/example-93.cpp
  example_93_LDADD = libclparser-argp.la
  example_93_CPPFLAGS = $(ARGPCPPFLAGS)

  example_gnu_1_SOURCES = examples/example-gnu-1.cpp
  example_gnu_1_LDADD = libclparser-argp.la
  example_gnu_1_CPPFLAGS = $(ARGPCPPFLAGS)
  example_gnu_2_SOURCES = examples/example-gnu-2.cpp
  example_gnu_2_LDADD = libclparser-argp.la
  example_gnu_2_CPPFLAGS = $(ARGPCPPFLAGS)
  example_gnu_3_SOURCES = examples/example-gnu-3.cpp
  example_gnu_3_LDADD = libclparser-argp.la
  example_gnu_3_CPPFLAGS = $(ARGPCPPFLAGS)
  example_gnu_4_SOURCES = examples/example-gnu-4.cpp
  example_gnu_4_LDADD = libclparser-argp.la
  example_gnu_4_CPPFLAGS = $(ARGPCPPFLAGS)
  example_gnu_3a_SOURCES = examples/example-gnu-3a.cpp
  example_gnu_3a_LDADD = libclparser-argp.la
  example_gnu_3a_CPPFLAGS = $(ARGPCPPFLAGS)
  example_gnu_3b_SOURCES = examples/example-gnu-3b.cpp
  example_gnu_3b_LDADD = libclparser-argp.la
  example_gnu_3b_CPPFLAGS = $(ARGPCPPFLAGS)
else
  bin_PROGRAMS =
endif

# Scripts
bin_SCRIPTS = compile-clparser-examples
CLEANFILES = $(bin_SCRIPTS)

# Distribute headers
include_HEADERS = include/clparser.hpp

# Distribute documentation
dist_doc_DATA = README README.md COPYING AUTHORS ChangeLog INSTALL NEWS \
doc/tutorial.md doc/tutorial-1.md doc/tutorial-2.md doc/tutorial-3.md \
doc/tutorial-4.md doc/tutorial-5.md doc/tutorial-6.md \
doc/tutorial-7.md doc/tutorial-8.md doc/tutorial-9.md \
doc/tutorial.html doc/tutorial-1.html doc/tutorial-2.html doc/tutorial-3.html \
doc/tutorial-4.html doc/tutorial-5.html doc/tutorial-6.html \
doc/tutorial-7.html doc/tutorial-8.html doc/tutorial-9.html

# Remove doc directory on uninstall
uninstall-local:
	-rm -r $(docdir)

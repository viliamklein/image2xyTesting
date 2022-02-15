# This file is part of the Astrometry.net suite.
# Licensed under a 3-clause BSD style license - see LICENSE

BASEDIR := /home/viliam/sources/astrometry.net-0.85
COMMON := $(BASEDIR)/util

CATALOGS := $(BASEDIR)/catalogs
CATS_INC := -I$(CATALOGS)
CATS_SLIB := $(CATALOGS)/libcatalogs.a
# CATS_LIB :=

.PHONY: all
all:

# Detect GSL -- this minimum version was chosen to match the version in gsl-an.
# Earlier versions would probably work fine.
SYSTEM_GSL ?= $(shell (pkg-config --atleast-version=1.14 gsl && echo "yes") || echo "no")
export SYSTEM_GSL

include makefile.common
include makefile.anfiles
include makefile.cfitsio

# ifneq ($(MAKECMDGOALS),clean)
#     include $(COMMON)/makefile.os-features
# endif

# $(COMMON)/makefile.os-features:
# 	$(MAKE) -C $(COMMON) makefile.os-features

SO=$(SHAREDLIB_SUFFIX)

LIBDIR = lib
ENGINE_LIB := $(LIBDIR)/libastrometry.a
ENGINE_SO := $(LIBDIR)/libastrometry.$(SO)

LDFLAGS := $(LDFLAGS_DEF)

LDLIBS := $(LDLIBS_DEF)
LDLIBS += $(ANFILES_LIB)

SLIB := $(ENGINE_LIB)
SLIB += $(CATS_SLIB)
SLIB += $(ANFILES_SLIB)

CFLAGS += $(CFLAGS_DEF)
CFLAGS += $(CATS_INC)
CFLAGS += $(ANFILES_CFLAGS)
CFLAGS += $(ANFILES_INC)
CFLAGS += -I$(ANUTILS_DIR)
CFLAGS += -I$(LIBDIR)
CFLAGS += $(CFITS_INC)

SRC := src
BIN := build
CXXFLAGS := -ffinite-math-only -fno-signaling-nans -pthread -march=native

SHAREDLIBFLAGS := $(SHAREDLIBFLAGS_DEF)

ALL_WCSLIB_TARGETS :=

MAIN_PROGS := image2xy 

PROGS := $(MAIN_PROGS)

ANLIBS := $(ANFILES_LIB) $(LIBKD_LIB) $(ANUTILS_LIB) $(GSL_LIB) $(QFITS_LIB)


# INSTALL_EXECS := $(PROGS)

# INSTALL_H := allquads.h augment-xylist.h axyfile.h \
# 	engine.h onefield.h solverutils.h build-index.h catalog.h \
# 	codefile.h codetree.h fits-guess-scale.h hpquads.h \
# 	image2xy-files.h merge-index.h \
# 	new-wcs.h quad-builder.h quad-utils.h resort-xylist.h \
# 	solvedfile.h solver.h tweak.h uniformize-catalog.h \
# 	unpermute-quads.h unpermute-stars.h verify.h \
# 	tweak2.h

# ALL_EXECS :=

# $(info ************  TEST VERSION ************)
# $(info $$var is [${PROGS}])

# all: $(QFITS_SLIB) $(LIBKD_LIB_FILE) \
# 	$(ANUTILS_LIB_FILE) $(ANFILES_LIB_FILE) \
# 	$(PROGS) 

EXECUTABLE := image2xy

all: $(BIN)/$(EXECUTABLE)

run: clean all
	clear
	./$(BIN)/$(EXECUTABLE)

# $(MAIN_PROGS): %: $(SRC)/%-main.o $(SLIB)

# $(info ************  TEST VERSION ************)
# $(info $$var is [${LDFLAGS}])

# .ONESHELL:

# sayhi:
#     @echo Hi
#     exit
#     @echo Bye

# image2xy: image2xy-main.o image2xy-files.o $(CC) -o $@  $(LDFLAGS) $^ $(CFITS_LIB) $(LDLIBS)
$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp 
	$(CC) $(CXXFLAGS) $(LDFLAGS) $(CFLAGS) $^ -o $@ \
	-L$(LIBDIR) -L$(BASEDIR)/util -L$(BASEDIR)/qfits-an \
	 -lanutils -lanbase -lanfiles -lqfits -lgsl -lgslcblas $(CFITS_LIB) $(LDLIBS) 

.PHONY: clean

clean:
	rm -f $(EXECS) $(EXTRA_EXECS) $(SOLVER_EXECS) $(MISC_EXECS) $(PROGS) \
		$(PIPELINE) $(PROSPECTUS) $(DEPS) $(FITS_UTILS) $(ALL_OBJ) \
		$(NODEP_OBJS) fitsverify \
		$(ALL_EXECS) $(GENERATED_FILES) $(ALL_TESTS_CLEAN) $(BIN)/* \
		 *.o *~ *.dep deps



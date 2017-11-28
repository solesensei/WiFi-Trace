# This makefile is bridge between
# a library stuff and stuff that the library exports to world
# and, particularly, exports into our project.
# This makefile manages header and library files we use.
# Every file in these folders updates with owner Makefile.

INCLUDE_DIR = include
LIBRARY_DIR = lib
EXTERNALS_BASEDIR = ../externals

TARGET = glm easybmp argvparser assimp
.DEFAULT_GOAL := $(TARGET)

# Avoid useless remaking of this Makefile.
Makefile : ;

# GLM library
# Path to GLM library
GLM_LOCATION = $(EXTERNALS_BASEDIR)/glm
# Makefile name of GLM
GLM_MAKEFILE = Makefile

.PHONY: glm
# Copy prepared for import files from GLM directory
glm:
	cp -r $(GLM_LOCATION) $(INCLUDE_DIR)

# Assimp library
# Path to ASSIMP library
ASSIMP_LOCATION = $(EXTERNALS_BASEDIR)/assimp
# Makefile name of ASSIMP
ASSIMP_MAKEFILE = Makefile # read it to mre information

.PHONY: assimp
# Copy prepared for import files from ASSIMP directory
assimp:
	cp -r $(ASSIMP_LOCATION)/include/* $(INCLUDE_DIR)
	cp -r $(ASSIMP_LOCATION)/lib/* $(LIBRARY_DIR)

# Argvparser library
# Path to Argvparser library
ARGVPARSER_LOCATION = $(EXTERNALS_BASEDIR)/argvparser
# Makefile name of Argvparser
ARGVPARSER_MAKEFILE = Makefile

# Path to files in the Argvparser directory to import in the main project
# Headers
ARGVPARSER_HEADERS = include/argvparser.h
# Statically compiled library (.a means archive)
ARGVPARSER_LIBRARY = lib/libargvparser.a

.PHONY: argvparser
# Copy prepared for import files from Argvparser directory
argvparser: $(ARGVPARSER_HEADERS) $(ARGVPARSER_LIBRARY)
	cp $(addprefix $(ARGVPARSER_LOCATION)/, $(ARGVPARSER_HEADERS)) $(INCLUDE_DIR)/
	cp $(addprefix $(ARGVPARSER_LOCATION)/, $(ARGVPARSER_LIBRARY)) $(LIBRARY_DIR)/

.PHONY: $(ARGVPARSER_HEADERS)
# Prepare library headers by calling to Argvparser makefile
$(ARGVPARSER_HEADERS):
	make -C $(ARGVPARSER_LOCATION) -f $(ARGVPARSER_MAKEFILE) $@

.PHONY: $(ARGVPARSER_LIBRARY)
# Prepare library by calling to Argvparser makefile
$(ARGVPARSER_LIBRARY):
	make -C $(ARGVPARSER_LOCATION) -f $(ARGVPARSER_MAKEFILE) $@

# EasyBMP library
# Path to EasyBMP library
EASYBMP_LOCATION = $(EXTERNALS_BASEDIR)/EasyBMP
# Makefile name of EasyBMP
EASYBMP_MAKEFILE = Makefile

# Path to files in the EasyBMP directory to import in the main project
# Headers
EASYBMP_HEADERS = include/EasyBMP_BMP.h \
		  include/EasyBMP_DataStructures.h \
		  include/EasyBMP.h \
		  include/EasyBMP_VariousBMPutilities.h
# Statically compiled library (.a means archive)
EASYBMP_LIBRARY = lib/libeasybmp.a

.PHONY: easybmp
# Copy prepared for import files from EasyBMP directory
easybmp: $(EASYBMP_HEADERS) $(EASYBMP_LIBRARY)
	cp $(addprefix $(EASYBMP_LOCATION)/, $(EASYBMP_HEADERS)) $(INCLUDE_DIR)/
	cp $(addprefix $(EASYBMP_LOCATION)/, $(EASYBMP_LIBRARY)) $(LIBRARY_DIR)/

.PHONY: $(EASYBMP_HEADERS)
# Prepare library headers by calling to EasyBMP makefile
$(EASYBMP_HEADERS):
	make -C $(EASYBMP_LOCATION) -f $(EASYBMP_MAKEFILE) $@

.PHONY: $(EASYBMP_LIBRARY)
# Prepare library by calling to EasyBMP makefile
$(EASYBMP_LIBRARY):
	make -C $(EASYBMP_LOCATION) -f $(EASYBMP_MAKEFILE) $@

.PHONY: clean
clean:
	rm -rf $(wildcard $(INCLUDE_DIR)/*) $(wildcard $(LIBRARY_DIR)/*)
	make -C $(EASYBMP_LOCATION) -f $(EASYBMP_MAKEFILE) $@
	make -C $(ARGVPARSER_LOCATION) -f $(ARGVPARSER_MAKEFILE) $@

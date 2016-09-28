# ============================================================================
# Local Makefile
# ============================================================================

default: installd

debug-all : debug debug-clean debug-install

release-all : release release-clean release-install

docs-all : docs docs-clean docs-install

.PHONY: debug-all release-all docs-all tags help installd

# ============================================================================
# Local Variables
# ============================================================================
MAJOR_VERSION = 1
MINOR_VERSION = 1
BUILD_VERSION = 50
VERSION_NUMBER = $(MAJOR_VERSION).$(MINOR_VERSION)
DOXYGEN_VERSION = $(VERSION_NUMBER).$(BUILD_VERSION)

PREFIX	   = lib
SUFFIX	   = sstl
TARGET	   = $(PREFIX)$(SUFFIX)
HTDOCS     = /c/xampp/htdocs/docs
HTDOCS_DIR = $(HTDOCS)/$(TARGET)-$(VERSION_NUMBER)
DIST_DIR   = $(WORKHOME)/libs/$(TARGET)-$(VERSION_NUMBER)
DIST_DBG   = $(DIST_DIR)/x86/dbg
DIST_REL   = $(DIST_DIR)/x86/rel
TAGS_DIR   = $(DIST_DIR)/tags

HELP_DIR   = docs/help
HTML_DIR   = $(HELP_DIR)/html

CP = cp -f
RSYNC = rsync -cvruptOm --no-o --no-g --delete --delete-excluded --exclude='.*.sw?'

# ============================================================================
# Targets
# ============================================================================

debug :
	@qmake --no-print-directory -C bin -f Makefile debug

release :
	@qmake --no-print-directory -C bin -f Makefile release

debug-install :
	@qmake --no-print-directory -C bin -f Makefile debug-install

release-install :
	@qmake --no-print-directory -C bin -f Makefile release-install

debug-clean :
	@qmake --no-print-directory -C bin -f Makefile debug-clean

release-clean :
	@qmake --no-print-directory -C bin -f Makefile release-clean

clean : debug-clean release-clean

$(DIST_DIR) :
	@mkdir -p $@

$(DIST_DBG) : $(DIST_DIR)
	@mkdir -p $@

$(DIST_REL) : $(DIST_DIR)
	@mkdir -p $@

installd : $(DIST_DBG)
	$(RSYNC) source/$(TARGET).h $(DIST_DIR)/
	$(RSYNC) source/$(SUFFIX)*.hpp $(DIST_DIR)/

installr : $(DIST_REL)
	$(RSYNC) source/$(TARGET).h $(DIST_DIR)/
	$(RSYNC) source/$(SUFFIX)*.hpp $(DIST_DIR)/

install : installd installr

$(HTML_DIR) :
	@mkdir -p $(HTML_DIR)

docs : $(HTML_DIR)
	( cat doxyfile; \
		echo "PROJECT_NUMBER=$(DOXYGEN_VERSION)"; \
		echo "OUTPUT_DIRECTORY=$(HELP_DIR)"; \
	    echo "GENERATE_TAGFILE=$(HELP_DIR)/$(TARGET).dxt" \
	) | doxygen -

docs-clean :
	@rm $(HTML_DIR)/*

docs-install : $(TAGS_DIR)
	$(RSYNC) $(HTML_DIR)/ $(HTDOCS_DIR)/
	$(RSYNC) $(HELP_DIR)/$(TARGET).dxt $(TAGS_DIR)/

$(TAGS_DIR) :
	@mkdir -p $(TAGS_DIR)

tags : $(TAGS_DIR)
	@pmake ctags -t $(TARGET)-$(VERSION_NUMBER)/tags/$(TARGET).tags

help :
	@echo -e "Makefile targets:\n"\
	      "debug             Builds debug target\n"\
	      "debug-clean       Clean up the debug target files\n"\
	      "debug-install     Installs, building if needed, the debug target\n"\
	      "release           Build the release target\n"\
	      "release-clean     Clean up the debug target files\n"\
	      "release-install   Installs, building if needed, the release target\n"\
	      "clean             Clean up all built targets\n"\
		  "install           Debug and Release install\n"\
		  "docs              Build the documentation through Doxygen\n"\
		  "docs-clean        Clean up all documentation\n"\
		  "docs-install      Copy documentation in the thumb drive\n"\
		  "tags              Build a tags file in the dist directory"
		


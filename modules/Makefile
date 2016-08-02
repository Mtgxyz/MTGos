include ../kernel.settings
SUBDIRS = $(arch)
all: 
	for dir in $(SUBDIRS); do \
	  $(MAKE) -C $$dir; \
	done
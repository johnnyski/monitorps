# Process w/ automake.  Or, autoreconf; make #
AUTOMAKE_OPTIONS = foreign

INCLUDES = -I$(prefix)/include -I/usr/local/include

bin_PROGRAMS = monitorps \
               delete_job_from_jobdb \
               update_job_status_gdbm list_job_status_db 

bin_SCRIPTS = send_status send_product

noinst_PROGRAMS = gui tellmonitor 

monitorps_SOURCES = monitorps.c monitorps.h \
                    ps.c comm.c comm.h  protocol.y protocol-lex.l ports.h

tellmonitor_SOURCES = tellmonitor.c ps.c comm.c comm.h
gui_SOURCES = gui.c comm.c comm.h ports.h

YFLAGS = -d

# Normally you don't need to define LIBS.  But, using _LDFLAGS doesn't work.
LIBS= @LIBS@ $(LEXLIB)

send_status: Makefile
	@for p in $(bin_SCRIPTS); do \
	   echo -n "Checking file $$p ... ";\
	   cp $$p $$p.in;\
	   sed -e 's&PROG_VERSION[ ]*=.*&PROG_VERSION = \"$(PACKAGE)-$(VERSION)\";&' \
		-e 's&#\!.*perl&#\! $(perlbin)&' \
		-e 's&(\@INC,.*&(\@INC, \"$(bindir)\"\)\;&' \
	   < $$p.in > $$p.new;\
	   if cmp -s $$p $$p.new; then \
          rm $$p.new; \
          echo "$$p remains untouched."; \
       else \
          mv $$p.new $$p; \
          echo "substitutions made in $$p."; \
       fi;\
	   rm -f $$p.in;\
    done


DISTCLEANFILES = protocol.c protocol.h protocol-lex.c 

protocol-lex.c: protocol.c

EXTRA_DIST = protocol.h $(bin_SCRIPTS)
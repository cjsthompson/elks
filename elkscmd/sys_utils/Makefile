BASEDIR=..

include $(BASEDIR)/Make.defs

###############################################################################
#
# Include standard packaging commands.

include $(BASEDIR)/Make.rules

###############################################################################

# clock enabled and has direct I/O port access
# knl, insmod removed as useless
# exitemu disabled because it calls INT directly to DOSEMU
PRGS = \
	init \
	getty \
	login \
	kill \
	mount \
	umount \
	passwd \
	reboot \
	shutdown \
	ps \
	meminfo \
	who \
	man \
	poweroff \
	chmem \
	clock \
	unreal16 \
	mouse \
	sercat \
	console \
	# EOL

init: init.o
	$(LD) $(LDFLAGS) -o init init.o $(LDLIBS)

getty: getty.o
	$(LD) $(LDFLAGS) -o getty getty.o $(LDLIBS)

login: getpass.o login.o
	$(LD) $(LDFLAGS) -o login getpass.o login.o $(LDLIBS)

kill: kill.o
	$(LD) $(LDFLAGS) -o kill kill.o $(LDLIBS)

mount: mount.o
	$(LD) $(LDFLAGS) -o mount mount.o $(LDLIBS)

umount: umount.o
	$(LD) $(LDFLAGS) -o umount umount.o $(LDLIBS)

passwd: getpass.o passwd.o
	$(LD) $(LDFLAGS) -o passwd getpass.o passwd.o $(LDLIBS)

reboot: reboot.o
	$(LD) $(LDFLAGS) -o reboot reboot.o $(LDLIBS)

shutdown: shutdown.o
	$(LD) $(LDFLAGS) -o shutdown shutdown.o $(LDLIBS)

ps: ps.o
	$(LD) $(LDFLAGS) -o ps ps.o $(LDLIBS)

meminfo: meminfo.o
	$(LD) $(LDFLAGS) -o meminfo meminfo.o $(LDLIBS)

who: who.o
	$(LD) $(LDFLAGS) -o who who.o $(LDLIBS)

clock: clock.o
	$(LD) $(LDFLAGS) -o clock clock.o $(LDLIBS)

exitemu: exitemu.o
	$(LD) $(LDFLAGS) -o exitemu exitemu.o $(LDLIBS)

knl: knl.o
	$(LD) $(LDFLAGS) -o knl knl.o $(LDLIBS)

man: man.o
	$(LD) $(LDFLAGS) -o man man.o $(LDLIBS)

poweroff: poweroff.o
	$(LD) $(LDFLAGS) -o poweroff poweroff.o $(LDLIBS)

chmem: chmem.o
	$(LD) $(LDFLAGS) -o chmem chmem.o $(LDLIBS)

mouse: mouse.o
	$(LD) $(LDFLAGS) -o mouse mouse.o $(LDLIBS)

sercat: sercat.o
	$(LD) $(LDFLAGS) -o sercat sercat.o $(LDLIBS)

console: console.o
	$(LD) $(LDFLAGS) -o console console.o $(LDLIBS)

unreal16: unreal16.o
	$(LD) -melks-libc -mcmodel=small -c unreal16.S -o unreal16.o
	$(LD) -melks-libc -mcmodel=small -nostdlib -o unreal16 unreal16.o

all: $(PRGS)

install: $(PRGS)
	$(INSTALL) $(PRGS) $(DESTDIR)/bin

clean:
	rm -f *.o core $(PRGS)

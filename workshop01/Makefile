CC = gcc
CFLAGS = -Wall
LIBDIR = lib
LIBSRC = $(LIBDIR)/aes.c
LIBOBJ = $(LIBDIR)/aes.o

# Artefacts
ARTEFACTS = artefact01 artefact02 artefact03 artefact04 artefact05

all: $(ARTEFACTS)

# Rules for optimisation levels
artefact01: artefact01.c
	$(CC) -O0 -o $@ $^ $(CFLAGS)

artefact02: artefact02.c
	$(CC) -O0 -o $@ $^ $(CFLAGS)

artefact03: artefact03.c $(LIBOBJ)
	$(CC) -O2 -o $@ $^ $(CFLAGS)

artefact04: artefact04.c
	$(CC) -O0 -o $@ $^ $(CFLAGS)

artefact05: artefact05.c
	$(CC) -O0 -o $@ $^ $(CFLAGS)
	upx $@

# Compile aes library
$(LIBOBJ): $(LIBSRC)
	$(CC) -c -O2 -o $@ $< $(CFLAGS)

clean:
	rm -f $(ARTEFACTS) $(LIBOBJ)


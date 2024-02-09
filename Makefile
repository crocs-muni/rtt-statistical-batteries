all: dieharder nist testu01

dieharder:
	make -C dieharder-src
	ln -s dieharder-src/install/bin/dieharder dieharder

nist:
	mkdir -p nist-sts-src/obj
	make -C nist-sts-src
	cd nist-sts-src/experiments && bash create-dir-script
	ln -s nist-sts-src/assess nist-sts

testu01:
	cd testu01-src && make
	ln -s testu01-src/install/bin/TestU01 testu01

static:
	make -C dieharder-src static
	cp dieharder-src/install/bin/dieharder .
	make -C nist-sts-src LDFLAGS="-static"
	cp nist-sts-src/assess nist-sts
	cd testu01-src && make
	cp testu01-src/install/bin/TestU01 testu01

clean:
	make -C dieharder-src clean || true
	make -C nist-sts-src clean || true
	make -C testu01-src clean || true
	rm -f dieharder nist-sts testu01

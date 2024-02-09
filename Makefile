all:
	./INSTALL

static:
	make -C dieharder-src static
	cp dieharder-src/install/bin/dieharder .
	make -C nist-sts-src LDFLAGS="-static"
	cp nist-sts-src/assess nist-sts
	cd testu01-src && make
	cp testu01-src/install/bin/TestU01 testu01

clean:
	make -C dieharder-src clean
	make -C nist-sts-src clean
	make -C testu01-src clean
	rm -f dieharder nist-sts testu01

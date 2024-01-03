all:
	./INSTALL

clean:
	make -C dieharder-src clean
	make -C nist-sts-src clean
	make -C testu01-src clean
	rm -f dieharder nist-sts testu01

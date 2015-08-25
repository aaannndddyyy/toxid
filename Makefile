APP=toxid
VERSION=1.00
RELEASE=1
ARCH_TYPE=`uname -m`
PREFIX?=/usr/local

all:
	gcc -Wall -std=c99 -pedantic -O3 -o ${APP} -Isrc src/*.c -ltoxcore

debug:
	gcc -Wall -std=c99 -pedantic -g -o ${APP} -Isrc src/*.c -ltoxcore

install:
	mkdir -p ${DESTDIR}/usr
	mkdir -p ${DESTDIR}${PREFIX}/bin
	install -m 755 -s src/lstox ${DESTDIR}${PREFIX}/bin
	install -m 755 -s src/toxavahi ${DESTDIR}${PREFIX}/bin
	install -m 755 -s ${APP} ${DESTDIR}${PREFIX}/bin
	mkdir -m 755 -p ${DESTDIR}${PREFIX}/share/man/man1
	install -m 644 man/${APP}.1.gz ${DESTDIR}${PREFIX}/share/man/man1

uninstall:
	rm -f ${DESTDIR}${PREFIX}/share/man/man1/${APP}.1.gz
	rm -f ${DESTDIR}${PREFIX}/bin/${APP}
	rm -f ${DESTDIR}${PREFIX}/bin/lstox
	rm -f ${DESTDIR}${PREFIX}/bin/toxavahi

clean:
	rm -f toxid





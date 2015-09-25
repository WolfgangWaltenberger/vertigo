all: man

man:
	docbook-to-man vertigo.sgml > vertigo.1

edit:
	vim vertigo.sgml

html:
	docbook2html vertigo.sgml 

ps:
	docbook2ps vertigo.sgml

pdf:
	docbook2pdf vertigo.sgml

show:
	man -l vertigo.1

clean:
	rm -f *.html vertigo.pdf vertigo.ps vertigo.out vertigo.log vertigo.texi vertigo.aux db2texi.refs

upload:
	cp snapshot.png *.html ~/afs/www/vertigo/
#	scp snapshot.png *.html stop.itp.tuwien.ac.at:/var/www/stop/docs/vertigo/

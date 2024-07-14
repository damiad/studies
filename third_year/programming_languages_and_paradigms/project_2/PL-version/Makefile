all:
	bnfc --functor -p Built OSkell.cf
	alex --ghc Built/LexOSkell.x
	happy --ghc --coerce --array Built/ParOSkell.y
	ghc --make Built/TestOSkell.hs -o Built/TestOSkell
	ghc -package mtl -Wall -i./Built -i./src src/Main.hs -o interpreter
clean:
	rm -rf Built
	rm -f src/*.o src/*.hi interpreter
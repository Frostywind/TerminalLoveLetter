build: 
	g++ -O3 -std=c++20 -o loveletter main.cpp

install:
	install -m 755 loveletter ~/.local/bin/

clean:
		$(RM) main

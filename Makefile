
all: 
	g++ ./*.cpp -std=c++0x -pthread -o IA

test1:
	./IA localhost 2042

test2:
	rm /home/paul/Documents/Empire/v2/empire-captain/IA
	ln -s /home/paul/Documents/AI/IA/IA /home/paul/Documents/Empire/v2/empire-captain/IA
	cp ./ai42.py /home/paul/Documents/Empire/v2/empire-captain
	

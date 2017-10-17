run: TicTacToe.jar
	java -jar TicTacToe.jar &

clean:
	rm -rf classes TicTacToe.jar

TicTacToe.jar: classes/TicTacToe.class
	@echo "Main-Class: TicTacToe" > Main-Class.txt
	jar cmf Main-Class.txt TicTacToe.jar -C classes .
	@rm Main-Class.txt

# Building TicTacToe.class causes javac automatically
# to find and build other necessary classes too
classes/TicTacToe.class: TicTacToe.java GameBoard.java
	mkdir -p classes
	javac -encoding UTF8 -d classes TicTacToe.java

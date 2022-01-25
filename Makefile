play:
	gcc -Wall -o play data_struct.c game.c main.c text_ui.c -lncurses 
run:
	./play
clean:
	-rm play

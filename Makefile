play:
	gcc -Wall -lncurses -o play data_struct.c game.c main.c text_ui.c
run:
	./play
clean:
	-rm play

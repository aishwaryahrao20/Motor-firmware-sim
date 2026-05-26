CC     = gcc
CFLAGS = -Wall -Wextra -g -O0
LIBS   = -lpthread -lm

SRC = main.c hal_sim.c motor_model.c pid.c rtos_sim.c
OUT = motor_sim

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) $(LIBS)

clean:
	rm -f $(OUT)
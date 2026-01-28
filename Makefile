CC=clang 
CFLAGS=-Wall -Iinclude

PROGRAMMES=test_terrain test_robot robot_terrain curiosity curiosity-test test_generation_terrains curiosity-perf curiosity-obs

all: $(PROGRAMMES)

######################################################################
#                       Règles de compilation                        #
######################################################################

# Compile .c in src/ into .o in src/
src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

src/test_terrain.o: src/test_terrain.c include/terrain.h

src/test_robot.o: src/test_robot.c include/robot.h

src/robot_terrain.o: src/robot_terrain.c include/terrain.h include/robot.h

src/robot.o: src/robot.c include/robot.h

src/terrain.o: src/terrain.c include/terrain.h

src/environnement.o: src/environnement.c include/environnement.h include/robot.h include/terrain.h include/observateur.h

src/programme.o: src/programme.c include/programme.h include/type_pile.h

src/interprete.o: src/interprete.c include/interprete.h include/environnement.h \
	include/programme.h include/type_pile.h include/robot.h include/terrain.h include/observateur.h

src/interprete%.o: src/interprete%.c include/interprete.h include/environnement.h \
	include/programme.h include/type_pile.h include/robot.h include/terrain.h include/observateur.h

src/type_pile.o: src/type_pile.c include/type_pile.h

src/curiosity.o: src/curiosity.c include/environnement.h include/programme.h \
	include/interprete.h include/robot.h include/terrain.h include/type_pile.h include/observateur.h

src/curiosity-test.o: src/curiosity-test.c include/environnement.h include/programme.h \
	include/interprete.h include/robot.h include/terrain.h include/type_pile.h include/observateur.h

src/generation_terrains.o : src/generation_terrains.c include/generation_terrains.h include/terrain.h

src/test_generation_terrains.o : src/test_generation_terrains.c include/generation_terrains.h include/terrain.h

src/curiosity-perf.o : src/curiosity-perf.c include/generation_terrains.h include/programme.h include/environnement.h  \
	include/interprete.h include/robot.h include/terrain.h include/type_pile.h include/observateur.h

src/observateur.o : src/observateur.c include/observateur.h

src/curiosity-obs.o : src/curiosity-obs.c include/programme.h include/environnement.h  \
	include/interprete.h include/robot.h include/terrain.h include/type_pile.h include/observateur.h

######################################################################
#                       Règles d'édition de liens                    #
######################################################################

test_terrain: src/test_terrain.o src/terrain.o
	$(CC) $^ -o $@

test_robot: src/test_robot.o src/robot.o
	$(CC) $^ -o $@

robot_terrain: src/robot_terrain.o src/terrain.o src/robot.o
	$(CC) $^ -o $@

curiosity: src/curiosity.o src/environnement.o src/programme.o src/interprete.o \
	src/robot.o src/terrain.o src/type_pile.o src/observateur.o
	$(CC) $^ -o $@

curiosity-test: src/curiosity-test.o src/environnement.o src/programme.o src/interprete.o \
	src/robot.o src/terrain.o src/type_pile.o src/observateur.o
	$(CC) $^ -o $@

curiosity-test%: src/curiosity-test.o src/environnement.o src/programme.o src/interprete%.o \
	src/robot.o src/terrain.o src/type_pile.o src/observateur.o
	$(CC) $^ -o $@

test_generation_terrains: src/test_generation_terrains.o src/generation_terrains.o src/terrain.o
	$(CC) $^ -o $@

curiosity-perf: src/curiosity-perf.o src/generation_terrains.o src/programme.o src/environnement.o src/interprete.o src/robot.o src/terrain.o src/type_pile.o src/observateur.o
	$(CC) $^ -o $@

curiosity-obs: src/curiosity-obs.o src/programme.o src/observateur.o src/environnement.o src/interprete.o src/robot.o src/terrain.o src/type_pile.o
	$(CC) $^ -o $@

clean:
	rm -f $(PROGRAMMES) src/*.o

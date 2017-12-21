#TARGETS=	example1 example2 example3
TARGETS=	example1 example2

CC=			g++

INCLUDES=	

all: $(TARGETS)

HDRS=		Common.h



TINYXML2_DIR=	../tinyxml2
TINYXML2_INC=	-I $(TINYXML2_DIR)
TINYXML2_LIB=	-L $(TINYXML2_DIR) -l tinyxml2
INCLUDES+=		$(TINYXML2_INC)




CFLAGS=		-g $(INCLUDES)

%.o:		%.cpp $(HDRS)
	$(CC) $(CFLAGS) -c -o $@ $*.cpp




EXAMPLE1_OBJS=		main.o Common.o
EXAMPLE1_LIB=		example1.a
$(EXAMPLE1_LIB):	$(EXAMPLE1_OBJS) $(HDRS)
	ar -r $(@) $(EXAMPLE1_OBJS)

EXAMPLE1_LIBS=		example1.a
example1:	example1.o $(EXAMPLE1_LIB) $(HDRS)
	$(CC) $(CFLAGS) -o $@ example1.o $(EXAMPLE1_LIBS)




EXAMPLE2_OBJS=		main.o Common.o CThread.o timeUtils.o
EXAMPLE2_LIB=		example2.a
$(EXAMPLE2_LIB):	$(EXAMPLE2_OBJS) $(HDRS)
	ar -r $(@) $(EXAMPLE2_OBJS)

EXAMPLE2_LIBS=		$(EXAMPLE2_LIB) -lpthread -lrt
example2:	example2.o $(EXAMPLE2_LIB) $(HDRS)
	$(CC) $(CFLAGS) -o $@ example2.o $(EXAMPLE2_LIBS)




#EXAMPLE3_OBJS=		Common.o CThread.o timeUtils.o
#EXAMPLE3_LIB=		example3.a
#$(EXAMPLE3_LIB):	$(EXAMPLE3_OBJS) $(HDRS)
	#ar -r $(@) $(EXAMPLE3_OBJS)
#
#EXAMPLE3_LIBS=		$(EXAMPLE3_LIB) -lpthread -lrt $(TINYXML2_LIB)
#example3:	example3.o $(EXAMPLE3_LIB) $(HDRS)
	#$(CC) $(CFLAGS) -o $@ example3.o $(EXAMPLE3_LIBS)
#
#
#
#

#EXAMPLE_LIBS=		$(EXAMPLE1_LIB) $(EXAMPLE2_LIB) $(EXAMPLE3_LIB)
EXAMPLE_LIBS=		$(EXAMPLE1_LIB) $(EXAMPLE2_LIB)

clean:
	rm -f *.o $(TARGETS) $(EXAMPLE_LIBS)

tar:
	tar cvfz template.tar.gz *.cpp *.h Makefile


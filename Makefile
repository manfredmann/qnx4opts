TARGET = qnx4opts
SRCDIR = ./src/
SRCS = re.cpp optparser.cpp main.cpp
BINDIR = ./bin/
OBJDIR = ./obj/
OBJS = $(SRCS:.cpp=.o)
CC = cc
INCLUDEDIRS += ./inc
LIBS = 

CFLAGS = -Oentx -ms -s -w2 -5 \
						$(addprefix -I, $(INCLUDEDIRS))
LDFLAGS = -M \
						$(addprefix -l, $(LIBS))

.PHONY:     			all clean

all:					$(BINDIR)$(TARGET)

clean:
						-rm -f $(OBJDIR)*.o $(BINDIR)$(TARGET) $(BINDIR)*.map *.err 

$(OBJDIR)%.o : $(SRCDIR)%.cpp
						$(CC) $(CFLAGS) -c -o $@ $<

$(BINDIR)$(TARGET): 	$(addprefix $(OBJDIR), $(OBJS))
						$(CC) $(LDFLAGS) -o $@ $^

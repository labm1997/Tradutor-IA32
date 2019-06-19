# Project folders

#BINDIR=bin
OBJDIR=obj
SRCDIR=src
INCDIR=inc

# Project compiler
CC=g++

# Compiler flags
CFLAGS=-I $(INCDIR) -Wall -std=c++11

# Get sources from source folder
SOURCES=$(shell find $(SRCDIR) -name *.cpp)

# For each source define .o
OBJECTS=$(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SOURCES))

# Linked binary
BINARY=tradutor

# Rule to make linked binary, depends on all .o and on bin folder
$(BINARY): $(OBJECTS)
	$(CC) $^ -o $@ $(CFLAGS)

install: $(BINARY)

run: $(BINARY)
	./$(BINARY) test_files/bin.asm

# Add (if exists) dependency rules for each .o
-include $(OBJECTS:.o=.d)

# Rule to make .o from .cpp and to create dependency rules
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | Makefile $(OBJDIR)
	$(CC) -c $< -o $@ $(CFLAGS)
	@printf "$(OBJDIR)/" "%s" > $(OBJDIR)/$*.d
	@$(CC) $(CFLAGS) -MM $< >> $(OBJDIR)/$*.d

# Rule to create folders
$(OBJDIR):
	mkdir $@

# Remove all generated files
clean: 
	rm -rf $(OBJDIR) $(BINARY)

OBJS = main.o headerFile_Inclusion.o removeComments.o replaceMacro.o 

target = my_preprocessor 

cc = gcc     

exe : ${OBJS}
	$(cc) $(OBJS) -o my_preprocessor
main.o : main.c
	$(cc) -c $^
headerFile_Inclusion.o : headerFile_Inclusion.c
	$(cc) -c $^
removeComments.o : removeComments.c
	$(cc) -c $^
replaceMacro.o : replaceMacro.c
	$(cc) -c $^
clear:
	@echo "clearing please wait..."
	@rm -rv *.o 
